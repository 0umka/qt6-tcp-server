#include "client.h"

/**
 * @brief Client::Client
 * @param socket
 * @param parent
 */
Client::Client(QTcpSocket* socket, QObject *parent)
    : QObject{parent},
    tcp_socket_(socket)
{
    tcp_socket_->setParent(this);


    ip_address_ = tcp_socket_->peerAddress().toString();

    connect(tcp_socket_, &QTcpSocket::readyRead, this, &Client::ProcessPendingDatagram);
    connect(tcp_socket_, &QTcpSocket::disconnected, this, &Client::SocketDisconnected);
}

Client::~Client()
{
    if (tcp_socket_)
        Stop();
}

/**
 * @brief Stop client, close it`s socket
 */
void Client::Stop()
{
    if (tcp_socket_ && !stopped_){
        stopped_ = true;
        disconnect(tcp_socket_, &QTcpSocket::readyRead, this, &Client::ProcessPendingDatagram);
        disconnect(tcp_socket_, &QTcpSocket::disconnected, this, &Client::SocketDisconnected);
        tcp_socket_->close();
        current_status_ = Statuses::kDisconnected;
    }
}

/**
 * @brief Emits disconnect and stops client
 */
void Client::SocketDisconnected()
{
    Stop();
    emit ClientDisconnected();
}

/**
 * @brief Client::GetIpAddress
 * @return
 */
QString Client::GetIpAddress()
{
    return ip_address_;
}

/**
 * @brief Client::GetStatus
 * @return status
 */
Client::Statuses Client::GetStatus()
{
    return current_status_;
}

/**
 * @brief Updates connection status
 */
void Client::UpdateStatus()
{
    if(packet_count_ > prev_packet_count_) {
        current_status_ = Statuses::kConnected;
    } else {
        current_status_ = Statuses::kNoData;
    }
    if(!tcp_socket_) {
        current_status_ = Statuses::kDisconnected;
    }
    prev_packet_count_ = packet_count_;
}

/**
 * @brief Reads socket`s bytes and emits receiving
 */
void Client::ProcessPendingDatagram()
{
    while(tcp_socket_->bytesAvailable()){
        QByteArray data = tcp_socket_->readAll();
        packet_count_++;
        emit DataReceived(QJsonDocument::fromJson(data).object());
    }
}
