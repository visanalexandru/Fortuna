//
// Created by gvisan on 12.08.2021.
//

#ifndef CHESSENGINE_ZOBRIST_H
#define CHESSENGINE_ZOBRIST_H

#include"defines.h"
#include<random>

/*This header contains methods to generate the zobrist keys.*/
namespace engine {
    /*Zobrist key for each piece on each square.*/
    extern u64 ZOBRIST_PIECES[C_NUM_PIECES][C_NUM_SQUARES];
    /*Zobrist key for each en passant target square.*/
    extern u64 ZOBRIST_EN_PASSANT[C_NUM_FILES];
    /*Zobrist key for each castling right.*/
    extern u64 ZOBRIST_CASTLING[C_NUM_CASTLING_RIGHTS];
    /*Zobrist key for the current side to move.*/
    extern u64 ZOBRIST_SIDE_TO_MOVE;

    /*The seed for the random number generator.*/
    const int ZOBRIST_SEED = 100;

    /*Index in the zobrist castling table for each castling right.*/
    const int ZOBRIST_WHITE_CASTLE_KINGSIDE = 0;
    const int ZOBRIST_WHITE_CASTLE_QUEENSIDE = 1;
    const int ZOBRIST_BLACK_CASTLE_KINGSIDE = 2;
    const int ZOBRIST_BLACK_CASTLE_QUEENSIDE = 3;


    void init_zobrist();

}


#endif //CHESSENGINE_ZOBRIST_H
