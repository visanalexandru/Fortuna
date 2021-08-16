//
// Created by gvisan on 29.07.2021.
//

#include "search.h"

namespace engine {
    Search::Search(Board &internal_board) : board(internal_board), move_gen(internal_board), move_order(internal_board),
                                            nodes(0), ply(0), abort_search(false), limits() {

    }


    bool Search::timeout() const {
        std::chrono::duration<double> difference = std::chrono::system_clock::now() - start_search;
        return difference.count() > limits.allotted_time;
    }

    int Search::nega_max(int depth, int alpha, int beta, Color side) {

        /*Check for timeout.*/
        if (timeout()) {
            abort_search = true;
            return C_VALUE_DRAW; /*This value won't be used anyway.*/
        }

        nodes++;

        int alpha_orig = alpha;
        /*Transposition table lookup.*/
        u64 hash = board.current_state->zobrist_key;
        TTEntry *entry = transposition_table.probe(hash);

        if (entry != nullptr && entry->depth >= depth) {
            if (entry->type == TT_EXACT) {
                return entry->score;
            } else if (entry->type == TT_LOWERBOUND) {
                alpha = std::max(alpha, entry->score);
            } else if (entry->type == TT_UPPERBOUND) {
                beta = std::min(beta, entry->score);
            }
            if (alpha >= beta)
                return entry->score;
        }


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

        move_order.order_moves(moves, entry,ply);

        Move best = create_empty_move();
        int score = -C_VALUE_INFINITE, move_score;

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
            if (alpha >= beta){
                move_order.set_killer(move,ply);
                break;
            }
        }

        /*Do not store the result into the transposition table as the search was inconclusive.*/
        if (abort_search) {
            return C_VALUE_DRAW;
        }

        /*Transposition table store.*/
        TTEntry new_entry{hash, TT_EXACT, best, score, (u8) depth};
        if (score <= alpha_orig) {
            new_entry.type = TT_UPPERBOUND;
        } else if (score >= beta) {
            new_entry.type = TT_LOWERBOUND;
        }
        transposition_table.save(new_entry);

        return score;
    }

    Move Search::nega_max_root(int depth) {
        Move best = create_empty_move();
        int score = -C_VALUE_INFINITE, move_score, alpha = -C_VALUE_INFINITE, beta = C_VALUE_INFINITE;

        u64 hash = board.current_state->zobrist_key;
        TTEntry *entry = transposition_table.probe(hash);

        Color side = board.color_to_play();
        nodes = 0;
        ply = 0;

        auto moves = move_gen.get_moves();
        move_order.order_moves(moves, entry,ply);

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

        /*Do not store the result into the transposition table as the search was inconclusive.*/
        if (abort_search) {
            return create_empty_move();
        }
        transposition_table.save({hash, TT_EXACT, best, score, (u8) depth});
        std::cout << "depth:" << depth << " move: " << move_to_string(best) << " score: " << score << " nodes searched "
                  << nodes
                  << std::endl;
        return best;
    }


    Move Search::iterative_deepening() {
        start_search = std::chrono::system_clock::now();
        abort_search = false;
        move_order.clear_killers();

        auto moves = move_gen.get_moves();

        /*If there are no moves in the position return an empty move.*/
        if (moves.empty()) {
            return create_empty_move();
        }
        /*Just choose a random move to fall back to if we abort the search immediately due to timeouts.*/
        Move best_move = moves.back();
        int current_depth = 1;

        while (current_depth <= limits.maximum_depth) {
            Move new_move = nega_max_root(current_depth);

            if (abort_search) {
                /*The search was aborted and inconclusive, just discard the move.*/
                break;
            } else {
                best_move = new_move;
            }
            current_depth++;
        }
        return best_move;
    }

}