#include "TextureLibrary.h"

#include "Utils.h"
#include <unordered_map>

std::unordered_map<const char*, Texture2D> Textures;

const Texture2D GetTexture(const char* fileName)
{
    if (!MapContains(Textures, fileName))
    {
        Texture2D newTexture = LoadTexture(fileName);
        Textures.insert({ fileName, newTexture });
        return newTexture;
    }
    return Textures[fileName];
}
