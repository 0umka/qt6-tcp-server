#include "main_window.h"
#include "ui_main_window.h"

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

    update_timer_ = new QTimer(this);
    connect(update_timer_, &QTimer::timeout, this, &MainWindow::UpdateTimerHandler);
    update_timer_->start(kUpdateTimeout);

    connect(ui->connectionsView, &QTableView::clicked, this, &MainWindow::ConnectionHandler);

    connect(server_thread_, &QThread::finished, server_thread_, &QObject::deleteLater);
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
