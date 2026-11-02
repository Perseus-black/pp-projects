#include "AudioVisual.hpp"
#include "Config.hpp"
#include "MathUtils.hpp"
#include <cmath>
#include <random>
#include <algorithm>

// ============================================================
// PARTICLE SYSTEM
// ============================================================

void ParticleSystem::emitAfterburner(
    const sf::Vector2f& jetPos,
    float jetHeading,
    bool isFullBurner
) {
    const float rad = MathUtils::toRadians(jetHeading);
    const sf::Vector2f forward{std::sin(rad), -std::cos(rad)};
    const sf::Vector2f lateral{std::cos(rad), std::sin(rad)};

    // Twin engine nozzle positions
    const sf::Vector2f nozzle1 = jetPos - forward * 16.f - lateral * 4.2f;
    const sf::Vector2f nozzle2 = jetPos - forward * 16.f + lateral * 4.2f;

    static std::mt19937 rng(42);
    std::uniform_real_distribution<float> jitterDist(-15.f, 15.f);
    std::uniform_real_distribution<float> lifeDist(0.12f, 0.28f);

    const int count = isFullBurner ? 3 : 1;
    const float baseSpeed = isFullBurner ? 130.f : 65.f;

    for (int i = 0; i < count; ++i) {
        for (const auto& nozzle : {nozzle1, nozzle2}) {
            Particle p;
            p.position = nozzle;
            p.velocity = -forward * baseSpeed + lateral * jitterDist(rng);
            p.maxLife = lifeDist(rng);
            p.life = 0.f;
            p.size = isFullBurner ? 3.5f : 2.0f;

            if (isFullBurner) {
                if (rng() % 3 == 0) {
                    p.color = sf::Color(100, 220, 255, 240); // Cyan core
                } else {
                    p.color = sf::Color(255, 140, 30, 230);  // Afterburner flame
                }
            } else {
                p.color = sf::Color(255, 190, 80, 160);
            }

            particles.push_back(p);
        }
    }
}

void ParticleSystem::emitWingtipVapor(
    const sf::Vector2f& jetPos,
    float jetHeading,
    float gLoad
) {
    if (gLoad < 4.5f)
        return;

    const float rad = MathUtils::toRadians(jetHeading);
    const sf::Vector2f forward{std::sin(rad), -std::cos(rad)};
    const sf::Vector2f lateral{std::cos(rad), std::sin(rad)};

    const sf::Vector2f leftTip = jetPos - forward * 2.f - lateral * 18.f;
    const sf::Vector2f rightTip = jetPos - forward * 2.f + lateral * 18.f;

    for (const auto& tip : {leftTip, rightTip}) {
        Particle p;
        p.position = tip;
        p.velocity = -forward * 20.f;
        p.maxLife = 0.35f;
        p.life = 0.f;
        p.size = 2.5f;
        const auto alpha = static_cast<std::uint8_t>(std::min(140.f, (gLoad - 4.5f) * 35.f));
        p.color = sf::Color(220, 235, 245, alpha);
        particles.push_back(p);
    }
}

void ParticleSystem::emitFlares(
    const sf::Vector2f& jetPos,
    float jetHeading
) {
    const float rad = MathUtils::toRadians(jetHeading);
    const sf::Vector2f forward{std::sin(rad), -std::cos(rad)};
    const sf::Vector2f lateral{std::cos(rad), std::sin(rad)};

    static std::mt19937 rng(1337);
    std::uniform_real_distribution<float> speedDist(50.f, 95.f);
    std::uniform_real_distribution<float> jitterDist(-25.f, 25.f);

    for (int side : {-1, 1}) {
        for (int i = 0; i < 4; ++i) {
            Particle p;
            p.position = jetPos - forward * 10.f + lateral * (static_cast<float>(side) * 5.f);
            p.velocity = lateral * (static_cast<float>(side) * speedDist(rng)) - forward * 25.f + lateral * jitterDist(rng);
            p.maxLife = 1.2f;
            p.life = 0.f;
            p.size = 4.0f;
            p.color = sf::Color(255, 245, 170, 255);
            particles.push_back(p);
        }
    }
}

