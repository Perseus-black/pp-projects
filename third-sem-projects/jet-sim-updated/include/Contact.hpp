#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

enum class AIState {
    PATROL,
    INTERCEPT,
    DOGFIGHT,
    EVADE
};

struct SAMSite {
    sf::Vector2f position{720.f, 580.f};
    float radarRange = 210.f;
    float lethalRange = 140.f;
    bool active = true;
    std::string name = "SA-10 GRUMBLE";
    float sweepAngle = 0.f;
};

struct Contact {
    sf::Vector2f position;
    sf::Vector2f velocity;

    int id = 0;
    bool friendly = false;

    std::string callsign;

    float heading = 0.f;
    float speed = 180.f;
    float altitude = 15000.f;

    float lastRadarHit = -1000.f;
    sf::Vector2f lastRadarPosition{};
    bool radarTrackActive = false;

    // AI & Threat State
    AIState aiState = AIState::PATROL;
    float stateTimer = 0.f;
    bool isTargetingPlayer = false;

    std::string getAIStateString() const;

    void update(
        float dt,
        float simulationTime,
        const sf::Vector2f& playerPos,
        float playerHeading,
        bool isSelectedByPlayer
    );
};
