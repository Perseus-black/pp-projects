#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>
#include <filesystem>

namespace MathUtils {
    float toRadians(float degrees);
    float normalizeAngle(float angle);
    float angleDifference(float from, float to);
    float distanceBetween(sf::Vector2f a, sf::Vector2f b);
    float bearingTo(sf::Vector2f from, sf::Vector2f to);
    float vectorHeading(sf::Vector2f velocity);
    float vectorSpeed(sf::Vector2f velocity);
    std::string number(float value, int decimals = 0);
    std::filesystem::path resolveAssetPath(
        const std::filesystem::path& relativePath,
        const std::vector<std::filesystem::path>& fallbackDirs
    );
}
