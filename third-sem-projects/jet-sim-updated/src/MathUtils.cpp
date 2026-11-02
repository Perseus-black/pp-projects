#include "MathUtils.hpp"
#include "Config.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>

namespace MathUtils {

float toRadians(float degrees) {
    return degrees * Config::PI / 180.f;
}

float normalizeAngle(float angle) {
    while (angle < 0.f)
        angle += 360.f;

    while (angle >= 360.f)
        angle -= 360.f;

    return angle;
}

float angleDifference(float from, float to) {
    float difference = normalizeAngle(to) - normalizeAngle(from);

    if (difference > 180.f)
        difference -= 360.f;

    if (difference < -180.f)
        difference += 360.f;

    return difference;
}

float distanceBetween(sf::Vector2f a, sf::Vector2f b) {
    const float dx = b.x - a.x;
    const float dy = b.y - a.y;
    return std::sqrt(dx * dx + dy * dy);
}

float bearingTo(sf::Vector2f from, sf::Vector2f to) {
    const float dx = to.x - from.x;
    const float dy = to.y - from.y;
    return normalizeAngle(std::atan2(dx, -dy) * 180.f / Config::PI);
}

float vectorHeading(sf::Vector2f velocity) {
    return normalizeAngle(std::atan2(velocity.x, -velocity.y) * 180.f / Config::PI);
}

float vectorSpeed(sf::Vector2f velocity) {
    return std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
}

std::string number(float value, int decimals) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(decimals) << value;
    return stream.str();
}

std::filesystem::path resolveAssetPath(
    const std::filesystem::path& relativePath,
    const std::vector<std::filesystem::path>& fallbackDirs
) {
    std::filesystem::path current = std::filesystem::current_path();

    for (const auto& dir : fallbackDirs) {
        const std::filesystem::path candidate = dir / relativePath;
        if (std::filesystem::exists(candidate)) {
            return candidate;
        }
    }

    if (std::filesystem::exists(current / relativePath)) {
        return current / relativePath;
    }

    return relativePath;
}

} // namespace MathUtils
