#include <iostream>

#include "drawing/Texture.h"


Texture::Texture(std::string file, bool alpha)
    : Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
    glGenTextures(1, &ID);
    loadTextureFromFile(file, alpha);
} 

void Texture::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, Internal_Format, width, height, 0, Image_Format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const
{
        glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::loadTextureFromFile(std::string file, bool alpha)
{
    if (alpha)
    {
        Internal_Format = GL_RGBA;
        Image_Format = GL_RGBA;
    }

    int width, height, nrChannels;
    unsigned char* data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);

    this->Generate(width, height, data);

    stbi_image_free(data);

}