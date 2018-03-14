#include "alter_link_description_migration.h"

// Qt
#include <QCoreApplication>
#include <QDebug>

// Internal
#include "settings_provider.h"

#include "link_description.h"

#include "generic_repository.h"

using namespace dto;
using namespace db;

bool AlterLinkDescriptionMigration::up()
{
    if (!m_query.prepare("DROP TABLE links") || !m_query.exec()) return false;

    if (!m_query.prepare("CREATE TABLE links ("
                         "id INTEGER PRIMARY KEY NOT NULL,"
                         "name STRING,"
                         "type SMALLINT,"
                         "parameters TEXT,"
                         "autoConnect BOOLEAN)") || !m_query.exec()) return false;

    GenericRepository<dto::LinkDescription> linkRepository("links");

    LinkDescriptionPtr defaultUdpLink = LinkDescriptionPtr::create();
    defaultUdpLink->setType(LinkDescription::Udp);
    defaultUdpLink->setName(qApp->translate("DefaultParamsMigration", "Default UDP Link"));
    defaultUdpLink->setParameter(LinkDescription::Port,
                                 settings::Provider::value(settings::communication::port).toInt());
    defaultUdpLink->setAutoConnect(true);
    linkRepository.save(defaultUdpLink);

    LinkDescriptionPtr defaultSerialLink = LinkDescriptionPtr::create();
    defaultSerialLink->setType(LinkDescription::Serial);
    defaultSerialLink->setName(qApp->translate("DefaultParamsMigration", "Default Serial Link"));
    defaultSerialLink->setParameter(LinkDescription::BaudRate,
                                    settings::Provider::value(settings::communication::baudRate).toInt());
    defaultSerialLink->setAutoConnect(true);
    linkRepository.save(defaultSerialLink);

    return DbMigration::up();
}

bool AlterLinkDescriptionMigration::down()
{
    if (!m_query.prepare("DROP TABLE links") || !m_query.exec()) return false;

    if (!m_query.prepare("CREATE TABLE links ("
                         "id INTEGER PRIMARY KEY NOT NULL,"
                         "name STRING,"
                         "type SMALLINT,"
                         "port INTEGER,"
                         "device STRING,"
                         "baudRate INTEGER,"
                         "autoConnect BOOLEAN)") || !m_query.exec()) return false;

    return true;
}

QDateTime AlterLinkDescriptionMigration::version() const
{
    return QDateTime::fromString("2018.03.08-14:58:50", format);
}