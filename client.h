#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QJsonObject>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QTcpSocket* socket, QObject *parent = nullptr);

    enum class Statuses {
        kConnected,
        kDisconnected,
        kNoData
    };

    struct Data {
        int PacketSize;
        QJsonObject JsonData;
    };

    void Start();
    void Stop();

signals:
    void StatusChanged(Client::Statuses Status);
    void DataReceived(QByteArray data);

private slots:
    void StatusTimerHandler();
    void ProcessPendingDatagram();

private:
    QTcpSocket* tcp_socket_;
    QTimer* status_timer_ = nullptr;
    int packet_count_ = 0;
    int prev_packet_count_ = 0;
    Statuses current_status_ = Statuses::kDisconnected;
    static constexpr int kStatusTimeout = 1000;
};

#endif // CLIENT_H
