//
// Created by gvisan on 26.07.2021.
//

#include "eval.h"

namespace engine {
    int get_phase(const Board &board) {
        int phase = C_PHASE_WEIGHT_SUM;
        u64 knights = board.current_position.placement[P_W_KNIGHT] | board.current_position.placement[P_B_KNIGHT];
        u64 bishops = board.current_position.placement[P_W_BISHOP] | board.current_position.placement[P_B_BISHOP];
        u64 rooks = board.current_position.placement[P_W_ROOK] | board.current_position.placement[P_B_ROOK];
        u64 queens = board.current_position.placement[P_W_QUEEN] | board.current_position.placement[P_B_QUEEN];

        phase -= popCount(knights) * C_KNIGHT_PHASE_WEIGHT;
        phase -= popCount(bishops) * C_BISHOP_PHASE_WEIGHT;
        phase -= popCount(rooks) * C_ROOK_PHASE_WEIGHT;
        phase -= popCount(queens) * C_QUEEN_PHASE_WEIGHT;
        phase = (phase * C_MAX_PHASE + (C_PHASE_WEIGHT_SUM / 2)) / C_PHASE_WEIGHT_SUM;
        return phase;
    }

    int evaluate(const Board &board) {
        int white_mg = 0, black_mg = 0, white_eg = 0, black_eg = 0;

        white_mg = white_eg = score_material<C_WHITE>(board);
        black_mg = black_eg = score_material<C_BLACK>(board);

        score_piece_placement<C_WHITE>(board, white_mg, white_eg);
        score_piece_placement<C_BLACK>(board, black_mg, black_eg);
        score_pawn_structure<C_WHITE>(board, white_mg, white_eg);
        score_pawn_structure<C_BLACK>(board, black_mg, black_eg);
        score_king_safety<C_WHITE>(board, white_mg, white_eg);
        score_king_safety<C_BLACK>(board, black_mg, black_eg);
        score_mobility<C_WHITE>(board, white_mg, white_eg);
        score_mobility<C_BLACK>(board, black_mg, black_eg);

        int opening = white_mg - black_mg;
        int end_game = white_eg - black_eg;

        int phase = get_phase(board);
        int eval = (opening * (C_MAX_PHASE - phase) + (end_game * phase)) / C_MAX_PHASE;
        return eval;
    }

    int evaluate_side(const Board &board, Color side) {
        int score = evaluate(board);
        return (side == C_WHITE) ? score : -score;
    }
}