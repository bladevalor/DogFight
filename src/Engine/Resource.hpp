#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cassert>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

enum class TextureId {
    Eagle,
    Raptor,
    Desert,
    Avenger,
    Bullet,
    Missile,
    HealthBoost,
    MissilePickup,
    FireSpread,
    FireRate,
    TitleScreen,
    ButtonNormal,
    ButtonPressed,
    ButtonSelected,
};
enum class FontID { Main };

template <typename Identifier, typename Resource> class ResourceHolder {
  public:
    void load(Identifier id, const std::string &filename) {
        std::unique_ptr<Resource> resource = std::make_unique<Resource>();

        if constexpr (std::is_same_v<Resource, sf::Texture>) {
            if (!resource->loadFromFile(filename)) {
                throw std::runtime_error("Cant Load Resource: " + filename);
            }
        } else if constexpr (std::is_same_v<Resource, sf::Font>) {
            if (!resource->openFromFile(filename)) {
                throw std::runtime_error("Cant Load Resource: " + filename);
            }
        } else {
            throw std::runtime_error("Unsupported Identifier type.");
        }

        insertResource(id, std::move(resource));
    }

    template <typename Parameter>
    void load(Identifier id, const std::string &filename,
              const Parameter &secondParam) {
        std::unique_ptr<Resource> resource = std::make_unique<Resource>();

        if constexpr (std::is_same_v<Resource, sf::Texture>) {
            if (!resource.loadFromFile(filename, secondParam)) {
                throw std::runtime_error("Cant Load Resource: " + filename);
            }
        } else {
            throw std::runtime_error(
                "sf::Font type only 'takes' one parameter.");
        }

        insertResource(id, std::move(resource));
    }

    Resource &get(Identifier id) {
        auto found = mResourceMap.find(id);
        assert(found != mResourceMap.end());
        return *found->second;
    }

    const Resource &get(Identifier id) const {
        auto found = mResourceMap.find(id);
        assert(found != mResourceMap.end());
        return *found->second;
    }

  private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;

    void insertResource(Identifier id, std::unique_ptr<Resource> resource) {
        auto inserted =
            mResourceMap.insert(std::make_pair(id, std::move(resource)));

        assert(inserted.second);
    }
};

typedef ResourceHolder<TextureId, sf::Texture> TextureHolder;
typedef ResourceHolder<FontID, sf::Font> FontHolder;
