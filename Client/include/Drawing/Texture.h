#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm\glm.hpp>
#include <GL/glew.h>
#include "drawing/stb_image.h"

class Texture
{
public:
    unsigned int ID;
    unsigned int Width, Height;
    
    unsigned int Internal_Format;
    unsigned int Image_Format;
    
    unsigned int Wrap_S;
    unsigned int Wrap_T;
    unsigned int Filter_Min;
    unsigned int Filter_Max;

    Texture();

    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    void Bind() const;
    void loadTextureFromFile(const char* file, bool alpha);
};

#endif
