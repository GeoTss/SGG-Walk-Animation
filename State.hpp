#pragma once

class GameObject;

inline u_int16_t getStateID() {
    static u_int16_t lastID = 0;
    return lastID++;
}

template<typename T> inline u_int16_t getStateTypeID() noexcept {
    static u_int16_t typeID = getStateID();
    return typeID;
}

class State{
public:
    GameObject* m_owner;
    bool m_interuptable, m_active;

    virtual void init() {}
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void update() {}
    virtual void handleState() = 0;
};