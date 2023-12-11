#pragma once

#include "GCM.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "State.hpp"

class IdleState : public State {
public:
    AnimationHandler m_idleAnimations;

    IdleState() : m_idleAnimations("./Martial-Hero/IdleSprites", 200.f, true) { m_interuptable = true; }

    void handleState() override {
        m_owner->getComponent<TransformComponent>()->m_direction_x = m_owner->getComponent<TransformComponent>()->m_direction_y = 0;
        m_owner->getComponent<SpriteComponent>()->m_animationHandler = &m_idleAnimations;
    }

};

class RunningState : public State {
public:
    AnimationHandler m_runningAnimations;

    RunningState() : m_runningAnimations("./Martial-Hero/RunSprites", 100.f, true) { m_interuptable = true; }

    void handleState() override {
        m_owner->getComponent<SpriteComponent>()->m_animationHandler = &m_runningAnimations;
    }
};

class LightAttackState : public State {
public:
    AnimationHandler m_lightAttackAnimations;

    LightAttackState() : m_lightAttackAnimations("./Martial-Hero/LightAttackSprites", 50.f, false) { m_interuptable = false; }

    void handleState() override {
        m_owner->getComponent<SpriteComponent>()->m_animationHandler = &m_lightAttackAnimations;
    }
};

class HeavyAttackState : public State {
public:
    AnimationHandler m_heavyAttackAnimations;

    HeavyAttackState() : m_heavyAttackAnimations("./Martial-Hero/HeavyAttackSprites", 100.f, false) { m_interuptable = false; }

    void handleState() override {
        m_owner->getComponent<SpriteComponent>()->m_animationHandler = &m_heavyAttackAnimations;
    }
};

class GotHitState : public State {
public:
    AnimationHandler m_gotHitAnimations;
    bool m_alreadyHit = false;
    
    GotHitState() : m_gotHitAnimations("./Martial-Hero/GotHitSprites", 100.f, false) { m_interuptable = false; }

    void handleState() override {
        m_owner->getComponent<SpriteComponent>()->m_animationHandler = &m_gotHitAnimations;
        m_alreadyHit = true;
    }
};

class Samurai : public GameObject {
public:

    IdleState m_idleState;
    RunningState m_runningState;
    LightAttackState m_lightAttackState;
    HeavyAttackState m_heavyAttackState;
    GotHitState m_gotHitState;

    Samurai(): m_idleState(), m_runningState(), m_lightAttackState(), m_heavyAttackState(), m_gotHitState() {
        addComponent<TransformComponent>(new TransformComponent(200.f, 200.f, 200.f, 200.f, 0.4f, 0));
        addComponent<SpriteComponent>(new SpriteComponent());
        std::cout << m_possibleStateInteruprionComponents.size() << "\n";

        m_idleState.m_owner = m_runningState.m_owner = m_lightAttackState.m_owner = m_heavyAttackState.m_owner = m_gotHitState.m_owner = this;
        m_state = &m_idleState;
        m_state->handleState();
    }

    void init() override {
        for (Component *&c : m_componentVector)
            c->init();
        m_state = &m_idleState;
    }

    bool canUpdateState() override{
        for(auto& c: m_possibleStateInteruprionComponents)
            if(c->stateInteruptions())
                return false;
        return true;
    }
    
    void updateState() override {
        
        m_state = &m_idleState;
        if (graphics::getKeyState(graphics::SCANCODE_D)) {
            m_state = &m_runningState;
            getComponent<TransformComponent>()->m_direction_x = 1;
        }
        else if (graphics::getKeyState(graphics::SCANCODE_A)) {
            m_state = &m_runningState;
            getComponent<TransformComponent>()->m_direction_x = -1;
        }
        else if (graphics::getKeyState(graphics::SCANCODE_L)){
            m_state = &m_lightAttackState;
        }
        else if (graphics::getKeyState(graphics::SCANCODE_H)){
            m_state = &m_heavyAttackState;
        }
        else if (graphics::getKeyState(graphics::SCANCODE_G)){
            m_state = &m_gotHitState;
        }

        m_state->handleState();
    }

    void update() override {
        if(canUpdateState())
            updateState();
        for (Component *&c : m_componentVector)
            c->update();
    }
};