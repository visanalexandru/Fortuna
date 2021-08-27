//
// Created by gvisan on 27.08.2021.
//

#ifndef CHESSENGINE_HISTORY_H
#define CHESSENGINE_HISTORY_H

#include"defines.h"

/*This class is used to save the game history, so we can check for repetitions.*/
namespace engine {
    class History {
    private:
        /*The game history buffer, holds zobrist keys of the visited positions.*/
        u64 game_history[C_MAX_PLY];

        /*The number of half-moves played.*/
        int ply;

    public:
        History();

        /*Adds a new position to the history, usually called when we make a move or start from another position.*/
        void push_position(u64 key);

        /*Removes the last position from the history, usually when we undo a move.*/
        void pop_position();

        /*Clears the game history, usually when we start another game.*/
        void clear();

        /*Checks if the current position has occurred before.*/
        bool is_repetition() const;

        /*Returns the number of half-moves played.*/
        int get_ply() const;
    };

    extern History game_history;
}


#endif //CHESSENGINE_HISTORY_H
