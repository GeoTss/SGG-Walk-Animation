#pragma once

#include "GCM.hpp"
#include "box.h"
#include "TransformComponent.hpp"
#include <functional>

class CollideComponent: public Component, public Box{
private:
    TransformComponent* m_transform;
public:

    CollideComponent(float hitbox_width = 0, float hitbox_height = 0) {
        m_width = hitbox_width;
        m_height = hitbox_height;
    }

    void init() override {
        m_transform = m_owner->addComponent<TransformComponent>();
    }

    void update(float dt) override {
        m_pos_x = m_transform->m_pos_x;
        m_pos_y = m_transform->m_pos_y;
    }
};