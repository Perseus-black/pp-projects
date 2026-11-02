#pragma once

#include <SFML/Graphics.hpp>
#include "Aircraft.hpp"
#include "Contact.hpp"
#include "AudioVisual.hpp"
#include "Weapons.hpp"
#include <deque>
#include <vector>
#include <optional>
#include <string>

namespace HUD {

void drawText(
    sf::RenderWindow& window,
    const sf::Font& font,
    const std::string& value,
    unsigned int size,
    sf::Vector2f position,
    sf::Color color = sf::Color(220, 230, 235)
);

void drawLine(
    sf::RenderWindow& window,
    sf::Vector2f start,
    sf::Vector2f end,
    sf::Color color,
    float thickness = 1.f
);

void drawPanel(
    sf::RenderWindow& window,
    sf::Vector2f position,
    sf::Vector2f size
);

void drawFlightTrail(
    sf::RenderWindow& window,
    const std::deque<TrailPoint>& trail
);

void drawRadarEcho(
    sf::RenderWindow& window,
    sf::Vector2f position,
    float age
);

void drawContact(
    sf::RenderWindow& window,
    const Contact& contact,
    bool selected
);

void drawSAMSite(
    sf::RenderWindow& window,
    const sf::Font& font,
    const SAMSite& sam
);

void drawMapBackground(
    sf::RenderWindow& window,
    const sf::Font& font
);

void drawRadarRings(
    sf::RenderWindow& window,
    const Aircraft& player
);

void drawRadarSweep(
    sf::RenderWindow& window,
    const Aircraft& player,
    float sweepAngle,
    bool radarVisible
);

void drawHeadingTape(
    sf::RenderWindow& window,
    const sf::Font& font,
    float heading
);

void drawBankIndicator(
    sf::RenderWindow& window,
    const sf::Font& font,
    float bank
);

void renderAll(
    sf::RenderWindow& window,
    const sf::Font& font,
    sf::Sprite& jetSprite,
    const Aircraft& player,
    const std::vector<Contact>& contacts,
    const SAMSite& samSite,
    ParticleSystem& particles,
    const WeaponsSystem& weapons,
    int selectedContact,
    float sweepAngle,
    bool radarVisible,
    bool autoNavigation,
    const std::optional<sf::Vector2f>& waypoint,
    const std::deque<std::string>& eventLog,
    float simulationTime
);

} // namespace HUD
