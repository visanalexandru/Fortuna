//
// Created by gvisan on 29.07.2021.
//

#ifndef CHESSENGINE_SEARCH_H
#define CHESSENGINE_SEARCH_H

#include<chrono>
#include"board.h"
#include"movegen.h"
#include"eval.h"
#include"moveorder.h"
#include"ttable.h"

namespace engine {
    const double C_DEFAULT_SEARCH_TIME = 5.0;

    /*This structure contains parameters for the search.*/
    struct SearchParameters {
        /*Time allotted for the search.*/
        double allotted_time;

        /*The maximum depth of the search.*/
        int maximum_depth;

        SearchParameters() : allotted_time(C_DEFAULT_SEARCH_TIME), maximum_depth(C_MAX_SEARCH_DEPTH) {

        }
    };

    /*This class is responsible for searching for the best move from a given position.*/
    class Search {
    private:
        /*The internal board.*/
        Board &board;

        /*The move generator.*/
        MoveGen move_gen;

        /*Move ordering.*/
        MoveOrder move_order;

        /*https://en.wikipedia.org/wiki/Negamax.*/
        /*This function returns the best score for the given side.
         * It is called by nega_max_root to get the best move in the current position.*/
        int nega_max(int depth, int alpha, int beta, Color side);

        /*Number of nodes searched.*/
        int nodes;

        /*The time point at which the search started, used to get the elapsed time of the search.*/
        std::chrono::time_point<std::chrono::system_clock> start_search;

        /*Checks if the search needs to stop as there is no more time left.*/
        bool timeout() const;

        /*This flag is set when a timeout occurs.*/
        bool abort_search;

        /*Returns the best move in the current position.*/
        Move nega_max_root(int depth);

    public:
        /*Search limits.*/
        SearchParameters limits;

        explicit Search(Board &internal_board);

        /*Starts the iterative deepening search and returns the best move,
         * with respect to the given search parameters.*/
        Move iterative_deepening();
    };
}


#endif //CHESSENGINE_SEARCH_H
