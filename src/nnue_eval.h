//
// Created by gvisan on 26.08.2021.
//

#ifndef CHESSENGINE_NNUE_EVAL_H
#define CHESSENGINE_NNUE_EVAL_H

#ifndef IS_64BIT
#define IS_64BIT
#endif

#include<string>
#include<vector>
#include"nnue/nnue.h"
#include "board.h"

/*This file contains functions that wrap the functionality of the neural network.*/

namespace engine {
    /*Initializes the neural network with the given weights file.*/
    void init_nnue(const std::string &file_name);

    /*Probes the nnue and returns the score of the position.*/
    int evaluate_nnue(const Board &board);

    /*The piece and square buffers, used to feed information to the net.*/
    extern int nnue_pieces[C_NUM_SQUARES];

    extern int nnue_squares[C_NUM_SQUARES];

}


#endif //CHESSENGINE_NNUE_EVAL_H
