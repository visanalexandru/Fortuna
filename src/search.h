//
// Created by gvisan on 29.07.2021.
//

#ifndef CHESSENGINE_SEARCH_H
#define CHESSENGINE_SEARCH_H

#include"board.h"
#include"movegen.h"
#include"eval.h"
#include"moveorder.h"

namespace engine {
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

        /*The number of plies from the root of the search.*/
        int ply;

        /*Number of nodes searched.*/
        int nodes;

    public:
        explicit Search(Board &internal_board);

        /*Returns the best move in the current position.*/
        Move nega_max_root(int depth);
    };
}


#endif //CHESSENGINE_SEARCH_H
