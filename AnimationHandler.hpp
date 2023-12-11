#pragma once

#include <sgg/graphics.h>

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