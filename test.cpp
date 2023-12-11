#include <iostream>
#include "GCM.hpp"
#include "Samurai.hpp"

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1024

class Game{
public:
    Manager m;

    Game(){
        Samurai* s = m.addGameObject<Samurai>();
        m.init();
    }
};

void update(float ms){
    Game* g = (Game*)graphics::getUserData();
    g->m.update();
}

void draw(){
    Game* g = (Game*)graphics::getUserData();
    g->m.draw();
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