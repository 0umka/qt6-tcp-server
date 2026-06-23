#include "proxy_data.h"

#include <QJsonArray>
#include <QJsonDocument>

ProxyData::ProxyData(Client* model, QObject *parent)
    : QAbstractTableModel{parent}
    , model_(model)
{
    connect(model_, &Client::DataReceived, this, &ProxyData::UpdateData);
}

/**
 * @inherits
 * @param parent
 * @return number of rows
 */
int ProxyData::rowCount(const QModelIndex &parent) const
{
    return rows_.size();
}

/**
 * @inherits
 * @param parent
 * @return
 */
int ProxyData::columnCount(const QModelIndex &parent) const
{
    return kColumns;
}

/**
 * @inherits
 * @param index
 * @param role
 * @return
 */
QVariant ProxyData::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const Row& row = rows_.at(index.row());
    if (index.column() == 0)
        return row.type;
    if (index.column() == 1)
        return row.key;

    const QJsonValue value = data_by_type_.value(row.type).toObject().value(row.key);
    if (value.isObject())
        return QString::fromUtf8(QJsonDocument(value.toObject()).toJson(QJsonDocument::Compact));
    if (value.isArray())
        return QString::fromUtf8(QJsonDocument(value.toArray()).toJson(QJsonDocument::Compact));
    return value.toVariant();
}

/**
 * @inherits
 * @param section
 * @param orientation
 * @param role
 * @return
 */
QVariant ProxyData::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};

    switch (section) {
    case 0: return "Type";
    case 1: return "Key";
    case 2: return "Value";
    default: return {};
    }
}

/**
 * @brief updates table`s model data by type
 * @param data
 */
void ProxyData::UpdateData(const QJsonObject& data)
{
    const QString type = data.value("type").toString("unknown");
    QJsonObject typed_data = data_by_type_.value(type).toObject();

    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
        if (it.key() == "type")
            continue;

        typed_data.insert(it.key(), it.value());

        int row = -1;
        for (int i = 0; i < rows_.size(); ++i) {
            if (rows_[i].type == type && rows_[i].key == it.key()) {
                row = i;
                break;
            }
        }

        if (row == -1) {
            row = rows_.size();
            beginInsertRows({}, row, row);
            rows_.push_back({type, it.key()});
            endInsertRows();
        }
    }

    data_by_type_.insert(type, typed_data);
    if (!rows_.isEmpty())
        emit dataChanged(index(0, 2), index(rows_.size() - 1, 2), {Qt::DisplayRole});
}