void ParticleSystem::emitMissileSmoke(
    const sf::Vector2f& pos,
    const sf::Vector2f& vel
) {
    Particle p;
    p.position = pos;
    p.velocity = -vel * 0.12f;
    p.maxLife = 0.7f;
    p.life = 0.f;
    p.size = 2.8f;
    p.color = sf::Color(210, 215, 220, 160);
    particles.push_back(p);
}

void ParticleSystem::emitExplosion(const sf::Vector2f& pos) {
    static std::mt19937 rng(8888);
    std::uniform_real_distribution<float> angleDist(0.f, 360.f);
    std::uniform_real_distribution<float> speedDist(20.f, 130.f);
    std::uniform_real_distribution<float> lifeDist(0.4f, 0.9f);
    std::uniform_real_distribution<float> sizeDist(3.5f, 7.0f);

    // Blast fireball sparks & shockwave particles
    for (int i = 0; i < 36; ++i) {
        const float angle = angleDist(rng) * Config::PI / 180.f;
        const float spd = speedDist(rng);

        Particle p;
        p.position = pos;
        p.velocity = sf::Vector2f{std::cos(angle) * spd, std::sin(angle) * spd};
        p.maxLife = lifeDist(rng);
        p.life = 0.f;
        p.size = sizeDist(rng);

        if (i % 3 == 0) {
            p.color = sf::Color(255, 240, 160, 255); // Flash center
        } else if (i % 3 == 1) {
            p.color = sf::Color(255, 110, 30, 240);  // Fireball
        } else {
            p.color = sf::Color(200, 50, 40, 220);   // Outer burn
        }

        particles.push_back(p);
    }
}

void ParticleSystem::update(float dt) {
    for (auto it = particles.begin(); it != particles.end();) {
        it->life += dt;
        if (it->life >= it->maxLife) {
            it = particles.erase(it);
        } else {
            it->velocity *= (1.f - std::min(1.f, 2.2f * dt));
            it->position += it->velocity * dt;

            const float progress = it->life / it->maxLife;
            const float alphaFade = 1.f - progress;
            it->color.a = static_cast<std::uint8_t>(255.f * alphaFade * 0.85f);
            it->size += 2.0f * dt;
            ++it;
        }
    }
}

void ParticleSystem::draw(sf::RenderWindow& window) {
    sf::CircleShape shape;
    for (const auto& p : particles) {
        shape.setRadius(p.size);
        shape.setOrigin({p.size, p.size});
        shape.setPosition(p.position);
        shape.setFillColor(p.color);
        window.draw(shape);
    }
}

// ============================================================
// AUDIO SYSTEM (PROCEDURAL PCM AUDIO SYNTHESIS)
// ============================================================

AudioManager::AudioManager() = default;

void AudioManager::generateSineTone(
    sf::SoundBuffer& buffer,
    float freq1,
    float freq2,
    float duration,
    float pulseRate
) {
    constexpr unsigned int sampleRate = 44100;
    const std::uint64_t sampleCount = static_cast<std::uint64_t>(sampleRate * duration);
    std::vector<std::int16_t> samples(sampleCount);

    for (std::uint64_t i = 0; i < sampleCount; ++i) {
        const float t = static_cast<float>(i) / sampleRate;

        const float currentFreq = (pulseRate > 0.f)
            ? (std::sin(2.f * Config::PI * pulseRate * t) > 0.f ? freq1 : freq2)
            : freq1;

        const float amp = (pulseRate > 0.f)
            ? (0.5f + 0.5f * std::sin(2.f * Config::PI * pulseRate * t))
            : 1.0f;

        const float val = std::sin(2.f * Config::PI * currentFreq * t) * amp;
        samples[i] = static_cast<std::int16_t>(val * 14000.f);
    }

    (void)buffer.loadFromSamples(samples.data(), sampleCount, 1, sampleRate, {sf::SoundChannel::Mono});
}

void AudioManager::generateNoiseBurst(sf::SoundBuffer& buffer, float duration) {
    constexpr unsigned int sampleRate = 44100;
    const std::uint64_t sampleCount = static_cast<std::uint64_t>(sampleRate * duration);
    std::vector<std::int16_t> samples(sampleCount);

    std::mt19937 rng(777);
    std::uniform_real_distribution<float> noiseDist(-1.0f, 1.0f);

    for (std::uint64_t i = 0; i < sampleCount; ++i) {
        const float t = static_cast<float>(i) / sampleRate;
        const float decay = std::exp(-8.f * t);
        const float val = noiseDist(rng) * decay;
        samples[i] = static_cast<std::int16_t>(val * 22000.f);
    }

    (void)buffer.loadFromSamples(samples.data(), sampleCount, 1, sampleRate, {sf::SoundChannel::Mono});
}

