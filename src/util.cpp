//
// Created by gvisan on 10.07.2021.
//

#include "util.h"

namespace engine {
    std::string dump_bitboard(u64 bit) {
        std::string result;
        u64 iter = 1;

        for (int i = 0; i < 64; i++) {
            result += (bit & iter) ? '1' : '0';
            if (i % 8 == 7)
                result += '\n';
            iter = iter << 1UL;
        }
        result += '\n';
        return result;
    }

    Piece notation_to_piece(char piece) {
        switch (piece) {
            case 'P':
                return P_W_PAWN;
            case 'N':
                return P_W_KNIGHT;
            case 'B':
                return P_W_BISHOP;
            case 'R':
                return P_W_ROOK;
            case 'Q':
                return P_W_QUEEN;
            case 'K':
                return P_W_KING;

            case 'p':
                return P_B_PAWN;
            case 'n':
                return P_B_KNIGHT;
            case 'b':
                return P_B_BISHOP;
            case 'r':
                return P_B_ROOK;
            case 'q':
                return P_B_QUEEN;
            case 'k':
                return P_B_KING;
            default:
                return P_NONE;
        }
    }

    char piece_to_notation(Piece piece) {
        switch (piece) {
            case P_W_PAWN:
                return 'P';
            case P_W_KNIGHT:
                return 'N';
            case P_W_BISHOP:
                return 'B';
            case P_W_ROOK:
                return 'R';
            case P_W_QUEEN:
                return 'Q';
            case P_W_KING:
                return 'K';

            case P_B_PAWN:
                return 'p';
            case P_B_KNIGHT:
                return 'n';
            case P_B_BISHOP:
                return 'b';
            case P_B_ROOK:
                return 'r';
            case P_B_QUEEN:
                return 'q';
            case P_B_KING:
                return 'k';
            case P_NONE:
                return '#';
        }

    }
}
