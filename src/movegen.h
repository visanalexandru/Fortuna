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
        /* The internal board.*/
        Board &board;

        /* Generates all quiet moves for the given attack bitboard. */
        void add_quiet_moves(Square origin, u64 attacks, Piece piece, std::vector<Move> &moves);

        /* Generates all capture moves for the given attack bitboard. */
        void add_capture_moves(Square origin, u64 attacks, Piece piece, std::vector<Move> &moves);

        /* Generates all king moves for white. */
        void add_white_king_moves(u64 black, u64 all, std::vector<Move> &moves);

        /* Generates all king moves for black. */
        void add_black_king_moves(u64 white, u64 all, std::vector<Move> &moves);

        /* Generates all knight moves for white.*/
        void add_white_knight_moves(u64 black, u64 all, std::vector<Move> &moves);

        /*Generates all knight moves for black.*/
        void add_black_knight_moves(u64 white, u64 all, std::vector<Move> &moves);

        /*Generates single and double pawn pushes for white.*/
        void add_white_pawn_pushes(u64 all, std::vector<Move> &moves);

        /*Generates single and double pawn pushes for black.*/
        void add_black_pawn_pushes(u64 all, std::vector<Move> &moves);

        /* Generates all pawn captures for white. */
        void add_white_pawn_captures(u64 black, std::vector<Move> &moves);

        /* Generates all pawn captures for black. */
        void add_black_pawn_captures(u64 white, std::vector<Move> &moves);

        /* Generates all en passant moves for white.*/
        void add_white_en_passant_moves(std::vector<Move> &moves);

        /* Generates all en passant moves for black.*/
        void add_black_en_passant_moves(std::vector<Move> &moves);

        /*Generates all pawn moves for white. */
        void add_white_pawn_moves(u64 black, u64 all, std::vector<Move> &moves);

        /*Generates all pawn moves for black. */
        void add_black_pawn_moves(u64 white, u64 all, std::vector<Move> &moves);

        /*Generates all rook moves for white. */
        void add_white_rook_moves(u64 black, u64 all, std::vector<Move> &moves);

        /*Generates all rook moves for black. */
        void add_black_rook_moves(u64 white, u64 all, std::vector<Move> &moves);

        /*Generates all bishop moves for white.*/
        void add_white_bishop_moves(u64 black, u64 all, std::vector<Move> &moves);

        /*Generates all bishop moves for black.*/
        void add_black_bishop_moves(u64 white, u64 all, std::vector<Move> &moves);

        /*Generates all queen moves for white.*/
        void add_white_queen_moves(u64 black, u64 all, std::vector<Move> &moves);

        /*Generates all queen moves for black.*/
        void add_black_queen_moves(u64 white, u64 all, std::vector<Move> &moves);

        /*Checks if white can attack the given square.*/
        bool can_white_attack_square(Square square, u64 all);

        /*Checks if black can attack the given square.*/
        bool can_black_attack_square(Square square, u64 all);

        /* Adds all white castling moves.*/
        void add_white_castling_moves(u64 all, std::vector<Move> &moves);

        /* Adds all black castling moves.*/
        void add_black_castling_moves(u64 all, std::vector<Move> &moves);

    public:
        explicit MoveGen(Board &internal_board);

        /* Returns all legal moves of the current position.*/
        std::vector<Move> get_moves();

        /*Returns the number of leaf nodes of the tree with the given depth.*/
        unsigned perft(unsigned depth);

    };
}


#endif //CHESSENGINE_MOVEGEN_H
