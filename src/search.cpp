//
// Created by gvisan on 29.07.2021.
//

#include "search.h"

namespace engine {
    Search::Search(Board &internal_board) : board(internal_board), move_gen(internal_board), move_order(internal_board),
                                            ply(0), nodes(0) {

    }

    int Search::nega_max(int depth, int alpha, int beta, Color side) {
        nodes++;
        if (depth == 0) {
            return evaluate_side(board, side);
        }

        auto moves = move_gen.get_moves();
        /*If there are no moves available.*/
        if (moves.empty()) {
            /*Checkmated.*/
            if (move_gen.is_in_check(side)) {
                return -C_VALUE_MATE;
            }/*Stalemate.*/
            else return C_VALUE_DRAW;
        }

        move_order.order_moves(moves, side);

        int value = -C_VALUE_INFINITE, move_score;

        for (const Move &move:moves) {
            board.make_move(move);
            ply++;

            move_score = adjust_mate_score(-nega_max(depth - 1, -beta, -alpha, get_opposite(side)));
            value = std::max(value, move_score);

            board.undo_move(move);
            ply--;

            alpha = std::max(alpha, value);
            if (alpha >= beta)
                break;
        }


        return value;
    }

    Move Search::nega_max_root(int depth) {
        Move best = create_empty_move();
        int score = -C_VALUE_INFINITE, move_score, alpha = -C_VALUE_INFINITE, beta = C_VALUE_INFINITE;
        Color side = board.color_to_play();
        ply = 0;
        nodes = 0;

        auto moves = move_gen.get_moves();
        move_order.order_moves(moves, side);

        for (const Move &move:moves) {
            board.make_move(move);
            ply++;
            move_score = adjust_mate_score(-nega_max(depth - 1, -beta, -alpha, get_opposite(side)));

            if (move_score > score) {
                score = move_score;
                best = move;
            }

            board.undo_move(move);
            ply--;

            alpha = std::max(alpha, score);
        }
        std::cout << "move: " << move_to_string(best) << " score: " << score << " nodes searched " << nodes
                  << std::endl;
        return best;
    }

}