#include "Contact.hpp"
#include "Config.hpp"
#include "MathUtils.hpp"
#include <algorithm>
#include <cmath>

std::string Contact::getAIStateString() const {
    switch (aiState) {
        case AIState::PATROL:
            return "PATROL";
        case AIState::INTERCEPT:
            return "INTERCEPT";
        case AIState::DOGFIGHT:
            return "DOGFIGHT";
        case AIState::EVADE:
            return "EVASIVE";
    }
    return "UNKNOWN";
}

void Contact::update(
    float dt,
    float simulationTime,
    const sf::Vector2f& playerPos,
    float playerHeading,
    bool isSelectedByPlayer
) {
    stateTimer += dt;

    // Sync heading and speed if initial or moving
    if (speed < 10.f) {
        speed = MathUtils::vectorSpeed(velocity);
        heading = MathUtils::vectorHeading(velocity);
    }

    if (friendly) {
        // Friendlies maintain steady patrol routes
        aiState = AIState::PATROL;
        isTargetingPlayer = false;
    } else {
        // ----------------------------------------------------
        // HOSTILE AI DECISION LOGIC (FSM)
        // ----------------------------------------------------
        const float distToPlayer = MathUtils::distanceBetween(position, playerPos);
        const float bearingToPlayer = MathUtils::bearingTo(position, playerPos);

        if (isSelectedByPlayer) {
            // Player has STT/Track locked this bandit -> Bandit goes defensive!
            aiState = AIState::EVADE;
            isTargetingPlayer = false;

            // Turn 90 degrees away from the threat vector and accelerate
            const float breakHeading = MathUtils::normalizeAngle(bearingToPlayer + 110.f);
            const float diff = MathUtils::angleDifference(heading, breakHeading);
            const float turnRate = std::clamp(diff * 2.5f, -70.f, 70.f);
            heading = MathUtils::normalizeAngle(heading + turnRate * dt);
            speed = std::min(450.f, speed + 100.f * dt);
        } else if (distToPlayer < 120.f) {
            // Dogfight envelope: Attempt to get on the player's 6 o'clock
            aiState = AIState::DOGFIGHT;
            isTargetingPlayer = true;

            const float desiredHeading = playerHeading;
            const float diff = MathUtils::angleDifference(heading, desiredHeading);
            const float turnRate = std::clamp(diff * 2.2f, -65.f, 65.f);
            heading = MathUtils::normalizeAngle(heading + turnRate * dt);
            speed = 280.f;
        } else if (distToPlayer < 280.f) {
            // Intercept envelope: Close distance with lead pursuit
            aiState = AIState::INTERCEPT;
            isTargetingPlayer = true;

            const float diff = MathUtils::angleDifference(heading, bearingToPlayer);
            const float turnRate = std::clamp(diff * 1.8f, -50.f, 50.f);
            heading = MathUtils::normalizeAngle(heading + turnRate * dt);
            speed = std::min(360.f, speed + 60.f * dt);
        } else {
            // Beyond engagement range: Patrol cruise
            aiState = AIState::PATROL;
            isTargetingPlayer = false;
            speed = 200.f;
        }
    }

    // --------------------------------------------------------
    // VECTOR INTEGRATION & MOVEMENT
    // --------------------------------------------------------
    const float movementScale = 0.12f;
    velocity.x = std::sin(MathUtils::toRadians(heading)) * speed;
    velocity.y = -std::cos(MathUtils::toRadians(heading)) * speed;

    position.x += velocity.x * dt * movementScale;
    position.y += velocity.y * dt * movementScale;

    // Bounce off horizontal map boundaries
    if (position.x < 18.f) {
        position.x = 18.f;
        heading = MathUtils::normalizeAngle(360.f - heading);
    } else if (position.x > Config::WINDOW_WIDTH - 18.f) {
        position.x = static_cast<float>(Config::WINDOW_WIDTH) - 18.f;
        heading = MathUtils::normalizeAngle(360.f - heading);
    }

    // Bounce off vertical map boundaries
    if (position.y < 18.f) {
        position.y = 18.f;
        heading = MathUtils::normalizeAngle(180.f - heading);
    } else if (position.y > Config::MAP_HEIGHT - 18.f) {
        position.y = static_cast<float>(Config::MAP_HEIGHT) - 18.f;
        heading = MathUtils::normalizeAngle(180.f - heading);
    }

    // Recompute velocity after wall deflection
    velocity.x = std::sin(MathUtils::toRadians(heading)) * speed;
    velocity.y = -std::cos(MathUtils::toRadians(heading)) * speed;

    // Simulated altitude variation
    altitude = std::clamp(
        altitude + std::sin(simulationTime * 0.2f + id) * 8.f * dt,
        3000.f,
        45000.f
    );
}
