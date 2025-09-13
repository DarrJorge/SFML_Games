//
// Created by Oleksandr Pogorelov.
//

#pragma once

#include <string>
#include <string_view>
#include <map>
#include <unordered_map>
#include <iostream>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ResourceManager final
{
public:
    ResourceManager() = delete;
    ~ResourceManager() = delete;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    static const sf::Texture& getTexture(const std::string_view& path)
    {
        return getResource<sf::Texture>(path, m_textures);
    }

    static const sf::Font& getFont(const std::string_view& path)
    {
        return getResource<sf::Font>(path, m_fonts);
    }

    static const sf::SoundBuffer& getSound(const std::string_view& path)
    {
        return getResource<sf::SoundBuffer>(path, m_sounds);
    }

    static void clear()
    {
        std::scoped_lock lock(globalMutex());
        m_textures.clear();
        m_fonts.clear();
        m_sounds.clear();
    }

private:
    inline static std::unordered_map<std::string, sf::Texture> m_textures{};
    inline static std::unordered_map<std::string, sf::Font> m_fonts{};
    inline static std::unordered_map<std::string, sf::SoundBuffer> m_sounds{};

    static std::mutex& globalMutex() {
        static std::mutex m;
        return m;
    }

    template<typename Resource>
    static bool loadResource(Resource& res, const std::string& path)
    {
        return res.loadFromFile(path);
    }

    static bool loadResource(sf::Font& font, const std::string& path)
    {
        return font.openFromFile(path);
    }

    template<typename Resource, typename Map>
    static const Resource& getResource(const std::string_view& path, Map& container)
    {
        std::string key(path);

        {
            std::scoped_lock lock(globalMutex());
            auto it = container.find(key);
            if (it != container.end())
                return it->second;
        }

        Resource res;
        if (!loadResource(res, key))
        {
            std::cerr << "[ResourceManager] Failed to load: " << path << std::endl;
            static Resource fallback{};
            return fallback;
        }

        {
            std::scoped_lock lock(globalMutex());
            auto [it, inserted] = container.emplace(path, std::move(res));
            if (inserted)
            {
                std::cout << "[ResourceManager] Loaded: " << path << std::endl;
            }
            return it->second;
        }
    }
};