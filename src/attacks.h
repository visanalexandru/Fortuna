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
    extern u64 RAY_ATTACKS[C_NUM_SQUARES][C_NUM_DIRECTIONS];

    /* Generates the king move bitboard. */
    u64 king_movement(u64 king_location);

    /* Generates the knight move bitboard. */
    u64 knight_movement(u64 knight_location);


    /* Initializes king attacks for all squares.*/
    void init_king_attacks();

    /* Initializes knight attacks for all squares.*/
    void init_knight_attacks();

    /* Initializes sliding rook attacks for all squares.*/
    void init_rook_attacks();

    /*Initializes sliding bishop attacks for all squares.*/
    void init_bishop_attacks();

    /* Returns the rook attack bitboard for the given square and blockers.*/
    u64 get_rook_attacks(Square square, u64 all);

    /* Returns the bishop attack bitboard for the given square and blockers.*/
    u64 get_bishop_attacks(Square square, u64 all);

    /* Initializes all attack tables. */
    void init_attack_tables();

}


#endif //CHESSENGINE_ATTACKS_H
