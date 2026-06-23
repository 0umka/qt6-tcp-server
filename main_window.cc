#include "main_window.h"
#include "ui_main_window.h"

#include <QDateTime>
#include <QHeaderView>
#include <QSizePolicy>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->logView->setMaximumHeight(QWIDGETSIZE_MAX);
    ui->logView->document()->setMaximumBlockCount(kMaxLogLines);

    server_thread_ = new QThread();
    server_ = new Server();

    server_->moveToThread(server_thread_);

    proxy_connections_ = new ProxyConnections(server_);
    ui->connectionsView->setModel(proxy_connections_);
    ui->connectionsView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->connectionsView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->connectionsView->verticalHeader()->setVisible(false);
    ui->connectionsView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->connectionsView, &QTableView::clicked, this, &MainWindow::ConnectionHandler);
    connect(ui->startBtn, &QPushButton::clicked, server_, &Server::Start);
    connect(ui->stopBtn, &QPushButton::clicked, server_, &Server::Stop);

    connect(server_thread_, &QThread::finished, server_thread_, &QObject::deleteLater);

    connect(server_, &Server::LogMessage, this, &MainWindow::AppendLog);
    server_thread_->start();
}

MainWindow::~MainWindow()
{
    if (server_thread_ && server_thread_->isRunning()) {
        server_thread_->quit();
        server_thread_->wait();
    }
    delete ui;
}

/**
 * @brief Sets model for table of chosen client
 * @param index
 */
void MainWindow::ConnectionHandler(const QModelIndex& index)
{
    if(!index.isValid())
        return;

    if (index.row() < 0 || index.row() >= server_->GetConnectionCount())
        return;

    ui->dataView->setModel(server_->GetData(index.row()));
    ui->dataView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->dataView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->dataView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

}

/**
 * @brief MainWindow::AppendLog
 * @param message
 */
void MainWindow::AppendLog(const QString& message)
{
    ui->logView->appendPlainText(QDateTime::currentDateTime().toString("HH:mm:ss ") + message);
}
