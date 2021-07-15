//
// Created by gvisan on 11.07.2021.
//

#include "movegen.h"

namespace engine {

    MoveGen::MoveGen(Board &internal_board) : board(internal_board) {

    }

    void MoveGen::add_quiet_moves(Square origin, u64 attacks, Piece piece, std::vector<Move> &moves) {
        Square destination;
        while (attacks) {
            destination = popLsb(attacks);
            moves.push_back(create_quiet_move(origin, destination, piece));
        }
    }

    void MoveGen::add_capture_moves(Square origin, u64 attacks, Piece piece, std::vector<Move> &moves) {
        Square destination;
        Piece captured;

        while (attacks) {
            destination = popLsb(attacks);
            captured = board.current_position.pieces[destination];
            moves.push_back(create_capture_move(origin, destination, piece, captured));
        }
    }


    void MoveGen::add_white_king_moves(u64 black, u64 all, std::vector<Move> &moves) {
        /* A position without a king is illegal, but we still check if we have no king.*/
        if (board.current_position.placement[P_W_KING] == 0)
            return;
        Square king_square = popLsb(board.current_position.placement[P_W_KING]);
        u64 attacks = KING_ATTACKS[king_square];
        u64 king_quiet_moves = attacks & (~all);
        u64 king_captures = attacks & black;
        add_quiet_moves(king_square, king_quiet_moves, P_W_KING, moves);
        add_capture_moves(king_square, king_captures, P_W_KING, moves);
    }

    void MoveGen::add_black_king_moves(u64 white, u64 all, std::vector<Move> &moves) {
        /* A position without a king is illegal, but we still check if we have no king.*/
        if (board.current_position.placement[P_B_KING] == 0)
            return;
        Square king_square = popLsb(board.current_position.placement[P_B_KING]);
        u64 attacks = KING_ATTACKS[king_square];
        u64 king_quiet_moves = attacks & (~all);
        u64 king_captures = attacks & white;
        add_quiet_moves(king_square, king_quiet_moves, P_B_KING, moves);
        add_capture_moves(king_square, king_captures, P_B_KING, moves);
    }

    void MoveGen::add_white_knight_moves(u64 black, u64 all, std::vector<Move> &moves) {
        u64 knights = board.current_position.placement[P_W_KNIGHT], attacks, knight_quiet_moves, knight_captures;
        Square knight_square;
        while (knights) {
            knight_square = popLsb(knights);
            attacks = KNIGHT_ATTACKS[knight_square];
            knight_quiet_moves = attacks & (~all);
            knight_captures = attacks & (black);
            add_quiet_moves(knight_square, knight_quiet_moves, P_W_KNIGHT, moves);
            add_capture_moves(knight_square, knight_captures, P_W_KNIGHT, moves);

        }
    }

    void MoveGen::add_black_knight_moves(u64 white, u64 all, std::vector<Move> &moves) {
        u64 knights = board.current_position.placement[P_B_KNIGHT], attacks, knight_quiet_moves, knight_captures;
        Square knight_square;
        while (knights) {
            knight_square = popLsb(knights);
            attacks = KNIGHT_ATTACKS[knight_square];
            knight_quiet_moves = attacks & (~all);
            knight_captures = attacks & (white);
            add_quiet_moves(knight_square, knight_quiet_moves, P_B_KNIGHT, moves);
            add_capture_moves(knight_square, knight_captures, P_B_KNIGHT, moves);
        }
    }

    void MoveGen::add_white_pawn_pushes(u64 all, std::vector<Move> &moves) {
        u64 pawns = board.current_position.placement[P_W_PAWN];

        u64 single_step = (pawns << 8u) & (~all);
        u64 promotions = single_step & mask_rank[RANK_8];
        u64 double_push = ((single_step & mask_rank[RANK_3]) << 8u) & (~all);

        Square destination, origin;

        u64 single_push = single_step & clear_rank[RANK_8];

        while (single_push) {
            destination = popLsb(single_push);
            origin = (Square) (destination - 8);
            moves.push_back(create_quiet_move(origin, destination, P_W_PAWN));
        }

        while (double_push) {
            destination = popLsb(double_push);
            origin = (Square) (destination - 16);
            moves.push_back(create_double_pawn_push_move(origin, destination, P_W_PAWN));
        }

        while (promotions) {
            destination = popLsb(promotions);
            origin = (Square) (destination - 8);
            moves.push_back(create_promotion_move(origin, destination, P_W_PAWN, P_W_KNIGHT));
            moves.push_back(create_promotion_move(origin, destination, P_W_PAWN, P_W_BISHOP));
            moves.push_back(create_promotion_move(origin, destination, P_W_PAWN, P_W_ROOK));
            moves.push_back(create_promotion_move(origin, destination, P_W_PAWN, P_W_QUEEN));
        }
    }

