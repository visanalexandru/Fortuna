//
// Created by gvisan on 22.07.2021.
//

#ifndef CHESSENGINE_EVAL_DEFINES_H
#define CHESSENGINE_EVAL_DEFINES_H

#include"defines.h"

namespace engine {
    const int C_PAWN_VALUE = 100;
    const int C_KNIGHT_VALUE = 320;
    const int C_BISHOP_VALUE = 330;
    const int C_ROOK_VALUE = 510;
    const int C_QUEEN_VALUE = 980;
    const int C_KING_VALUE = 10000;

    const int C_KNIGHT_PHASE_WEIGHT = 1;
    const int C_BISHOP_PHASE_WEIGHT = 1;
    const int C_ROOK_PHASE_WEIGHT = 2;
    const int C_QUEEN_PHASE_WEIGHT = 4;

    const int C_PHASE_WEIGHT_SUM =
            C_KNIGHT_PHASE_WEIGHT * 4 + C_BISHOP_PHASE_WEIGHT * 4 + C_ROOK_PHASE_WEIGHT * 4 + C_QUEEN_PHASE_WEIGHT * 2;
    const int C_MAX_PHASE = 256;

    const int C_PIECE_VALUES[] = {C_PAWN_VALUE, C_KNIGHT_VALUE, C_BISHOP_VALUE, C_ROOK_VALUE, C_QUEEN_VALUE,
                                  C_KING_VALUE, 0};

    const int C_VALUE_INFINITE = 60000;

    const int C_VALUE_MATE = 30000;
    const int C_VALUE_DRAW = 0;

    const int C_MAX_SEARCH_DEPTH = 256;

    const int C_DOUBLED_PAWN_PENALTY[C_NUM_PHASES] = {-20, -30};

    const int C_PASSED_PAWN_BONUS[C_NUM_PHASES][C_NUM_RANKS] = {{0, 0, 0, 10, 15, 20, 45, 0},
                                                                {0, 10, 15, 20, 30, 40, 90, 0}};

    const int C_STRONG_PAWN_SHIELD_BONUS[C_NUM_PHASES]={6,0};
    const int C_WEAK_PAWN_SHIELD_BONUS[C_NUM_PHASES]={3,0};

    inline int get_piece_value(PieceType type) {
        return C_PIECE_VALUES[type];
    }

    /*Adjusts the mate score for a new ply.*/
    inline int adjust_mate_score(int score) {
        if (score >= C_VALUE_MATE - C_MAX_SEARCH_DEPTH) {
            return score - 1;
        }

        if (score <= -C_VALUE_MATE + C_MAX_SEARCH_DEPTH) {
            return score + 1;
        }
        return score;
    }


