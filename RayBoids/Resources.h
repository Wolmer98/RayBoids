#pragma once
#include "raylib.h"

class Resources
{
    
private:
    static inline Resources* instance;

public:

    Texture2D backgroundTexture;
    Texture2D wallTexture;

    static Resources* GetResources();
    void LoadResources();
};

