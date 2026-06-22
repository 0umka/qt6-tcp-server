#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>

#include "client.h"
#include "proxy_data.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    void Start();
    void Stop();

    std::unordered_map<Client*, ProxyData*> GetActiveConnections() const {return active_connections_; }
    std::vector<Client*> GetActiveClients() const {return active_clients_list_; }

signals:
    void ClientStatusChanged(Client::Statuses Status);
    void ClientDataReceived();
    void ConnectionsChanged();
    void ServerStarted();

private slots:
    void ConnectClient();
    void RemoveClient();

private:
    QTcpServer* tcp_server_ = nullptr;
    static constexpr int kPort = 12345;
    std::unordered_map<Client*, ProxyData*> active_connections_;
    std::vector<Client*> active_clients_list_;
};

#endif // SERVER_H
