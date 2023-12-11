#include <sgg/graphics.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1024

class AnimationHandler {
public:
    enum class m_AnimationType { SINGLE, CONTINUOUS };
protected:
    m_AnimationType m_type;
    std::vector<std::string> m_animations;
    int m_animation_count;
    int m_currAnimation = 0;
    bool m_inAnimation = false;
    float m_animationStart = 0;
    float m_animationDelay;
    bool m_cancelableAnimation;

public:

    AnimationHandler(const char *path, float delay, bool cancelable) {
        m_animations = graphics::preloadBitmaps(path);
        m_animation_count = m_animations.size();
        m_animationDelay = delay;

        if (cancelable)
            m_type = m_AnimationType::SINGLE;
        else
            m_type = m_AnimationType::CONTINUOUS;

        m_cancelableAnimation = cancelable;

        std::sort(m_animations.begin(), m_animations.end());
        for (auto &str : m_animations)
            std::cout << "Loaded animation: " << str << "\n";
    };

    m_AnimationType getType() { return m_type; }
    bool isCancelable() { return m_cancelableAnimation; }

    inline bool isInAnimation() { return m_inAnimation = (graphics::getGlobalTime() - m_animationStart) < m_animationDelay; }

    void playAnimation(graphics::Brush &br) {
        m_animationStart = graphics::getGlobalTime();
        m_currAnimation = ++m_currAnimation % m_animation_count;
        br.texture = m_animations[m_currAnimation];
    }
    bool animationCycleEnded() { return m_currAnimation == 0; }

    void reset() {
        m_currAnimation = 0;
        m_inAnimation = false;
    }
};

class CollisionBehaviour;

class Entity {
public:
    enum class m_StateEnum { IDLE, RUNNING, ATTACKING_LIGHT, ATTACKING_HEAVY, GETTING_HIT, JUMPING, FALLING, DEAD };
    bool m_ApplyStateEffects;

    graphics::Brush m_br;
    float m_center_x = 50.f;
    float m_center_y = 100.f;
    int m_direction = 0;
    float m_speed = 2.f;
    float m_health = 400.f;
    float m_lightAttackDamage = 130.f;
    float m_heavyAttackDamage = 260.f;
    m_StateEnum m_state;

    AnimationHandler *m_currentAnimation;
    CollisionBehaviour *m_col;

    virtual void update() {}
    virtual void draw() {}
};

class CollisionBehaviour {
public:
    Entity *m_owner;
    virtual void onCollision(Entity *&) { std::cout << "Default collision behaviour\n"; };
    virtual bool collides(Entity *&e) { return ((m_owner->m_center_x < e->m_center_x + 50) && (m_owner->m_center_x + 50 > e->m_center_x)); }
};

class LightAttackCollisionBehaviour : public CollisionBehaviour {
public:
    bool collides(Entity *&e) override {
        return ((m_owner->m_center_x < e->m_center_x + 100) && (m_owner->m_center_x + 100 > e->m_center_x));
    }

    void onCollision(Entity *&e) override {
        std::cout << "Light attack collision behaviour\n";
        if (m_owner->m_ApplyStateEffects) {
            e->m_health = e->m_health - m_owner->m_lightAttackDamage;
            e->m_state = Entity::m_StateEnum::GETTING_HIT;
        }
        std::cout << e->m_health << "\n";
    }
};

class HeavyAttackCollisionBehaviour : public CollisionBehaviour {
public:
    bool collides(Entity *&e) override {
        return ((m_owner->m_center_x < e->m_center_x + 100) && (m_owner->m_center_x + 100 > e->m_center_x));
    }

    void onCollision(Entity *&e) override {
        std::cout << "Heavy attack collision behaviour\n";
        if (m_owner->m_ApplyStateEffects) {
            e->m_health = e->m_health - m_owner->m_heavyAttackDamage;
            e->m_state = Entity::m_StateEnum::GETTING_HIT;
        }
        std::cout << e->m_health << "\n";
    }
};


class Manager {
public:
    std::vector<Entity *> m_entities;

    template<typename T>
    void addEntity() {
        m_entities.push_back(new T);
    }

    void update() {
        for (auto &e : m_entities)
            e->update();
    }

    void draw() {
        for (auto &e : m_entities)
            e->draw();
    }

    void checkCollisions() {
        for (int i{}; i < m_entities.size(); ++i) {
            for (int j{ i + 1 }; j < m_entities.size(); ++j) {
                if (m_entities[i]->m_col->collides(m_entities[j])) {
                    m_entities[i]->m_col->onCollision(m_entities[j]);
                    std::cout << "Collided\n";
                }
            }
        }
    }
};

class Samurai : public Entity {
private:
    enum class m_StateEnum { IDLE, RUNNING, ATTACKING_LIGHT, ATTACKING_HEAVY, GETTING_HIT, JUMPING, DEAD };
    m_StateEnum m_state;

    AnimationHandler m_idleAnimations;
    AnimationHandler m_runningAnimations;
    AnimationHandler m_lightAttackAnimations;
    AnimationHandler m_heavyAttackAnimations;
    AnimationHandler m_gotHitAnimations;

