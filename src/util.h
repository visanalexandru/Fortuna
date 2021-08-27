//
// Created by gvisan on 10.07.2021.
//

#ifndef CHESSENGINE_UTIL_H
#define CHESSENGINE_UTIL_H

#include"defines.h"
#include<string>

namespace engine {

    /* Converts the given bitboard to a string for debugging purposes.*/
    std::string dump_bitboard(u64 bit);

    /* Converts a fen notation character to the corresponding piece. */
    Piece notation_to_piece(char piece);

    /* Converts a piece to its corresponding fen notation. */
    char piece_to_notation(Piece piece);

    /* Converts a position given by a square to the corresponding bitboard. */
    inline u64 square_to_bitboard(Square square) {
        return 1ull << square;
    }

    /* Converts the given coordinates to the corresponding square. */
    inline Square position_to_square(int file, int rank) {
        return Square((rank) * 8 + file);
    }

    /* Converts a string that describes a target square eg. "e4" to the corresponding square. */
    inline Square notation_to_square(const std::string &square) {
        char file = square[0], rank = square[1];
        return position_to_square(file - 'a', rank - '1');
    }

    inline std::string square_to_notation(Square square) {
        unsigned file = square % 8, rank = square / 8;
        std::string result;
        result.push_back(file + 'a');
        result.push_back(rank + '1');
        return result;
    }


    /* Returns the index of the first set bit, or -1 if the bitboard is empty.*/
    inline int bitScanForward(u64 &x) {
        if (x == 0)
            return -1;

        return __builtin_ffsll(x) - 1;
    }

    /* Returns the square corresponding to the index of the first set bit of the input.*/
    /* This function expects a non-zero value as input.*/
    inline Square lsb(u64 &x) {
        return (Square) bitScanForward(x);
    }

    /* Returns the index of the last set bit or -1 if the bitboard is empty.*/
    inline int bitScanReverse(u64 &x) {
        if (x == 0)
            return -1;
        return 63 - __builtin_clzll(x);
    }

    /*Returns the square corresponding to the index of the last set bit of the input.*/
    /*This function expects a non-zero value as input.*/

    inline Square msb(u64 &x) {
        return (Square) bitScanReverse(x);
    }

    inline int popCount(u64 x) {
        return __builtin_popcountll(x);
    }

    inline Square popLsb(u64 &x) {
        Square res = lsb(x);
        x &= x - 1;
        return res;
    }

    constexpr inline Color get_opposite(Color color) {
        return (Color) (1 - color);
    }

    constexpr inline Piece get_piece(PieceType type, Color color) {
        if (type == PT_NONE)
            return P_NONE;
        return (Piece) (type + C_NUM_PIECE_TYPES * color);
    }

    constexpr inline PieceType get_piece_type(Piece piece) {
        if (piece == P_NONE)
            return PT_NONE;
        return (PieceType) (piece % C_NUM_PIECE_TYPES);
    }

    constexpr inline File get_file(Square square) {
        return (File) (square % 8);
    }

    constexpr inline Rank get_rank(Square square) {
        return (Rank) (square / 8);
    }

    /*Converts the given piece to its corresponding nnue piece index.*/
    constexpr int get_nnue_piece(Piece piece) {
        switch (piece) {
            case P_W_KING:
                return 1;
            case P_W_PAWN:
                return 6;
            case P_W_KNIGHT:
                return 5;
            case P_W_BISHOP:
                return 4;
            case P_W_ROOK:
                return 3;
            case P_W_QUEEN:
                return 2;

            case P_B_KING:
                return 7;
            case P_B_PAWN:
                return 12;
            case P_B_KNIGHT:
                return 11;
            case P_B_BISHOP:
                return 10;
            case P_B_ROOK:
                return 9;
            case P_B_QUEEN:
                return 8;
            case P_NONE:
                return 0;
        }
        return 0;
    }

}


#endif //CHESSENGINE_UTIL_H
