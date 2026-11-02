#include "HUD.hpp"
#include "Config.hpp"
#include "MathUtils.hpp"
#include <algorithm>
#include <cmath>

namespace HUD {

void drawText(
    sf::RenderWindow& window,
    const sf::Font& font,
    const std::string& value,
    unsigned int size,
    sf::Vector2f position,
    sf::Color color
) {
    sf::Text text(font, value, size);
    text.setPosition(position);
    text.setFillColor(color);
    window.draw(text);
}

void drawLine(
    sf::RenderWindow& window,
    sf::Vector2f start,
    sf::Vector2f end,
    sf::Color color,
    float thickness
) {
    const sf::Vector2f delta{
        end.x - start.x,
        end.y - start.y
    };

    const float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    if (length < 0.1f)
        return;

    sf::RectangleShape line({length, thickness});
    line.setPosition(start);
    line.setOrigin({0.f, thickness / 2.f});
    line.setRotation(sf::degrees(std::atan2(delta.y, delta.x) * 180.f / Config::PI));
    line.setFillColor(color);

    window.draw(line);
}

void drawPanel(
    sf::RenderWindow& window,
    sf::Vector2f position,
    sf::Vector2f size
) {
    sf::RectangleShape panel(size);
    panel.setPosition(position);
    panel.setFillColor(sf::Color(4, 15, 21, 235));
    panel.setOutlineColor(sf::Color(48, 110, 115));
    panel.setOutlineThickness(1.f);
    window.draw(panel);
}

void drawFlightTrail(
    sf::RenderWindow& window,
    const std::deque<TrailPoint>& trail
) {
    for (std::size_t i = 1; i < trail.size(); ++i) {
        const float age = (trail[i - 1].age + trail[i].age) * 0.5f;
        const float fade = std::clamp(1.f - age / 4.f, 0.f, 1.f);
        const auto alpha = static_cast<std::uint8_t>(fade * 150.f);

        drawLine(
            window,
            trail[i - 1].position,
            trail[i].position,
            sf::Color(70, 180, 255, alpha),
            2.f
        );
    }
}

void drawRadarEcho(
    sf::RenderWindow& window,
    sf::Vector2f position,
    float age
) {
    const float fade = std::clamp(1.f - age / Config::TRACK_MEMORY, 0.f, 1.f);
    if (fade <= 0.f)
        return;

    const auto alpha = static_cast<std::uint8_t>(fade * 190.f);
    const float radius = 3.f + (1.f - fade) * 7.f;

    sf::CircleShape echo(radius);
    echo.setOrigin({radius, radius});
    echo.setPosition(position);
    echo.setFillColor(sf::Color(70, 255, 170, alpha));
    window.draw(echo);

    sf::CircleShape ring(radius + 3.f);
    ring.setOrigin({radius + 3.f, radius + 3.f});
    ring.setPosition(position);
    ring.setFillColor(sf::Color::Transparent);
    ring.setOutlineColor(sf::Color(70, 255, 170, static_cast<std::uint8_t>(alpha / 2)));
    ring.setOutlineThickness(1.f);
    window.draw(ring);
}

void drawContact(
    sf::RenderWindow& window,
    const Contact& contact,
    bool selected
) {
    const sf::Color color = contact.friendly
        ? sf::Color(70, 220, 150)
        : sf::Color(255, 100, 90);

    sf::ConvexShape diamond;
    diamond.setPointCount(4);
    diamond.setPoint(0, {0.f, -8.f});
    diamond.setPoint(1, {8.f, 0.f});
    diamond.setPoint(2, {0.f, 8.f});
    diamond.setPoint(3, {-8.f, 0.f});
    diamond.setPosition(contact.position);
    diamond.setFillColor(sf::Color::Transparent);
    diamond.setOutlineColor(selected ? sf::Color::White : color);
    diamond.setOutlineThickness(selected ? 2.5f : 1.8f);
    window.draw(diamond);

    const float heading = MathUtils::vectorHeading(contact.velocity);
    const sf::Vector2f directionEnd{
        contact.position.x + std::sin(MathUtils::toRadians(heading)) * 20.f,
        contact.position.y - std::cos(MathUtils::toRadians(heading)) * 20.f
    };

    drawLine(window, contact.position, directionEnd, color, 1.5f);

    if (selected) {
        sf::CircleShape selection(14.f);
        selection.setOrigin({14.f, 14.f});
        selection.setPosition(contact.position);
        selection.setFillColor(sf::Color::Transparent);
        selection.setOutlineColor(sf::Color(230, 240, 245, 180));
        selection.setOutlineThickness(1.f);
        window.draw(selection);
    }
}

void drawSAMSite(
    sf::RenderWindow& window,
    const sf::Font& font,
    const SAMSite& sam
) {
    if (!sam.active)
        return;

    // Outer Surveillance Ring (faint red)
    sf::CircleShape outer(sam.radarRange);
    outer.setOrigin({sam.radarRange, sam.radarRange});
    outer.setPosition(sam.position);
    outer.setFillColor(sf::Color::Transparent);
    outer.setOutlineColor(sf::Color(180, 50, 50, 70));
    outer.setOutlineThickness(1.f);
    window.draw(outer);

    // Inner Lethal Engagement Ring
    sf::CircleShape inner(sam.lethalRange);
    inner.setOrigin({sam.lethalRange, sam.lethalRange});
    inner.setPosition(sam.position);
    inner.setFillColor(sf::Color(200, 30, 30, 18));
    inner.setOutlineColor(sf::Color(255, 60, 60, 160));
    inner.setOutlineThickness(1.5f);
    window.draw(inner);

    // SAM Site Icon: Launch Radar Emplacement
    sf::RectangleShape base({14.f, 14.f});
    base.setOrigin({7.f, 7.f});
    base.setPosition(sam.position);
    base.setFillColor(sf::Color(220, 45, 45, 200));
    base.setOutlineColor(sf::Color(255, 200, 200));
    base.setOutlineThickness(1.2f);
    window.draw(base);

    drawLine(window, {sam.position.x - 11.f, sam.position.y}, {sam.position.x + 11.f, sam.position.y}, sf::Color(255, 120, 120), 1.2f);
    drawLine(window, {sam.position.x, sam.position.y - 11.f}, {sam.position.x, sam.position.y + 11.f}, sf::Color(255, 120, 120), 1.2f);

    drawText(
        window,
        font,
        sam.name + " [SAM SITE]",
        9,
        {sam.position.x - 45.f, sam.position.y + 12.f},
        sf::Color(255, 120, 120)
    );
}

void drawMapBackground(
    sf::RenderWindow& window,
    const sf::Font& font
) {
    for (int x = 0; x <= static_cast<int>(Config::WINDOW_WIDTH); x += 50) {
        drawLine(
            window,
            {static_cast<float>(x), 0.f},
            {static_cast<float>(x), static_cast<float>(Config::MAP_HEIGHT)},
            sf::Color(24, 52, 59)
        );
    }

    for (int y = 0; y <= static_cast<int>(Config::MAP_HEIGHT); y += 50) {
        drawLine(
            window,
            {0.f, static_cast<float>(y)},
            {static_cast<float>(Config::WINDOW_WIDTH), static_cast<float>(y)},
            sf::Color(24, 52, 59)
        );
    }

    drawText(
        window,
        font,
        "SECTOR 07 [TACTICAL COMBAT AIRSPACE]",
        10,
        {Config::WINDOW_WIDTH / 2.f + 120.f, 52.f},
        sf::Color(65, 125, 125)
    );
}

void drawRadarRings(
    sf::RenderWindow& window,
    const Aircraft& player
) {
    for (float radius : {60.f, 120.f, Config::RADAR_RANGE}) {
        sf::CircleShape ring(radius);
        ring.setOrigin({radius, radius});
        ring.setPosition(player.position);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineColor(sf::Color(38, 100, 94, 160));
        ring.setOutlineThickness(1.f);
        window.draw(ring);
    }
}

void drawRadarSweep(
    sf::RenderWindow& window,
    const Aircraft& player,
    float sweepAngle,
    bool radarVisible
) {
    if (!radarVisible)
        return;

    for (int i = 5; i >= 1; --i) {
        const float tailAngle = MathUtils::normalizeAngle(sweepAngle - i * 2.4f);
        const sf::Vector2f tailEnd{
            player.position.x + std::sin(MathUtils::toRadians(tailAngle)) * Config::RADAR_RANGE,
            player.position.y - std::cos(MathUtils::toRadians(tailAngle)) * Config::RADAR_RANGE
        };

        const auto alpha = static_cast<std::uint8_t>(20 + (6 - i) * 12);
        drawLine(window, player.position, tailEnd, sf::Color(45, 210, 135, alpha), 1.f);
    }

    const sf::Vector2f sweepEnd{
        player.position.x + std::sin(MathUtils::toRadians(sweepAngle)) * Config::RADAR_RANGE,
        player.position.y - std::cos(MathUtils::toRadians(sweepAngle)) * Config::RADAR_RANGE
    };

    drawLine(window, player.position, sweepEnd, sf::Color(75, 255, 165), 2.f);
}

void drawHeadingTape(
    sf::RenderWindow& window,
    const sf::Font& font,
    float heading
) {
    const float centerX = Config::WINDOW_WIDTH / 2.f;
    const float y = 14.f;
    const float width = 270.f;

    sf::RectangleShape backing({width, 45.f});
    backing.setPosition({centerX - width / 2.f, y});
    backing.setFillColor(sf::Color(4, 15, 21, 220));
    backing.setOutlineColor(sf::Color(48, 110, 115));
    backing.setOutlineThickness(1.f);
    window.draw(backing);

    for (int offset = -5; offset <= 5; ++offset) {
        const float tickHeading = MathUtils::normalizeAngle(heading + offset * 10.f);
        const float x = centerX + offset * 22.f;
        const float tickHeight = (offset % 2 == 0) ? 12.f : 7.f;

        drawLine(window, {x, y + 4.f}, {x, y + 4.f + tickHeight}, sf::Color(100, 180, 175));

        if (offset % 2 == 0) {
            drawText(
                window,
                font,
                MathUtils::number(tickHeading),
                9,
                {x - 10.f, y + 20.f},
                sf::Color(170, 205, 200)
            );
        }
    }

    drawLine(window, {centerX, y + 1.f}, {centerX, y + 17.f}, sf::Color(255, 210, 80), 2.f);

    drawText(
        window,
        font,
        MathUtils::number(heading) + " DEG",
        10,
        {centerX - 23.f, y + 31.f},
        sf::Color(255, 220, 120)
    );
}

void drawBankIndicator(
    sf::RenderWindow& window,
    const sf::Font& font,
    float bank
) {
    const sf::Vector2f center{Config::WINDOW_WIDTH / 2.f, 103.f};

    drawLine(window, {center.x - 52.f, center.y}, {center.x + 52.f, center.y}, sf::Color(90, 130, 130));

    for (int degrees = -45; degrees <= 45; degrees += 15) {
        const float x = center.x + static_cast<float>(degrees);
        const float length = (degrees % 30 == 0) ? 8.f : 5.f;
        drawLine(window, {x, center.y - length / 2.f}, {x, center.y + length / 2.f}, sf::Color(100, 150, 150));
    }

    const float markerX = center.x + std::clamp(bank, -45.f, 45.f);

    sf::ConvexShape marker;
    marker.setPointCount(3);
    marker.setPoint(0, {markerX, center.y - 7.f});
    marker.setPoint(1, {markerX - 5.f, center.y - 15.f});
    marker.setPoint(2, {markerX + 5.f, center.y - 15.f});
    marker.setFillColor(sf::Color(255, 205, 80));
    window.draw(marker);

    drawText(
        window,
        font,
        "BANK " + MathUtils::number(bank) + " deg",
        10,
        {center.x - 35.f, center.y + 12.f},
        sf::Color(255, 220, 120)
    );
}

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
) {
    window.clear(sf::Color(5, 16, 22));

    // Map Background & Range Rings
    drawMapBackground(window, font);
    drawRadarRings(window, player);

    // SAM Site Ground Installation
    drawSAMSite(window, font, samSite);

    // Radar Echoes
    for (const Contact& contact : contacts) {
        const float age = simulationTime - contact.lastRadarHit;
        if (age >= 0.f && age <= Config::TRACK_MEMORY) {
            drawRadarEcho(window, contact.lastRadarPosition, age);
        }
    }

    // Radar Sweep Line
    drawRadarSweep(window, player, sweepAngle, radarVisible);

    // Waypoint
    if (waypoint) {
        drawLine(window, player.position, *waypoint, sf::Color(230, 195, 70), 1.5f);

        sf::CircleShape marker(7.f);
        marker.setOrigin({7.f, 7.f});
        marker.setPosition(*waypoint);
        marker.setFillColor(sf::Color::Transparent);
        marker.setOutlineColor(sf::Color(255, 210, 70));
        marker.setOutlineThickness(2.f);
        window.draw(marker);

        drawText(
            window,
            font,
            "WP",
            11,
            {waypoint->x + 9.f, waypoint->y - 8.f},
            sf::Color(255, 210, 70)
        );
    }

    // Flight Trail & FX Particles
    drawFlightTrail(window, player.flightTrail);
    particles.draw(window);

    // Active Missiles
    weapons.draw(window);

    // Contacts
    for (std::size_t i = 0; i < contacts.size(); ++i) {
        const Contact& contact = contacts[i];
        const float trackAge = simulationTime - contact.lastRadarHit;
        if (trackAge > Config::TRACK_MEMORY)
            continue;

        const bool selected = (static_cast<int>(i) == selectedContact);
        drawContact(window, contact, selected);

        const float range = MathUtils::distanceBetween(player.position, contact.position);
        const float bearing = MathUtils::bearingTo(player.position, contact.position);

        const sf::Color labelColor = contact.friendly
            ? sf::Color(90, 235, 170)
            : sf::Color(255, 125, 110);

        std::string extraTag = "";
        if (!contact.friendly) {
            extraTag = " [" + contact.getAIStateString() + "]";
        }

        drawText(
            window,
            font,
            contact.callsign + "  " + MathUtils::number(range) + "  " + MathUtils::number(bearing) + " deg" + extraTag,
            10,
            {contact.position.x + 12.f, contact.position.y - 25.f},
            labelColor
        );

        drawText(
            window,
            font,
            "HDG " + MathUtils::number(MathUtils::vectorHeading(contact.velocity)) +
            "  SPD " + MathUtils::number(MathUtils::vectorSpeed(contact.velocity)),
            9,
            {contact.position.x + 12.f, contact.position.y - 12.f},
            sf::Color(155, 190, 185)
        );

        // Lock & Shoot Cue
        if (selected && !contact.friendly) {
            const bool inRange = weapons.isTargetInRange(range);
            const sf::Color shootColor = inRange ? sf::Color(100, 255, 100) : sf::Color(255, 180, 50);
            drawText(
                window,
                font,
                inRange ? "[IN RANGE - SHOOT]" : "[OUT OF RANGE]",
                11,
                {contact.position.x - 45.f, contact.position.y + 18.f},
                shootColor
            );
        }
    }

    // Player Jet Sprite
    jetSprite.setPosition(player.position);
    jetSprite.setRotation(sf::degrees(player.heading));
    window.draw(jetSprite);

    drawText(
        window,
        font,
        "OWN",
        11,
        {player.position.x + 13.f, player.position.y + 10.f},
        sf::Color(100, 210, 255)
    );

    // Operations & Avionics Panel
    drawPanel(window, {12.f, 12.f}, {270.f, 212.f});
    drawText(window, font, "F-22 AVIONICS & OPERATIONS", 13, {22.f, 18.f}, sf::Color(120, 225, 205));
    drawText(window, font, "HDG  " + MathUtils::number(player.heading) + " deg", 12, {22.f, 38.f});
    drawText(window, font, "SPD  " + MathUtils::number(player.speed) + " / " + MathUtils::number(player.targetSpeed) + " kt", 12, {22.f, 56.f});
    drawText(window, font, "ALT  " + MathUtils::number(player.altitude) + " ft", 12, {22.f, 74.f});

    const sf::Color throtColor = (player.throttleStage == ThrottleStage::AFTERBURNER)
        ? sf::Color(255, 140, 50)
        : sf::Color(120, 225, 205);
    drawText(window, font, "PWR  " + player.getThrottleStageString(), 12, {22.f, 92.f}, throtColor);

    sf::Color gColor = sf::Color(200, 220, 220);
    if (player.isOverG) {
        gColor = (static_cast<int>(simulationTime * 6.f) % 2 == 0)
            ? sf::Color(255, 50, 50)
            : sf::Color(255, 255, 255);
    } else if (player.gLoad > 6.0f) {
        gColor = sf::Color(255, 190, 50);
    }
    drawText(window, font, "G-LD " + MathUtils::number(player.gLoad, 1) + " G" + (player.isOverG ? " [OVER-G]" : ""), 12, {22.f, 110.f}, gColor);

    const float fuelPct = (player.fuel / player.maxFuel) * 100.f;
    sf::Color fuelColor = sf::Color(100, 230, 150);
    if (player.fuel <= 0.f) {
        fuelColor = sf::Color(255, 60, 60);
    } else if (player.isBingoFuel) {
        fuelColor = sf::Color(255, 200, 60);
    }
    drawText(window, font, "FUEL " + MathUtils::number(player.fuel) + " lbs (" + MathUtils::number(fuelPct) + "%)", 12, {22.f, 128.f}, fuelColor);

    drawText(window, font, "NAV  " + std::string(autoNavigation ? "AUTO" : "MANUAL"), 12, {22.f, 146.f}, autoNavigation ? sf::Color(100, 230, 150) : sf::Color(255, 200, 90));
    drawText(window, font, "RDR  " + std::string(radarVisible ? "SEARCH ON" : "SEARCH OFF"), 12, {22.f, 164.f}, radarVisible ? sf::Color(100, 230, 150) : sf::Color(255, 150, 100));

    // Weapons Bay Readout
    drawText(
        window,
        font,
        "WPN  [" + weapons.getSelectedWeaponName() + "] x" + std::to_string(weapons.getSelectedWeaponCount()),
        12,
        {22.f, 184.f},
        (weapons.getSelectedWeaponCount() > 0) ? sf::Color(255, 215, 80) : sf::Color(255, 80, 80)
    );

    // Threat Zone Check (SAM Site)
    const float distToSAM = MathUtils::distanceBetween(player.position, samSite.position);
    const bool inSAMLethal = (distToSAM <= samSite.lethalRange);

    // Tactical Warnings
    if (inSAMLethal) {
        const bool flash = (static_cast<int>(simulationTime * 5.f) % 2 == 0);
        if (flash) {
            drawPanel(window, {Config::WINDOW_WIDTH / 2.f - 200.f, 140.f}, {400.f, 32.f});
            drawText(window, font, "! SAM RADAR LOCK - SA-10 THREAT ZONE !", 13, {Config::WINDOW_WIDTH / 2.f - 180.f, 147.f}, sf::Color(255, 40, 40));
        }
    } else if (player.isStalled) {
        const bool flash = (static_cast<int>(simulationTime * 5.f) % 2 == 0);
        if (flash) {
            drawPanel(window, {Config::WINDOW_WIDTH / 2.f - 180.f, 140.f}, {360.f, 32.f});
            drawText(window, font, "! WARNING: STALL - INCREASE SPEED !", 13, {Config::WINDOW_WIDTH / 2.f - 165.f, 147.f}, sf::Color(255, 60, 60));
        }
    } else if (player.isOverG) {
        drawPanel(window, {Config::WINDOW_WIDTH / 2.f - 150.f, 140.f}, {300.f, 32.f});
        drawText(window, font, "! CAUTION: HIGH G LOAD !", 13, {Config::WINDOW_WIDTH / 2.f - 110.f, 147.f}, sf::Color(255, 180, 50));
    } else if (player.isBingoFuel) {
        drawPanel(window, {Config::WINDOW_WIDTH / 2.f - 130.f, 140.f}, {260.f, 32.f});
        drawText(window, font, "! CAUTION: BINGO FUEL !", 13, {Config::WINDOW_WIDTH / 2.f - 100.f, 147.f}, sf::Color(255, 200, 60));
    }

    if (waypoint) {
        const float wpRange = MathUtils::distanceBetween(player.position, *waypoint);
        const float wpBearing = MathUtils::bearingTo(player.position, *waypoint);
        drawText(
            window,
            font,
            "WP " + MathUtils::number(wpRange) + " / " + MathUtils::number(wpBearing) + " deg",
            10,
            {12.f, 228.f},
            sf::Color(255, 210, 90)
        );
    }

    // Heading Tape & Bank Indicator
    drawHeadingTape(window, font, player.heading);
    drawBankIndicator(window, font, player.bank);

    // Contact Track Panel
    drawPanel(window, {Config::WINDOW_WIDTH - 262.f, 12.f}, {250.f, 190.f});
    drawText(window, font, "CONTACT TRACK", 13, {Config::WINDOW_WIDTH - 252.f, 20.f}, sf::Color(120, 225, 205));

    if (selectedContact >= 0 && static_cast<std::size_t>(selectedContact) < contacts.size()) {
        const Contact& contact = contacts[static_cast<std::size_t>(selectedContact)];
        const bool trackFresh = (simulationTime - contact.lastRadarHit <= Config::TRACK_MEMORY);
        const float range = MathUtils::distanceBetween(player.position, contact.position);
        const float bearing = MathUtils::bearingTo(player.position, contact.position);

        drawText(window, font, "ID     C" + std::to_string(contact.id) + " / " + contact.callsign, 11, {Config::WINDOW_WIDTH - 252.f, 44.f});
        drawText(window, font, "TYPE   " + std::string(contact.friendly ? "FRIENDLY" : "HOSTILE BANDIT"), 12, {Config::WINDOW_WIDTH - 252.f, 62.f}, contact.friendly ? sf::Color(90, 235, 170) : sf::Color(255, 110, 90));

        const sf::Color aiModeColor = (contact.aiState == AIState::EVADE)
            ? sf::Color(255, 80, 80)
            : (contact.aiState == AIState::INTERCEPT || contact.aiState == AIState::DOGFIGHT)
                ? sf::Color(255, 180, 50)
                : sf::Color(180, 220, 210);

        drawText(window, font, "AI ACT " + contact.getAIStateString(), 12, {Config::WINDOW_WIDTH - 252.f, 80.f}, aiModeColor);
        drawText(window, font, "RANGE  " + MathUtils::number(range, 1), 12, {Config::WINDOW_WIDTH - 252.f, 98.f});
        drawText(window, font, "BRG    " + MathUtils::number(bearing) + " deg", 12, {Config::WINDOW_WIDTH - 252.f, 116.f});
        drawText(window, font, "HDG    " + MathUtils::number(MathUtils::vectorHeading(contact.velocity)) + " deg", 12, {Config::WINDOW_WIDTH - 252.f, 134.f});
        drawText(window, font, "SPD    " + MathUtils::number(MathUtils::vectorSpeed(contact.velocity)) + " kt", 12, {Config::WINDOW_WIDTH - 252.f, 152.f});
        drawText(window, font, "ALT    " + MathUtils::number(contact.altitude) + " ft (SIM)", 12, {Config::WINDOW_WIDTH - 252.f, 170.f});
        drawText(window, font, trackFresh ? "LIVE " + MathUtils::number(simulationTime - contact.lastRadarHit, 1) + "s" : "STALE", 10, {Config::WINDOW_WIDTH - 65.f, 170.f}, trackFresh ? sf::Color(100, 230, 150) : sf::Color(255, 150, 100));
    } else {
        drawText(window, font, "No contact selected.", 12, {Config::WINDOW_WIDTH - 252.f, 50.f});
        drawText(window, font, "Click track to lock / inspect,", 12, {Config::WINDOW_WIDTH - 252.f, 72.f});
        drawText(window, font, "or map to set a waypoint.", 12, {Config::WINDOW_WIDTH - 252.f, 92.f});
    }

    // Compass
    const sf::Vector2f compassCenter{Config::WINDOW_WIDTH - 55.f, Config::MAP_HEIGHT - 65.f};
    sf::CircleShape compass(31.f);
    compass.setOrigin({31.f, 31.f});
    compass.setPosition(compassCenter);
    compass.setFillColor(sf::Color(4, 15, 21, 180));
    compass.setOutlineColor(sf::Color(55, 120, 125));
    compass.setOutlineThickness(1.f);
    window.draw(compass);

    drawText(window, font, "N", 11, {compassCenter.x - 4.f, compassCenter.y - 43.f});
    drawText(window, font, "S", 11, {compassCenter.x - 4.f, compassCenter.y + 29.f});
    drawText(window, font, "W", 11, {compassCenter.x - 43.f, compassCenter.y - 7.f});
    drawText(window, font, "E", 11, {compassCenter.x + 33.f, compassCenter.y - 7.f});

    const sf::Vector2f compassPointer{
        compassCenter.x + std::sin(MathUtils::toRadians(player.heading)) * 23.f,
        compassCenter.y - std::cos(MathUtils::toRadians(player.heading)) * 23.f
    };
    drawLine(window, compassCenter, compassPointer, sf::Color(255, 210, 80), 2.f);

    // Event Log
    drawPanel(window, {0.f, static_cast<float>(Config::MAP_HEIGHT)}, {static_cast<float>(Config::WINDOW_WIDTH), static_cast<float>(Config::WINDOW_HEIGHT - Config::MAP_HEIGHT)});
    drawText(window, font, "EVENT LOG", 12, {12.f, 707.f}, sf::Color(120, 225, 205));

    for (std::size_t i = 0; i < eventLog.size(); ++i) {
        drawText(
            window,
            font,
            eventLog[i],
            10,
            {130.f, 706.f + static_cast<float>(i) * 13.f},
            (i == 0) ? sf::Color(225, 235, 235) : sf::Color(130, 155, 160)
        );
    }

    drawText(
        window,
        font,
        "Steer: Left/Right | Throttle: 1-IDLE 2-CRUISE 3-MIL 4-AB | Space: Fire Missile | W: Cycle Wpn | F: Flare | Click: Select",
        9,
        {12.f, 792.f},
        sf::Color(105, 135, 140)
    );

    window.display();
}

} // namespace HUD
