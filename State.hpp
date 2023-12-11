#pragma once

class GameObject;

class State{
public:
    GameObject* m_owner;
    bool m_interuptable;
    virtual void handleState() = 0;
};