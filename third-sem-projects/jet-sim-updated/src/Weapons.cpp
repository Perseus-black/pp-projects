#include "Weapons.hpp"
#include "Config.hpp"
#include "MathUtils.hpp"
#include <algorithm>
#include <cmath>

void WeaponsSystem::cycleWeapon() {
    selectedWeapon = (selectedWeapon == WeaponType::AIM_120D)
        ? WeaponType::AIM_9X
        : WeaponType::AIM_120D;
}

std::string WeaponsSystem::getSelectedWeaponName() const {
    return (selectedWeapon == WeaponType::AIM_120D)
        ? "AIM-120D AMRAAM"
        : "AIM-9X SIDEWINDER";
}

int WeaponsSystem::getSelectedWeaponCount() const {
    return (selectedWeapon == WeaponType::AIM_120D)
        ? aim120Count
        : aim9xCount;
}

float WeaponsSystem::getMaxRange() const {
    return (selectedWeapon == WeaponType::AIM_120D) ? 290.f : 145.f;
}

bool WeaponsSystem::isTargetInRange(float distance) const {
    return (distance <= getMaxRange());
}

bool WeaponsSystem::launchMissile(
    const sf::Vector2f& playerPos,
    float playerHeading,
    int targetContactId,
    const std::vector<Contact>& contacts,
    std::deque<std::string>& eventLog,
    AudioManager& audio
) {
    if (getSelectedWeaponCount() <= 0) {
        eventLog.push_front("WEAPONS BAY EMPTY: " + getSelectedWeaponName());
        while (eventLog.size() > 6) eventLog.pop_back();
        return false;
    }

    if (targetContactId < 0 || static_cast<std::size_t>(targetContactId) >= contacts.size()) {
        eventLog.push_front("NO LOCK: ACQUIRE RADAR TRACK BEFORE LAUNCH");
        while (eventLog.size() > 6) eventLog.pop_back();
        return false;
    }

    const Contact& target = contacts[static_cast<std::size_t>(targetContactId)];
    const float dist = MathUtils::distanceBetween(playerPos, target.position);

    if (dist > getMaxRange()) {
        eventLog.push_front("OUT OF RANGE: " + target.callsign + " > " + MathUtils::number(getMaxRange()) + "u");
        while (eventLog.size() > 6) eventLog.pop_back();
        return false;
    }

    // Deduct ordnance
    if (selectedWeapon == WeaponType::AIM_120D) {
        --aim120Count;
        eventLog.push_front("FOX THREE: AIM-120D LAUNCHED -> " + target.callsign);
    } else {
        --aim9xCount;
        eventLog.push_front("FOX TWO: AIM-9X LAUNCHED -> " + target.callsign);
    }
    while (eventLog.size() > 6) eventLog.pop_back();

    audio.playMissileLaunch();

    // Spawn Missile
    Missile m;
    const float rad = MathUtils::toRadians(playerHeading);
    m.position = playerPos + sf::Vector2f{std::sin(rad) * 16.f, -std::cos(rad) * 16.f};
    m.heading = playerHeading;
    m.speed = 300.f;
    m.maxSpeed = (selectedWeapon == WeaponType::AIM_120D) ? 660.f : 550.f;
    m.maxLife = (selectedWeapon == WeaponType::AIM_120D) ? 7.0f : 5.0f;
    m.type = selectedWeapon;
    m.targetContactId = target.id;
    m.active = true;

    activeMissiles.push_back(m);
    return true;
}

void WeaponsSystem::update(
    float dt,
    std::vector<Contact>& contacts,
    ParticleSystem& particles,
    std::deque<std::string>& eventLog,
    int& selectedContact
) {
    for (auto& missile : activeMissiles) {
        if (!missile.active)
            continue;

        missile.life += dt;
        if (missile.life >= missile.maxLife) {
            missile.active = false;
            continue;
        }

        // Rocket Motor Acceleration
        missile.speed = std::min(missile.maxSpeed, missile.speed + 340.f * dt);

        // Find Target Contact
        auto targetIt = std::find_if(contacts.begin(), contacts.end(), [&](const Contact& c) {
            return c.id == missile.targetContactId;
        });

        if (targetIt != contacts.end()) {
            const sf::Vector2f targetPos = targetIt->position;

            // Proportional Navigation (PN) Guidance Law
            const float losAngle = MathUtils::bearingTo(missile.position, targetPos);
            const float angleDiff = MathUtils::angleDifference(missile.heading, losAngle);

            const float turnRateAuthority = (missile.type == WeaponType::AIM_9X) ? 140.f : 95.f;
            const float commandedTurn = std::clamp(angleDiff * 3.5f, -turnRateAuthority, turnRateAuthority);

            missile.heading = MathUtils::normalizeAngle(missile.heading + commandedTurn * dt);

            // Intercept & Proximity Fuse Hit Detection
            const float dist = MathUtils::distanceBetween(missile.position, targetPos);
            if (dist <= 14.f) {
                // Detonation!
                particles.emitExplosion(targetPos);
                eventLog.push_front("TARGET SPLASHED: " + targetIt->callsign + " DESTROYED");
                while (eventLog.size() > 6) eventLog.pop_back();

                // Remove destroyed contact
                const int destroyedId = targetIt->id;
                contacts.erase(targetIt);

                if (selectedContact >= 0 &&
                    static_cast<std::size_t>(selectedContact) < contacts.size() &&
                    contacts[static_cast<std::size_t>(selectedContact)].id == destroyedId) {
                    selectedContact = -1;
                } else if (selectedContact >= static_cast<int>(contacts.size())) {
                    selectedContact = -1;
                }

                missile.active = false;
                continue;
            }
        }

        // Velocity & Position Integration
        const float movementScale = 0.12f;
        missile.velocity.x = std::sin(MathUtils::toRadians(missile.heading)) * missile.speed;
        missile.velocity.y = -std::cos(MathUtils::toRadians(missile.heading)) * missile.speed;

        missile.position += missile.velocity * dt * movementScale;

        // Exhaust Smoke Plume
        particles.emitMissileSmoke(missile.position, missile.velocity);
    }

    // Clean inactive missiles
    activeMissiles.erase(
        std::remove_if(activeMissiles.begin(), activeMissiles.end(), [](const Missile& m) {
            return !m.active;
        }),
        activeMissiles.end()
    );
}

void WeaponsSystem::draw(sf::RenderWindow& window) const {
    for (const auto& m : activeMissiles) {
        if (!m.active)
            continue;

        const float rad = MathUtils::toRadians(m.heading);
        const sf::Vector2f dir{std::sin(rad), -std::cos(rad)};

        // Missile body line
        const sf::Vector2f nose = m.position + dir * 6.f;
        const sf::Vector2f tail = m.position - dir * 6.f;

        sf::RectangleShape body({10.f, 2.0f});
        body.setOrigin({5.f, 1.0f});
        body.setPosition(m.position);
        body.setRotation(sf::degrees(m.heading - 90.f));
        body.setFillColor(sf::Color(240, 245, 250));
        window.draw(body);

        // Rocket exhaust plume point
        sf::CircleShape plume(2.2f);
        plume.setOrigin({2.2f, 2.2f});
        plume.setPosition(tail);
        plume.setFillColor(sf::Color(255, 170, 40));
        window.draw(plume);
    }
}
