#include "combat_presenter.h"

// Qt
#include <QMap>
#include <QVariant>

// Internal
#include "domain_entry.h"

#include "db_facade.h"
#include "vehicle_description.h"

#include "telemetry_service.h"

#include "location_map_presenter.h"
#include "vehicle_presenter.h"

using namespace presentation;

class CombatPresenter::Impl
{
public:
    db::DbFacade* dbFacade;
    domain::TelemetryService* telemetryService;

    AbstractMapPresenter* map;
    QMap<int, VehiclePresenter*> vehicles;
};

CombatPresenter::CombatPresenter(domain::DomainEntry* entry, QObject* parent):
    BasePresenter(parent),
    d(new Impl())
{
    d->dbFacade = entry->dbFacade();
    d->telemetryService = entry->telemetryService();

    d->map = new LocationMapPresenter(entry, this);

    connect(d->telemetryService, &domain::TelemetryService::attitudeChanged,
            this, &CombatPresenter::onAttitudeChanged);

    for (const db::VehicleDescriptionPtr& vehicle: d->dbFacade->vehicles())
    {
       this->onVehicleAdded(vehicle);
    }

    connect(d->dbFacade, &db::DbFacade::vehicleAdded, this, &CombatPresenter::onVehicleAdded);
    connect(d->dbFacade, &db::DbFacade::vehicleRemoved, this, &CombatPresenter::onVehicleRemoved);
    connect(d->dbFacade, &db::DbFacade::vehicleChanged, this, &CombatPresenter::onVehicleChanged);
}

CombatPresenter::~CombatPresenter()
{}

void CombatPresenter::updateVehicles()
{
    QList<QObject*> objectList;
    for (VehiclePresenter* vehicle: d->vehicles.values())
    {
        objectList.append(vehicle);
    }

    this->setViewProperty(PROPERTY(vehicles), QVariant::fromValue(objectList));
}

void CombatPresenter::connectView(QObject* view)
{
    d->map->setView(view->findChild<QObject*>(NAME(map)));

    this->updateVehicles();
}

void CombatPresenter::onVehicleAdded(const db::VehicleDescriptionPtr& vehicle)
{
    d->vehicles[vehicle->id()] = new VehiclePresenter(vehicle, this);

    d->vehicles[vehicle->id()]->setAttitude(d->telemetryService->attitude(vehicle->id()));

    this->updateVehicles();
}

void CombatPresenter::onVehicleRemoved(const db::VehicleDescriptionPtr& vehicle)
{
    d->vehicles.remove(vehicle->id());
    this->updateVehicles();
}

void CombatPresenter::onVehicleChanged(const db::VehicleDescriptionPtr& vehicle)
{
    d->vehicles[vehicle->id()]->updateVehicle();
}

void CombatPresenter::onAttitudeChanged(int vehicleId, const domain::Attitude& attitude)
{
    if (d->vehicles.contains(vehicleId)) d->vehicles[vehicleId]->setAttitude(attitude);
}
