//
// Created by gvisan on 12.07.2021.
//

#ifndef CHESSENGINE_ATTACKS_H
#define CHESSENGINE_ATTACKS_H

#include"defines.h"
#include"tables.h"
#include"util.h"
#include"magic.h"

namespace engine {
    extern u64 KING_ATTACKS[C_NUM_SQUARES];
    extern u64 KNIGHT_ATTACKS[C_NUM_SQUARES];
    extern u64 PAWN_ATTACKS[C_NUM_SQUARES][2];
    extern u64 RAY_ATTACKS[C_NUM_SQUARES][C_NUM_DIRECTIONS];
    extern u64 ROOK_MASKS[C_NUM_SQUARES];
    extern u64 BISHOP_MASKS[C_NUM_SQUARES];
    extern u64 ROOK_MAGIC[C_NUM_SQUARES][magic::ROOK_MAGIC_MAX];
    extern u64 BISHOP_MAGIC[C_NUM_SQUARES][magic::BISHOP_MAGIC_MAX];

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

    /*Initializes pawn attacks for all squares.*/
    void init_pawn_attacks();

    /*Initializes the rook masks used to compute the magic tables.*/
    void init_rook_masks();

    /*Initializes the bishop masks used to compute the magic tables.*/
    void init_bishop_masks();

    /*Initializes the rook magic table.*/
    void init_rook_magic();

    /*Initializes the bishop magic table.*/
    void init_bishop_magic();

    /* Returns the rook attack bitboard for the given square and blockers.*/
    u64 get_rook_attacks(Square square, u64 all);

    /* Returns the bishop attack bitboard for the given square and blockers.*/
    u64 get_bishop_attacks(Square square, u64 all);

    /*Get the rook attack bitboard for the given square and blockers by querying the magic tables.*/
    u64 get_magic_rook_attacks(Square square, u64 all);

    /*Get the bishop attack bitboard for the given square and blockers by querying the magic tables.*/
    u64 get_magic_bishop_attacks(Square square, u64 all);

    /* Initializes all tables. */
    void init_tables();

}


#endif //CHESSENGINE_ATTACKS_H
