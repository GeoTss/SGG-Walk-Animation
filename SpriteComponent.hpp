#pragma once

#include "GCM.hpp"
#include "AnimationHandler.hpp"
#include "TransformComponent.hpp"
#include <sgg/graphics.h>
#include <iostream>

class SpriteComponent: public Component{
private:
    graphics::Brush m_br;
    TransformComponent* m_transform;
public:
    AnimationHandler* m_animationHandler;

    SpriteComponent() { m_canInteruptState = true; }

    bool isInAnimation() const { return m_animationHandler->isInAnimation(); }
    AnimationHandler::m_AnimationType getAnimationType() const { return m_animationHandler->getType(); }

    void init() override {
        if(!m_owner->hasComponent<TransformComponent>())
            m_owner->addComponent<TransformComponent>(new TransformComponent);
        m_transform = m_owner->getComponent<TransformComponent>();
    }

    void update() override {
        if(!m_animationHandler->isInAnimation())
            m_animationHandler->playAnimation(m_br);
    }

    void draw() override {
        graphics::drawRect(m_transform->m_pos_x, m_transform->m_pos_y, m_transform->m_width, m_transform->m_height, m_br);
    }

    bool stateInteruptions() override {
        if(isInAnimation() && !m_animationHandler->isCancelable())
            return true;
        if(!m_animationHandler->isCancelable() && !m_animationHandler->animationCycleEnded())
            return true;
        return false;
    }
};