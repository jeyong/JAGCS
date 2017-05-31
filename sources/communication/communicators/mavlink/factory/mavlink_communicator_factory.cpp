#include "mavlink_communicator_factory.h"

// Internal
#include "settings_provider.h"
#include "mavlink_communicator.h"

#include "ping_handler.h"
#include "heartbeat_handler.h"
#include "system_status_handler.h"
#include "attitude_handler.h"
#include "position_handler.h"
#include "home_position_handler.h"
#include "gps_handler.h"
#include "vfr_hud_handler.h"
#include "wind_handler.h"
#include "nav_controller_handler.h"
#include "command_handler.h"
#include "mission_handler.h"

using namespace comm;

MavLinkCommunicatorFactory::MavLinkCommunicatorFactory(db::DbFacade* dbFacade,
        domain::TelemetryService* telemetryService,
        domain::CommandService* commandService,
        quint8 systemId, quint8 componentId):
    ICommunicatorFactory(),
    m_dbFacade(dbFacade),
    m_telemetryService(telemetryService),
    m_commandService(commandService),
    m_systemId(systemId),
    m_componentId(componentId)
{}

AbstractCommunicator* MavLinkCommunicatorFactory::create()
{
    auto communicator = new MavLinkCommunicator(m_systemId, m_componentId);

    new PingHandler(communicator);
    new HeartbeatHandler(m_telemetryService, communicator);
    new SystemStatusHandler(m_telemetryService, communicator);
    new AttitudeHandler(m_telemetryService, communicator);
    new PositionHandler(m_telemetryService, communicator);
    new HomePositionHandler(m_telemetryService, communicator);
    new GpsHandler(m_telemetryService, communicator);
    new VfrHudHandler(m_telemetryService, communicator);
    new WindHandler(m_telemetryService, communicator);
    new NavControllerHandler(m_telemetryService, communicator);
    // TODO: RADIO_STATUS
    new CommandHandler(m_commandService, communicator);
    new MissionHandler(m_dbFacade, m_commandService, communicator);

    return communicator;
}