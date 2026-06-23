#include "proxy_connections.h"

ProxyConnections::ProxyConnections(Server* model, QObject *parent)
    : QAbstractTableModel{parent}
    , model_(model)
{
    connect(model_, &Server::ConnectionsChanged, this, &ProxyConnections::Reset);
    connect(model_, &Server::ClientStatusChanged, this, &ProxyConnections::UpdateStatuses);
}

void ProxyConnections::Reset()
{
    beginResetModel();
    endResetModel();
}

void ProxyConnections::UpdateStatuses()
{
    int rows = rowCount();
    if (rows > 0)
        emit dataChanged(index(0, 1), index(rows - 1, 1), {Qt::DisplayRole});
}

int ProxyConnections::rowCount(const QModelIndex &parent) const
{
    return model_->GetConnectionCount();
}

/**
 * @inherits
 * @param parent
 * @return
 */
int ProxyConnections::columnCount(const QModelIndex &parent) const
{
    return kColumns;
}

/**
 * @inherits
 * @param index
 * @param role
 * @return
 */
QVariant ProxyConnections::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= model_->GetConnectionCount())
        return {};

    QVariant data;

    Client* client = model_->GetClient(index.row());
    Client::Statuses status = client->GetStatus();
    switch(role) {
    case Qt::DisplayRole: {
        switch (index.column()) {
        case 0: data = client->GetIpAddress(); break;
        case 1:
            switch (status) {
            case Client::Statuses::kConnected: return "Connected";
            case Client::Statuses::kDisconnected: return "Disconnected";
            case Client::Statuses::kNoData: return "No data";
            } break;
        } break;
    }
    }
    return data;
}

QVariant ProxyConnections::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};

    switch (section) {
    case 0: return "IP";
    case 1: return "Status";
    default: return {};
    }
}
