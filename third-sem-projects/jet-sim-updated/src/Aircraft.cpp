#include "Aircraft.hpp"
#include "Config.hpp"
#include "MathUtils.hpp"
#include <algorithm>
#include <cmath>

void Aircraft::setThrottleStage(ThrottleStage stage) {
    if (fuel <= 0.f && stage != ThrottleStage::IDLE) {
        return; // Engine flameout
    }

    throttleStage = stage;
    switch (throttleStage) {
        case ThrottleStage::IDLE:
            targetSpeed = 120.f;
            break;
        case ThrottleStage::CRUISE:
            targetSpeed = 280.f;
            break;
        case ThrottleStage::MIL:
            targetSpeed = 500.f;
            break;
        case ThrottleStage::AFTERBURNER:
            targetSpeed = 850.f;
            break;
    }
}

std::string Aircraft::getThrottleStageString() const {
    switch (throttleStage) {
        case ThrottleStage::IDLE:
            return "IDLE";
        case ThrottleStage::CRUISE:
            return "CRUISE";
        case ThrottleStage::MIL:
            return "MIL (100%)";
        case ThrottleStage::AFTERBURNER:
            return "AB (BURNER)";
    }
    return "NORM";
}

void Aircraft::update(
    float dt,
    bool autoNavigation,
    std::optional<sf::Vector2f>& waypoint,
    float commandedTurnRate
) {
    // ----------------------------------------------------
    // FUEL SYSTEM
    // ----------------------------------------------------
    float burnRate = 0.5f; // lbs per second
    switch (throttleStage) {
        case ThrottleStage::IDLE:
            burnRate = 0.2f;
            break;
        case ThrottleStage::CRUISE:
            burnRate = 0.5f;
            break;
        case ThrottleStage::MIL:
            burnRate = 1.6f;
            break;
        case ThrottleStage::AFTERBURNER:
            burnRate = 5.2f;
            break;
    }

    if (fuel > 0.f) {
        fuel = std::max(0.f, fuel - burnRate * dt);
        isBingoFuel = (fuel <= 2500.f);
    } else {
        // Flameout
        throttleStage = ThrottleStage::IDLE;
        targetSpeed = 100.f;
    }

    // ----------------------------------------------------
    // ACCELERATION & ENGINE RESPONSE
    // ----------------------------------------------------
    float acceleration = 38.f;
    if (throttleStage == ThrottleStage::AFTERBURNER) {
        acceleration = 75.f;
    } else if (throttleStage == ThrottleStage::MIL) {
        acceleration = 50.f;
    }

    const float speedDifference = targetSpeed - speed;
    const float speedStep = acceleration * dt;
    speed += std::clamp(speedDifference, -speedStep, speedStep);

    // ----------------------------------------------------
    // WAYPOINT AUTOPILOT
    // ----------------------------------------------------
    if (autoNavigation && waypoint) {
        const float desiredHeading = MathUtils::bearingTo(position, *waypoint);
        const float difference = MathUtils::angleDifference(heading, desiredHeading);

        commandedTurnRate = std::clamp(difference * 1.8f, -55.f, 55.f);
        heading = MathUtils::normalizeAngle(heading + commandedTurnRate * dt);

        if (MathUtils::distanceBetween(position, *waypoint) < 16.f) {
            waypoint.reset();
        }
    }

    // ----------------------------------------------------
    // G-LOAD & INDUCED DRAG (ENERGY-MANEUVERABILITY)
    // ----------------------------------------------------
    const float vMps = speed * 0.5144f;
    const float omega = std::abs(commandedTurnRate) * Config::PI / 180.f;
    const float centripetalA = vMps * omega;
    gLoad = std::sqrt(1.0f + std::pow(centripetalA / 9.81f, 2.0f));

    isOverG = (gLoad >= 9.0f);

    // High-G turn bleeds airspeed (induced drag)
    if (gLoad > 1.2f) {
        const float inducedDrag = (gLoad - 1.0f) * 16.f * dt;
        speed = std::max(95.f, speed - inducedDrag);
    }

    // ----------------------------------------------------
    // STALL BEHAVIOR
    // ----------------------------------------------------
    if (speed < 115.f) {
        isStalled = true;
        // Severe altitude loss during stall
        altitude = std::max(500.f, altitude - 650.f * dt);
    } else {
        isStalled = false;
        // Altitude changes smoothly with excess kinetic energy
        altitude = std::clamp(
            altitude + (speed - 220.f) * 1.2f * dt,
            1000.f,
            55000.f
        );
    }

    // ----------------------------------------------------
    // BANK INDICATOR
    // ----------------------------------------------------
    const float targetBank = std::clamp(commandedTurnRate * 0.55f, -35.f, 35.f);
    bank += (targetBank - bank) * std::min(1.f, 3.5f * dt);

    // ----------------------------------------------------
    // AIRCRAFT MOVEMENT
    // ----------------------------------------------------
    const float movementScale = 0.12f;

    position.x += std::sin(MathUtils::toRadians(heading)) * speed * dt * movementScale;
    position.y -= std::cos(MathUtils::toRadians(heading)) * speed * dt * movementScale;

    position.x = std::clamp(
        position.x,
        18.f,
        static_cast<float>(Config::WINDOW_WIDTH) - 18.f
    );

    position.y = std::clamp(
        position.y,
        18.f,
        static_cast<float>(Config::MAP_HEIGHT) - 18.f
    );
}

void Aircraft::updateTrail(float dt) {
    trailSampleTimer += dt;

    if (trailSampleTimer >= 0.055f) {
        trailSampleTimer = 0.f;
        flightTrail.push_back({position, 0.f});
    }

    for (TrailPoint& point : flightTrail) {
        point.age += dt;
    }

    while (!flightTrail.empty() && flightTrail.front().age > 4.f) {
        flightTrail.pop_front();
    }
}
