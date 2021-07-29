//
// Created by gvisan on 27.07.2021.
//

#include"eval_defines.h"

namespace engine {
    int PST[C_NUM_PIECE_TYPES][2][C_NUM_PHASES][C_NUM_SQUARES];

    void init_pst() {
        for (int piece = 0; piece < C_NUM_PIECE_TYPES; piece++) {
            for (int phase = 0; phase < C_NUM_PHASES; phase++) {
                for (int square = 0; square < C_NUM_SQUARES; square++) {
                    /*White, mirrored.*/
                    PST[piece][C_WHITE][phase][square] = C_PST[piece][phase][C_NUM_SQUARES - square - 1];

                    /*Black.*/
                    PST[piece][C_BLACK][phase][square] = C_PST[piece][phase][square];
                }
            }
        }
    }
}
