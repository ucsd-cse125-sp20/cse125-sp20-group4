#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm\glm.hpp>
#include <GL/glew.h>

#include <vector>

#include "drawing/stb_image.h"

class Texture
{
public:
    unsigned int ID;
    
    unsigned int Internal_Format;
    unsigned int Image_Format;
    
    unsigned int Wrap_S;
    unsigned int Wrap_T;
    unsigned int Filter_Min;
    unsigned int Filter_Max;

    Texture(std::string file, bool alpha);

    void Bind() const;

private:
    void Generate(unsigned int width, unsigned int height, unsigned char* data);

    void loadTextureFromFile(std::string file, bool alpha);
};

#endif
