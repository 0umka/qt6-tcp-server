#ifndef PROXY_DATA_H
#define PROXY_DATA_H

#include <QAbstractTableModel>
#include <QJsonObject>
#include <QVector>
#include "client.h"

class ProxyData : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ProxyData(Client* model, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private slots:
    void UpdateData(const QJsonObject& data);

private:
    struct Row {
        QString type;
        QString key;
    };

    Client* model_ = nullptr;
    QVector<Row> rows_;
    QJsonObject data_by_type_;

    static constexpr int kColumns = 3;
};

#endif // PROXY_DATA_H
