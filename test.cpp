#include <iostream>
#include "GCM.hpp"
#include "TransformComponent.hpp"
#include "Samurai.hpp"
#include "Dummy.hpp"
#include "Game.hpp"

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280

void checkCollisions(const std::vector<GameObject*>& e_list) {
    for (int i{}; i < e_list.size(); ++i) {
        for (int j{i+1}; j < e_list.size(); ++j) {
            if (e_list[i]->getComponent<CollideComponent>()->intersect(e_list[j]->getComponent<CollideComponent>()))
                std::cout << "Intersection " << "\n";
            else
                std::cout << "Not intersecting\n";

        }
    }
}


void update(float ms) {
    Game *g = (Game *)graphics::getUserData();
    // checkCollisions(g->m.getEntityList());
    g->m.refresh();
    g->m.update(ms);

    TransformComponent* m_player_pos = g->m_player->getComponent<TransformComponent>();

    g->m_global_offset_x = g->m_canvas_width / 2.0f - m_player_pos->m_pos_x/4.f;
    g->m_global_offset_y = g->m_canvas_height / 2.0f - m_player_pos->m_pos_y/4.f;
}

inline void drawBackground(Game*& _g) {
    graphics::drawRect(_g->m_canvas_width / 2.0f + _g->m_global_offset_x,
        _g->m_canvas_height / 2.0f + _g->m_global_offset_y - 11.9f,
        25.0f, 20.f,
        _g->m_bkg_brush);
}

void draw() {
    Game *g = (Game *)graphics::getUserData();
    drawBackground(g);
    // graphics::drawRect(g->m_canvas_width/2, g->m_canvas_height/2, 2, 2, g->m_player->getComponent<SpriteComponent>()->m_br);
    // graphics::drawRect(g->m_canvas_width/2, g->m_canvas_height/2, 0.3, 0.5, g->m_hitbox_brush);
    g->m.draw();
}

int main() {

    graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MyWindow");

    Game game;

    graphics::setUserData(&game);
    graphics::setUpdateFunction(&update);
    graphics::setDrawFunction(&draw);

    graphics::setCanvasSize(16, 5);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    graphics::startMessageLoop();
    graphics::stopMessageLoop();


    return 0;
}