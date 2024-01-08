#pragma once

#include <sgg/graphics.h>
#include <vector>

class BackgroundManager{
public:
    std::vector<std::string> m_layers;

    BackgroundManager(){
        m_layers = graphics::preloadBitmaps("./assets/Dark-Forest-Bkg/PNG");
        std::sort(m_layers.begin(), m_layers.end());
    }

};