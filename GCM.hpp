#pragma once

#include <vector>
#include <array>
#include <iostream>
#include "AnimationHandler.hpp"
#include "State.hpp"
#include "box.h"
#include <list>

#define MAX_COMPONENTS 16
#define MAX_STATES 7

class Component;
class GameObject;
class Manager;

class Component {
public:
    GameObject *m_owner;
    bool m_canInteruptState;

    virtual void init() {}
    virtual void update(float dt) {}
    virtual void draw() {}
    virtual bool stateInteruptions() { return false; }
};

inline u_int16_t getComponentID() {
    static u_int16_t lastID = 0;
    return lastID++;
}

template<typename T> inline u_int16_t getComponentTypeID() noexcept {
    static u_int16_t typeID = getComponentID();
    return typeID;
}

class GameObject {
protected:

    State *m_state;

    std::vector<Component *> m_componentVector;
    std::vector<Component *> m_possibleStateInteruprionComponents;
    std::array<Component *, MAX_COMPONENTS> m_componentArray;
    // std::list<State *> m_stateList; // O(1) insertion-deletion time and we will traverse the whole list every update so traverse will always be O(n);


    u_int16_t m_componentBitset;
public:
    bool m_active;
    Manager* m_manager;

    GameObject() {
        m_active = true;
    }

    bool isActive() const { return m_active; }

    virtual void init() {
        for (Component *&c : m_componentVector)
            c->init();
    }
    virtual void update(float dt) {
        for (Component *&c : m_componentVector)
            c->update(dt);
    }
    virtual void draw() {
        for (Component *&c : m_componentVector)
            c->draw();
    }

    template<typename T>
    bool hasComponent() { return (m_componentBitset >> getComponentTypeID<T>()) & 1; }

    template<typename T>
    T *getComponent() {
        return static_cast<T *>(m_componentArray[getComponentTypeID<T>()]);
    }

    template<typename T, typename... TArgs>
    T *addComponent(TArgs&&... args) {
        if (hasComponent<T>())
            return getComponent<T>();

        T *c_ptr(new T(std::forward<TArgs>(args)...));
        c_ptr->m_owner = this;
        c_ptr->init();
        m_componentBitset = m_componentBitset | (1 << getComponentTypeID<T>());
        m_componentArray[getComponentTypeID<T>()] = c_ptr;
        if (c_ptr->m_canInteruptState)
            m_possibleStateInteruprionComponents.emplace_back(c_ptr);

        m_componentVector.emplace_back(c_ptr);
        return c_ptr;
    }

    virtual bool canUpdateState() { return false; }
    virtual void updateState() {}
};

class Manager {
private:
    std::vector<GameObject *> m_entityList;
public:

    const auto &getEntityList() const { return m_entityList; }

    template<typename T, typename... TArgs>
    T *addGameObject(TArgs&&... args) {
        T *newObj = new T(std::forward<TArgs>(args)...);
        ((GameObject*)newObj)->m_active = true;
        ((GameObject*)newObj)->m_manager = this;
        m_entityList.emplace_back(newObj);
        return newObj;
    }

    void init() {
        for (auto &obj : m_entityList)
            obj->init();
    }

    void update(float dt) {
        for (auto &obj : m_entityList)
            obj->update(dt);
    }

    void refresh() {
        m_entityList.erase(
            std::remove_if(m_entityList.begin(), m_entityList.end(),
                [](const GameObject *obj) { return !obj->isActive(); }
            ),
            m_entityList.end()
        );
    }

    void draw() {
        for (auto &obj : m_entityList)
            obj->draw();
    }
};