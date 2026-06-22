#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QJsonObject>
#include <QString>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QTcpSocket* socket, QObject *parent = nullptr);
    ~Client();

    enum class Statuses {
        kConnected,
        kDisconnected,
        kNoData
    };

    struct Data {
        int packet_size;
        QJsonObject json_data;
    };

    void Start();
    void Stop(bool notify = true);

    QString GetIpAddress();
    Statuses GetStatus();

signals:
    void StatusChanged(Client::Statuses Status);
    void DataReceived(QByteArray data);
    void ClientDisconnected();

private slots:
    void StatusTimerHandler();
    void ProcessPendingDatagram();
    void SocketDisconnected();

private:
    QTcpSocket* tcp_socket_;
    QString ip_address_;
    QTimer* status_timer_ = nullptr;
    bool stopped_ = false;
    int packet_count_ = 0;
    int prev_packet_count_ = 0;
    Statuses current_status_ = Statuses::kDisconnected;
    static constexpr int kStatusTimeout = 1000;
};

#endif // CLIENT_H