    /*Piece square tables taken from https://gitlab.com/tsoj/Googleplex_Starthinker.*/
    const int C_PST[C_NUM_PIECE_TYPES][C_NUM_PHASES][C_NUM_SQUARES] =
            {
                    // PAWN
                    {
                            // OPENING
                            {
                                    0,   0,   0,   0,   0,   0,   0,   0,
                                    45,  45,  45,  45,  45,  45,  45,  45,
                                    10,  10,  20,  30,  30,  20,  10,  10,
                                    5,   5,   10,  25,  25,  10,  5,   5,
                                    0,   0,   0,   20,  20,  0,   0,   0,
                                    5,   -5,  -10, 0,   0,   -10, -5,  5,
                                    5,   10,  10, -20, -20, 10, 10,  5,
                                    0,   0,   0,   0,   0,   0,   0,   0
                            },
                            // ENDGAME
                            {
                                    0,   0,   0,   0,   0,   0,   0,   0,
                                    90,  90,  90,  90, 90, 90,  90,  90,
                                    30,  30,  40, 45, 45, 40, 40,  30,
                                    20,  20,  20, 25, 25, 20, 20,  20,
                                    0,   0,   0,  20, 20, 0,  0,   0,
                                    -5,  -5,  -10, -10, -10, -10, -5,  -5,
                                    -15, -15, -15, -20, -20, -15, -15, -15,
                                    0,   0,   0,   0,   0,   0,   0,   0
                            }
                    },
                    // KNIGHT
                    {
                            // OPENING
                            {
                                    -50, -40, -30, -30, -30, -30, -40, -50,
                                    -40, -20, 0,   0,   0,   0,   -20, -40,
                                    -30, 0,   10,  15,  15,  10,  0,   -30,
                                    -30, 5,   15,  20,  20,  15,  5,   -30,
                                    -30, 0,   15,  20,  20,  15,  0,   -30,
                                    -30, 5,   10,  15,  15,  10,  5,   -30,
                                    -40, -20, 0,  5,   5,   0,  -20, -40,
                                    -50, -40, -30, -30, -30, -30, -40, -50,
                            },
                            // ENDGAME
                            {
                                    -50, -40, -30, -30, -30, -30, -40, -50,
                                    -40, -20, 0,   0,  0,  0,   -20, -40,
                                    -30, 0,   10, 15, 15, 10, 0,   -30,
                                    -30, 5,   15, 20, 20, 15, 5,   -30,
                                    -30, 0,   15, 20, 20, 15, 0,   -30,
                                    -30, 5,   10,  15,  15,  10,  5,   -30,
                                    -40, -20, 0,   5,   5,   0,   -20, -40,
                                    -50, -40, -30, -30, -30, -30, -40, -50,
                            }
                    },
                    // BISHOP
                    {
                            // OPENING
                            {
                                    -20, -10, -10, -10, -10, -10, -10, -20,
                                    -10, 0,   0,   0,   0,   0,   0,   -10,
                                    -10, 0,   5,   10,  10,  5,   0,   -10,
                                    -10, 5,   5,   10,  10,  5,   5,   -10,
                                    -10, 0,   10,  10,  10,  10,  0,   -10,
                                    -10, 10,  10,  10,  10,  10,  10,  -10,
                                    -10, 5,   0,  0,   0,   0,  5,   -10,
                                    -20, -10, -10, -10, -10, -10, -10, -20,
                            },
                            // ENDGAME
                            {
                                    -20, -10, -10, -10, -10, -10, -10, -20,
                                    -10, 0,   0,   0,  0,  0,   0,   -10,
                                    -10, 0,   5,  10, 10, 5,  0,   -10,
                                    -10, 5,   5,  10, 10, 5,  5,   -10,
                                    -10, 0,   10, 10, 10, 10, 0,   -10,
                                    -10, 10,  10,  10,  10,  10,  10,  -10,
                                    -10, 5,   0,   0,   0,   0,   5,   -10,
                                    -20, -10, -10, -10, -10, -10, -10, -20,
                            }
                    },
                    // ROOK
                    {
                            // OPENING
                            {
                                    0,   0,   0,   0,   0,   0,   0,   0,
                                    5,   10,  10,  10,  10,  10,  10,  5,
                                    -5,  0,   0,   0,   0,   0,   0,   -5,
                                    -5,  0,   0,   0,   0,   0,   0,   -5,
                                    -5,  0,   0,   0,   0,   0,   0,   -5,
                                    -5,  0,   0,   0,   0,   0,   0,   -5,
                                    -5,  0,   0,  0,   0,   0,  0,   -5,
                                    0,   0,   0,   5,   5,   0,   0,   0
                            },
                            // ENDGAME
                            {
                                    0,   0,   0,   0,   0,   0,   0,   0,
                                    0,   5,   5,   5,  5,  5,   5,   0,
                                    -5,  0,   0,  0,  0,  0,  0,   -5,
                                    -5,  0,   0,  0,  0,  0,  0,   -5,
                                    -5,  0,   0,  0,  0,  0,  0,   -5,
                                    -5,  0,   0,   0,   0,   0,   0,   -5,
                                    -5,  0,   0,   0,   0,   0,   0,   -5,
                                    0,   0,   0,   0,   0,   0,   0,   0
                            }
                    },
                    // QUEEN
                    {
                            // OPENING
                            {
                                    -20, -10, -10, -5,  -5,  -10, -10, -20,
                                    -10, 0,   0,   0,   0,   0,   0,   -10,
                                    -10, 0,   5,   5,   5,   5,   0,   -10,
                                    -5,  0,   5,   5,   5,   5,   0,   -5,
                                    0,   0,   5,   5,   5,   5,   0,   -5,
                                    -10, 5,   5,   5,   5,   5,   0,   -10,
                                    -10, 0,   5,  0,   0,   0,  0,   -10,
                                    -20, -10, -10, -5,  -5,  -10, -10, -20
                            },
                            // ENDGAME
                            {
                                    -20, -10, -10, -5,  -5,  -10, -10, -20,
                                    -10, 0,   0,   0,  0,  0,   0,   -10,
                                    -10, 0,   5,  5,  5,  5,  0,   -10,
                                    -5,  0,   5,  5,  5,  5,  0,   -5,
                                    0,   0,   5,  5,  5,  5,  0,   -5,
                                    -10, 0,   5,   5,   5,   5,   0,   -10,
                                    -10, 0,   0,   0,   0,   0,   0,   -10,
                                    -20, -10, -10, -5,  -5,  -10, -10, -20
                            }
                    },
                    // KING
                    {
                            // OPENING
                            {
                                    -30, -40, -40, -50, -50, -40, -40, -30,
                                    -30, -40, -40, -50, -50, -40, -40, -30,
                                    -30, -40, -40, -50, -50, -40, -40, -30,
                                    -30, -40, -40, -50, -50, -40, -40, -30,
                                    -20, -30, -30, -40, -40, -30, -30, -20,
                                    -10, -20, -20, -20, -20, -20, -20, -10,
                                    20,  20,  0,  0,   0,   0,  20,  20,
                                    20,  30,  10,  0,   0,   10,  30,  20
                            },
                            // ENDGAME
                            {
                                    -50, -40, -30, -20, -20, -30, -40, -50,
                                    -30, -20, -10, 0,  0,  -10, -20, -30,
                                    -30, -10, 20, 30, 30, 20, -10, -30,
                                    -30, -10, 30, 40, 40, 30, -10, -30,
                                    -30, -10, 30, 40, 40, 30, -10, -30,
                                    -30, -10, 20,  30,  30,  20,  -10, -30,
                                    -30, -30, 0,   0,   0,   0,   -30, -30,
                                    -50, -30, -30, -30, -30, -30, -30, -50
                            }
                    },
            };

    /*Precalculated piece square tables, mirrored for white.*/
    extern int PST[C_NUM_PIECE_TYPES][2][C_NUM_PHASES][C_NUM_SQUARES];

    /*Initializes the piece square tables.*/
    void init_pst();

}

#endif //CHESSENGINE_EVAL_DEFINES_H
