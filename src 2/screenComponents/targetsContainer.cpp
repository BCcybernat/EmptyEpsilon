#include "targetsContainer.h"
#include "playerInfo.h"
#include "spaceObjects/playerSpaceship.h"

TargetsContainer::TargetsContainer()
{
    route_selection_index = -1;
    waypoint_selection_index = -1;
    allow_waypoint_selection = false;
}

void TargetsContainer::clear()
{
    waypoint_selection_index = -1;
    entries.clear();
}

void TargetsContainer::add(P<SpaceObject> obj)
{
    if (obj && !entries.has(obj))
        entries.push_back(obj);
}

void TargetsContainer::set(P<SpaceObject> obj)
{
    if (obj)
    {
        if (entries.size() > 0)
        {
            entries[0] = obj;
            if (entries.size() > 1)
                entries.resize(1);
        }else{
            entries.push_back(obj);
        }
    }
    else
    {
        clear();
    }
    waypoint_selection_index = -1;
}

void TargetsContainer::set(PVector<SpaceObject> objs)
{
    waypoint_selection_index = -1;
    entries = objs;
}

void TargetsContainer::setToClosestTo(sf::Vector2f position, float max_range, ESelectionType selection_type)
{
    P<SpaceObject> target;
    PVector<Collisionable> list = CollisionManager::queryArea(position - sf::Vector2f(max_range, max_range), position + sf::Vector2f(max_range, max_range));
    foreach(Collisionable, obj, list)
    {
        P<SpaceObject> spaceObject = obj;
        if (spaceObject && spaceObject != my_spaceship)
        {
            switch(selection_type)
            {
            case Selectable:
                if (!spaceObject->canBeSelectedBy(my_spaceship))
                    continue;
                break;
            case Targetable:
                if (!spaceObject->canBeTargetedBy(my_spaceship))
                    continue;
                break;
            }
            if (!target || sf::length(position - spaceObject->getPosition()) < sf::length(position - target->getPosition()))
                target = spaceObject;
        }
    }

    if (my_spaceship && allow_waypoint_selection)
    {
        for(int r = 0; r < PlayerSpaceship::max_routes; r++)
        {
            for(int n=0; n<PlayerSpaceship::max_waypoints_in_route; n++)
            {
                sf::Vector2f wp = my_spaceship->waypoints[r][n];
                if (wp < empty_waypoint && (wp - position) < max_range)
                {
                    if (!target || sf::length(position - wp) < sf::length(position - target->getPosition()))
                    {
                        clear();
                        route_selection_index = r;
                        waypoint_selection_index = n;
                        waypoint_selection_position = wp;
                        return;
                    }
                }
            }
        }
    }
    set(target);
}

int TargetsContainer::getWaypointIndex()
{
    if (!my_spaceship || waypoint_selection_index < 0)
        waypoint_selection_index = -1;
    else
    {
        if (waypoint_selection_index >= PlayerSpaceship::max_waypoints_in_route)
            waypoint_selection_index = -1;
        else if (my_spaceship->waypoints[route_selection_index][waypoint_selection_index] >= empty_waypoint)
            waypoint_selection_index = -1;
        else if (my_spaceship->waypoints[route_selection_index][waypoint_selection_index] != waypoint_selection_position)
            waypoint_selection_index = -1;
    }
    return waypoint_selection_index;
}

int TargetsContainer::getRouteIndex(){
    return route_selection_index;
}

void TargetsContainer::setRouteIndex(int index){
    if (index < PlayerSpaceship::max_routes && index >= -1){
        route_selection_index = index;
    }
    waypoint_selection_index = -1;
}

void TargetsContainer::setWaypointIndex(int index)
{
    waypoint_selection_index = index;
    if (my_spaceship && index >= 0 && index < PlayerSpaceship::max_waypoints_in_route && my_spaceship->waypoints[route_selection_index][index] < empty_waypoint)
        waypoint_selection_position = my_spaceship->waypoints[route_selection_index][index];
}

sf::Vector2f TargetsContainer::getWaypointPosition()
{
    if (my_spaceship && waypoint_selection_index >= 0)
        return waypoint_selection_position;
    else 
        return sf::Vector2f();
}
