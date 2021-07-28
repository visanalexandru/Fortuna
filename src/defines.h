//
// Created by gvisan on 10.07.2021.
//
#ifndef CHESSENGINE_DEFINES_H
#define CHESSENGINE_DEFINES_H

#include<cstdint>
#include<string>


namespace engine {

    typedef std::uint64_t u64;
    typedef std::uint8_t u8;
    typedef std::uint16_t u16;
    typedef std::uint32_t u32;

    const int C_MAX_PLY = 512;
    const int C_NUM_PIECES = 12;
    const int C_NUM_SQUARES = 64;
    const int C_NUM_DIRECTIONS = 8;
    const int C_MAX_MOVES = 256;
    const int C_NUM_PIECE_TYPES = 6;
    const int C_NUM_PHASES = 2;

    const std::string C_BASE_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    enum Color : u8 {
        C_WHITE,
        C_BLACK
    };

    enum Piece : u8 {
        P_W_PAWN = 0,
        P_W_KNIGHT,
        P_W_BISHOP,
        P_W_ROOK,
        P_W_QUEEN,
        P_W_KING,
        P_B_PAWN,
        P_B_KNIGHT,
        P_B_BISHOP,
        P_B_ROOK,
        P_B_QUEEN,
        P_B_KING,
        P_NONE
    };

    enum PieceType : u8 {
        PT_PAWN = 0,
        PT_KNIGHT,
        PT_BISHOP,
        PT_ROOK,
        PT_QUEEN,
        PT_KING,
        PT_NONE
    };

    enum MoveType : u8 {
        M_QUIET,
        M_CAPTURE,
        M_QUEENSIDE_CASTLE,
        M_KINGSIDE_CASTLE,
        M_EN_PASSANT,
        M_PROMOTION,
        M_PROMOTION_CAPTURE,
        M_DOUBLE_PAWN_PUSH,
    };

    enum Square : u8 {
        SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
        SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
        SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
        SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
        SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
        SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
        SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
        SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
        SQ_NONE = 64,
    };

    enum File {
        FILE_A = 0,
        FILE_B,
        FILE_C,
        FILE_D,
        FILE_E,
        FILE_F,
        FILE_G,
        FILE_H
    };

    enum Rank {
        RANK_1 = 0,
        RANK_2,
        RANK_3,
        RANK_4,
        RANK_5,
        RANK_6,
        RANK_7,
        RANK_8
    };

    enum Direction {
        D_NORTH,
        D_NORTH_EAST,
        D_EAST,
        D_SOUTH_EAST,
        D_SOUTH,
        D_SOUTH_WEST,
        D_WEST,
        D_NORTH_WEST
    };

    enum Phase {
        P_MIDDLEGAME,
        P_ENDGAME
    };

    enum State : u32 {
        S_SIDE_TO_MOVE = 1, /* The side currently moving. */
        S_WHITE_CASTLE_K = 1u << 1u, /* Can white castle kingside. */
        S_WHITE_CASTLE_Q = 1u << 2u, /* Can white castle queenside.*/
        S_BLACK_CASTLE_K = 1u << 3u, /* Can black castle kingside. */
        S_BLACK_CASTLE_Q = 1u << 4u, /* Can black castle queenside.*/
        S_EN_PASSANT = 1u << 5u /* This flag is set if the last move was a double pawn push. */
    };
}


#endif //CHESSENGINE_DEFINES_H
