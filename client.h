#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
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

    void Start();
    void Stop();
    void UpdateStatus();

    QString GetIpAddress();
    Statuses GetStatus();

signals:
    void StatusChanged(Client::Statuses Status);
    void DataReceived(QJsonObject data);
    void ClientDisconnected();

private slots:
    void ProcessPendingDatagram();
    void SocketDisconnected();

private:
    QTcpSocket* tcp_socket_;
    QString ip_address_;
    bool stopped_ = false;
    int packet_count_ = 0;
    int prev_packet_count_ = 0;
    Statuses current_status_ = Statuses::kDisconnected;
};

#endif // CLIENT_H
