//
// Created by gvisan on 11.07.2021.
//

#ifndef CHESSENGINE_MOVEGEN_H
#define CHESSENGINE_MOVEGEN_H

#include "board.h"
#include"attacks.h"
#include<vector>

namespace engine {
    /* This class is responsible for generating moves for a given configuration of the board.*/
    class MoveGen {
    private:
        /*The move buffer.*/
        std::vector<Move> moves;

        /*The legal move buffer.*/
        std::vector<Move> legal_moves;

        /* The internal board.*/
        Board &board;

        /* Generates all quiet moves for the given attack bitboard. */
        void add_quiet_moves(Square origin, u64 attacks, Piece piece);

        /* Generates all capture moves for the given attack bitboard. */
        void add_capture_moves(Square origin, u64 attacks, Piece piece);

        /* Generates all king moves for the given side. */
        template<Color side>
        void add_king_moves(u64 opposite, u64 all);

        /* Generates all knight moves for the given side.*/
        template<Color side>
        void add_knight_moves(u64 opposite, u64 all);

        /*Generates all rook moves for the given side. */
        template<Color side>
        void add_rook_moves(u64 opposite, u64 all);

        /*Generates all bishop moves for the given side.*/
        template<Color side>
        void add_bishop_moves(u64 opposite, u64 all);

        /*Generates all queen moves for the given side.*/
        template<Color side>
        void add_queen_moves(u64 opposite, u64 all);

        /*Generates single and double pawn pushes for white.*/
        void add_white_pawn_pushes(u64 all);

        /*Generates single and double pawn pushes for black.*/
        void add_black_pawn_pushes(u64 all);

        /* Generates all pawn captures for white. */
        void add_white_pawn_captures(u64 black);

        /* Generates all pawn captures for black. */
        void add_black_pawn_captures(u64 white);

        /* Generates all en passant moves for white.*/
        void add_white_en_passant_moves();

        /* Generates all en passant moves for black.*/
        void add_black_en_passant_moves();

        /*Generates all pawn moves for white. */
        void add_white_pawn_moves(u64 black, u64 all);

        /*Generates all pawn moves for black. */
        void add_black_pawn_moves(u64 white, u64 all);

        /*Checks if the given side can attack the given square.*/
        template<Color side>
        bool can_attack_square(Square square, u64 all);

        /* Adds all white castling moves.*/
        void add_white_castling_moves(u64 all);

        /* Adds all black castling moves.*/
        void add_black_castling_moves(u64 all);

        /*Checks move for legality by making and then unmaking it, seeing if it leaves the king in check.*/
        bool is_legal(const Move &move, Color color);

        /*Returns the pinned pieces bitboard for the given color.*/
        u64 get_pinned_pieces(Color color, u64 white, u64 black);

    public:
        explicit MoveGen(Board &internal_board);

        /* Returns all legal moves of the current position.*/
        std::vector<Move> get_moves();

        /*Returns the number of leaf nodes of the tree with the given depth.*/
        unsigned perft(unsigned depth);

        /*Checks if the given color is in check.*/
        bool is_in_check(Color color);
    };
}


#endif //CHESSENGINE_MOVEGEN_H
