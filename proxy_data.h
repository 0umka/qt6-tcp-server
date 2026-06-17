#ifndef PROXY_DATA_H
#define PROXY_DATA_H

#include <QAbstractTableModel>
#include "client.h"

class ProxyData : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ProxyData(Client* model, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    static constexpr int kColumns = 2;
};

#endif // PROXY_DATA_H
