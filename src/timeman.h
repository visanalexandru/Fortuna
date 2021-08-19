//
// Created by gvisan on 19.08.2021.
//

#ifndef CHESSENGINE_TIMEMAN_H
#define CHESSENGINE_TIMEMAN_H

#include"defines.h"

/*This header contains functions that calculate how much time we need to allocate for a move.*/
namespace engine {

    /*The expected number of remaining moves in the game.*/
    const int C_EXPECTED_MOVES = 20;

    /*Used to avoid communication latency problems.*/
    const double C_EPSILON = 0.05;

    /*This value is used when we receive a go infinite command.*/
    const double C_INFINITE_TIME = 20000.0;

    /*Computes the amount of time allocated for the given side to search for a move.*/
    double get_time(unsigned white_time, unsigned black_time, unsigned white_increment, unsigned black_increment, Color side);

    /*Converts a duration in milliseconds to seconds.*/
    double milliseconds_to_seconds(unsigned milliseconds);

}


#endif //CHESSENGINE_TIMEMAN_H
