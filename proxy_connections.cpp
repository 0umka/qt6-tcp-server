#include "proxy_connections.h"

ProxyConnections::ProxyConnections(Server* model, QObject *parent)
    : QAbstractTableModel{parent}
    , model_(model)
{}

int ProxyConnections::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return model_->GetActiveClients().size();
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
    QVariant data;

    Client* client = model_->GetActiveClients().at(index.row());
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