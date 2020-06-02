#ifndef EVENTS_H
#define EVENTS_H

#include <EventClasses/event.h>

#include <EventClasses/Object/Movement/moveforwardevent.h>
#include <EventClasses/Object/Movement/movebackwardevent.h>
#include <EventClasses/Object/Movement/moverightevent.h>
#include <EventClasses/Object/Movement/moveleftevent.h>

#include <EventClasses/Object/Movement/stopforwardevent.h>
#include <EventClasses/Object/Movement/stopbackwardevent.h>
#include <EventClasses/Object/Movement/stoprightevent.h>
#include <EventClasses/Object/Movement/stopleftevent.h>
#include <EventClasses/UpdateEvent.h>
#include <EventClasses/Server/DeleteEvent.h>
#include <EventClasses/Server/NotifyEvent.h>

#include <EventClasses/Object/RotateEvent.h>
#include <EventClasses/GameState/place.h>
#include <EventClasses/GameState/pickup.h>
#include <EventClasses/GameState/join.h>

#include <EventClasses/GameState/ready.h>
#include <EventClasses/GameState/use.h>
#endif