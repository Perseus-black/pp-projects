#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

// ============================================================
// CONFIGURATION
// ============================================================

constexpr float PI = 3.14159265f;

constexpr float RADAR_RANGE = 180.f;
constexpr float SWEEP_SPEED = 90.f;
constexpr float TRACK_MEMORY = 8.f;

constexpr unsigned int WINDOW_WIDTH = 900;
constexpr unsigned int MAP_HEIGHT = 700;
constexpr unsigned int WINDOW_HEIGHT = 810;

// ============================================================
// MATHEMATICAL UTILITIES
// ============================================================

float toRadians(float degrees) {
    return degrees * PI / 180.f;
}

float normalizeAngle(float angle) {
    while (angle < 0.f)
        angle += 360.f;

    while (angle >= 360.f)
        angle -= 360.f;

    return angle;
}

float angleDifference(float from, float to) {
    float difference =
        normalizeAngle(to) - normalizeAngle(from);

    if (difference > 180.f)
        difference -= 360.f;

    if (difference < -180.f)
        difference += 360.f;

    return difference;
}

float distanceBetween(
    sf::Vector2f a,
    sf::Vector2f b
) {
    const float dx = b.x - a.x;
    const float dy = b.y - a.y;

    return std::sqrt(dx * dx + dy * dy);
}

// Heading convention:
// North = 000 degrees
// East  = 090 degrees
// South = 180 degrees
// West  = 270 degrees

float bearingTo(
    sf::Vector2f from,
    sf::Vector2f to
) {
    const float dx = to.x - from.x;
    const float dy = to.y - from.y;

    return normalizeAngle(
        std::atan2(dx, -dy) * 180.f / PI
    );
}

float vectorHeading(sf::Vector2f velocity) {
    return normalizeAngle(
        std::atan2(velocity.x, -velocity.y) *
        180.f / PI
    );
}

float vectorSpeed(sf::Vector2f velocity) {
    return std::sqrt(
        velocity.x * velocity.x +
        velocity.y * velocity.y
    );
}

std::string number(
    float value,
    int decimals = 0
) {
    std::ostringstream stream;

    stream << std::fixed
           << std::setprecision(decimals)
           << value;

    return stream.str();
}

// ============================================================
// DATA STRUCTURES
// ============================================================

struct Aircraft {
    sf::Vector2f position{450.f, 350.f};

    float heading = 0.f;

    // Current and selected speeds.
    float speed = 180.f;
    float targetSpeed = 180.f;

    float altitude = 12000.f;

    // Visual indication only; not a real flight calculation.
    float bank = 0.f;
};

struct Contact {
    sf::Vector2f position;
    sf::Vector2f velocity;

    int id;
    bool friendly;

    std::string callsign;

    float altitude;

    float lastRadarHit = -1000.f;

    sf::Vector2f lastRadarPosition{};

    bool radarTrackActive = false;
};

struct TrailPoint {
    sf::Vector2f position;
    float age = 0.f;
};

// ============================================================
// EVENT LOG
// ============================================================

void addLog(
    std::deque<std::string>& log,
    const std::string& message
) {
    log.push_front(message);

    while (log.size() > 6)
        log.pop_back();
}

// ============================================================
// DRAWING UTILITIES
// ============================================================

void drawText(
    sf::RenderWindow& window,
    const sf::Font& font,
    const std::string& value,
    unsigned int size,
    sf::Vector2f position,
    sf::Color color = sf::Color(220, 230, 235)
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
    float thickness = 1.f
) {
    const sf::Vector2f delta{
        end.x - start.x,
        end.y - start.y
    };

    const float length = std::sqrt(
        delta.x * delta.x +
        delta.y * delta.y
    );

    if (length < 0.1f)
        return;

    sf::RectangleShape line({
        length,
        thickness
    });

    line.setPosition(start);

    line.setOrigin({
        0.f,
        thickness / 2.f
    });

    line.setRotation(
        sf::degrees(
            std::atan2(delta.y, delta.x) *
            180.f / PI
        )
    );

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

    panel.setFillColor(
        sf::Color(4, 15, 21, 235)
    );

    panel.setOutlineColor(
        sf::Color(48, 110, 115)
    );

    panel.setOutlineThickness(1.f);

    window.draw(panel);
}

// ============================================================
// AIRCRAFT TRAIL
// ============================================================

void drawFlightTrail(
    sf::RenderWindow& window,
    const std::deque<TrailPoint>& trail
) {
    for (std::size_t i = 1; i < trail.size(); ++i) {
        const float age =
            (trail[i - 1].age + trail[i].age) * 0.5f;

        const float fade = std::clamp(
            1.f - age / 4.f,
            0.f,
            1.f
        );

        const auto alpha =
            static_cast<std::uint8_t>(fade * 150.f);

        drawLine(
            window,
            trail[i - 1].position,
            trail[i].position,
            sf::Color(70, 180, 255, alpha),
            2.f
        );
    }
}

