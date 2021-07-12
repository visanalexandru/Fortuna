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
                return WPawn;
            case 'N':
                return WKnight;
            case 'B':
                return WBishop;
            case 'R':
                return WRook;
            case 'Q':
                return WQueen;
            case 'K':
                return WKing;

            case 'p':
                return BPawn;
            case 'n':
                return BKnight;
            case 'b':
                return BBishop;
            case 'r':
                return BRook;
            case 'q':
                return BQueen;
            case 'k':
                return BKing;
            default:
                return None;
        }
    }

    char piece_to_notation(Piece piece) {
        switch (piece) {
            case WPawn:
                return 'P';
            case WKnight:
                return 'N';
            case WBishop:
                return 'B';
            case WRook:
                return 'R';
            case WQueen:
                return 'Q';
            case WKing:
                return 'K';

            case BPawn:
                return 'p';
            case BKnight:
                return 'n';
            case BBishop:
                return 'b';
            case BRook:
                return 'r';
            case BQueen:
                return 'q';
            case BKing:
                return 'k';
            case None:
                return '#';
        }

    }
}
