//
// Created by gvisan on 17.08.2021.
//

#include "uci.h"

namespace engine {
    UCI::UCI() : board(), search(board), move_gen(board), stop_uci(false) {
    }

    Move UCI::find_move(const std::string &move) {
        auto moves = move_gen.get_moves<GT_NORMAL>();
        for (const Move &to_check:moves) {
            if (move_to_string(to_check) == move) {
                return to_check;
            }
        }
        return create_empty_move();
    }

    void UCI::send_info() const {
        std::cout << "id name Fortuna" << std::endl;
        std::cout << "id author Visan Alexandru" << std::endl;

        std::cout << "option name Hash type spin default " << C_DEFAULT_HASH_SIZE << " min " << C_MINIMUM_HASH_SIZE
                  << " max " << C_MAXIMUM_HASH_SIZE << std::endl;

        std::cout << "uciok" << std::endl;
    }

    void UCI::process(std::stringstream &command) {
        std::string header;
        command >> header;

        if (header == "uci") {
            send_info();
        } else if (header == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (header == "ucinewgame") {
            board.load_fen(C_BASE_POSITION);
            transposition_table.clear();
        } else if (header == "position") {
            process_position(command);
        } else if (header == "quit") {
            search.stop_thread();
            stop_uci = true;
        } else if (header == "go") {
            process_search(command);
        } else if (header == "stop") {
            search.stop_thread();
        } else if (header == "board") {
            std::cout << board << std::endl;
        } else if (header == "setoption") {
            process_option(command);
        } else std::cout << "Unknown command" << std::endl;
    }

    void UCI::process_position(std::stringstream &position) {
        /*Information about the position (is it the start position or we need to parse a fen string).*/
        std::string header, move;
        position >> header;
        /*Clear the game history.*/
        game_history.clear();

        if (header == "startpos") {
            board.load_fen(C_BASE_POSITION);
        } else if (header == "fen") {
            std::string chunk, fen_string;
            /*We need to parse a fen string made up from 6 chunks.*/
            for (int i = 0; i < 6; i++) {
                position >> chunk;
                if (!fen_string.empty())
                    fen_string += " ";
                fen_string += chunk;
            }
            board.load_fen(fen_string);
        }

        /*The first position in the history.*/
        game_history.push_position(board.current_state->zobrist_key, true);

        position >> move;
        if (move == "moves") {
            /*We need to make moves.*/
            while (position >> move) {
                /*We get the corresponding move from the move string.*/
                Move corresponding = find_move(move);
                board.make_move(corresponding);
                game_history.push_position(board.current_state->zobrist_key, is_irreversible(corresponding));
            }
        }
    }


    void UCI::process_search(std::stringstream &parameters) {
        std::string header;

        bool compute_time = false;
        unsigned white_time_ms = 0, black_time_ms = 0, white_inc_ms = 0, black_inc_ms, move_time_ms;

        /*Set the default limits.*/
        search.limits.maximum_depth = C_MAX_SEARCH_DEPTH;
        search.limits.allotted_time = C_DEFAULT_SEARCH_TIME;

        while (parameters >> header) {
            if (header == "wtime") {
                parameters >> white_time_ms;
                compute_time = true;
            } else if (header == "btime") {
                parameters >> black_time_ms;
                compute_time = true;
            } else if (header == "winc") {
                parameters >> white_inc_ms;
                compute_time = true;
            } else if (header == "binc") {
                parameters >> black_inc_ms;
                compute_time = true;
            } else if (header == "depth") {
                parameters >> search.limits.maximum_depth;
                search.limits.allotted_time = C_INFINITE_TIME;
            } else if (header == "infinite") {
                search.limits.maximum_depth = C_MAX_SEARCH_DEPTH;
                search.limits.allotted_time = C_INFINITE_TIME;
            } else if (header == "movetime") {
                parameters >> move_time_ms;
                search.limits.allotted_time = milliseconds_to_seconds(move_time_ms);
            }
        }
        if (compute_time) {
            search.limits.allotted_time = get_time(white_time_ms, black_time_ms, white_inc_ms, black_inc_ms,
                                                   board.color_to_play());
        }
        search.start_thread();
    }


    void UCI::process_option(std::stringstream &option) {
        std::string token, name;
        /*We pass over the "name" string to get the option's name.*/
        option >> token >> name;

        if (name == "Hash") {
            /*We pass over the "value" string.*/
            option >> token;
            /*We get the new transposition table size.*/
            int hash_size_mb;
            option >> hash_size_mb;
            transposition_table.reserve(hash_size_mb);
        } else {
            std::cout << "Unknown option" << std::endl;
        }

    }

    void UCI::start() {
        std::string command;
        std::stringstream buffer;
        while (!stop_uci) {
            std::getline(std::cin, command);
            buffer.clear();
            buffer.str(command);
            process(buffer);
        }

    }

}