void AudioManager::generateRocketRoar(sf::SoundBuffer& buffer, float duration) {
    constexpr unsigned int sampleRate = 44100;
    const std::uint64_t sampleCount = static_cast<std::uint64_t>(sampleRate * duration);
    std::vector<std::int16_t> samples(sampleCount);

    std::mt19937 rng(999);
    std::uniform_real_distribution<float> noiseDist(-1.0f, 1.0f);

    for (std::uint64_t i = 0; i < sampleCount; ++i) {
        const float t = static_cast<float>(i) / sampleRate;
        const float rumble = std::sin(2.f * Config::PI * 180.f * t) * 0.4f;
        const float whiteNoise = noiseDist(rng) * 0.6f;
        const float envelope = (t < 0.1f) ? (t / 0.1f) : std::exp(-3.5f * (t - 0.1f));

        const float val = (rumble + whiteNoise) * envelope;
        samples[i] = static_cast<std::int16_t>(val * 24000.f);
    }

    (void)buffer.loadFromSamples(samples.data(), sampleCount, 1, sampleRate, {sf::SoundChannel::Mono});
}

void AudioManager::init() {
    if (initialized)
        return;

    // Synthesize Tactical PCM Tones
    generateSineTone(lockToneBuf, 2200.f, 2200.f, 0.5f, 8.f);
    generateSineTone(samWarningBuf, 2800.f, 2000.f, 0.4f, 14.f);
    generateSineTone(stallAlertBuf, 650.f, 650.f, 0.5f, 4.f);
    generateNoiseBurst(flarePopBuf, 0.25f);
    generateRocketRoar(missileLaunchBuf, 0.8f);

    lockToneSound = std::make_unique<sf::Sound>(lockToneBuf);
    lockToneSound->setLooping(true);
    lockToneSound->setVolume(45.f);

    samWarningSound = std::make_unique<sf::Sound>(samWarningBuf);
    samWarningSound->setLooping(true);
    samWarningSound->setVolume(60.f);

    stallAlertSound = std::make_unique<sf::Sound>(stallAlertBuf);
    stallAlertSound->setLooping(true);
    stallAlertSound->setVolume(55.f);

    flarePopSound = std::make_unique<sf::Sound>(flarePopBuf);
    flarePopSound->setVolume(70.f);

    missileLaunchSound = std::make_unique<sf::Sound>(missileLaunchBuf);
    missileLaunchSound->setVolume(85.f);

    initialized = true;
}

void AudioManager::setLockTone(bool active) {
    if (!initialized || !lockToneSound)
        return;

    if (active && lockToneSound->getStatus() != sf::Sound::Status::Playing) {
        lockToneSound->play();
    } else if (!active && lockToneSound->getStatus() == sf::Sound::Status::Playing) {
        lockToneSound->stop();
    }
}

void AudioManager::setSAMWarning(bool active) {
    if (!initialized || !samWarningSound)
        return;

    if (active && samWarningSound->getStatus() != sf::Sound::Status::Playing) {
        samWarningSound->play();
    } else if (!active && samWarningSound->getStatus() == sf::Sound::Status::Playing) {
        samWarningSound->stop();
    }
}

void AudioManager::setStallAlert(bool active) {
    if (!initialized || !stallAlertSound)
        return;

    if (active && stallAlertSound->getStatus() != sf::Sound::Status::Playing) {
        stallAlertSound->play();
    } else if (!active && stallAlertSound->getStatus() == sf::Sound::Status::Playing) {
        stallAlertSound->stop();
    }
}

void AudioManager::playFlarePop() {
    if (!initialized || !flarePopSound)
        return;

    flarePopSound->stop();
    flarePopSound->play();
}

void AudioManager::playMissileLaunch() {
    if (!initialized || !missileLaunchSound)
        return;

    missileLaunchSound->stop();
    missileLaunchSound->play();
}
