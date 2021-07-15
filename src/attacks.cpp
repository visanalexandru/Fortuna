//
// Created by gvisan on 12.07.2021.
//

#include "attacks.h"

namespace engine {
    u64 KING_ATTACKS[C_NUM_SQUARES];
    u64 KNIGHT_ATTACKS[C_NUM_SQUARES];
    u64 RAY_ATTACKS[C_NUM_SQUARES][C_NUM_DIRECTIONS];
    u64 ROOK_MASKS[C_NUM_SQUARES];
    u64 BISHOP_MASKS[C_NUM_SQUARES];
    u64 ROOK_MAGIC[C_NUM_SQUARES][magic::ROOK_MAGIC_MAX];
    u64 BISHOP_MAGIC[C_NUM_SQUARES][magic::BISHOP_MAGIC_MAX];

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
            KING_ATTACKS[square] = king_movement(square_to_bitboard((Square) square));
        }
    }

    void init_knight_attacks() {
        for (int square = 0; square < C_NUM_SQUARES; square++) {
            KNIGHT_ATTACKS[square] = knight_movement(square_to_bitboard((Square) square));
        }
    }

    void init_rook_attacks() {
        u64 bit;
        for (int square = 0; square < C_NUM_SQUARES; square++) {

            if (square >= 8) {
                bit = square_to_bitboard((Square) (square - 8));
                RAY_ATTACKS[square][D_SOUTH] = bit | RAY_ATTACKS[square - 8][D_SOUTH];
            }
            if (square % 8 != 0) {
                bit = square_to_bitboard((Square) (square - 1));
                RAY_ATTACKS[square][D_WEST] = bit | RAY_ATTACKS[square - 1][D_WEST];
            }
        }

        for (int square = C_NUM_SQUARES - 1; square >= 0; square--) {

            if (square < C_NUM_SQUARES - 8) {
                bit = square_to_bitboard((Square) (square + 8));
                RAY_ATTACKS[square][D_NORTH] = bit | RAY_ATTACKS[square + 8][D_NORTH];
            }
            if (square % 8 != 7) {
                bit = square_to_bitboard((Square) (square + 1));
                RAY_ATTACKS[square][D_EAST] = bit | RAY_ATTACKS[square + 1][D_EAST];
            }
        }
    }

    void init_bishop_attacks() {
        u64 bit;
        for (int square = 0; square < C_NUM_SQUARES; square++) {
            if (square >= 8 && square % 8 != 0) {
                bit = square_to_bitboard((Square) (square - 9));
                RAY_ATTACKS[square][D_SOUTH_WEST] = bit | RAY_ATTACKS[square - 9][D_SOUTH_WEST];
            }
            if (square >= 8 && square % 8 != 7) {
                bit = square_to_bitboard((Square) (square - 7));
                RAY_ATTACKS[square][D_SOUTH_EAST] = bit | RAY_ATTACKS[square - 7][D_SOUTH_EAST];
            }
        }

        for (int square = C_NUM_SQUARES - 1; square >= 0; square--) {
            if (square < C_NUM_SQUARES - 8 && square % 8 != 0) {
                bit = square_to_bitboard((Square) (square + 7));
                RAY_ATTACKS[square][D_NORTH_WEST] = bit | RAY_ATTACKS[square + 7][D_NORTH_WEST];
            }
            if (square < C_NUM_SQUARES - 8 && square % 8 != 7) {
                bit = square_to_bitboard((Square) (square + 9));
                RAY_ATTACKS[square][D_NORTH_EAST] = bit | RAY_ATTACKS[square + 9][D_NORTH_EAST];
            }
        }
    }

    void init_rook_masks() {
        for (int square = 0; square < C_NUM_SQUARES; square++) {
            ROOK_MASKS[square] = (RAY_ATTACKS[square][D_NORTH] & clear_rank[RANK_8]) |
                                 (RAY_ATTACKS[square][D_SOUTH] & clear_rank[RANK_1]) |
                                 (RAY_ATTACKS[square][D_EAST] & clear_file[FILE_H]) |
                                 (RAY_ATTACKS[square][D_WEST] & clear_file[FILE_A]);
        }
    }


    void init_bishop_masks() {
        for (int square = 0; square < C_NUM_SQUARES; square++) {
            BISHOP_MASKS[square] = (RAY_ATTACKS[square][D_SOUTH_WEST] & clear_rank[RANK_1] & clear_file[FILE_A]) |
                                   (RAY_ATTACKS[square][D_SOUTH_EAST] & clear_rank[RANK_1] & clear_file[FILE_H]) |
                                   (RAY_ATTACKS[square][D_NORTH_WEST] & clear_rank[RANK_8] & clear_file[FILE_A]) |
                                   (RAY_ATTACKS[square][D_NORTH_EAST] & clear_rank[RANK_8] & clear_file[FILE_H]);
        }
    }

    u64 get_rook_attacks(Square square, u64 all) {
        Square hit;
        u64 attacks = 0, ray, ray_hit;

        /* South direction.*/
        ray = RAY_ATTACKS[square][D_SOUTH], ray_hit = ray & all;
        attacks |= ray;
        if (ray_hit) {
            hit = msb(ray_hit);
            attacks &= ~RAY_ATTACKS[hit][D_SOUTH];
        }

        /* North direction.*/
        ray = RAY_ATTACKS[square][D_NORTH], ray_hit = ray & all;
        attacks |= ray;
        if (ray_hit) {
            hit = lsb(ray_hit);
            attacks &= ~RAY_ATTACKS[hit][D_NORTH];
        }

        /* West direction.*/
        ray = RAY_ATTACKS[square][D_WEST], ray_hit = ray & all;
        attacks |= ray;
        if (ray_hit) {
            hit = msb(ray_hit);
            attacks &= ~RAY_ATTACKS[hit][D_WEST];
        }

        /* East direction.*/
        ray = RAY_ATTACKS[square][D_EAST], ray_hit = ray & all;
        attacks |= ray;
        if (ray_hit) {
            hit = lsb(ray_hit);
            attacks &= ~RAY_ATTACKS[hit][D_EAST];
        }

        return attacks;
    }

    u64 get_bishop_attacks(Square square, u64 all) {
        Square hit;
        u64 attacks = 0, ray, ray_hit;

        /* South-west direction.*/
        ray = RAY_ATTACKS[square][D_SOUTH_WEST], ray_hit = ray & all;
        attacks |= ray;
        if (ray_hit) {
            hit = msb(ray_hit);
            attacks &= ~RAY_ATTACKS[hit][D_SOUTH_WEST];
        }

        /* North-west direction.*/
        ray = RAY_ATTACKS[square][D_NORTH_WEST], ray_hit = ray & all;
        attacks |= ray;
        if (ray_hit) {
            hit = lsb(ray_hit);
            attacks &= ~RAY_ATTACKS[hit][D_NORTH_WEST];
        }

        /* South-east direction.*/
        ray = RAY_ATTACKS[square][D_SOUTH_EAST], ray_hit = ray & all;
        attacks |= ray;
        if (ray_hit) {
            hit = msb(ray_hit);
            attacks &= ~RAY_ATTACKS[hit][D_SOUTH_EAST];
        }

        /* North-east direction.*/
        ray = RAY_ATTACKS[square][D_NORTH_EAST], ray_hit = ray & all;
        attacks |= ray;
        if (ray_hit) {
            hit = lsb(ray_hit);
            attacks &= ~RAY_ATTACKS[hit][D_NORTH_EAST];
        }

        return attacks;
    }


    void init_rook_magic() {
        unsigned long long index;
        u64 mask, sub_mask;
        for (int square = 0; square < C_NUM_SQUARES; square++) {
            mask = ROOK_MASKS[square];
            sub_mask = mask;

            //We iterate through all the sub-masks of the occupancy mask.
            while (sub_mask) {
                index = (sub_mask * magic::MAGIC_NUM_R[square]) >> (64u - magic::BITS_R[square]);
                ROOK_MAGIC[square][index] = get_rook_attacks((Square) square, sub_mask);
                sub_mask = (sub_mask - 1) & mask;
            }
            //Don't forget about the zero sub-mask
            index = (sub_mask * magic::MAGIC_NUM_R[square]) >> (64u - magic::BITS_R[square]);
            ROOK_MAGIC[square][index] = get_rook_attacks((Square) square, sub_mask);
        }
    }

    void init_bishop_magic() {
        unsigned long long index;
        u64 mask, sub_mask;
        for (int square = 0; square < C_NUM_SQUARES; square++) {
            mask = BISHOP_MASKS[square];
            sub_mask = mask;

            //We iterate through all the sub-masks of the occupancy mask.
            while (sub_mask) {
                index = (sub_mask * magic::MAGIC_NUM_B[square]) >> (64u - magic::BITS_B[square]);
                BISHOP_MAGIC[square][index] = get_bishop_attacks((Square) square, sub_mask);
                sub_mask = (sub_mask - 1) & mask;
            }
            //Don't forget about the zero sub-mask
            index = (sub_mask * magic::MAGIC_NUM_B[square]) >> (64u - magic::BITS_B[square]);
            BISHOP_MAGIC[square][index] = get_bishop_attacks((Square) square, sub_mask);
        }
    }


    u64 get_magic_rook_attacks(Square square, u64 all) {
        u64 blockers = all & ROOK_MASKS[square];

        unsigned long index = (blockers * magic::MAGIC_NUM_R[square]) >> (64u - magic::BITS_R[square]);
        return ROOK_MAGIC[square][index];
    }

    u64 get_magic_bishop_attacks(Square square, u64 all) {
        u64 blockers = all & BISHOP_MASKS[square];

        unsigned long index = (blockers * magic::MAGIC_NUM_B[square]) >> (64u - magic::BITS_B[square]);
        return BISHOP_MAGIC[square][index];
    }

    void init_tables() {
        init_king_attacks();
        init_knight_attacks();
        init_rook_attacks();
        init_bishop_attacks();
        init_rook_masks();
        init_bishop_masks();
        init_rook_magic();
        init_bishop_magic();
    }
}