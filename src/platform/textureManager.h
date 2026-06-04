#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <raylib.h>
#include <map>
#include <string>

// Texture Manager - handles all procedural texture creation and management
class TextureManager {
private:
    std::map<std::string, Texture2D> textures;

public:
    void AddTexture(const std::string& name, Texture2D texture) {
        textures[name] = texture;
    }

    Texture2D GetTexture(const std::string& name) {
        if (textures.find(name) != textures.end()) {
            return textures[name];
        }
        return {};
    }

    void UnloadAll() {
        for (auto& pair : textures) {
            UnloadTexture(pair.second);
        }
        textures.clear();
    }

    ~TextureManager() {
        UnloadAll();
    }
};

#endif // TEXTURE_MANAGER_H
