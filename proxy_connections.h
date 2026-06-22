#ifndef PROXY_CONNECTIONS_H
#define PROXY_CONNECTIONS_H

#include <QAbstractTableModel>
#include "server.h"

class ProxyConnections : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ProxyConnections(Server* model, QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private slots:
    void Reset();
    void UpdateStatuses();

private:
    Server* model_ = nullptr;

    static constexpr int kColumns = 2;
};

#endif // PROXY_CONNECTIONS_H