// ============================================================
// RADAR ECHOES
// ============================================================

void drawRadarEcho(
    sf::RenderWindow& window,
    sf::Vector2f position,
    float age
) {
    const float fade = std::clamp(
        1.f - age / TRACK_MEMORY,
        0.f,
        1.f
    );

    if (fade <= 0.f)
        return;

    const auto alpha =
        static_cast<std::uint8_t>(fade * 190.f);

    const float radius =
        3.f + (1.f - fade) * 7.f;

    // Bright central echo.
    sf::CircleShape echo(radius);

    echo.setOrigin({
        radius,
        radius
    });

    echo.setPosition(position);

    echo.setFillColor(
        sf::Color(70, 255, 170, alpha)
    );

    window.draw(echo);

    // Expanding, fading ring.
    sf::CircleShape ring(radius + 3.f);

    ring.setOrigin({
        radius + 3.f,
        radius + 3.f
    });

    ring.setPosition(position);

    ring.setFillColor(
        sf::Color::Transparent
    );

    ring.setOutlineColor(
        sf::Color(
            70,
            255,
            170,
            static_cast<std::uint8_t>(alpha / 2)
        )
    );

    ring.setOutlineThickness(1.f);

    window.draw(ring);
}

// ============================================================
// CONTACT SYMBOLS
// ============================================================

void drawContact(
    sf::RenderWindow& window,
    const Contact& contact,
    bool selected
) {
    const sf::Color color = contact.friendly
        ? sf::Color(70, 220, 150)
        : sf::Color(255, 100, 90);

    // Diamond-shaped tactical symbol.
    sf::ConvexShape diamond;

    diamond.setPointCount(4);

    diamond.setPoint(0, {0.f, -8.f});
    diamond.setPoint(1, {8.f, 0.f});
    diamond.setPoint(2, {0.f, 8.f});
    diamond.setPoint(3, {-8.f, 0.f});

    diamond.setPosition(contact.position);

    diamond.setFillColor(
        sf::Color::Transparent
    );

    diamond.setOutlineColor(
        selected ? sf::Color::White : color
    );

    diamond.setOutlineThickness(
        selected ? 2.5f : 1.8f
    );

    window.draw(diamond);

    // Heading vector.
    const float heading =
        vectorHeading(contact.velocity);

    const sf::Vector2f directionEnd{
        contact.position.x +
            std::sin(toRadians(heading)) * 20.f,

        contact.position.y -
            std::cos(toRadians(heading)) * 20.f
    };

    drawLine(
        window,
        contact.position,
        directionEnd,
        color,
        1.5f
    );

    // Selection ring.
    if (selected) {
        sf::CircleShape selection(14.f);

        selection.setOrigin({
            14.f,
            14.f
        });

        selection.setPosition(contact.position);

        selection.setFillColor(
            sf::Color::Transparent
        );

        selection.setOutlineColor(
            sf::Color(230, 240, 245, 180)
        );

        selection.setOutlineThickness(1.f);

        window.draw(selection);
    }
}

// ============================================================
// VECTOR MAP BACKGROUND
// ============================================================

void drawMapBackground(
    sf::RenderWindow& window,
    const sf::Font& font)
{
    // Draw the background grid.
    for (int x = 0; x <= static_cast<int>(WINDOW_WIDTH); x += 50)
    {
        drawLine(
            window,
            {static_cast<float>(x), 0.f},
            {static_cast<float>(x), static_cast<float>(MAP_HEIGHT)},
            sf::Color(24, 52, 59)
        );
    }

    for (int y = 0; y <= static_cast<int>(MAP_HEIGHT); y += 50)
    {
        drawLine(
            window,
            {0.f, static_cast<float>(y)},
            {static_cast<float>(WINDOW_WIDTH), static_cast<float>(y)},
            sf::Color(24, 52, 59)
        );
    }

    // Keep the sector label away from the heading tape and panels.
    drawText(
        window,
        font,
        "SECTOR 07",
        10,
        {WINDOW_WIDTH / 2.f + 145.f, 52.f},
        sf::Color(65, 125, 125)
    );
}

// ============================================================
// RADAR DISPLAY
// ============================================================

