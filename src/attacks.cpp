//
// Created by gvisan on 12.07.2021.
//

#include "attacks.h"

namespace engine {
    u64 king_attacks[C_NUM_SQUARES];
    u64 knight_attacks[C_NUM_SQUARES];

    u64 king_movement(u64 king_location) {
        u64 clip_file_h = king_location & clear_file[FILE_H];
        u64 clip_file_a = king_location & clear_file[FILE_A];

        u64 moves = 0;
        moves |= clip_file_a << 7u;
        moves |= king_location << 8u;
        moves |= clip_file_h << 9u;
        moves |= clip_file_h << 1u;

        moves |= clip_file_h >> 7u;
        moves |= king_location >> 8u;
        moves |= clip_file_a >> 9u;
        moves |= clip_file_a >> 1u;
        return moves;
    }

    u64 knight_movement(u64 knight_location) {
        u64 clip_file_a = knight_location & clear_file[FILE_A];
        u64 clip_file_ab = clip_file_a & clear_file[FILE_B];

        u64 clip_file_h = knight_location & clear_file[FILE_H];
        u64 clip_file_hg = clip_file_h & clear_file[FILE_G];

        u64 moves = 0;
        moves |= clip_file_a >> 17u;
        moves |= clip_file_h >> 15u;
        moves |= clip_file_hg >> 6u;
        moves |= clip_file_ab >> 10u;
        moves |= clip_file_ab << 6u;
        moves |= clip_file_hg << 10u;
        moves |= clip_file_a << 15u;
        moves |= clip_file_h << 17u;

        return moves;
    }

    void init_king_attacks() {
        for (int square = 0; square < C_NUM_SQUARES; square++) {
            king_attacks[square] = king_movement(square_to_bitboard((Square) square));
        }
    }

    void init_knight_attacks() {
        for (int square = 0; square < C_NUM_SQUARES; square++) {
            knight_attacks[square] = knight_movement(square_to_bitboard((Square) square));
        }
    }

    void init_attack_tables() {
        init_king_attacks();
        init_knight_attacks();
    }
}