//
// Created by gvisan on 30.07.2021.
//

#include "moveorder.h"

namespace engine {
    MoveOrder::MoveOrder(Board &internal_board) : board(internal_board), mvv_lva() {

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

    int MoveOrder::score_move(const Move &move, TTEntry *entry) {

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
            default:
                return 0;
        }

    }

    void MoveOrder::order_moves(std::vector<Move> &moves, TTEntry *entry) {
        for (Move &move:moves) {
            move.score = score_move(move, entry);
        }
        std::sort(moves.begin(), moves.end(), compare_moves);
    }

}