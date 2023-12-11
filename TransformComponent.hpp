#pragma once

#include "GCM.hpp"
#include "box.h"

class TransformComponent: public Component, public Box{
public:
    float m_speed;
    int m_direction_x, m_direction_y;

    TransformComponent(float pos_x = 0, float pos_y = 0, float height = 0, float width = 0, float speed = 0, int direction_x = 0, int direction_y = 0): Box(pos_x, pos_y, height, width), m_speed{speed}, m_direction_x{direction_x}, m_direction_y{direction_y} { m_canInteruptState = false; }

    void update() override {
        m_pos_x += m_direction_x * m_speed;
        m_pos_y += m_direction_y * m_speed;
    }
};