#include "server.h"

Server::Server(QObject *parent)
    : QObject{parent}
{}

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
        for(auto& client : active_connections_) {
            disconnect(client.first, &Client::ClientDisconnected, this, &Server::RemoveClient);
            client.first->Stop(false);
            client.first->deleteLater();
        }
        active_connections_.clear();
        active_clients_list_.clear();
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
            connect(client, &Client::StatusChanged, this, &Server::ClientStatusChanged);
            connect(client, &Client::DataReceived, this, &Server::ClientDataReceived);
            connect(client, &Client::ClientDisconnected, this, &Server::RemoveClient);
            active_connections_[client] = new ProxyData(client, client);
            active_clients_list_.push_back(client);
            emit ConnectionsChanged();
        }
    }
}

void Server::RemoveClient()
{
    Client* client = qobject_cast<Client*>(sender());
    if (!client)
        return;

    active_connections_.erase(client);
    for (auto it = active_clients_list_.begin(); it != active_clients_list_.end(); ++it) {
        if (*it == client) {
            active_clients_list_.erase(it);
            break;
        }
    }
    client->deleteLater();
    emit ConnectionsChanged();
}
