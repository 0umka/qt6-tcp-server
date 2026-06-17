#include "server.h"

Server::Server(QObject *parent)
    : QObject{parent}
{}

Server::~Server()
{
    if(tcp_server_){
        Stop();
    }
}

void Server::Stop()
{
    if (tcp_server_){
        disconnect(tcp_server_, &QTcpServer::pendingConnectionAvailable, this, &Server::ConnectClient);
        tcp_server_->close();
        delete tcp_server_;
        tcp_server_ = nullptr;
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
            active_connections_[client] = new ProxyData(client, client);
            active_clients_list_.push_back(client);
        }
    }
}

