#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <deque>
#include <unordered_set>
#include <optional>
#include <filesystem>
#include <csignal>
#include <fstream>
#include "Config.hpp"
#include "MathUtils.hpp"
#include "Aircraft.hpp"
#include "Contact.hpp"
#include "Radar.hpp"
#include "HUD.hpp"
#include "AudioVisual.hpp"
#include "Weapons.hpp"

static void crashHandler(int sig) {
    std::ofstream log("/tmp/jet-sim.log", std::ios::app);
    log << "[FATAL] Process received signal: " << sig << std::endl;
    std::exit(sig);
}

int main(int argc, char* argv[]) {
    // Ignore terminal hangup and broken pipes so closing parent shell does not force-close simulator
    std::signal(SIGHUP, SIG_IGN);
    std::signal(SIGPIPE, SIG_IGN);
    std::signal(SIGSEGV, crashHandler);
    std::signal(SIGFPE, crashHandler);
    std::signal(SIGABRT, crashHandler);

    try {
        sf::RenderWindow window(
        sf::VideoMode({
            Config::WINDOW_WIDTH,
            Config::WINDOW_HEIGHT
        }),
        "F-22 Tactical Map Simulator [Modernized]"
    );

    window.setFramerateLimit(60);

    const std::filesystem::path executableDir =
        (argc > 0 && argv[0] != nullptr)
            ? std::filesystem::absolute(std::filesystem::path(argv[0])).parent_path()
            : std::filesystem::current_path();

    const std::vector<std::filesystem::path> assetDirs = {
        executableDir,
        std::filesystem::current_path(),
        executableDir / "assets",
        std::filesystem::current_path() / "assets",
        "/usr/share/fonts/truetype/dejavu"
    };

    // --------------------------------------------------------
    // ASSET LOADING: F-22 TEXTURE
    // --------------------------------------------------------
    sf::Texture jetTexture;
    const std::filesystem::path jetTexturePath =
        MathUtils::resolveAssetPath("f22.png", assetDirs);

    if (!jetTexture.loadFromFile(jetTexturePath.string())) {
        std::cerr << "Error: Could not load " << jetTexturePath.string() << ".\n";
        return 1;
    }

    jetTexture.setSmooth(true);
    sf::Sprite jetSprite(jetTexture);
    const sf::Vector2u textureSize = jetTexture.getSize();
    jetSprite.setOrigin({
        static_cast<float>(textureSize.x) / 2.f,
        static_cast<float>(textureSize.y) / 2.f
    });
    jetSprite.setScale({0.12f, 0.12f});

    // --------------------------------------------------------
    // ASSET LOADING: FONT
    // --------------------------------------------------------
    sf::Font font;
    const std::filesystem::path fontPath =
        MathUtils::resolveAssetPath("DejaVuSans.ttf", assetDirs);

    if (!font.openFromFile(fontPath.string())) {
        std::cerr << "Error: Could not load " << fontPath.string() << ".\n";
        return 1;
    }

    // --------------------------------------------------------
    // SIMULATION STATE & COMBAT SYSTEMS
    // --------------------------------------------------------
    Aircraft player;
    RadarSystem radar;
    SAMSite samSite{{710.f, 560.f}, 210.f, 135.f, true, "SA-10 GRUMBLE", 0.f};

    ParticleSystem particles;
    AudioManager audio;
    audio.init();

    WeaponsSystem weapons;

    std::vector<Contact> contacts = {
        {{650.f, 300.f}, {-35.f, 20.f}, 1, false, "VIPER 1", 0.f, 180.f, 18000.f},
        {{200.f, 180.f}, {25.f, 30.f},  2, true,  "EAGLE 2", 0.f, 200.f, 22000.f},
        {{350.f, 550.f}, {15.f, -25.f}, 3, false, "BANDIT 3", 0.f, 210.f, 15000.f},
        {{730.f, 460.f}, {-18.f, -22.f}, 4, true, "HAWK 4",  0.f, 220.f, 28000.f}
    };

    std::deque<std::string> eventLog;
    eventLog.push_front("SIMULATION INITIALIZED - COMBAT ORDNANCE ARMED");
    eventLog.push_front("ARMAMENT: 4x AIM-120D AMRAAM, 2x AIM-9X");
    eventLog.push_front("RADAR SEARCH ACTIVE");

    sf::Clock frameClock;
    float simulationTime = 0.f;
    bool autoNavigation = true;
    std::optional<sf::Vector2f> waypoint;
    int selectedContact = -1;

    std::unordered_set<sf::Keyboard::Key> pressedActionKeys;

    bool prevStallState = false;
    bool prevOverGState = false;
    bool prevBingoState = false;
    bool prevSAMThreat = false;

    // ========================================================
    // MAIN LOOP
    // ========================================================
    while (window.isOpen()) {
        const float dt = std::min(frameClock.restart().asSeconds(), 0.05f);
        simulationTime += dt;

        // ----------------------------------------------------
        // EVENTS
        // ----------------------------------------------------
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }

                // Throttle Stage Selectors: 1, 2, 3, 4
                if (key->code == sf::Keyboard::Key::Num1) {
                    player.setThrottleStage(ThrottleStage::IDLE);
                    eventLog.push_front("POWER: IDLE (120 KT)");
                    while (eventLog.size() > 6) eventLog.pop_back();
                }

                if (key->code == sf::Keyboard::Key::Num2) {
                    player.setThrottleStage(ThrottleStage::CRUISE);
                    eventLog.push_front("POWER: CRUISE (280 KT)");
                    while (eventLog.size() > 6) eventLog.pop_back();
                }

                if (key->code == sf::Keyboard::Key::Num3) {
                    player.setThrottleStage(ThrottleStage::MIL);
                    eventLog.push_front("POWER: MILITARY POWER (500 KT)");
                    while (eventLog.size() > 6) eventLog.pop_back();
                }

                if (key->code == sf::Keyboard::Key::Num4 || key->code == sf::Keyboard::Key::B) {
                    if (pressedActionKeys.insert(key->code).second) {
                        if (player.throttleStage == ThrottleStage::AFTERBURNER) {
                            player.setThrottleStage(ThrottleStage::MIL);
                            eventLog.push_front("AFTERBURNER DISENGAGED -> MIL POWER");
                        } else {
                            player.setThrottleStage(ThrottleStage::AFTERBURNER);
                            eventLog.push_front("AFTERBURNER ENGAGED -> STAGE 4");
                        }
                        while (eventLog.size() > 6) eventLog.pop_back();
                    }
                }

                // Weapon Selection Toggle: W
                if (key->code == sf::Keyboard::Key::W) {
                    if (pressedActionKeys.insert(key->code).second) {
                        weapons.cycleWeapon();
                        eventLog.push_front("WEAPON SELECTED: " + weapons.getSelectedWeaponName());
                        while (eventLog.size() > 6) eventLog.pop_back();
                    }
                }

                // Fire Missile: Space
                if (key->code == sf::Keyboard::Key::Space) {
                    if (pressedActionKeys.insert(key->code).second) {
                        weapons.launchMissile(
                            player.position,
                            player.heading,
                            selectedContact,
                            contacts,
                            eventLog,
                            audio
                        );
                    }
                }

                // Countermeasure Flares: F
                if (key->code == sf::Keyboard::Key::F) {
                    if (pressedActionKeys.insert(key->code).second) {
                        particles.emitFlares(player.position, player.heading);
                        audio.playFlarePop();
                        eventLog.push_front("COUNTERMEASURES: FLARES DISCHARGED");
                        while (eventLog.size() > 6) eventLog.pop_back();
                    }
                }

                if (key->code == sf::Keyboard::Key::R) {
                    if (pressedActionKeys.insert(key->code).second) {
                        radar.visible = !radar.visible;
                        eventLog.push_front(radar.visible ? "RADAR SEARCH ENABLED" : "RADAR SEARCH DISABLED");
                        while (eventLog.size() > 6) eventLog.pop_back();
                    }
                }

                if (key->code == sf::Keyboard::Key::A) {
                    if (pressedActionKeys.insert(key->code).second) {
                        autoNavigation = !autoNavigation;
                        eventLog.push_front(autoNavigation ? "AUTO NAVIGATION ENABLED" : "AUTO NAVIGATION DISABLED");
                        while (eventLog.size() > 6) eventLog.pop_back();
                    }
                }

                if (key->code == sf::Keyboard::Key::C) {
                    if (pressedActionKeys.insert(key->code).second) {
                        waypoint.reset();
                        eventLog.push_front("WAYPOINT CLEARED");
                        while (eventLog.size() > 6) eventLog.pop_back();
                    }
                }

                if (key->code == sf::Keyboard::Key::Home) {
                    if (pressedActionKeys.insert(key->code).second) {
                        player.setThrottleStage(ThrottleStage::CRUISE);
                        eventLog.push_front("CRUISE SPEED SELECTED");
                        while (eventLog.size() > 6) eventLog.pop_back();
                    }
                }
            }

            // Mouse Click: Target Selection or Waypoint
            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left && mouse->position.y < static_cast<int>(Config::MAP_HEIGHT)) {
                    const sf::Vector2f click = window.mapPixelToCoords(mouse->position);
                    int clickedContact = -1;
                    float closestDistance = 17.f;

                    for (std::size_t i = 0; i < contacts.size(); ++i) {
                        const bool detected = (simulationTime - contacts[i].lastRadarHit <= Config::TRACK_MEMORY);
                        if (!detected) continue;

                        const float d = MathUtils::distanceBetween(click, contacts[i].position);
                        if (d <= 17.f && (clickedContact < 0 || d < closestDistance)) {
                            closestDistance = d;
                            clickedContact = static_cast<int>(i);
                        }
                    }

                    if (clickedContact >= 0) {
                        selectedContact = clickedContact;
                        eventLog.push_front("TRACK LOCKED: " + contacts[static_cast<std::size_t>(selectedContact)].callsign + " (STT ACTIVE)");
                        while (eventLog.size() > 6) eventLog.pop_back();
                    } else {
                        waypoint = click;
                        selectedContact = -1;
                        eventLog.push_front("WAYPOINT SET: " + MathUtils::number(click.x) + "," + MathUtils::number(click.y));
                        while (eventLog.size() > 6) eventLog.pop_back();
                    }
                }
            }
        }

        // Clean released action keys
        for (auto it = pressedActionKeys.begin(); it != pressedActionKeys.end();) {
            if (!sf::Keyboard::isKeyPressed(*it)) {
                it = pressedActionKeys.erase(it);
            } else {
                ++it;
            }
        }

        // ----------------------------------------------------
        // MANUAL FLIGHT CONTROLS
        // ----------------------------------------------------
        const bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
        const bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
        const bool upPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
        const bool downPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);

        float commandedTurnRate = 0.f;

        if (leftPressed || rightPressed) {
            const float turnInput = (rightPressed ? 1.f : 0.f) - (leftPressed ? 1.f : 0.f);
            if (turnInput != 0.f && autoNavigation) {
                autoNavigation = false;
                eventLog.push_front("MANUAL TURN: AUTOPILOT DISENGAGED");
                while (eventLog.size() > 6) eventLog.pop_back();
            }

            float turnAuthority = 80.f;
            if (player.isStalled) {
                turnAuthority = 32.f;
            }
            commandedTurnRate = turnInput * turnAuthority;
            player.heading = MathUtils::normalizeAngle(player.heading + commandedTurnRate * dt);
        }

        if (upPressed) {
            player.targetSpeed = std::min(880.f, player.targetSpeed + 120.f * dt);
        }

        if (downPressed) {
            player.targetSpeed = std::max(100.f, player.targetSpeed - 120.f * dt);
        }

        // ----------------------------------------------------
        // UPDATE SIMULATION
        // ----------------------------------------------------
        player.update(dt, autoNavigation, waypoint, commandedTurnRate);
        player.updateTrail(dt);

        // Visual Particles Update
        if (player.throttleStage == ThrottleStage::AFTERBURNER) {
            particles.emitAfterburner(player.position, player.heading, true);
        } else if (player.speed > 350.f) {
            particles.emitAfterburner(player.position, player.heading, false);
        }

        particles.emitWingtipVapor(player.position, player.heading, player.gLoad);
        particles.update(dt);

        // Combat Ordnance Physics
        weapons.update(dt, contacts, particles, eventLog, selectedContact);

        // Check avionics state transitions for alerts
        if (player.isStalled && !prevStallState) {
            eventLog.push_front("ALERT: STALL WARNING - RECOVER ATTITUDE");
            while (eventLog.size() > 6) eventLog.pop_back();
        }
        prevStallState = player.isStalled;

        if (player.isOverG && !prevOverGState) {
            eventLog.push_front("WARNING: OVER-G EXCEEDED 9.0G");
            while (eventLog.size() > 6) eventLog.pop_back();
        }
        prevOverGState = player.isOverG;

        if (player.isBingoFuel && !prevBingoState) {
            eventLog.push_front("WARNING: BINGO FUEL - RTB REQUIRED");
            while (eventLog.size() > 6) eventLog.pop_back();
        }
        prevBingoState = player.isBingoFuel;

        // Check SAM Threat Zone
        const float distToSAM = MathUtils::distanceBetween(player.position, samSite.position);
        const bool inSAMThreat = (distToSAM <= samSite.lethalRange);
        if (inSAMThreat && !prevSAMThreat) {
            eventLog.push_front("THREAT WARNING: ENTERED SA-10 LETHAL ENVELOPE!");
            while (eventLog.size() > 6) eventLog.pop_back();
        }
        prevSAMThreat = inSAMThreat;

        for (std::size_t i = 0; i < contacts.size(); ++i) {
            const bool isSelected = (selectedContact == static_cast<int>(i));
            contacts[i].update(dt, simulationTime, player.position, player.heading, isSelected);
        }

        radar.update(dt, simulationTime, player, contacts, selectedContact, eventLog);

        // ----------------------------------------------------
        // TACTICAL AUDIO SYSTEM UPDATE
        // ----------------------------------------------------
        const bool hasHostileLock = (selectedContact >= 0 &&
            static_cast<std::size_t>(selectedContact) < contacts.size() &&
            !contacts[static_cast<std::size_t>(selectedContact)].friendly);

        audio.setLockTone(hasHostileLock);
        audio.setSAMWarning(inSAMThreat);
        audio.setStallAlert(player.isStalled);

        // ----------------------------------------------------
        // RENDER
        // ----------------------------------------------------
        HUD::renderAll(
            window,
            font,
            jetSprite,
            player,
            contacts,
            samSite,
            particles,
            weapons,
            selectedContact,
            radar.sweepAngle,
            radar.visible,
            autoNavigation,
            waypoint,
            eventLog,
            simulationTime
        );
        }
    } catch (const std::exception& e) {
        std::ofstream log("/tmp/jet-sim.log", std::ios::app);
        log << "[EXCEPTION] " << e.what() << std::endl;
        std::cerr << "CRITICAL ERROR: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::ofstream log("/tmp/jet-sim.log", std::ios::app);
        log << "[UNKNOWN EXCEPTION] An unknown crash occurred." << std::endl;
        std::cerr << "CRITICAL ERROR: Unknown exception caught." << std::endl;
        return 2;
    }

    return 0;
}
