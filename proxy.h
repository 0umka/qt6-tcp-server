#ifndef PROXY_H
#define PROXY_H

#include <QAbstractTableModel>

class Proxy : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Proxy(QObject *parent = nullptr);
};

#endif // PROXY_H