    void MoveGen::add_black_pawn_pushes(u64 all, std::vector<Move> &moves) {
        u64 pawns = board.current_position.placement[P_B_PAWN];

        u64 single_step = (pawns >> 8u) & (~all);
        u64 promotions = single_step & mask_rank[RANK_1];
        u64 double_push = ((single_step & mask_rank[RANK_6]) >> 8u) & (~all);

        Square destination, origin;

        u64 single_push = single_step & clear_rank[RANK_1];

        while (single_push) {
            destination = popLsb(single_push);
            origin = (Square) (destination + 8);
            moves.push_back(create_quiet_move(origin, destination, P_B_PAWN));
        }

        while (double_push) {
            destination = popLsb(double_push);
            origin = (Square) (destination + 16);
            moves.push_back(create_double_pawn_push_move(origin, destination, P_B_PAWN));
        }

        while (promotions) {
            destination = popLsb(promotions);
            origin = (Square) (destination + 8);
            moves.push_back(create_promotion_move(origin, destination, P_B_PAWN, P_B_KNIGHT));
            moves.push_back(create_promotion_move(origin, destination, P_B_PAWN, P_B_BISHOP));
            moves.push_back(create_promotion_move(origin, destination, P_B_PAWN, P_B_ROOK));
            moves.push_back(create_promotion_move(origin, destination, P_B_PAWN, P_B_QUEEN));
        }
    }


    void MoveGen::add_white_pawn_captures(u64 black, std::vector<Move> &moves) {
        u64 pawns = board.current_position.placement[P_W_PAWN];

        u64 left_attacks = ((pawns & clear_file[FILE_H]) << 9u) & black;
        u64 right_attacks = ((pawns & clear_file[FILE_A]) << 7u) & black;

        u64 left_captures = left_attacks & clear_rank[RANK_8];
        u64 right_captures = right_attacks & clear_rank[RANK_8];

        u64 left_promotion_captures = left_attacks & mask_rank[RANK_8];
        u64 right_promotion_captures = right_attacks & mask_rank[RANK_8];

        Square origin, destination;
        Piece captured;

        while (left_captures) {
            destination = popLsb(left_captures);
            origin = (Square) (destination - 9);
            captured = board.current_position.pieces[destination];
            moves.push_back(create_capture_move(origin, destination, P_W_PAWN, captured));
        }

        while (right_captures) {
            destination = popLsb(right_captures);
            origin = (Square) (destination - 7);
            captured = board.current_position.pieces[destination];
            moves.push_back(create_capture_move(origin, destination, P_W_PAWN, captured));
        }

        while (left_promotion_captures) {
            destination = popLsb(left_promotion_captures);
            origin = (Square) (destination - 9);
            captured = board.current_position.pieces[destination];
            moves.push_back(create_promotion_capture_move(origin, destination, P_W_PAWN, captured, P_W_KNIGHT));
            moves.push_back(create_promotion_capture_move(origin, destination, P_W_PAWN, captured, P_W_BISHOP));
            moves.push_back(create_promotion_capture_move(origin, destination, P_W_PAWN, captured, P_W_ROOK));
            moves.push_back(create_promotion_capture_move(origin, destination, P_W_PAWN, captured, P_W_QUEEN));
        }

        while (right_promotion_captures) {
            destination = popLsb(right_promotion_captures);
            origin = (Square) (destination - 7);
            captured = board.current_position.pieces[destination];

            moves.push_back(create_promotion_capture_move(origin, destination, P_W_PAWN, captured, P_W_KNIGHT));
            moves.push_back(create_promotion_capture_move(origin, destination, P_W_PAWN, captured, P_W_BISHOP));
            moves.push_back(create_promotion_capture_move(origin, destination, P_W_PAWN, captured, P_W_ROOK));
            moves.push_back(create_promotion_capture_move(origin, destination, P_W_PAWN, captured, P_W_QUEEN));
        }
    }

