//
// Created by gvisan on 27.08.2021.
//

#include "history.h"

namespace engine {
    History game_history;

    History::History() : game_history(), ply(0) {

    }

    void History::push_position(u64 key) {
        game_history[ply] = key;
        ply++;
    }

    void History::pop_position() {
        ply--;
    }

    void History::clear() {
        ply = 0;
    }

    bool History::is_repetition() const {
        if (ply == 0)
            return false;

        u64 current_position = game_history[ply - 1];
        int current_ply = ply - 3;

        while (current_ply >= 0) {
            if (game_history[current_ply] == current_position)
                return true;
            current_ply -= 2;
        }
        return false;
    }

    int History::get_ply() const {
        return ply;
    }

}