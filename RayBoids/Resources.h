#pragma once
#include "raylib.h"

class Resources
{
    
private:
    static inline Resources* instance;

public:

    Texture2D backgroundTexture;
    Texture2D wallTexture;
    Texture2D playerTexture;
    Music music;

    static Resources* GetResources();
    void LoadResources();
    void UnloadResources();
};

