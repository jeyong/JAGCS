#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

// Qt
#include <QObject>
#include <QHostAddress>

// Internal
#include <i_link.h>

namespace domain
{
    class ConnectionManager : public QObject
    {
        Q_OBJECT

    public:
        explicit ConnectionManager(QObject* parent = nullptr);
        ~ConnectionManager() override;

        QList<data_source::ILink*> links() const;

    public slots:
        void addNewSerialLink(const QString& portName, qint32 baudRate);
        void addNewUdpLink(int hostPort, const QString& address, int port);

        void removeLink(data_source::ILink* link);

    signals:
        void linksChanged(QList<data_source::ILink*>);

    private:
        class Impl;
        Impl* const d;
    };
}

#endif // CONNECTION_MANAGER_H
