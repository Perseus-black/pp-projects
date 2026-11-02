#pragma once

#include "Aircraft.hpp"
#include "Contact.hpp"
#include <vector>
#include <deque>
#include <string>

class RadarSystem {
public:
    float sweepAngle = 0.f;
    bool visible = true;

    void update(
        float dt,
        float simulationTime,
        const Aircraft& player,
        std::vector<Contact>& contacts,
        int& selectedContact,
        std::deque<std::string>& eventLog
    );
};
