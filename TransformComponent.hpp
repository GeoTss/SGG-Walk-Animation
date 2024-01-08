#pragma once

#include "GCM.hpp"
#include "box.h"

class TransformComponent: public Component{
public:
    float m_pos_x, m_pos_y;
    float m_speed;
    int m_direction_x, m_direction_y;
    int m_prev_direction_x = 1;

    // TransformComponent() {}
    // TransformComponent(int direction_x, int direction_y, float speed): m_direction_x{direction_x}, m_direction_y{direction_y}, m_speed{speed} {}

    TransformComponent(float pos_x = 0, float pos_y = 0, float speed = 0, int direction_x = 0, int direction_y = 0): m_pos_x{pos_x}, m_pos_y{pos_y}, m_speed{speed}, m_direction_x{direction_x}, m_direction_y{direction_y} { 
        m_canInteruptState = false;
    }

    void update(float dt) override {
        float delta_time = dt/1000.f;
        if(m_direction_x != 0)
            m_prev_direction_x = m_direction_x;
        m_pos_x += m_direction_x * m_speed * delta_time;
        m_pos_y += m_direction_y * m_speed * delta_time;
    }
};