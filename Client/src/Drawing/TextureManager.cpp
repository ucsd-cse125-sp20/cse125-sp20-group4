#include "drawing/TextureManager.h"

std::unordered_map<std::string, Texture*> TextureManager::textures;

Texture * TextureManager::loadTexture(std::string file, bool alpha)
{
	return new Texture(file, alpha);
}

void TextureManager::initializeTextures()
{
	textures["default"] = loadTexture("Textures/default.jpg", false);
	textures["floor"] = loadTexture("Textures/floor.jpg", false);
	textures["ceiling"] = loadTexture("Textures/ceiling.jpg", false);
	textures["wall"] = loadTexture("Textures/wall.jpg", false);
	textures["smoke"] = loadTexture("Textures/smoke.png", true);
	textures["water"] = loadTexture("Textures/water.png", true);
	textures["shopper"] = loadTexture("Textures/shopper.png", true);
	textures["barrier"] = loadTexture("Textures/barrier.png", true);
}

void TextureManager::deleteTextures()
{
	for (auto it = textures.begin(); it != textures.end(); it++) {
		delete(it->second);
	}
}

Texture * TextureManager::get(const std::string texture)
{
	return textures[texture];
}