    CollisionBehaviour m_defaultCollision;
    LightAttackCollisionBehaviour m_lightAttackBehaviour;
    HeavyAttackCollisionBehaviour m_heavyAttackBehaviour;

public:
    Samurai() :
        m_idleAnimations("./Martial-Hero/IdleSprites", 200.f, true),
        m_runningAnimations("./Martial-Hero/RunSprites", 100.f, true),
        m_lightAttackAnimations("./Martial-Hero/LightAttackSprites", 50.f, false),
        m_heavyAttackAnimations("./Martial-Hero/HeavyAttackSprites", 80.f, false),
        m_gotHitAnimations("./Martial-Hero/GotHitSprites", 100.f, false),
        m_defaultCollision(),
        m_lightAttackBehaviour(),
        m_heavyAttackBehaviour() {

        m_state = m_StateEnum::IDLE;
        m_currentAnimation = &m_idleAnimations;
        m_ApplyStateEffects = true;
        // m_br.outline_color[0] = 0.f;
        // m_br.outline_color[1] = 0.f;
        // m_br.outline_color[2] = 0.f;

        m_col = &m_defaultCollision;
        m_defaultCollision.m_owner = this;
        m_lightAttackBehaviour.m_owner = this;
        m_heavyAttackBehaviour.m_owner = this;

        graphics::setScale(1, 4);
    }

    void update() override {
        if (m_currentAnimation->isInAnimation() && !m_currentAnimation->isCancelable()) {
            m_ApplyStateEffects = false;
            return;
        }
        if (m_currentAnimation->getType() == AnimationHandler::m_AnimationType::CONTINUOUS && !m_currentAnimation->animationCycleEnded()) {
            m_ApplyStateEffects = false;
            m_currentAnimation->playAnimation(m_br);
            return;
        }
        m_state = m_StateEnum::IDLE;
        if (graphics::getKeyState(graphics::SCANCODE_D))
            m_state = m_StateEnum::RUNNING;
        else if (graphics::getKeyState(graphics::SCANCODE_A))
            m_state = m_StateEnum::ATTACKING_LIGHT;
        else if (graphics::getKeyState(graphics::SCANCODE_H))
            m_state = m_StateEnum::ATTACKING_HEAVY;
        else if (graphics::getKeyState(graphics::SCANCODE_G))
            m_state = m_StateEnum::GETTING_HIT;

        AnimationHandler *prevAnimation = m_currentAnimation;
        switch (m_state) {
        case m_StateEnum::IDLE:
            m_direction = 0;
            m_currentAnimation = &m_idleAnimations;
            m_col = &m_defaultCollision;
            m_ApplyStateEffects = true;
            break;
        case m_StateEnum::RUNNING:
            m_direction = 1;
            m_currentAnimation = &m_runningAnimations;
            break;
        case m_StateEnum::ATTACKING_LIGHT:
            m_direction = 0.5;
            m_currentAnimation = &m_lightAttackAnimations;
            m_col = &m_lightAttackBehaviour;
            break;
        case m_StateEnum::ATTACKING_HEAVY:
            m_currentAnimation = &m_heavyAttackAnimations;
            m_col = &m_heavyAttackBehaviour;
            break;
        case m_StateEnum::GETTING_HIT:
            m_direction = -1;
            m_currentAnimation = &m_gotHitAnimations;
            break;
        default:
            break;
        }
        m_center_x += m_direction * m_speed;

        if (prevAnimation != m_currentAnimation)
            m_currentAnimation->playAnimation(m_br);
        else if (!prevAnimation->isInAnimation())
            prevAnimation->playAnimation(m_br);
    }

    void draw() override {
        graphics::drawRect(m_center_x, m_center_y, 200, 200, m_br);
    }

};

class Dummy : public Entity {
public:
    CollisionBehaviour m_defaultCollision;
    AnimationHandler m_idleAnimations;
    AnimationHandler m_gotHitAnimations;

    Dummy() : m_idleAnimations("./Martial-Hero/IdleSprites", 200.f, true), m_gotHitAnimations("./Martial-Hero/GotHitSprites", 100.f, false) {
        m_center_x = 300.f;
        m_state = m_StateEnum::IDLE;
        m_currentAnimation = &m_idleAnimations;
        m_defaultCollision.m_owner = this;
        m_col = &m_defaultCollision;

    }

    void update() override {
        if (m_currentAnimation->isInAnimation() && !m_currentAnimation->isCancelable()) {
            m_ApplyStateEffects = false;
            return;
        }
        if (m_currentAnimation->getType() == AnimationHandler::m_AnimationType::CONTINUOUS && !m_currentAnimation->animationCycleEnded()) {
            m_ApplyStateEffects = false;
            m_currentAnimation->playAnimation(m_br);
            return;
        }

        AnimationHandler *prevAnimation = m_currentAnimation;
        switch (m_state) {
        case m_StateEnum::IDLE:
            m_currentAnimation = &m_idleAnimations;
            break;
        case m_StateEnum::GETTING_HIT:
            m_currentAnimation = &m_gotHitAnimations;
            break;
        default:
            break;
        }

        m_state = m_StateEnum::IDLE;

        if (prevAnimation != m_currentAnimation)
            m_currentAnimation->playAnimation(m_br);
        else if (!prevAnimation->isInAnimation())
            prevAnimation->playAnimation(m_br);
    }

    void draw() override {
        graphics::drawRect(m_center_x, m_center_y, 200, 200, m_br);
    }
};

struct Game {
    Manager m_manager;

    Game() {
        m_manager.addEntity<Samurai>();
        // m_manager.addEntity<Dummy>();
    }
};

void update(float ms) {
    Game *g = (Game *)graphics::getUserData();
    g->m_manager.update();
    g->m_manager.checkCollisions();
}

void draw() {
    Game *g = (Game *)graphics::getUserData();
    g->m_manager.draw();
}

int main() {

    graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MyWindow");

    Game game;
    graphics::setUserData(&game);
    graphics::setUpdateFunction(&update);
    graphics::setDrawFunction(&draw);

    graphics::startMessageLoop();
    graphics::stopMessageLoop();

    return 0;
}