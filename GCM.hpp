#pragma once

#include <vector>
#include <array>
#include <iostream>
#include "AnimationHandler.hpp"
#include "State.hpp"

#define MAX_COMPONENTS 16

class Component;
class GameObject;

class Component {
public:
    GameObject *m_owner;
    bool m_canInteruptState;

    virtual void init() {}
    virtual void update() {}
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

class GameObject: public Box {
protected:
    State *m_state;
    bool m_active;

    std::vector<Component *> m_componentVector;
    std::vector<Component*> m_possibleStateInteruprionComponents;
    std::array<Component *, MAX_COMPONENTS> m_componentArray;

    u_int16_t m_componentBitset;
public:

    GameObject() {
        m_active = true;
    }

    void setState(State *state) { m_state = state; }

    bool isActive() const { return m_active; }

    virtual void init() {
        for (Component *&c : m_componentVector)
            c->init();
    }
    virtual void update() {
        for (Component *&c : m_componentVector)
            c->update();
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

    template<typename T>
    void addComponent(T *&&c_ptr) {
        if (hasComponent<T>())
            return;
        c_ptr->m_owner = this;
        c_ptr->init();
        m_componentBitset = m_componentBitset | (1 << getComponentTypeID<T>());
        m_componentArray[getComponentTypeID<T>()] = c_ptr;
        if(c_ptr->m_canInteruptState)
            m_possibleStateInteruprionComponents.emplace_back(c_ptr);
        
        m_componentVector.emplace_back(std::move(c_ptr));
    }

    virtual bool canUpdateState() { return false; }
    virtual void updateState() {}
};

class Manager {
private:
    std::vector<GameObject *> m_entityList;
public:

    template<typename T>
    T *addGameObject() {
        T *newObj = new T;
        m_entityList.emplace_back(newObj);
        return newObj;
    }

    void init() {
        for (auto &obj : m_entityList)
            obj->init();
    }

    void update() {
        for (auto &obj : m_entityList)
            obj->update();
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