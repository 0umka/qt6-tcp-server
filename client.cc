#include "client.h"

Client::Client(QTcpSocket* socket, QObject *parent)
    : QObject{parent},
    tcp_socket_(socket)
{
    status_timer_ = new QTimer(this);
    connect(status_timer_, &QTimer::timeout, this, &Client::StatusTimerHandler);
    status_timer_->start(kStatusTimeout);

    connect(tcp_socket_, &QTcpSocket::readyRead, this, &Client::ProcessPendingDatagram);
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
