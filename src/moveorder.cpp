//
// Created by gvisan on 30.07.2021.
//

#include "moveorder.h"

namespace engine {
    MoveOrder::MoveOrder(Board &internal_board) : board(internal_board), mvv_lva(), first_killer(), second_killer() {
        clear_killers();
        /*Initializing mvv-lva table.*/
        PieceType victims[] = {PT_PAWN, PT_KNIGHT, PT_BISHOP, PT_ROOK, PT_QUEEN};
        PieceType attackers[] = {PT_KING, PT_QUEEN, PT_ROOK, PT_BISHOP, PT_KNIGHT, PT_PAWN};

        int index = 0;
        for (PieceType victim:victims) {
            for (PieceType attacker:attackers) {
                mvv_lva[victim][attacker] = index;
                index++;
            }
        }

    }

    int MoveOrder::score_move(const Move &move, TTEntry *entry, int ply) {

        /*If it's a hash move.*/
        if (entry != nullptr && entry->move == move) {
            return C_HASH_MOVE_BONUS;
        }

        switch (move.type) {
            case M_PROMOTION:
                return C_PROMOTION_BONUS + get_piece_value(get_piece_type(move.promotion));
            case M_PROMOTION_CAPTURE:
                return C_PROMOTION_BONUS + get_piece_value(get_piece_type(move.promotion)) +
                       get_piece_value(get_piece_type(move.captured));
            case M_CAPTURE:
                return C_CAPTURE_BONUS + mvv_lva[get_piece_type(move.captured)][get_piece_type(move.moved)];
            case M_QUIET:
                if (move == first_killer[ply])
                    return C_FIRST_KILLER_BONUS;
                if (move == second_killer[ply])
                    return C_SECOND_KILLER_BONUS;
                return 0;
            default:
                return 0;
        }

    }

    int MoveOrder::score_capture(const Move &move) {
        switch (move.type) {
            case M_PROMOTION_CAPTURE:
                return C_PROMOTION_BONUS + get_piece_value(get_piece_type(move.promotion)) +
                       get_piece_value(get_piece_type(move.captured));
            case M_CAPTURE:
                return mvv_lva[get_piece_type(move.captured)][get_piece_type(move.moved)];
            default:
                return 0;
        }
    }

    void MoveOrder::order_moves(std::vector<Move> &moves, TTEntry *entry, int ply) {
        for (Move &move:moves) {
            move.score = score_move(move, entry, ply);
        }
        std::sort(moves.begin(), moves.end(), compare_moves);
    }

    void MoveOrder::order_captures(std::vector<Move> &moves) {
        for (Move &capture:moves) {
            capture.score = score_capture(capture);
        }
        std::sort(moves.begin(), moves.end(), compare_moves);
    }

    void MoveOrder::set_killer(const Move &move, int ply) {
        if (move.type == M_QUIET && move != first_killer[ply]) {
            second_killer[ply] = first_killer[ply];
            first_killer[ply] = move;
        }
    }

    void MoveOrder::clear_killers() {
        for (int i = 0; i < C_MAX_SEARCH_DEPTH; i++) {
            first_killer[i] = second_killer[i] = create_empty_move();
        }
    }

}