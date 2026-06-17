#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QThread>

#include "server.h"
#include "proxy_connections.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void UpdateTimerHandler();
    void ConnectionHandler(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    ProxyConnections* proxy_connections_ = nullptr;
    Server* server_ = nullptr;
    QTimer* update_timer_ = nullptr;
    QThread* server_thread_ = nullptr;
    static constexpr int kUpdateTimeout = 1000;
};
#endif // MAIN_WINDOW_H
