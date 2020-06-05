#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <unordered_map>
#include <vector>

#include "drawing/texture.h"

class TextureManager {
private:
    static std::unordered_map<std::string, Texture*> textures;

    static Texture * loadTexture(std::string file, bool alpha);

public:
    static void initializeTextures();
    static void deleteTextures();


    static Texture * get(const std::string texture);

};
#endif