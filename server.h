#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include <QString>
#include <vector>

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
    void Pause();

    struct Connection {
        Client* client = nullptr;
        ProxyData* data = nullptr;
    };

    int GetConnectionCount() const {return static_cast<int>(active_connections_.size()); }
    Client* GetClient(int row) const {return active_connections_.at(row).client; }
    ProxyData* GetData(int row) const {return active_connections_.at(row).data; }

signals:
    void ClientStatusChanged();
    void ConnectionsChanged();
    void LogMessage(const QString& message);
    void ServerStarted();

private slots:
    void ConnectClient();
    void RemoveClient();
    void UpdateClientStatuses();

private:
    QTcpServer* tcp_server_ = nullptr;
    QTimer* status_timer_ = nullptr;
    static constexpr int kPort = 12345;
    static constexpr int kStatusTimeout = 1000;
    std::vector<Connection> active_connections_;
};

#endif // SERVER_H