    void MoveGen::add_black_pawn_captures(u64 white, std::vector<Move> &moves) {
        u64 pawns = board.current_position.placement[P_B_PAWN];

        u64 left_attacks = ((pawns & clear_file[FILE_A]) >> 9u) & white;
        u64 right_attacks = ((pawns & clear_file[FILE_H]) >> 7u) & white;

        u64 left_captures = left_attacks & clear_rank[RANK_1];
        u64 right_captures = right_attacks & clear_rank[RANK_1];

        u64 left_promotion_captures = left_attacks & mask_rank[RANK_1];
        u64 right_promotion_captures = right_attacks & mask_rank[RANK_1];

        Square origin, destination;
        Piece captured;

        while (left_captures) {
            destination = popLsb(left_captures);
            origin = (Square) (destination + 9);
            captured = board.current_position.pieces[destination];
            moves.push_back(create_capture_move(origin, destination, P_B_PAWN, captured));
        }

        while (right_captures) {
            destination = popLsb(right_captures);
            origin = (Square) (destination + 7);
            captured = board.current_position.pieces[destination];
            moves.push_back(create_capture_move(origin, destination, P_B_PAWN, captured));
        }

        while (left_promotion_captures) {
            destination = popLsb(left_promotion_captures);
            origin = (Square) (destination + 9);
            captured = board.current_position.pieces[destination];
            moves.push_back(create_promotion_capture_move(origin, destination, P_B_PAWN, captured, P_B_KNIGHT));
            moves.push_back(create_promotion_capture_move(origin, destination, P_B_PAWN, captured, P_B_BISHOP));
            moves.push_back(create_promotion_capture_move(origin, destination, P_B_PAWN, captured, P_B_ROOK));
            moves.push_back(create_promotion_capture_move(origin, destination, P_B_PAWN, captured, P_B_QUEEN));
        }

        while (right_promotion_captures) {
            destination = popLsb(right_promotion_captures);
            origin = (Square) (destination + 7);
            captured = board.current_position.pieces[destination];

            moves.push_back(create_promotion_capture_move(origin, destination, P_B_PAWN, captured, P_B_KNIGHT));
            moves.push_back(create_promotion_capture_move(origin, destination, P_B_PAWN, captured, P_B_BISHOP));
            moves.push_back(create_promotion_capture_move(origin, destination, P_B_PAWN, captured, P_B_ROOK));
            moves.push_back(create_promotion_capture_move(origin, destination, P_B_PAWN, captured, P_B_QUEEN));
        }
    }

    void MoveGen::add_white_en_passant_moves(std::vector<Move> &moves) {
        if (board.current_state->state & State::S_EN_PASSANT) {
            u64 pawns = board.current_position.placement[P_W_PAWN];
            Square ep_square = board.current_state->ep_square;
            u64 ep_target = square_to_bitboard(ep_square);

            u64 ep_target_left = (ep_target & clear_file[FILE_A]) >> 9u;
            u64 ep_target_right = (ep_target & clear_file[FILE_H]) >> 7u;

            u64 ep_attackers = (ep_target_left | ep_target_right) & pawns;

            Square origin;
            while (ep_attackers) {
                origin = popLsb(ep_attackers);
                moves.push_back(create_en_passant_move(origin, ep_square, P_W_PAWN, P_B_PAWN));
            }
        }
    }

    void MoveGen::add_black_en_passant_moves(std::vector<Move> &moves) {
        if (board.current_state->state & State::S_EN_PASSANT) {
            u64 pawns = board.current_position.placement[P_B_PAWN];
            Square ep_square = board.current_state->ep_square;
            u64 ep_target = square_to_bitboard(ep_square);

            u64 ep_target_left = (ep_target & clear_file[FILE_H]) << 9u;
            u64 ep_target_right = (ep_target & clear_file[FILE_A]) << 7u;

            u64 ep_attackers = (ep_target_left | ep_target_right) & pawns;

            Square origin;
            while (ep_attackers) {
                origin = popLsb(ep_attackers);
                moves.push_back(create_en_passant_move(origin, ep_square, P_B_PAWN, P_W_PAWN));
            }
        }
    }

    void MoveGen::add_white_pawn_moves(u64 black, u64 all, std::vector<Move> &moves) {
        add_white_pawn_pushes(all, moves);
        add_white_pawn_captures(black, moves);
        add_white_en_passant_moves(moves);
    }

    void MoveGen::add_black_pawn_moves(u64 white, u64 all, std::vector<Move> &moves) {
        add_black_pawn_pushes(all, moves);
        add_black_pawn_captures(white, moves);
        add_black_en_passant_moves(moves);
    }

    void MoveGen::add_white_rook_moves(u64 black, u64 all, std::vector<Move> &moves) {
        u64 rooks = board.current_position.placement[P_W_ROOK], rook_attacks, rook_captures, rook_quiet_moves;
        Square rook_square;
        while (rooks) {
            rook_square = popLsb(rooks);
            rook_attacks = get_magic_rook_attacks(rook_square, all);

            rook_captures = rook_attacks & black;
            rook_quiet_moves = rook_attacks & (~all);

            add_capture_moves(rook_square, rook_captures, P_W_ROOK, moves);
            add_quiet_moves(rook_square, rook_quiet_moves, P_W_ROOK, moves);
        }
    }

