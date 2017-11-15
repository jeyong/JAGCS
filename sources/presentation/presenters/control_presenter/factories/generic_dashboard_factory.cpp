#include "generic_dashboard_factory.h"

// Internal
#include "service_registry.h"
#include "telemetry_service.h"
#include "telemetry.h"

#include "vehicle.h"
#include "mission_item.h"

#include "dashboard_presenter.h"
#include "map_handle.h"

#include "status_presenter.h"
#include "satellite_presenter.h"
#include "compass_presenter.h"
#include "navigator_presenter.h"
#include "battery_presenter.h"
#include "home_presenter.h"
#include "wind_presenter.h"
#include "mission_instrument_presenter.h"

using namespace presentation;

GenericDashboardFactory::GenericDashboardFactory(MapHandle* handle, const dao::VehiclePtr& vehicle):
    IDashboardFactory(),
    m_handle(handle),
    m_vehicle(vehicle)
{}

DashboardPresenter* GenericDashboardFactory::create()
{
    domain::Telemetry* node = domain::ServiceRegistry::telemetryService()->vehicleNode(
                                  m_vehicle->id());
    if (!node) return nullptr;

    DashboardPresenter* dashboard = new DashboardPresenter(m_vehicle->id());
    m_satellite = new SatellitePresenter(node->childNode(domain::Telemetry::Satellite), dashboard);
    m_status = new StatusPresenter(node->childNode(domain::Telemetry::Status), dashboard);
    m_compass = new CompassPresenter(node->childNode(domain::Telemetry::Compass), dashboard);
    m_navigator = new NavigatorPresenter(node->childNode(domain::Telemetry::Navigator), dashboard);

    dashboard->addInstrument("satellite", 100);
    dashboard->addInstrumentPresenter("satellite", m_satellite);

    dashboard->addInstrument("navigator", 300);
    dashboard->addInstrumentPresenter("navigator", m_status);
    dashboard->addInstrumentPresenter("navigator", m_compass);
    dashboard->addInstrumentPresenter("navigator", m_satellite);
    dashboard->addInstrumentPresenter("navigator", m_navigator);
    dashboard->addInstrumentPresenter("navigator", new HomePresenter(
                                          node->childNode(domain::Telemetry::Position),
                                          node->childNode(domain::Telemetry::HomePosition), dashboard));
    dashboard->addInstrumentPresenter("navigator", new WindPresenter(
                                          node->childNode(domain::Telemetry::Wind), dashboard));

    dashboard->addInstrument("battery", 400);
    dashboard->addInstrumentPresenter("battery", new BatteryPresenter(
                                          node->childNode(domain::Telemetry::Battery), dashboard));

    dashboard->addInstrument("status", 500);
    dashboard->addInstrumentPresenter("status", m_status);

    MissionInstrumentPresenter* mission = new MissionInstrumentPresenter(m_vehicle->id(), dashboard);

    QObject::connect(mission, &MissionInstrumentPresenter::missionItemSelected,
                     m_handle, &MapHandle::selectMissionItem);
    QObject::connect(m_handle, &MapHandle::missionItemSelected,
                     mission, &MissionInstrumentPresenter::selectMissionItem);
    QObject::connect(m_handle, &MapHandle::holded,
                     mission, &MissionInstrumentPresenter::goToItem);

    dashboard->addInstrument("mission", 600);
    dashboard->addInstrumentPresenter("mission", m_status);
    dashboard->addInstrumentPresenter("mission", mission);

    return dashboard;
}
