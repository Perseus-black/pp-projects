#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>
#include <string>
#include "Contact.hpp"
#include "AudioVisual.hpp"

enum class WeaponType {
    AIM_120D,
    AIM_9X
};

struct Missile {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float heading = 0.f;
    float speed = 250.f;
    float maxSpeed = 620.f;
    float life = 0.f;
    float maxLife = 6.5f;
    int targetContactId = -1;
    WeaponType type = WeaponType::AIM_120D;
    bool active = true;
};

class WeaponsSystem {
public:
    int aim120Count = 4;
    int aim9xCount = 2;
    WeaponType selectedWeapon = WeaponType::AIM_120D;

    std::vector<Missile> activeMissiles;

    void cycleWeapon();
    std::string getSelectedWeaponName() const;
    int getSelectedWeaponCount() const;
    float getMaxRange() const;
    bool isTargetInRange(float distance) const;

    bool launchMissile(
        const sf::Vector2f& playerPos,
        float playerHeading,
        int targetContactId,
        const std::vector<Contact>& contacts,
        std::deque<std::string>& eventLog,
        AudioManager& audio
    );

    void update(
        float dt,
        std::vector<Contact>& contacts,
        ParticleSystem& particles,
        std::deque<std::string>& eventLog,
        int& selectedContact
    );

    void draw(sf::RenderWindow& window) const;
};
