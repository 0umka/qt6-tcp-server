#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>

#include "client.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    void Start();
    void Stop();

signals:
    void ClientStatusChanged(Client::Statuses Status);
    void ClientDataReceived();
    void ServerStarted();

private slots:
    void ConnectClient();

private:
    QTcpServer* tcp_server_ = nullptr;
    static constexpr int kPort = 12345;
    std::list<Client*> active_connections_;
};

#endif // SERVER_H
