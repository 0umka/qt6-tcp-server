#include "server.h"

#include <QJsonDocument>

Server::Server(QObject *parent)
    : QObject{parent}
{
    status_timer_ = new QTimer(this);
    connect(status_timer_, &QTimer::timeout, this, &Server::UpdateClientStatuses);
    status_timer_->start(kStatusTimeout);
}

Server::~Server()
{
    if(tcp_server_){
        Stop();
    }
    delete tcp_server_;
    tcp_server_ = nullptr;
}

void Server::Stop()
{
    if (tcp_server_){
        disconnect(tcp_server_, &QTcpServer::pendingConnectionAvailable, this, &Server::ConnectClient);
        tcp_server_->close();
        for(auto& connection : active_connections_) {
            disconnect(connection.client, &Client::ClientDisconnected, this, &Server::RemoveClient);
            connection.client->Stop();
            connection.client->deleteLater();
        }
        active_connections_.clear();
        delete tcp_server_;
        tcp_server_ = nullptr;
        emit ConnectionsChanged();
    }
}

void Server::Start()
{
    if (tcp_server_)
        Stop();

    tcp_server_ = new QTcpServer(this);

    if (tcp_server_->listen(QHostAddress::AnyIPv4, kPort)) {
        connect(tcp_server_, &QTcpServer::pendingConnectionAvailable, this, &Server::ConnectClient);
        emit ServerStarted();
    }
}

void Server::ConnectClient()
{
    if (tcp_server_->hasPendingConnections()) {
            QTcpSocket* connection = tcp_server_->nextPendingConnection();
        if(connection){
            Client* client =  new Client(connection, this);
            connect(client, &Client::ClientDisconnected, this, &Server::RemoveClient);
            connect(client, &Client::DataReceived, this, [this](const QJsonObject& data) {
                if (data.value("type").toString() != "Log")
                    return;

                const QString message = data.value("message").toString();
                emit LogMessage(message.isEmpty()
                                    ? QString::fromUtf8(QJsonDocument(data).toJson(QJsonDocument::Compact))
                                    : message);
            });
            active_connections_.push_back({client, new ProxyData(client, client)});
            emit LogMessage(QString("Connected: %1").arg(client->GetIpAddress()));
            emit ConnectionsChanged();
        }
    }
}

void Server::RemoveClient()
{
    Client* client = qobject_cast<Client*>(sender());
    if (!client)
        return;

    emit LogMessage(QString("Disconnected: %1").arg(client->GetIpAddress()));

    for (auto it = active_connections_.begin(); it != active_connections_.end(); ++it) {
        if (it->client == client) {
            active_connections_.erase(it);
            break;
        }
    }
    client->deleteLater();
    emit ConnectionsChanged();
}

void Server::UpdateClientStatuses()
{
    for (auto& connection : active_connections_)
        connection.client->UpdateStatus();

    emit ClientStatusChanged();
}
