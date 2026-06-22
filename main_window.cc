#include "main_window.h"
#include "ui_main_window.h"

#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server_thread_ = new QThread();
    server_ = new Server();

    server_->moveToThread(server_thread_);

    proxy_connections_ = new ProxyConnections(server_);
    ui->connectionsView->setModel(proxy_connections_);
    ui->connectionsView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->connectionsView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->connectionsView->verticalHeader()->setVisible(false);
    ui->connectionsView->setSelectionBehavior(QAbstractItemView::SelectRows);

    update_timer_ = new QTimer(this);
    connect(update_timer_, &QTimer::timeout, this, &MainWindow::UpdateTimerHandler);
    update_timer_->start(kUpdateTimeout);

    connect(ui->connectionsView, &QTableView::clicked, this, &MainWindow::ConnectionHandler);

    connect(server_thread_, &QThread::finished, server_thread_, &QObject::deleteLater);

    connect(ui->startBtn, &QPushButton::clicked, server_, &Server::Start);
    connect(ui->stopBtn, &QPushButton::clicked, server_, &Server::Stop);
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

void MainWindow::UpdateTimerHandler()
{

}

void MainWindow::ConnectionHandler(const QModelIndex& index)
{
    if(!index.isValid())
        return;

    // std::unique_ptr<ProxyData> model = server_->GetActiveConnections()[index.row()];

}
