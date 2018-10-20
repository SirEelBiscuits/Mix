#pragma once

#include "Pool.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <cstdint>

namespace Mix
{

class World;

struct BaseEvent
{
    using Id = uint8_t;
protected:
    static Id nextId;
};

template <typename T>
struct Event : BaseEvent
{
    // Returns the unique id of Event<T>
    static Id getId()
    {
        static auto id = nextId++;
        return id;
    }
};

class EventManager
{
public:
    EventManager(World &world) : world(world) {}

    template <typename T>
    void emitEvent(T event);

    template <typename T, typename ... Args>
    void emitEvent(Args && ... args);

    template <typename T>
    std::vector<T> getEvents();

    void destroyEvents();

private:
    template <typename T>
    Pool<T>* accommodateEvent();

    std::unordered_map<std::type_index, std::unique_ptr<AbstractPool>> eventPools;

    World &world;
};

template <typename T>
void EventManager::emitEvent(T event)
{
    auto eventPool = accommodateEvent<T>();
    eventPool->add(event);
}

template <typename T, typename ... Args>
void EventManager::emitEvent(Args && ... args)
{
    T event(std::forward<Args>(args) ...);
    emitEvent<T>(event);
}

template <typename T>
Pool<T>* EventManager::accommodateEvent()
{
    if (eventPools.find(std::type_index(typeid(T))) == eventPools.end()) {
        auto pool = std::make_unique<Pool<T>>();
        eventPools.insert(std::make_pair(std::type_index(typeid(T)), pool));
    }

    return eventPools[std::type_index(typeid(T))].get();
}

template <typename T>
std::vector<T> EventManager::getEvents()
{
    return static_cast<Pool<T>*>(eventPools[std::type_index(typeid(T))].get())->getData();
}

}
