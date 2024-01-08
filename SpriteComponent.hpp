#pragma once

#include "GCM.hpp"
#include "AnimationHandler.hpp"
#include "TransformComponent.hpp"
#include "CollideComponent.hpp"
#include <sgg/graphics.h>
#include <iostream>

#define SHOW_HITBOX

class MovingSpriteComponent: public Component{
private:
    TransformComponent* m_transform;
    #ifdef SHOW_HITBOX
    CollideComponent* m_hitbox;
    graphics::Brush m_hitboxBrush;
    #endif
    
    int m_height, m_width;
public:
    graphics::Brush m_br;
    AnimationHandler* m_animationHandler;

    MovingSpriteComponent(int height, int width): m_height{height}, m_width{width} {
        #ifdef SHOW_HITBOX
        m_hitboxBrush.outline_color[0] = 255.f;
        m_hitboxBrush.outline_color[1] = 0.f;
        m_hitboxBrush.outline_color[2] = 0.f;
        m_hitboxBrush.fill_opacity = 0.f;
        #endif
        m_canInteruptState = true; 
    }

    bool isInAnimation() const { return m_animationHandler->isInAnimation(); }
    AnimationHandler::m_AnimationType getAnimationType() const { return m_animationHandler->getType(); }

    void init() override {
        // if(!m_owner->hasComponent<TransformComponent>())
        //     m_owner->addComponent<TransformComponent>();
        m_transform = m_owner->addComponent<TransformComponent>();
        #ifdef SHOW_HITBOX
        m_hitbox = m_owner->getComponent<CollideComponent>();
        #endif
    }

    void update(float dt) override {
        if(!m_animationHandler->isInAnimation())
            m_animationHandler->playAnimation(m_br);
    }

    void draw() override {
        graphics::setScale(m_transform->m_prev_direction_x, 1);
        graphics::drawRect(m_transform->m_pos_x, m_transform->m_pos_y, 
                        m_width, m_height,
                        m_br);
        graphics::resetPose();
        #ifdef SHOW_HITBOX
        graphics::drawRect(m_hitbox->m_pos_x, m_hitbox->m_pos_y, m_hitbox->m_width, m_hitbox->m_height, m_hitboxBrush);
        #endif
    }

    bool stateInteruptions() override {
        if(isInAnimation() && !m_animationHandler->isCancelable())
            return true;
        if(!m_animationHandler->isCancelable() && !m_animationHandler->animationCycleEnded())
            return true;
        return false;
    }
};

class StationarySpriteComponent: public Component{
private:
    float m_pos_x, m_pos_y;

    #ifdef SHOW_HITBOX
    CollideComponent* m_hitbox;
    graphics::Brush m_hitboxBrush;
    TransformComponent* m_transform;
    #endif
    
    int m_height, m_width;
public:
    graphics::Brush m_br;
    AnimationHandler* m_animationHandler;

    StationarySpriteComponent(int height, int width, float pos_x, float pos_y): m_height{height}, m_width{width}, m_pos_x{pos_x}, m_pos_y{pos_y} {
        #ifdef SHOW_HITBOX
        m_hitboxBrush.outline_color[0] = 255.f;
        m_hitboxBrush.outline_color[1] = 0.f;
        m_hitboxBrush.outline_color[2] = 0.f;
        m_hitboxBrush.fill_opacity = 0.f;
        #endif
        m_canInteruptState = true; 
    }

    bool isInAnimation() const { return m_animationHandler->isInAnimation(); }
    AnimationHandler::m_AnimationType getAnimationType() const { return m_animationHandler->getType(); }

    void init() override {
        // if(!m_owner->hasComponent<TransformComponent>())
        //     m_owner->addComponent<TransformComponent>();
        m_transform = m_owner->getComponent<TransformComponent>();
        #ifdef SHOW_HITBOX
        m_hitbox = m_owner->getComponent<CollideComponent>();
        #endif
    }

    void update(float dt) override {
        if(!m_animationHandler->isInAnimation())
            m_animationHandler->playAnimation(m_br);
    }

    void draw() override {
        graphics::setScale(m_transform->m_prev_direction_x, 1);
        graphics::drawRect(m_pos_x, m_pos_y, 
                        m_width, m_height, 
                        m_br);
        graphics::resetPose();
        #ifdef SHOW_HITBOX
        graphics::drawRect(m_pos_x, m_pos_y, m_hitbox->m_width, m_hitbox->m_height, m_hitboxBrush);
        #endif
    }

    bool stateInteruptions() override {
        if(isInAnimation() && !m_animationHandler->isCancelable())
            return true;
        if(!m_animationHandler->isCancelable() && !m_animationHandler->animationCycleEnded())
            return true;
        return false;
    }
};