#pragma once

#include "GCM.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"

class Dummy: public GameObject{
public:
    Dummy(float pos_x = 0, float pos_y = 0, float sprite_width = 0, float sprite_height = 0, float hitbox_width = 0, float hitbox_height = 0) {
        addComponent<TransformComponent>(pos_x, pos_y, 2, 1, 0);
        addComponent<MovingSpriteComponent>(sprite_height, sprite_width);
        getComponent<MovingSpriteComponent>()->m_animationHandler = new AnimationHandler("./assets/Martial-Hero/IdleSprites", 200.f, false);
        addComponent<CollideComponent>(hitbox_width, hitbox_height);
    }
};