//
// Created by gvisan on 10.07.2021.
//

#ifndef CHESSENGINE_BOARD_H
#define CHESSENGINE_BOARD_H

#include<iostream>
#include<string>
#include<sstream>

#include "defines.h"
#include "tables.h"
#include "util.h"
#include "move.h"


namespace engine {
    /* This class manages the internal state of the chess board.*/

    class Board {
    private:

        /* Structure that holds the board state.*/
        struct BoardData {
            /*The piece-placement bitboards.*/
            u64 placement[c_num_pieces];

            /* An array used to retrieve piece type information. */;
            Piece pieces[c_num_squares];
        };

        /* Structure that holds the irreversible board state. Each move makes a copy of this structure.*/
        struct BoardState {
            /* Holds the castling and side to move information. */
            u8 state;

            /* If the last move was a double pawn push, remember the en passant target square.*/
            Square ep_square;
        };

        /* Keep an array of irreversible states, to easily undo moves.*/
        BoardState history[c_max_ply];

        /* Adds the piece to the given square. */
        void set_piece_at(Square square, Piece piece);

        /* Removes the piece from the given square.*/
        void remove_piece_at(Square square, Piece piece);

        /* Executes a quiet move. */
        void make_quiet_move(const Move &move);

        /* Reverts a quiet move. */
        void undo_quiet_move(const Move &move);


    public:
        /* The current position's data. */
        BoardData current_position;

        /* A pointer to the current state of the position.*/
        BoardState *current_state;

        /* Parse the fen string and construct the board representation.*/
        void load_fen(const std::string &fen);

        /* Converts the board to a string. */
        std::string to_string() const;

        /* Makes a move, updating the internal state accordingly. */
        void make_move(const Move &move);

        /* Reverts a move, updating the internal state accordingly.*/
        void undo_move(const Move &move);

        Board();
    };
}


#endif //CHESSENGINE_BOARD_H
