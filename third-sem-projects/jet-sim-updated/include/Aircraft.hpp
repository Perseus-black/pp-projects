#pragma once

#include <SFML/System/Vector2.hpp>
#include <deque>
#include <optional>
#include <string>

struct TrailPoint {
    sf::Vector2f position;
    float age = 0.f;
};

enum class ThrottleStage {
    IDLE,
    CRUISE,
    MIL,
    AFTERBURNER
};

class Aircraft {
public:
    sf::Vector2f position{450.f, 350.f};

    float heading = 0.f;

    // Speeds (in knots)
    float speed = 180.f;
    float targetSpeed = 180.f;

    float altitude = 12000.f;
    float bank = 0.f;

    // Flight Dynamics & Avionics
    ThrottleStage throttleStage = ThrottleStage::CRUISE;
    float gLoad = 1.0f;
    float fuel = 18000.f;            // Pounds (lbs), max capacity: 18,000 lbs
    const float maxFuel = 18000.f;
    bool isStalled = false;
    bool isOverG = false;
    bool isBingoFuel = false;        // Under 2,500 lbs

    // Flight trail
    std::deque<TrailPoint> flightTrail;
    float trailSampleTimer = 0.f;

    void setThrottleStage(ThrottleStage stage);
    std::string getThrottleStageString() const;

    void update(
        float dt,
        bool autoNavigation,
        std::optional<sf::Vector2f>& waypoint,
        float commandedTurnRate
    );

    void updateTrail(float dt);
};
