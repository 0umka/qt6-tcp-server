#include "client.h"

Client::Client(QTcpSocket* socket, QObject *parent)
    : QObject{parent},
    tcp_socket_(socket)
{
    tcp_socket_->setParent(this);
    ip_address_ = tcp_socket_->peerAddress().toString();
    status_timer_ = new QTimer(this);
    connect(status_timer_, &QTimer::timeout, this, &Client::StatusTimerHandler);
    status_timer_->start(kStatusTimeout);

    connect(tcp_socket_, &QTcpSocket::readyRead, this, &Client::ProcessPendingDatagram);
    connect(tcp_socket_, &QTcpSocket::disconnected, this, &Client::SocketDisconnected);
}

Client::~Client()
{
    if (tcp_socket_)
        Stop(false);
}

void Client::Stop(bool notify)
{
    if (tcp_socket_ && !stopped_){
        stopped_ = true;
        disconnect(tcp_socket_, &QTcpSocket::readyRead, this, &Client::ProcessPendingDatagram);
        disconnect(tcp_socket_, &QTcpSocket::disconnected, this, &Client::SocketDisconnected);
        tcp_socket_->close();
        current_status_ = Statuses::kDisconnected;
        if (notify)
            emit StatusChanged(current_status_);
    }
}

void Client::SocketDisconnected()
{
    Stop(false);
    emit ClientDisconnected();
}

QString Client::GetIpAddress()
{
    return ip_address_;
}

Client::Statuses Client::GetStatus()
{
    return current_status_;
}

void Client::StatusTimerHandler()
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
    emit StatusChanged(current_status_);
}

void Client::ProcessPendingDatagram()
{
    while(tcp_socket_->bytesAvailable()){
        QByteArray data = tcp_socket_->readAll();
        packet_count_++;
        emit DataReceived(data);
    }
}