void drawRadarRings(
    sf::RenderWindow& window,
    const Aircraft& player
) {
    for (float radius : {60.f, 120.f, RADAR_RANGE}) {
        sf::CircleShape ring(radius);

        ring.setOrigin({
            radius,
            radius
        });

        ring.setPosition(player.position);

        ring.setFillColor(
            sf::Color::Transparent
        );

        ring.setOutlineColor(
            sf::Color(38, 100, 94, 160)
        );

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

    // Fading sweep tail.
    for (int i = 5; i >= 1; --i) {
        const float tailAngle = normalizeAngle(
            sweepAngle - i * 2.4f
        );

        const sf::Vector2f tailEnd{
            player.position.x +
                std::sin(toRadians(tailAngle)) *
                RADAR_RANGE,

            player.position.y -
                std::cos(toRadians(tailAngle)) *
                RADAR_RANGE
        };

        const auto alpha =
            static_cast<std::uint8_t>(
                20 + (6 - i) * 12
            );

        drawLine(
            window,
            player.position,
            tailEnd,
            sf::Color(45, 210, 135, alpha),
            1.f
        );
    }

    // Main sweep line.
    const sf::Vector2f sweepEnd{
        player.position.x +
            std::sin(toRadians(sweepAngle)) *
            RADAR_RANGE,

        player.position.y -
            std::cos(toRadians(sweepAngle)) *
            RADAR_RANGE
    };

    drawLine(
        window,
        player.position,
        sweepEnd,
        sf::Color(75, 255, 165),
        2.f
    );
}

// ============================================================
// FLIGHT HUD
// ============================================================

void drawHeadingTape(
    sf::RenderWindow& window,
    const sf::Font& font,
    float heading
) {
    const float centerX = WINDOW_WIDTH / 2.f;
    const float y = 14.f;
    const float width = 270.f;

    sf::RectangleShape backing({
        width,
        45.f
    });

    backing.setPosition({
        centerX - width / 2.f,
        y
    });

    backing.setFillColor(
        sf::Color(4, 15, 21, 220)
    );

    backing.setOutlineColor(
        sf::Color(48, 110, 115)
    );

    backing.setOutlineThickness(1.f);

    window.draw(backing);

    for (int offset = -5; offset <= 5; ++offset) {
        const float tickHeading = normalizeAngle(
            heading + offset * 10.f
        );

        const float x = centerX + offset * 22.f;

        const float tickHeight =
            offset % 2 == 0 ? 12.f : 7.f;

        drawLine(
            window,
            {x, y + 4.f},
            {x, y + 4.f + tickHeight},
            sf::Color(100, 180, 175)
        );

        if (offset % 2 == 0) {
            drawText(
                window,
                font,
                number(tickHeading),
                9,
                {x - 10.f, y + 20.f},
                sf::Color(170, 205, 200)
            );
        }
    }

    drawLine(
        window,
        {centerX, y + 1.f},
        {centerX, y + 17.f},
        sf::Color(255, 210, 80),
        2.f
    );

    drawText(
        window,
        font,
        number(heading) + " DEG",
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
    const sf::Vector2f center{
        WINDOW_WIDTH / 2.f,
        103.f
    };

    drawLine(
        window,
        {center.x - 52.f, center.y},
        {center.x + 52.f, center.y},
        sf::Color(90, 130, 130)
    );

    for (
        int degrees = -45;
        degrees <= 45;
        degrees += 15
    ) {
        const float x =
            center.x + static_cast<float>(degrees);

        const float length =
            degrees % 30 == 0 ? 8.f : 5.f;

        drawLine(
            window,
            {x, center.y - length / 2.f},
            {x, center.y + length / 2.f},
            sf::Color(100, 150, 150)
        );
    }

    const float markerX =
        center.x + std::clamp(bank, -45.f, 45.f);

    sf::ConvexShape marker;

    marker.setPointCount(3);

    marker.setPoint(0, {markerX, center.y - 7.f});
    marker.setPoint(1, {markerX - 5.f, center.y - 15.f});
    marker.setPoint(2, {markerX + 5.f, center.y - 15.f});

    marker.setFillColor(
        sf::Color(255, 205, 80)
    );

    window.draw(marker);

    drawText(
        window,
        font,
        "BANK " + number(bank) + " deg",
        10,
        {center.x - 35.f, center.y + 12.f},
        sf::Color(255, 220, 120)
    );
}

// ============================================================
// MAIN
// ============================================================

int main() {
    sf::RenderWindow window(
        sf::VideoMode({
            WINDOW_WIDTH,
            WINDOW_HEIGHT
        }),
        "F-22 Map Simulator"
    );

    window.setFramerateLimit(60);

    // --------------------------------------------------------
    // LOAD YOUR OWN F-22 PNG
    // --------------------------------------------------------

    sf::Texture jetTexture;

    if (!jetTexture.loadFromFile("f22.png")) {
        std::cerr
            << "Error: Could not load f22.png.\n"
            << "Place f22.png in the program's working directory.\n";

        return 1;
    }

    jetTexture.setSmooth(true);

    sf::Sprite jetSprite(jetTexture);

    const sf::Vector2u textureSize =
        jetTexture.getSize();

    jetSprite.setOrigin({
        static_cast<float>(textureSize.x) / 2.f,
        static_cast<float>(textureSize.y) / 2.f
    });

    // Adjust this if your PNG has different dimensions.
    jetSprite.setScale({0.12f, 0.12f});

    // Assumption: the nose of the PNG points upward.
    // If your image points right, a rotation offset is needed.

    // --------------------------------------------------------
    // FONT
    // --------------------------------------------------------

    sf::Font font;

    if (!font.openFromFile(
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
    )) {
        std::cerr
            << "Error: Could not load DejaVuSans.ttf.\n";

        return 1;
    }

    // --------------------------------------------------------
    // SIMULATION OBJECTS
    // --------------------------------------------------------

    Aircraft player;

    std::vector<Contact> contacts = {
        {
            {650.f, 300.f},
            {-35.f, 20.f},
            1,
            false,
            "VIPER 1",
            18000.f
        },
        {
            {200.f, 180.f},
            {25.f, 30.f},
            2,
            true,
            "EAGLE 2",
            22000.f
        },
        {
            {350.f, 550.f},
            {15.f, -25.f},
            3,
            false,
            "BANDIT 3",
            15000.f
        },
        {
            {730.f, 460.f},
            {-18.f, -22.f},
            4,
            true,
            "HAWK 4",
            28000.f
        }
    };

    std::deque<std::string> eventLog;
    std::deque<TrailPoint> flightTrail;

    addLog(eventLog, "SIMULATION INITIALIZED");
    addLog(eventLog, "RADAR SEARCH ACTIVE");

    sf::Clock frameClock;

    float simulationTime = 0.f;
    float sweepAngle = 0.f;
    float trailSampleTimer = 0.f;

    bool radarVisible = true;
    bool autoNavigation = true;

    std::optional<sf::Vector2f> waypoint;

    // -1 means no selected contact.
    int selectedContact = -1;

    // ========================================================
    // MAIN LOOP
    // ========================================================

    while (window.isOpen()) {
        const float dt = std::min(
            frameClock.restart().asSeconds(),
            0.05f
        );

        simulationTime += dt;

        // ----------------------------------------------------
        // EVENTS
        // ----------------------------------------------------

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* key =
                event->getIf<sf::Event::KeyPressed>()) {

                if (key->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }

                if (key->code == sf::Keyboard::Key::R) {
                    radarVisible = !radarVisible;

                    addLog(
                        eventLog,
                        radarVisible
                            ? "RADAR SEARCH ENABLED"
                            : "RADAR SEARCH DISABLED"
                    );
                }

                if (key->code == sf::Keyboard::Key::A) {
                    autoNavigation = !autoNavigation;

                    addLog(
                        eventLog,
                        autoNavigation
                            ? "AUTO NAVIGATION ENABLED"
                            : "AUTO NAVIGATION DISABLED"
                    );
                }

                if (key->code == sf::Keyboard::Key::C) {
                    waypoint.reset();

                    addLog(
                        eventLog,
                        "WAYPOINT CLEARED"
                    );
                }

                if (key->code == sf::Keyboard::Key::Home) {
                    player.targetSpeed = 180.f;

                    addLog(
                        eventLog,
                        "CRUISE SPEED SELECTED"
                    );
                }
            }

            // Left-click: select a detected contact or set waypoint.
            if (const auto* mouse =
                event->getIf<sf::Event::MouseButtonPressed>()) {

                if (
                    mouse->button == sf::Mouse::Button::Left &&
                    mouse->position.y < static_cast<int>(MAP_HEIGHT)
                ) {
                    const sf::Vector2f click =
                        window.mapPixelToCoords(mouse->position);

                    int clickedContact = -1;

                    for (
                        std::size_t i = 0;
                        i < contacts.size();
                        ++i
                    ) {
                        const bool detected =
                            simulationTime -
                            contacts[i].lastRadarHit <= TRACK_MEMORY;

                        if (
                            detected &&
                            distanceBetween(
                                click,
                                contacts[i].position
                            ) <= 17.f
                        ) {
                            clickedContact =
                                static_cast<int>(i);

                            break;
                        }
                    }

                    if (clickedContact >= 0) {
                        selectedContact = clickedContact;

                        addLog(
                            eventLog,
                            "TRACK SELECTED: " +
                            contacts[
                                static_cast<std::size_t>(
                                    selectedContact
                                )
                            ].callsign
                        );
                    } else {
                        waypoint = click;
                        selectedContact = -1;

                        addLog(
                            eventLog,
                            "WAYPOINT SET: " +
                            number(click.x) + "," +
                            number(click.y)
                        );
                    }
                }
            }
        }

        // ----------------------------------------------------
        // MANUAL FLIGHT CONTROLS
        // ----------------------------------------------------

        const bool leftPressed =
            sf::Keyboard::isKeyPressed(
                sf::Keyboard::Key::Left
            );

        const bool rightPressed =
            sf::Keyboard::isKeyPressed(
                sf::Keyboard::Key::Right
            );

        const bool upPressed =
            sf::Keyboard::isKeyPressed(
                sf::Keyboard::Key::Up
            );

        const bool downPressed =
            sf::Keyboard::isKeyPressed(
                sf::Keyboard::Key::Down
            );

        float commandedTurnRate = 0.f;

        // Manual steering disables autopilot so the two systems
        // do not fight over the aircraft heading.
        if (leftPressed || rightPressed) {
            if (autoNavigation) {
                autoNavigation = false;

                addLog(
                    eventLog,
                    "MANUAL TURN: AUTOPILOT DISENGAGED"
                );
            }

            const float turnInput =
                (rightPressed ? 1.f : 0.f) -
                (leftPressed ? 1.f : 0.f);

            commandedTurnRate = turnInput * 75.f;

            player.heading = normalizeAngle(
                player.heading +
                commandedTurnRate * dt
            );
        }

        // Up/down adjust target speed, not instant speed.
        if (upPressed) {
            player.targetSpeed = std::min(
                720.f,
                player.targetSpeed + 100.f * dt
            );
        }

        if (downPressed) {
            player.targetSpeed = std::max(
                120.f,
                player.targetSpeed - 100.f * dt
            );
        }

        // ----------------------------------------------------
        // SMOOTH ACCELERATION
        // ----------------------------------------------------

        const float acceleration = 38.f;

        const float speedDifference =
            player.targetSpeed - player.speed;

        const float speedStep = acceleration * dt;

        player.speed += std::clamp(
            speedDifference,
            -speedStep,
            speedStep
        );

        // ----------------------------------------------------
        // WAYPOINT AUTOPILOT
        // ----------------------------------------------------

        if (
            autoNavigation &&
            waypoint
        ) {
            const float desiredHeading = bearingTo(
                player.position,
                *waypoint
            );

            const float difference = angleDifference(
                player.heading,
                desiredHeading
            );

            // Proportional turn command, capped at 55 degrees/sec.
            commandedTurnRate = std::clamp(
                difference * 1.8f,
                -55.f,
                55.f
            );

            player.heading = normalizeAngle(
                player.heading +
                commandedTurnRate * dt
            );

            if (
                distanceBetween(
                    player.position,
                    *waypoint
                ) < 16.f
            ) {
                waypoint.reset();

                addLog(
                    eventLog,
                    "WAYPOINT REACHED"
                );
            }
        }

        // ----------------------------------------------------
        // BANK INDICATOR
        // ----------------------------------------------------

        const float targetBank = std::clamp(
            commandedTurnRate * 0.55f,
            -35.f,
            35.f
        );

        // Smoothly return toward level when not turning.
        player.bank +=
            (targetBank - player.bank) *
            std::min(1.f, 3.5f * dt);

        // ----------------------------------------------------
        // AIRCRAFT MOVEMENT
        // ----------------------------------------------------

        const float movementScale = 0.12f;

        player.position.x +=
            std::sin(toRadians(player.heading)) *
            player.speed *
            dt *
            movementScale;

        player.position.y -=
            std::cos(toRadians(player.heading)) *
            player.speed *
            dt *
            movementScale;

        player.position.x = std::clamp(
            player.position.x,
            18.f,
            static_cast<float>(WINDOW_WIDTH) - 18.f
        );

        player.position.y = std::clamp(
            player.position.y,
            18.f,
            static_cast<float>(MAP_HEIGHT) - 18.f
        );

        // Simplified training altitude response.
        player.altitude = std::clamp(
            player.altitude +
            (player.speed - 180.f) * 0.8f * dt,
            5000.f,
            40000.f
        );

        // ----------------------------------------------------
        // FLIGHT TRAIL
        // ----------------------------------------------------

        trailSampleTimer += dt;

        if (trailSampleTimer >= 0.055f) {
            trailSampleTimer = 0.f;

            flightTrail.push_back({
                player.position,
                0.f
            });
        }

        for (TrailPoint& point : flightTrail) {
            point.age += dt;
        }

        while (
            !flightTrail.empty() &&
            flightTrail.front().age > 4.f
        ) {
            flightTrail.pop_front();
        }

        // ----------------------------------------------------
        // CONTACT MOVEMENT
        // ----------------------------------------------------

        for (Contact& contact : contacts) {
            contact.position.x +=
                contact.velocity.x * dt;

            contact.position.y +=
                contact.velocity.y * dt;

            // Bounce off horizontal map boundaries.
            if (
                contact.position.x < 18.f ||
                contact.position.x > WINDOW_WIDTH - 18.f
            ) {
                contact.velocity.x *= -1.f;

                contact.position.x = std::clamp(
                    contact.position.x,
                    18.f,
                    static_cast<float>(WINDOW_WIDTH) - 18.f
                );
            }

            // Bounce off vertical map boundaries.
            if (
                contact.position.y < 18.f ||
                contact.position.y > MAP_HEIGHT - 18.f
            ) {
                contact.velocity.y *= -1.f;

                contact.position.y = std::clamp(
                    contact.position.y,
                    18.f,
                    static_cast<float>(MAP_HEIGHT) - 18.f
                );
            }

            // Small, simulated altitude variation.
            contact.altitude = std::clamp(
                contact.altitude +
                std::sin(
                    simulationTime * 0.2f + contact.id
                ) * 8.f * dt,
                3000.f,
                45000.f
            );
        }

        // ----------------------------------------------------
        // RADAR SWEEP AND TRACK UPDATES
        // ----------------------------------------------------

        sweepAngle = normalizeAngle(
            sweepAngle + SWEEP_SPEED * dt
        );

        for (Contact& contact : contacts) {
            const float range = distanceBetween(
                player.position,
                contact.position
            );

            const float bearing = bearingTo(
                player.position,
                contact.position
            );

            const bool insideRange =
                range <= RADAR_RANGE;

            const bool sweepCrossing =
                std::abs(
                    angleDifference(
                        sweepAngle,
                        bearing
                    )
                ) <= 3.5f;

            // The R key enables/disables radar detections.
            if (
                radarVisible &&
                insideRange &&
                sweepCrossing
            ) {
                contact.lastRadarHit = simulationTime;

                contact.lastRadarPosition =
                    contact.position;

                if (!contact.radarTrackActive) {
                    addLog(
                        eventLog,
                        "RADAR CONTACT: " +
                        contact.callsign +
                        " ACQUIRED"
                    );

                    contact.radarTrackActive = true;
                }
            }

            // Expire tracks that have not been refreshed.
            if (
                contact.radarTrackActive &&
                simulationTime -
                    contact.lastRadarHit > TRACK_MEMORY
            ) {
                addLog(
                    eventLog,
                    "RADAR TRACK LOST: " +
                    contact.callsign
                );

                contact.radarTrackActive = false;

                if (
                    selectedContact >= 0 &&
                    contacts[
                        static_cast<std::size_t>(
                            selectedContact
                        )
                    ].id == contact.id
                ) {
                    selectedContact = -1;
                }
            }
        }

        // ====================================================
        // DRAW THE TACTICAL DISPLAY
        // ====================================================

        window.clear(
            sf::Color(5, 16, 22)
        );

        // ----------------------------------------------------
        // MAP BACKGROUND
        // ----------------------------------------------------

        drawMapBackground(window, font);

        // ----------------------------------------------------
        // RADAR RANGE RINGS
        // ----------------------------------------------------

        drawRadarRings(window, player);

        // ----------------------------------------------------
        // RADAR ECHOES
        // ----------------------------------------------------

        for (const Contact& contact : contacts) {
            const float age =
                simulationTime - contact.lastRadarHit;

            if (
                age >= 0.f &&
                age <= TRACK_MEMORY
            ) {
                drawRadarEcho(
                    window,
                    contact.lastRadarPosition,
                    age
                );
            }
        }

        // ----------------------------------------------------
        // RADAR SWEEP
        // ----------------------------------------------------

        drawRadarSweep(
            window,
            player,
            sweepAngle,
            radarVisible
        );

        // ----------------------------------------------------
        // WAYPOINT
        // ----------------------------------------------------

        if (waypoint) {
            drawLine(
                window,
                player.position,
                *waypoint,
                sf::Color(230, 195, 70),
                1.5f
            );

            sf::CircleShape marker(7.f);

            marker.setOrigin({
                7.f,
                7.f
            });

            marker.setPosition(*waypoint);

            marker.setFillColor(
                sf::Color::Transparent
            );

            marker.setOutlineColor(
                sf::Color(255, 210, 70)
            );

            marker.setOutlineThickness(2.f);

            window.draw(marker);

            drawText(
                window,
                font,
                "WP",
                11,
                {
                    waypoint->x + 9.f,
                    waypoint->y - 8.f
                },
                sf::Color(255, 210, 70)
            );
        }

        // ----------------------------------------------------
        // AIRCRAFT TRAIL
        // ----------------------------------------------------

        drawFlightTrail(
            window,
            flightTrail
        );

        // ----------------------------------------------------
        // DETECTED CONTACTS
        // ----------------------------------------------------

        for (
            std::size_t i = 0;
            i < contacts.size();
            ++i
        ) {
            const Contact& contact = contacts[i];

            const float trackAge =
                simulationTime -
                contact.lastRadarHit;

            const bool detected =
                trackAge <= TRACK_MEMORY;

            if (!detected)
                continue;

            const bool selected =
                static_cast<int>(i) == selectedContact;

            drawContact(
                window,
                contact,
                selected
            );

            const float range = distanceBetween(
                player.position,
                contact.position
            );

            const float bearing = bearingTo(
                player.position,
                contact.position
            );

            const sf::Color labelColor =
                contact.friendly
                    ? sf::Color(90, 235, 170)
                    : sf::Color(255, 125, 110);

            // Callsign, range and bearing.
            drawText(
                window,
                font,
                contact.callsign +
                    "  " +
                    number(range) +
                    "  " +
                    number(bearing) +
                    " deg",
                10,
                {
                    contact.position.x + 12.f,
                    contact.position.y - 25.f
                },
                labelColor
            );

            // Heading and speed.
            drawText(
                window,
                font,
                "HDG " +
                    number(vectorHeading(contact.velocity)) +
                    "  SPD " +
                    number(vectorSpeed(contact.velocity)),
                9,
                {
                    contact.position.x + 12.f,
                    contact.position.y - 12.f
                },
                sf::Color(155, 190, 185)
            );
        }
        

        // ----------------------------------------------------
        // PLAYER AIRCRAFT SPRITE
        // ----------------------------------------------------

        jetSprite.setPosition(
            player.position
        );

        jetSprite.setRotation(
            sf::degrees(player.heading)
        );

        window.draw(jetSprite);

        drawText(
            window,
            font,
            "OWN",
            11,
            {
                player.position.x + 13.f,
                player.position.y + 10.f
            },
            sf::Color(100, 210, 255)
        );

        // ----------------------------------------------------
        // FLIGHT INFORMATION PANEL
        // ----------------------------------------------------

        drawPanel(
            window,
            {12.f, 12.f},
            {245.f, 157.f}
        );

        drawText(
            window,
            font,
            "AIR OPERATIONS / TRAINING",
            13,
            {22.f, 20.f},
            sf::Color(120, 225, 205)
        );

        drawText(
            window,
            font,
            "HDG  " +
                number(player.heading) +
                " deg",
            13,
            {22.f, 46.f}
        );

        drawText(
            window,
            font,
            "SPD  " +
                number(player.speed) +
                " / " +
                number(player.targetSpeed) +
                " kt*",
            12,
            {22.f, 68.f}
        );

        drawText(
            window,
            font,
            "ALT  " +
                number(player.altitude) +
                " ft*",
            13,
            {22.f, 90.f}
        );

        drawText(
            window,
            font,
            "NAV  " +
                std::string(
                    autoNavigation ? "AUTO" : "MANUAL"
                ),
            13,
            {22.f, 112.f},
            autoNavigation
                ? sf::Color(100, 230, 150)
                : sf::Color(255, 200, 90)
        );

        drawText(
            window,
            font,
            "RDR  " +
                std::string(
                    radarVisible
                        ? "SEARCH ON"
                        : "SEARCH OFF"
                ),
            12,
            {22.f, 135.f},
            radarVisible
                ? sf::Color(100, 230, 150)
                : sf::Color(255, 150, 100)
        );

        if (waypoint) {
            const float wpRange = distanceBetween(
                player.position,
                *waypoint
            );

            const float wpBearing = bearingTo(
                player.position,
                *waypoint
            );

            drawText(
                window,
                font,
                "WP " +
                    number(wpRange) +
                    " / " +
                    number(wpBearing) +
                    " deg",
                10,
                {22.f, 157.f},
                sf::Color(255, 210, 90)
            );
        }

        // ----------------------------------------------------
        // HEADING TAPE AND BANK INDICATOR
        // ----------------------------------------------------

        drawHeadingTape(
            window,
            font,
            player.heading
        );

        drawBankIndicator(
            window,
            font,
            player.bank
        );

        // ----------------------------------------------------
        // SELECTED CONTACT PANEL
        // ----------------------------------------------------

        drawPanel(
            window,
            {
                WINDOW_WIDTH - 262.f,
                12.f
            },
            {250.f, 176.f}
        );

        drawText(
            window,
            font,
            "CONTACT TRACK",
            13,
            {
                WINDOW_WIDTH - 252.f,
                20.f
            },
            sf::Color(120, 225, 205)
        );

        if (
            selectedContact >= 0 &&
            static_cast<std::size_t>(selectedContact) <
                contacts.size()
        ) {
            const Contact& contact =
                contacts[
                    static_cast<std::size_t>(
                        selectedContact
                    )
                ];

            const bool trackFresh =
                simulationTime -
                    contact.lastRadarHit <= TRACK_MEMORY;

            const float range = distanceBetween(
                player.position,
                contact.position
            );

            const float bearing = bearingTo(
                player.position,
                contact.position
            );

            drawText(
                window,
                font,
                "ID     C" +
                    std::to_string(contact.id) +
                    " / " +
                    contact.callsign,
                11,
                {
                    WINDOW_WIDTH - 252.f,
                    46.f
                }
            );

            drawText(
                window,
                font,
                "TYPE   " +
                    std::string(
                        contact.friendly
                            ? "FRIENDLY"
                            : "UNKNOWN"
                    ),
                12,
                {
                    WINDOW_WIDTH - 252.f,
                    66.f
                },
                contact.friendly
                    ? sf::Color(90, 235, 170)
                    : sf::Color(255, 125, 110)
            );

            drawText(
                window,
                font,
                "RANGE  " + number(range, 1),
                12,
                {
                    WINDOW_WIDTH - 252.f,
                    87.f
                }
            );

            drawText(
                window,
                font,
                "BRG    " +
                    number(bearing) +
                    " deg",
                12,
                {
                    WINDOW_WIDTH - 252.f,
                    107.f
                }
            );

            drawText(
                window,
                font,
                "HDG    " +
                    number(vectorHeading(contact.velocity)) +
                    " deg",
                12,
                {
                    WINDOW_WIDTH - 252.f,
                    127.f
                }
            );

            drawText(
                window,
                font,
                "SPD    " +
                    number(vectorSpeed(contact.velocity)) +
                    " u/s",
                12,
                {
                    WINDOW_WIDTH - 252.f,
                    147.f
                }
            );

            drawText(
                window,
                font,
                "ALT    " +
                    number(contact.altitude) +
                    " ft*",
                12,
                {
                    WINDOW_WIDTH - 252.f,
                    167.f
                }
            );

            drawText(
                window,
                font,
                trackFresh ? "LIVE" : "STALE",
                10,
                {
                    WINDOW_WIDTH - 65.f,
                    167.f
                },
                trackFresh
                    ? sf::Color(100, 230, 150)
                    : sf::Color(255, 150, 100)
            );
        } else {
            drawText(
                window,
                font,
                "No contact selected.",
                12,
                {
                    WINDOW_WIDTH - 252.f,
                    50.f
                }
            );

            drawText(
                window,
                font,
                "Click a detected symbol",
                12,
                {
                    WINDOW_WIDTH - 252.f,
                    72.f
                }
            );

            drawText(
                window,
                font,
                "to inspect its track.",
                12,
                {
                    WINDOW_WIDTH - 252.f,
                    92.f
                }
            );
        }

        // ----------------------------------------------------
        // COMPASS
        // ----------------------------------------------------

        const sf::Vector2f compassCenter{
            WINDOW_WIDTH - 55.f,
            MAP_HEIGHT - 65.f
        };

        sf::CircleShape compass(31.f);

        compass.setOrigin({
            31.f,
            31.f
        });

        compass.setPosition(compassCenter);

        compass.setFillColor(
            sf::Color(4, 15, 21, 180)
        );

        compass.setOutlineColor(
            sf::Color(55, 120, 125)
        );

        compass.setOutlineThickness(1.f);

        window.draw(compass);

        drawText(
            window, font, "N", 11,
            {
                compassCenter.x - 4.f,
                compassCenter.y - 43.f
            }
        );

        drawText(
            window, font, "S", 11,
            {
                compassCenter.x - 4.f,
                compassCenter.y + 29.f
            }
        );

        drawText(
            window, font, "W", 11,
            {
                compassCenter.x - 43.f,
                compassCenter.y - 7.f
            }
        );

        drawText(
            window, font, "E", 11,
            {
                compassCenter.x + 33.f,
                compassCenter.y - 7.f
            }
        );

        const sf::Vector2f compassPointer{
            compassCenter.x +
                std::sin(toRadians(player.heading)) * 23.f,

            compassCenter.y -
                std::cos(toRadians(player.heading)) * 23.f
        };

        drawLine(
            window,
            compassCenter,
            compassPointer,
            sf::Color(255, 210, 80),
            2.f
        );

        // ----------------------------------------------------
        // EVENT LOG
        // ----------------------------------------------------

        drawPanel(
            window,
            {
                0.f,
                static_cast<float>(MAP_HEIGHT)
            },
            {
                static_cast<float>(WINDOW_WIDTH),
                static_cast<float>(
                    WINDOW_HEIGHT - MAP_HEIGHT
                )
            }
        );

        drawText(
            window,
            font,
            "EVENT LOG",
            12,
            {12.f, 707.f},
            sf::Color(120, 225, 205)
        );

        for (
            std::size_t i = 0;
            i < eventLog.size();
            ++i
        ) {
            drawText(
                window,
                font,
                eventLog[i],
                11,
                {
                    130.f,
                    706.f + static_cast<float>(i) * 16.f
                },
                i == 0
                    ? sf::Color(225, 235, 235)
                    : sf::Color(130, 155, 160)
            );
        }

        drawText(
            window,
            font,
            "Arrows: steer/speed | A: autopilot | R: radar | "
            "C: clear WP | Home: cruise | L-click: waypoint/contact | Esc: exit",
            9,
            {12.f, 792.f},
            sf::Color(105, 135, 140)
        );

        drawText(
            window,
            font,
            "*Training values; not real-world measurements",
            9,
            {
                WINDOW_WIDTH - 250.f,
                778.f
            },
            sf::Color(105, 135, 140)
        );

        window.display();
    }

    return 0;
}