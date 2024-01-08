#pragma once

#include "GCM.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "CollideComponent.hpp"
#include "State.hpp"

struct SamuraiBaseInfo{
    constexpr static const float m_width = 0.3;
    constexpr static const float m_height = 0.5;
    constexpr static const float m_speed = 25;
    constexpr static const float m_lightAttackHorRange = 1.2f;
    static const int m_lightAttackVerRange = 0;
    constexpr static const float m_heavyAttackHorRange = 1.5f;
    constexpr static const float m_heavyAttackVerRange = 0.3f;
    static const int m_lightAttackDamage = 100;
    static const int m_heavyAttackDamage = 200;
};

class IdleState : public State {
public:
    AnimationHandler m_idleAnimations;
    TransformComponent* m_transform;
    StationarySpriteComponent* m_sprite;

    IdleState() : m_idleAnimations("./assets/Martial-Hero/IdleSprites", 200.f, true) { m_interuptable = true; }

    void init() override {
        m_transform = m_owner->getComponent<TransformComponent>();
        m_sprite = m_owner->getComponent<StationarySpriteComponent>();
    }

    void handleState() override {
        m_transform->m_direction_x = m_transform->m_direction_y = 0;
        m_sprite->m_animationHandler = &m_idleAnimations;
    }

};

class RunningState : public State {
public:
    AnimationHandler m_runningAnimations;
    StationarySpriteComponent* m_sprite;

    RunningState() : m_runningAnimations("./assets/Martial-Hero/RunSprites", 100.f, true) { m_interuptable = true; }

    void init() override {
        m_sprite = m_owner->getComponent<StationarySpriteComponent>();
    }

    void handleState() override {
        m_sprite->m_animationHandler = &m_runningAnimations;
    }
};

class LightAttackState : public State {
public:
    AnimationHandler m_lightAttackAnimations;
    TransformComponent* m_transform;
    StationarySpriteComponent* m_sprite;

    LightAttackState() : m_lightAttackAnimations("./assets/Martial-Hero/LightAttackSprites", 50.f, false) { m_interuptable = false; }

    void init() override {
        m_transform = m_owner->getComponent<TransformComponent>();
        m_sprite = m_owner->getComponent<StationarySpriteComponent>();
    }

    void handleState() override {
        m_sprite->m_animationHandler = &m_lightAttackAnimations;
        m_transform->m_direction_x = 0;
    }
};

class HeavyAttackState : public State {
public:
    AnimationHandler m_heavyAttackAnimations;
    TransformComponent* m_transform;
    StationarySpriteComponent* m_sprite;

    HeavyAttackState() : m_heavyAttackAnimations("./assets/Martial-Hero/HeavyAttackSprites", 100.f, false) { m_interuptable = false; }

    void init() override {
        m_transform = m_owner->getComponent<TransformComponent>();
        m_sprite = m_owner->getComponent<StationarySpriteComponent>();
    }

    void handleState() override {
        m_sprite->m_animationHandler = &m_heavyAttackAnimations;
        m_transform->m_direction_x = 0;
    }
};

class GotHitState : public State {
public:
    AnimationHandler m_gotHitAnimations;
    StationarySpriteComponent* m_sprite;
    bool m_alreadyHit = false;
    
    GotHitState() : m_gotHitAnimations("./assets/Martial-Hero/GotHitSprites", 100.f, false) { m_interuptable = false; }

    void init() override {
        m_sprite = m_owner->getComponent<StationarySpriteComponent>();
    }

    void handleState() override {
        m_sprite->m_animationHandler = &m_gotHitAnimations;
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

    Samurai(float pos_x = 0, float pos_y = 0, float sprite_width = 0, float sprite_height = 0): m_idleState(), m_runningState(), m_lightAttackState(), m_heavyAttackState(), m_gotHitState() {
        addComponent<TransformComponent>(pos_x, pos_y, SamuraiBaseInfo::m_speed, 0, 0);
        addComponent<StationarySpriteComponent>(sprite_height, sprite_width, 4.f, 4.f);
        addComponent<CollideComponent>(SamuraiBaseInfo::m_width, SamuraiBaseInfo::m_height);
        std::cout << m_possibleStateInteruprionComponents.size() << "\n";
        
        m_idleState.m_owner = m_runningState.m_owner = m_lightAttackState.m_owner = m_heavyAttackState.m_owner = m_gotHitState.m_owner = this;
        m_idleState.init();
        m_runningState.init();
        m_lightAttackState.init();
        m_heavyAttackState.init();
        m_gotHitState.init();

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
        State* prevState = m_state;
        
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
        // if(prevState != m_state)
            m_state->handleState();
    }

    void update(float dt) override {
        if(canUpdateState())
            updateState();
        for (Component *&c : m_componentVector)
            c->update(dt);

        TransformComponent* t = getComponent<TransformComponent>();
        if(t->m_direction_x != 0)
            std::cout <<  t->m_direction_x * t->m_speed * (dt/1000.f) << "\n";
        // std::cout << getComponent<TransformComponent>()->m_direction_x << "\n";
        // std::cout << getComponent<TransformComponent>()->m_pos_x << ", " << getComponent<TransformComponent>()->m_pos_y << "\n";
    }
};