    void MoveGen::add_black_rook_moves(u64 white, u64 all, std::vector<Move> &moves) {
        u64 rooks = board.current_position.placement[P_B_ROOK], rook_attacks, rook_captures, rook_quiet_moves;
        Square rook_square;
        while (rooks) {
            rook_square = popLsb(rooks);
            rook_attacks = get_magic_rook_attacks(rook_square, all);

            rook_captures = rook_attacks & white;
            rook_quiet_moves = rook_attacks & (~all);

            add_capture_moves(rook_square, rook_captures, P_B_ROOK, moves);
            add_quiet_moves(rook_square, rook_quiet_moves, P_B_ROOK, moves);
        }
    }

    void MoveGen::add_white_bishop_moves(u64 black, u64 all, std::vector<Move> &moves) {
        u64 bishops = board.current_position.placement[P_W_BISHOP], bishop_attacks, bishop_captures, bishop_quiet_moves;
        Square bishop_square;
        while (bishops) {
            bishop_square = popLsb(bishops);
            bishop_attacks = get_magic_bishop_attacks(bishop_square, all);

            bishop_captures = bishop_attacks & black;
            bishop_quiet_moves = bishop_attacks & (~all);

            add_capture_moves(bishop_square, bishop_captures, P_W_BISHOP, moves);
            add_quiet_moves(bishop_square, bishop_quiet_moves, P_W_BISHOP, moves);
        }
    }

    void MoveGen::add_black_bishop_moves(u64 white, u64 all, std::vector<Move> &moves) {
        u64 bishops = board.current_position.placement[P_B_BISHOP], bishop_attacks, bishop_captures, bishop_quiet_moves;
        Square bishop_square;
        while (bishops) {
            bishop_square = popLsb(bishops);
            bishop_attacks = get_magic_bishop_attacks(bishop_square, all);

            bishop_captures = bishop_attacks & white;
            bishop_quiet_moves = bishop_attacks & (~all);

            add_capture_moves(bishop_square, bishop_captures, P_B_BISHOP, moves);
            add_quiet_moves(bishop_square, bishop_quiet_moves, P_B_BISHOP, moves);
        }
    }

    void MoveGen::add_white_queen_moves(u64 black, u64 all, std::vector<Move> &moves) {
        u64 queens = board.current_position.placement[P_W_QUEEN], queen_attacks, queen_captures, queen_quiet_moves;
        Square queen_square;
        while (queens) {
            queen_square = popLsb(queens);
            queen_attacks = get_magic_bishop_attacks(queen_square, all) | get_magic_rook_attacks(queen_square, all);

            queen_captures = queen_attacks & black;
            queen_quiet_moves = queen_attacks & (~all);

            add_capture_moves(queen_square, queen_captures, P_W_QUEEN, moves);
            add_quiet_moves(queen_square, queen_quiet_moves, P_W_QUEEN, moves);
        }
    }

    void MoveGen::add_black_queen_moves(u64 white, u64 all, std::vector<Move> &moves) {
        u64 queens = board.current_position.placement[P_B_QUEEN], queen_attacks, queen_captures, queen_quiet_moves;
        Square queen_square;
        while (queens) {
            queen_square = popLsb(queens);
            queen_attacks = get_magic_bishop_attacks(queen_square, all) | get_magic_rook_attacks(queen_square, all);

            queen_captures = queen_attacks & white;
            queen_quiet_moves = queen_attacks & (~all);

            add_capture_moves(queen_square, queen_captures, P_B_QUEEN, moves);
            add_quiet_moves(queen_square, queen_quiet_moves, P_B_QUEEN, moves);
        }
    }

    std::vector<Move> MoveGen::get_moves() {
        u64 white = board.current_position.placement[P_W_PAWN] |
                    board.current_position.placement[P_W_KNIGHT] |
                    board.current_position.placement[P_W_BISHOP] |
                    board.current_position.placement[P_W_ROOK] |
                    board.current_position.placement[P_W_QUEEN] |
                    board.current_position.placement[P_W_KING];

        u64 black = board.current_position.placement[P_B_PAWN] |
                    board.current_position.placement[P_B_KNIGHT] |
                    board.current_position.placement[P_B_BISHOP] |
                    board.current_position.placement[P_B_ROOK] |
                    board.current_position.placement[P_B_QUEEN] |
                    board.current_position.placement[P_B_KING];

        u64 all = white | black;
        std::vector<Move> result;

        if (board.current_state->state & State::S_SIDE_TO_MOVE) {
            add_black_king_moves(white, all, result);
            add_black_knight_moves(white, all, result);
            add_black_pawn_moves(white, all, result);
            add_black_rook_moves(white, all, result);
            add_black_bishop_moves(white, all, result);
            add_black_queen_moves(white, all, result);
        } else {
            add_white_king_moves(black, all, result);
            add_white_knight_moves(black, all, result);
            add_white_pawn_moves(black, all, result);
            add_white_rook_moves(black, all, result);
            add_white_bishop_moves(black, all, result);
            add_white_queen_moves(black, all, result);
        }
        return result;
    }

}