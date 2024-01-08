#pragma once

#include <iostream>
#include "GCM.hpp"
#include "TransformComponent.hpp"
#include "Samurai.hpp"
#include "Dummy.hpp"


class Game {
public:
    Manager m;
    GameObject* m_player;

    graphics::Brush m_bkg_brush;
    graphics::Brush m_hitbox_brush;

    float m_global_offset_x = 0.f;
    float m_global_offset_y = 0.f;

    float m_canvas_width = 8.f;
    float m_canvas_height = 8.f;

    Game() {
        m_bkg_brush.outline_opacity = 0.f;
        m_bkg_brush.texture = "./assets/Dark-Forest-Bkg/Preview/Background.png";

        m_hitbox_brush.outline_color[0] = 255.f;
        m_hitbox_brush.outline_color[1] = 0.f;
        m_hitbox_brush.outline_color[2] = 0.f;
        m_hitbox_brush.fill_opacity = 0;

        m_player = m.addGameObject<Samurai>(1.f, 1.f, 2.f, 2.f);
        m.addGameObject<Dummy>(5.f, 1.f, 2.f, 2.f, 0.3f, 0.5f);
        m.init();

        // std::cout << d->getComponent<TransformComponent>()->m_width << "\n";

    }
};