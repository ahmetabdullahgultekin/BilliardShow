#ifndef BILLIARDSHOW_MINIMAP_H
#define BILLIARDSHOW_MINIMAP_H

#pragma once

#include "../Renderer/Renderer.h"

class Minimap {
public:
    Minimap(Renderer *renderer, float tableWidth, float tableDepth);

    void Render(int windowWidth, int windowHeight);

private:
    Renderer *renderer;
    float tableWidth, tableDepth;
};


#endif //BILLIARDSHOW_MINIMAP_H
