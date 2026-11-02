#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>

struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float life = 0.f;
    float maxLife = 1.f;
    float size = 2.f;
};

class ParticleSystem {
public:
    std::vector<Particle> particles;

    void emitAfterburner(
        const sf::Vector2f& jetPos,
        float jetHeading,
        bool isFullBurner
    );

    void emitWingtipVapor(
        const sf::Vector2f& jetPos,
        float jetHeading,
        float gLoad
    );

    void emitFlares(
        const sf::Vector2f& jetPos,
        float jetHeading
    );

    void emitMissileSmoke(
        const sf::Vector2f& pos,
        const sf::Vector2f& vel
    );

    void emitExplosion(
        const sf::Vector2f& pos
    );

    void update(float dt);
    void draw(sf::RenderWindow& window);
};

class AudioManager {
public:
    AudioManager();
    ~AudioManager() = default;

    void init();

    void setLockTone(bool active);
    void setSAMWarning(bool active);
    void setStallAlert(bool active);
    void playFlarePop();
    void playMissileLaunch();

private:
    sf::SoundBuffer lockToneBuf;
    sf::SoundBuffer samWarningBuf;
    sf::SoundBuffer stallAlertBuf;
    sf::SoundBuffer flarePopBuf;
    sf::SoundBuffer missileLaunchBuf;

    std::unique_ptr<sf::Sound> lockToneSound;
    std::unique_ptr<sf::Sound> samWarningSound;
    std::unique_ptr<sf::Sound> stallAlertSound;
    std::unique_ptr<sf::Sound> flarePopSound;
    std::unique_ptr<sf::Sound> missileLaunchSound;

    bool initialized = false;

    void generateSineTone(sf::SoundBuffer& buffer, float freq1, float freq2, float duration, float pulseRate);
    void generateNoiseBurst(sf::SoundBuffer& buffer, float duration);
    void generateRocketRoar(sf::SoundBuffer& buffer, float duration);
};
