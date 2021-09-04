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

        /* Captures and pawn moves are irreversible (they reset the half-move clock), so we can save time in detecting
         * repetition by only considering the positions that were reached after a irreversible move.
         * This array holds for each position in the history the last position index that was reached after a irreversible move.*/
        int last_irreversible[C_MAX_PLY];

        /*The number of half-moves played.*/
        int ply;

    public:
        History();

        /*Adds a new position to the history, usually called when we make a move or start from another position.*/
        /*The first added position must pass a true value to the "irreversible" parameter.*/
        void push_position(u64 key, bool irreversible);

        /*Removes the last position from the history, usually when we undo a move.*/
        void pop_position();

        /*Clears the game history, usually when we start another game.*/
        void clear();

        /*Checks if the current position has occurred before.*/
        bool is_repetition() const;

        /*Returns the number of half-moves played.*/
        int get_ply() const;

        /*Returns the ply of the position reached by the last irreversible move. The starting position, at ply 0 is also considered irreversible.*/
        int get_last_irreversible() const;
    };

    extern History game_history;
}


#endif //CHESSENGINE_HISTORY_H
