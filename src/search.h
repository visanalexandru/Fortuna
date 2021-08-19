//
// Created by gvisan on 29.07.2021.
//

#ifndef CHESSENGINE_SEARCH_H
#define CHESSENGINE_SEARCH_H

#include<chrono>
#include<atomic>
#include<thread>
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

    /*Returns the elapsed time since the given time point in seconds.*/
    double seconds_since_time_point(const std::chrono::time_point<std::chrono::system_clock> &point);

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

        /*The distance to the root in the search.*/
        int ply;

        /*The time point at which the search started, used to get the elapsed time of the search.*/
        std::chrono::time_point<std::chrono::system_clock> start_search;

        /*Checks if the search needs to stop as there is no more time left.*/
        bool timeout() const;

        /*This flag is set when a timeout or forced stop occurs.*/
        bool abort_search;

        /*Returns the best move in the current position.*/
        Move nega_max_root(int depth);

        /*This flag is set when a GUI forced stop occurs.*/
        std::atomic_bool stop;

        /*This flag is set while the thread is busy calculating.*/
        std::atomic_bool searching;

        /*The search thread pointer, all calculation is done on this thread.*/
        std::shared_ptr<std::thread> search_thread;

        /*This method wraps the iterative_deepening method, to be called on a separate thread.*/
        void search();

    public:
        /*Search limits.*/
        SearchParameters limits;

        explicit Search(Board &internal_board);

        /*Starts the iterative deepening search and returns the best move,
         * with respect to the given search parameters.*/
        Move iterative_deepening();

        /*Starts the thread containing the iterative deepening search.*/
        void start_thread();

        /*Sets the stop flag so the search immediately stops, and waits for the search thread to finish.*/
        void stop_thread();

    };
}


#endif //CHESSENGINE_SEARCH_H
