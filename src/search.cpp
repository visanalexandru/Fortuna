//
// Created by gvisan on 29.07.2021.
//

#include "search.h"

namespace engine {
    Search::Search(Board &internal_board) : board(internal_board), move_gen(internal_board), move_order(internal_board),
                                            nodes(0), ply(0), abort_search(false), stop(false), searching(false),
                                            search_thread(nullptr), limits() {

    }


    double seconds_since_time_point(const std::chrono::time_point<std::chrono::system_clock> &point) {
        std::chrono::duration<double> difference = std::chrono::system_clock::now() - point;
        return difference.count();
    }

    bool Search::timeout() const {
        return seconds_since_time_point(start_search) > limits.allotted_time;
    }


    int Search::quiescence(int alpha, int beta, Color side) {
        nodes++;
        int stand_pat = evaluate_side(board, side);
        if (stand_pat >= beta) {
            return beta;
        }
        if (alpha < stand_pat) {
            alpha = stand_pat;
        }

        auto captures = move_gen.get_moves<GT_CAPTURES_ONLY>();
        move_order.order_captures(captures);
        int score;

        for (const Move &capture:captures) {
            board.make_move(capture);
            score = -quiescence(-beta, -alpha, get_opposite(side));
            board.undo_move(capture);
            if (score >= beta) {
                return beta;
            }
            if (score > alpha) {
                alpha = score;
            }
        }
        return alpha;
    }

    int Search::nega_max(int depth, int alpha, int beta, Color side) {

        /*Check for timeout or a forced stop.*/
        if (timeout() || stop) {
            abort_search = true;
            return C_VALUE_DRAW; /*This value won't be used anyway.*/
        }

        /*Check for threefold repetition.*/
        if (game_history.is_repetition()) {
            return C_VALUE_DRAW;
        }
        bool in_check = move_gen.is_in_check(side);

        /*Check extensions.*/
        if (in_check) {
            depth++;
        }

        if (depth == 0) {
            return quiescence(alpha, beta, side);
        }

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

        auto moves = move_gen.get_moves<GT_NORMAL>();
        /*If there are no moves available.*/
        if (moves.empty()) {
            /*Checkmated.*/
            if (in_check) {
                return -C_VALUE_MATE;
            }/*Stalemate.*/
            else return C_VALUE_DRAW;
        }

        move_order.order_moves(moves, entry, ply);

        Move best = create_empty_move();
        int score = -C_VALUE_INFINITE, move_score, move_count = 0;
        bool gives_check;

        for (const Move &move:moves) {
            board.make_move(move);
            game_history.push_position(board.current_state->zobrist_key,is_irreversible(move));
            ply++;

            gives_check = move_gen.is_in_check(get_opposite(side));

            /*Late move reductions.*/
            if (move_count >= C_LMR_MOVE_COUNT &&
                depth >= 3 &&
                !is_tactical(move) &&
                !in_check &&
                !gives_check) {

                /*Check if the score can be greater than alpha.*/
                move_score = -nega_max(depth - 2, -alpha - 1, -alpha, get_opposite(side));

            } else move_score = alpha + 1;

            /*The score can be greater than alpha, search with a full window.*/
            if (move_score > alpha) {
                move_score = adjust_mate_score(-nega_max(depth - 1, -beta, -alpha, get_opposite(side)));
                if (move_score > score) {
                    score = move_score;
                    best = move;
                }
                alpha = std::max(alpha, score);
            }

            board.undo_move(move);
            game_history.pop_position();
            ply--;

            if (alpha >= beta) {
                move_order.set_killer(move, ply);
                break;
            }
            move_count++;
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
        auto start_time_point = std::chrono::system_clock::now();

        u64 hash = board.current_state->zobrist_key;
        TTEntry *entry = transposition_table.probe(hash);

        Color side = board.color_to_play();
        nodes = 0;
        ply = 0;

        auto moves = move_gen.get_moves<GT_NORMAL>();
        move_order.order_moves(moves, entry, ply);

        for (const Move &move:moves) {
            board.make_move(move);
            game_history.push_position(board.current_state->zobrist_key,is_irreversible(move));
            ply++;

            move_score = adjust_mate_score(-nega_max(depth - 1, -beta, -alpha, get_opposite(side)));

            if (move_score > score) {
                score = move_score;
                best = move;
            }

            board.undo_move(move);
            game_history.pop_position();
            ply--;

            alpha = std::max(alpha, score);
        }

        /*Do not store the result into the transposition table as the search was inconclusive.*/
        if (abort_search) {
            return create_empty_move();
        }
        double elapsed = seconds_since_time_point(start_time_point);
        transposition_table.save({hash, TT_EXACT, best, score, (u8) depth});
        std::cout << "info depth " << depth
                  << " time " << (unsigned) (elapsed * 1000.0)
                  << " score cp " << score
                  << " nps " << (unsigned) (nodes / elapsed)
                  << " nodes " << nodes
                  << " pv ";
        output_principal_variation(depth, std::cout);
        std::cout << std::endl;
        return best;
    }


    Move Search::iterative_deepening() {
        start_search = std::chrono::system_clock::now();
        abort_search = false;
        move_order.clear_killers();

        auto moves = move_gen.get_moves<GT_NORMAL>();

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

    void Search::search() {
        searching = true;
        Move best_move = iterative_deepening();
        std::cout << "bestmove " << move_to_string(best_move) << std::endl;
        searching = false;
    }


    void Search::start_thread() {
        /*Call join() to be sure that the thread has completed the search.*/
        if (search_thread) {
            search_thread->join();
        }

        /*Set the force stop top flag to false.*/
        stop = false;

        /*Create and start new thread.*/
        search_thread = std::make_shared<std::thread>(&Search::search, this);
    }

    void Search::stop_thread() {
        stop = true;

        /*Wait for the thread to finish.*/
        if (search_thread) {
            search_thread->join();
        }

        search_thread = nullptr;
    }


    void Search::output_principal_variation(int depth, std::ostream &stream) {
        /*We store the pv so we can undo the moves.*/
        std::vector<Move> path;

        /*The transposition table node.*/
        TTEntry *here = transposition_table.probe(board.current_state->zobrist_key);

        int current_depth = 0;

        while (current_depth < depth && here != nullptr) {

            Move move = here->move;
            stream << move_to_string(move) << " ";
            path.push_back(move);

            /*We go deeper down the line.*/
            board.make_move(move);

            /*We update the entry pointer.*/
            here = transposition_table.probe(board.current_state->zobrist_key);

            current_depth++;
        }

        /*We undo the principal variation moves.*/
        for (auto iter = path.rbegin(); iter != path.rend(); iter++) {
            board.undo_move(*iter);
        }
    }

}