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
#include "attacks.h"
#include"eval_defines.h"
#include"zobrist.h"


namespace engine {
    /* This class manages the internal state of the chess board.*/

    class Board {
    private:

        /* Structure that holds the board state.*/
        struct BoardData {
            /*The piece-placement bitboards.*/
            u64 placement[C_NUM_PIECES];

            /*All-piece bitboard.*/
            u64 all;

            /* An array used to retrieve piece type information. */;
            Piece pieces[C_NUM_SQUARES];
        };

        /* Structure that holds the irreversible board state. Each move makes a copy of this structure.*/
        struct BoardState {
            /* Holds the castling and side to move information. */
            u8 state;

            /* If the last move was a double pawn push, remember the en passant target square.*/
            Square ep_square;

            /*The zobrist key of the current position.*/
            u64 zobrist_key;
        };

        /* Keep an array of irreversible states, to easily undo moves.*/
        BoardState history[C_MAX_PLY];

        /* Adds the piece to the given square. */
        void set_piece_at(Square square, Piece piece);

        /* Removes the piece from the given square.*/
        void remove_piece_at(Square square, Piece piece);

        /* Executes a quiet move. */
        void make_quiet_move(const Move &move);

        /* Reverts a quiet move. */
        void undo_quiet_move(const Move &move);

        /*Executes a capture move.*/
        void make_capture_move(const Move &move);

        /*Reverts a capture move.*/
        void undo_capture_move(const Move &move);

        /*Executes a promotion move.*/
        void make_promotion_move(const Move &move);

        /*Reverts a promotion move.*/
        void undo_promotion_move(const Move &move);

        /*Executes a promotion-capture move.*/
        void make_promotion_capture_move(const Move &move);

        /*Reverts a promotion-capture move.*/
        void undo_promotion_capture_move(const Move &move);

        /*Executes an en-passant move.*/
        void make_en_passant_move(const Move &move);

        /*Reverts an en-passant move.*/
        void undo_en_passant_move(const Move &move);

        /*Executes a kingside-castle move.*/
        void make_kingside_castle_move(const Move &move);

        /*Reverts a kingside-castle move.*/
        void undo_kingside_castle_move(const Move &move);

        /*Executes a queenside-castle move.*/
        void make_queenside_castle_move(const Move &move);

        /*Reverts a queenside-castle move.*/
        void undo_queenside_castle_move(const Move &move);

        /*Updates en passant flag and target square.*/
        void update_en_passant_status(const Move &move);

        /*Updates castling rights affected by the move.*/
        void update_castling_status(const Move &move);

        /*Updates the status flags affected by the move.*/
        void update_status(const Move &move);

    public:
        /* The current position's data. */
        BoardData current_position;

        /* A pointer to the current state of the position.*/
        BoardState *current_state;

        /* Parse the fen string and construct the board representation.*/
        void load_fen(const std::string &fen);

        /* Makes a move, updating the internal state accordingly. */
        void make_move(const Move &move);

        /* Reverts a move, updating the internal state accordingly.*/
        void undo_move(const Move &move);

        /* Returns the current color to move.*/
        Color color_to_play() const;

        /*Returns the smallest attacker of the given square, of the given side.
         *It also returns the square of the attacker through the "attacker" parameter.*/
        PieceType get_smallest_attacker(Square square, Color side, Square &attacker) const;

        /*Statically evaluates captures on the given square, and returns the score for the given side.*/
        int see(Square square, Color side);

        /*Statically evaluates a capture.*/
        int see(const Move &capture, Color side);

        Board();
    };

    /*Outputs a text based board representation.*/
    std::ostream &operator<<(std::ostream &stream, const Board &board);
}


#endif //CHESSENGINE_BOARD_H
