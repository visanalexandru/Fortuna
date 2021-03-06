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
    extern u64 ROOK_ATTACKS[C_NUM_SQUARES];
    extern u64 BISHOP_ATTACKS[C_NUM_SQUARES];
    extern u64 PAWN_ATTACKS[C_NUM_SQUARES][2];
    extern u64 RAY_ATTACKS[C_NUM_SQUARES][C_NUM_DIRECTIONS];
    extern u64 ROOK_MASKS[C_NUM_SQUARES];
    extern u64 BISHOP_MASKS[C_NUM_SQUARES];
    extern u64 ROOK_MAGIC[C_NUM_SQUARES][magic::ROOK_MAGIC_MAX];
    extern u64 BISHOP_MAGIC[C_NUM_SQUARES][magic::BISHOP_MAGIC_MAX];
    extern u64 IN_BETWEEN[C_NUM_SQUARES][C_NUM_SQUARES];
    extern u64 LINE[C_NUM_SQUARES][C_NUM_SQUARES];
    extern u64 PAWN_FORWARD[C_NUM_SQUARES][2];
    extern u64 PAWN_FRONT_SPANS[C_NUM_SQUARES][2];
    extern u64 STRONG_PAWN_SHIELD[C_NUM_SQUARES][2];
    extern u64 WEAK_PAWN_SHIELD[C_NUM_SQUARES][2];

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

    /*Initializes the in-between table.*/
    void init_in_between();

    /*Initializes all pawn-structure tables.*/
    void init_pawn_structure();

    /*Initializes the lines table.*/
    void init_lines();

    /* Returns the rook attack bitboard for the given square and blockers.*/
    u64 get_rook_attacks(Square square, u64 all);

    /* Returns the bishop attack bitboard for the given square and blockers.*/
    u64 get_bishop_attacks(Square square, u64 all);

    /*Get the rook attack bitboard for the given square and blockers by querying the magic tables.*/
    u64 get_magic_rook_attacks(Square square, u64 all);

    /*Get the bishop attack bitboard for the given square and blockers by querying the magic tables.*/
    u64 get_magic_bishop_attacks(Square square, u64 all);

    /*Get rook attacks through blockers, useful for finding pinned pieces.*/
    u64 xray_rook_attacks(Square square, u64 blockers, u64 all);

    /*Get bishop attacks through blockers, useful for finding pinned pieces.*/
    u64 xray_bishop_attacks(Square square, u64 blockers, u64 all);

    /* Initializes all tables. */
    void init_tables();

}


#endif //CHESSENGINE_ATTACKS_H
