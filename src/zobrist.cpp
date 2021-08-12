//
// Created by gvisan on 12.08.2021.
//

#include "zobrist.h"

namespace engine {
    u64 ZOBRIST_PIECES[C_NUM_PIECES][C_NUM_SQUARES];
    u64 ZOBRIST_EN_PASSANT[C_NUM_FILES];
    u64 ZOBRIST_CASTLING[C_NUM_CASTLING_RIGHTS];
    u64 ZOBRIST_SIDE_TO_MOVE;

    void init_zobrist() {
        std::mt19937_64 pnrg;
        pnrg.seed(ZOBRIST_SEED);

        for (int piece = 0; piece < C_NUM_PIECES; piece++) {
            for (int square = 0; square < C_NUM_SQUARES; square++) {
                ZOBRIST_PIECES[piece][square] = pnrg();
            }
        }
        for (int ep_square = 0; ep_square < C_NUM_FILES; ep_square++) {
            ZOBRIST_EN_PASSANT[ep_square] = pnrg();
        }

        ZOBRIST_CASTLING[ZOBRIST_WHITE_CASTLE_KINGSIDE] = pnrg();
        ZOBRIST_CASTLING[ZOBRIST_WHITE_CASTLE_QUEENSIDE] = pnrg();
        ZOBRIST_CASTLING[ZOBRIST_BLACK_CASTLE_KINGSIDE] = pnrg();
        ZOBRIST_CASTLING[ZOBRIST_BLACK_CASTLE_QUEENSIDE] = pnrg();

        ZOBRIST_SIDE_TO_MOVE = pnrg();

    }

}