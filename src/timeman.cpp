//
// Created by gvisan on 19.08.2021.
//

#include "timeman.h"

namespace engine {
    double get_time(unsigned white_time, unsigned black_time, unsigned white_increment, unsigned black_increment, Color side) {
        unsigned own_time, own_increment;
        if (side == C_WHITE) {
            own_time = white_time;
            own_increment = white_increment;
        } else {
            own_time = black_time;
            own_increment = black_increment;
        }
        unsigned move_time_ms = (own_time + (own_increment) * (C_EXPECTED_MOVES - 1));
        move_time_ms = move_time_ms / C_EXPECTED_MOVES;

        double move_time = milliseconds_to_seconds(move_time_ms);

        return std::max(move_time - C_EPSILON, C_EPSILON);
    }

    double milliseconds_to_seconds(unsigned milliseconds) {
        return ((double) milliseconds) / 1000.0;
    }

}