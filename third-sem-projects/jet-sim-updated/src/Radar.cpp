#include "Radar.hpp"
#include "Config.hpp"
#include "MathUtils.hpp"
#include <cmath>

void RadarSystem::update(
    float dt,
    float simulationTime,
    const Aircraft& player,
    std::vector<Contact>& contacts,
    int& selectedContact,
    std::deque<std::string>& eventLog
) {
    sweepAngle = MathUtils::normalizeAngle(sweepAngle + Config::SWEEP_SPEED * dt);

    for (Contact& contact : contacts) {
        const float range = MathUtils::distanceBetween(player.position, contact.position);
        const float bearing = MathUtils::bearingTo(player.position, contact.position);

        const bool insideRange = (range <= Config::RADAR_RANGE);
        const bool sweepCrossing = (std::abs(MathUtils::angleDifference(sweepAngle, bearing)) <= 3.5f);

        if (visible && insideRange && sweepCrossing) {
            contact.lastRadarHit = simulationTime;
            contact.lastRadarPosition = contact.position;

            if (!contact.radarTrackActive) {
                eventLog.push_front("RADAR CONTACT: " + contact.callsign + " ACQUIRED");
                while (eventLog.size() > 6) {
                    eventLog.pop_back();
                }
                contact.radarTrackActive = true;
            }
        }

        // Expire tracks that have not been refreshed.
        if (contact.radarTrackActive && (simulationTime - contact.lastRadarHit > Config::TRACK_MEMORY)) {
            eventLog.push_front("RADAR TRACK LOST: " + contact.callsign);
            while (eventLog.size() > 6) {
                eventLog.pop_back();
            }

            contact.radarTrackActive = false;

            if (selectedContact >= 0 &&
                static_cast<std::size_t>(selectedContact) < contacts.size() &&
                contacts[static_cast<std::size_t>(selectedContact)].id == contact.id) {
                selectedContact = -1;
            }
        }
    }
}
