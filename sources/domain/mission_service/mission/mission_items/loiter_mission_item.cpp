#include "loiter_mission_item.h"

using namespace domain;

LoiterMissionItem::LoiterMissionItem(Mission* mission):
    PositionMissionItem(mission, Loiter, true)
{}

float LoiterMissionItem::radius() const
{
    return m_radius;
}

void LoiterMissionItem::setRadius(float radius)
{
    if (m_radius == radius) return;

    m_radius = radius;
    emit radiusChanged(radius);
}