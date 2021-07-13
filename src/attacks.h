//
// Created by gvisan on 12.07.2021.
//

#ifndef CHESSENGINE_ATTACKS_H
#define CHESSENGINE_ATTACKS_H

#include"defines.h"
#include"tables.h"
#include"util.h"

namespace engine {
    extern u64 KING_ATTACKS[C_NUM_SQUARES];
    extern u64 KNIGHT_ATTACKS[C_NUM_SQUARES];

    /* Generates the king move bitboard. */
    u64 king_movement(u64 king_location);

    /* Generates the knight move bitboard. */
    u64 knight_movement(u64 knight_location);


    /* Initializes king attacks for all squares.*/
    void init_king_attacks();

    /* Initializes knight attacks for all squares.*/
    void init_knight_attacks();

    /* Initializes all attack tables. */
    void init_attack_tables();

}


#endif //CHESSENGINE_ATTACKS_H
