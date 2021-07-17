//
// Created by gvisan on 11.07.2021.
//

#include "movegen.h"
#include<cassert>

namespace engine {

    MoveGen::MoveGen(Board &internal_board) : board(internal_board) {
        moves.reserve(C_MAX_MOVES);
        legal_moves.reserve(C_MAX_MOVES);
    }

    void MoveGen::add_quiet_moves(Square origin, u64 attacks, Piece piece) {
        Square destination;
        while (attacks) {
            destination = popLsb(attacks);
            moves.push_back(create_quiet_move(origin, destination, piece));
        }
    }

    void MoveGen::add_capture_moves(Square origin, u64 attacks, Piece piece) {
        Square destination;
        Piece captured;

        while (attacks) {
            destination = popLsb(attacks);
            captured = board.current_position.pieces[destination];
            moves.push_back(create_capture_move(origin, destination, piece, captured));
        }
    }


    void MoveGen::add_white_king_moves(u64 black, u64 all) {
        /* A position without a king is illegal, but we still check if we have no king.*/
        if (board.current_position.placement[P_W_KING] == 0)
            return;
        u64 king_bitboard = board.current_position.placement[P_W_KING];
        Square king_square = popLsb(king_bitboard);
        u64 attacks = KING_ATTACKS[king_square];
        u64 king_quiet_moves = attacks & (~all);
        u64 king_captures = attacks & black;
        add_quiet_moves(king_square, king_quiet_moves, P_W_KING);
        add_capture_moves(king_square, king_captures, P_W_KING);
    }

    void MoveGen::add_black_king_moves(u64 white, u64 all) {
        /* A position without a king is illegal, but we still check if we have no king.*/
        if (board.current_position.placement[P_B_KING] == 0)
            return;
        u64 king_bitboard = board.current_position.placement[P_B_KING];
        Square king_square = popLsb(king_bitboard);
        u64 attacks = KING_ATTACKS[king_square];
        u64 king_quiet_moves = attacks & (~all);
        u64 king_captures = attacks & white;
        add_quiet_moves(king_square, king_quiet_moves, P_B_KING);
        add_capture_moves(king_square, king_captures, P_B_KING);
    }

    void MoveGen::add_white_knight_moves(u64 black, u64 all) {
        u64 knights = board.current_position.placement[P_W_KNIGHT], attacks, knight_quiet_moves, knight_captures;
        Square knight_square;
        while (knights) {
            knight_square = popLsb(knights);
            attacks = KNIGHT_ATTACKS[knight_square];
            knight_quiet_moves = attacks & (~all);
            knight_captures = attacks & (black);
            add_quiet_moves(knight_square, knight_quiet_moves, P_W_KNIGHT);
            add_capture_moves(knight_square, knight_captures, P_W_KNIGHT);

        }
    }

    void MoveGen::add_black_knight_moves(u64 white, u64 all) {
        u64 knights = board.current_position.placement[P_B_KNIGHT], attacks, knight_quiet_moves, knight_captures;
        Square knight_square;
        while (knights) {
            knight_square = popLsb(knights);
            attacks = KNIGHT_ATTACKS[knight_square];
            knight_quiet_moves = attacks & (~all);
            knight_captures = attacks & (white);
            add_quiet_moves(knight_square, knight_quiet_moves, P_B_KNIGHT);
            add_capture_moves(knight_square, knight_captures, P_B_KNIGHT);
        }
    }

    void MoveGen::add_white_pawn_pushes(u64 all) {
        u64 pawns = board.current_position.placement[P_W_PAWN];

        u64 single_step = (pawns << 8u) & (~all);
        u64 promotions = single_step & MASK_RANK[RANK_8];
        u64 double_push = ((single_step & MASK_RANK[RANK_3]) << 8u) & (~all);

        Square destination, origin;

        u64 single_push = single_step & CLEAR_RANK[RANK_8];

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

    void MoveGen::add_black_pawn_pushes(u64 all) {
        u64 pawns = board.current_position.placement[P_B_PAWN];

        u64 single_step = (pawns >> 8u) & (~all);
        u64 promotions = single_step & MASK_RANK[RANK_1];
        u64 double_push = ((single_step & MASK_RANK[RANK_6]) >> 8u) & (~all);

        Square destination, origin;

        u64 single_push = single_step & CLEAR_RANK[RANK_1];

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


    void MoveGen::add_white_pawn_captures(u64 black) {
        u64 pawns = board.current_position.placement[P_W_PAWN];

        u64 left_attacks = ((pawns & CLEAR_FILE[FILE_H]) << 9u) & black;
        u64 right_attacks = ((pawns & CLEAR_FILE[FILE_A]) << 7u) & black;

        u64 left_captures = left_attacks & CLEAR_RANK[RANK_8];
        u64 right_captures = right_attacks & CLEAR_RANK[RANK_8];

        u64 left_promotion_captures = left_attacks & MASK_RANK[RANK_8];
        u64 right_promotion_captures = right_attacks & MASK_RANK[RANK_8];

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

    void MoveGen::add_black_pawn_captures(u64 white) {
        u64 pawns = board.current_position.placement[P_B_PAWN];

        u64 left_attacks = ((pawns & CLEAR_FILE[FILE_A]) >> 9u) & white;
        u64 right_attacks = ((pawns & CLEAR_FILE[FILE_H]) >> 7u) & white;

        u64 left_captures = left_attacks & CLEAR_RANK[RANK_1];
        u64 right_captures = right_attacks & CLEAR_RANK[RANK_1];

        u64 left_promotion_captures = left_attacks & MASK_RANK[RANK_1];
        u64 right_promotion_captures = right_attacks & MASK_RANK[RANK_1];

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

    void MoveGen::add_white_en_passant_moves() {
        if (board.current_state->state & State::S_EN_PASSANT) {
            u64 pawns = board.current_position.placement[P_W_PAWN];
            Square ep_square = board.current_state->ep_square;
            u64 ep_target = square_to_bitboard(ep_square);

            u64 ep_target_left = (ep_target & CLEAR_FILE[FILE_A]) >> 9u;
            u64 ep_target_right = (ep_target & CLEAR_FILE[FILE_H]) >> 7u;

            u64 ep_attackers = (ep_target_left | ep_target_right) & pawns;

            Square origin;
            while (ep_attackers) {
                origin = popLsb(ep_attackers);
                moves.push_back(create_en_passant_move(origin, ep_square, P_W_PAWN, P_B_PAWN));
            }
        }
    }

    void MoveGen::add_black_en_passant_moves() {
        if (board.current_state->state & State::S_EN_PASSANT) {
            u64 pawns = board.current_position.placement[P_B_PAWN];
            Square ep_square = board.current_state->ep_square;
            u64 ep_target = square_to_bitboard(ep_square);

            u64 ep_target_left = (ep_target & CLEAR_FILE[FILE_H]) << 9u;
            u64 ep_target_right = (ep_target & CLEAR_FILE[FILE_A]) << 7u;

            u64 ep_attackers = (ep_target_left | ep_target_right) & pawns;

            Square origin;
            while (ep_attackers) {
                origin = popLsb(ep_attackers);
                moves.push_back(create_en_passant_move(origin, ep_square, P_B_PAWN, P_W_PAWN));
            }
        }
    }

    void MoveGen::add_white_pawn_moves(u64 black, u64 all) {
        add_white_pawn_pushes(all);
        add_white_pawn_captures(black);
        add_white_en_passant_moves();
    }

    void MoveGen::add_black_pawn_moves(u64 white, u64 all) {
        add_black_pawn_pushes(all);
        add_black_pawn_captures(white);
        add_black_en_passant_moves();
    }

    void MoveGen::add_white_rook_moves(u64 black, u64 all) {
        u64 rooks = board.current_position.placement[P_W_ROOK], rook_attacks, rook_captures, rook_quiet_moves;
        Square rook_square;
        while (rooks) {
            rook_square = popLsb(rooks);
            rook_attacks = get_magic_rook_attacks(rook_square, all);

            rook_captures = rook_attacks & black;
            rook_quiet_moves = rook_attacks & (~all);

            add_capture_moves(rook_square, rook_captures, P_W_ROOK);
            add_quiet_moves(rook_square, rook_quiet_moves, P_W_ROOK);
        }
    }

    void MoveGen::add_black_rook_moves(u64 white, u64 all) {
        u64 rooks = board.current_position.placement[P_B_ROOK], rook_attacks, rook_captures, rook_quiet_moves;
        Square rook_square;
        while (rooks) {
            rook_square = popLsb(rooks);
            rook_attacks = get_magic_rook_attacks(rook_square, all);

            rook_captures = rook_attacks & white;
            rook_quiet_moves = rook_attacks & (~all);

            add_capture_moves(rook_square, rook_captures, P_B_ROOK);
            add_quiet_moves(rook_square, rook_quiet_moves, P_B_ROOK);
        }
    }

    void MoveGen::add_white_bishop_moves(u64 black, u64 all) {
        u64 bishops = board.current_position.placement[P_W_BISHOP], bishop_attacks, bishop_captures, bishop_quiet_moves;
        Square bishop_square;
        while (bishops) {
            bishop_square = popLsb(bishops);
            bishop_attacks = get_magic_bishop_attacks(bishop_square, all);

            bishop_captures = bishop_attacks & black;
            bishop_quiet_moves = bishop_attacks & (~all);

            add_capture_moves(bishop_square, bishop_captures, P_W_BISHOP);
            add_quiet_moves(bishop_square, bishop_quiet_moves, P_W_BISHOP);
        }
    }

    void MoveGen::add_black_bishop_moves(u64 white, u64 all) {
        u64 bishops = board.current_position.placement[P_B_BISHOP], bishop_attacks, bishop_captures, bishop_quiet_moves;
        Square bishop_square;
        while (bishops) {
            bishop_square = popLsb(bishops);
            bishop_attacks = get_magic_bishop_attacks(bishop_square, all);

            bishop_captures = bishop_attacks & white;
            bishop_quiet_moves = bishop_attacks & (~all);

            add_capture_moves(bishop_square, bishop_captures, P_B_BISHOP);
            add_quiet_moves(bishop_square, bishop_quiet_moves, P_B_BISHOP);
        }
    }

    void MoveGen::add_white_queen_moves(u64 black, u64 all) {
        u64 queens = board.current_position.placement[P_W_QUEEN], queen_attacks, queen_captures, queen_quiet_moves;
        Square queen_square;
        while (queens) {
            queen_square = popLsb(queens);
            queen_attacks = get_magic_bishop_attacks(queen_square, all) | get_magic_rook_attacks(queen_square, all);

            queen_captures = queen_attacks & black;
            queen_quiet_moves = queen_attacks & (~all);

            add_capture_moves(queen_square, queen_captures, P_W_QUEEN);
            add_quiet_moves(queen_square, queen_quiet_moves, P_W_QUEEN);
        }
    }

    void MoveGen::add_black_queen_moves(u64 white, u64 all) {
        u64 queens = board.current_position.placement[P_B_QUEEN], queen_attacks, queen_captures, queen_quiet_moves;
        Square queen_square;
        while (queens) {
            queen_square = popLsb(queens);
            queen_attacks = get_magic_bishop_attacks(queen_square, all) | get_magic_rook_attacks(queen_square, all);

            queen_captures = queen_attacks & white;
            queen_quiet_moves = queen_attacks & (~all);

            add_capture_moves(queen_square, queen_captures, P_B_QUEEN);
            add_quiet_moves(queen_square, queen_quiet_moves, P_B_QUEEN);
        }
    }

    bool MoveGen::can_white_attack_square(Square square, u64 all) {
        u64 bishop_attacks = get_magic_bishop_attacks(square, all);
        u64 rook_attacks = get_magic_rook_attacks(square, all);

        if (bishop_attacks & board.current_position.placement[P_W_BISHOP])
            return true;

        if (rook_attacks & board.current_position.placement[P_W_ROOK])
            return true;

        if ((bishop_attacks | rook_attacks) & board.current_position.placement[P_W_QUEEN]) {
            return true;
        }

        if (KNIGHT_ATTACKS[square] & board.current_position.placement[P_W_KNIGHT])
            return true;
        if (KING_ATTACKS[square] & board.current_position.placement[P_W_KING])
            return true;

        /*We take the opposite color pawn attack to know if any white pawns attack this square.*/
        if (PAWN_ATTACKS[square][C_BLACK] & board.current_position.placement[P_W_PAWN])
            return true;

        return false;
    }

    bool MoveGen::can_black_attack_square(Square square, u64 all) {
        u64 bishop_attacks = get_magic_bishop_attacks(square, all);
        u64 rook_attacks = get_magic_rook_attacks(square, all);

        if (bishop_attacks & board.current_position.placement[P_B_BISHOP])
            return true;

        if (rook_attacks & board.current_position.placement[P_B_ROOK])
            return true;

        if ((bishop_attacks | rook_attacks) & board.current_position.placement[P_B_QUEEN]) {
            return true;
        }

        if (KNIGHT_ATTACKS[square] & board.current_position.placement[P_B_KNIGHT])
            return true;
        if (KING_ATTACKS[square] & board.current_position.placement[P_B_KING])
            return true;

        /*We take the opposite color pawn attack to know if any black pawns attack this square.*/
        if (PAWN_ATTACKS[square][C_WHITE] & board.current_position.placement[P_B_PAWN])
            return true;

        return false;
    }

    void MoveGen::add_white_castling_moves(u64 all) {
        if (can_black_attack_square(SQ_E1, all))
            return;

        bool occupied, attacked, can_castle;

        /*Kingside castling.*/
        occupied = ((square_to_bitboard(SQ_F1) | square_to_bitboard(SQ_G1)) & all);
        can_castle = board.current_state->state & S_WHITE_CASTLE_K;

        if (can_castle && !occupied) {
            attacked = can_black_attack_square(SQ_F1, all) ||
                       can_black_attack_square(SQ_G1, all);

            if (!attacked) {
                moves.push_back(create_kingside_castle_move(SQ_E1, SQ_G1, P_W_KING));
            }
        }
        /*Queenside castling.*/
        occupied = ((square_to_bitboard(SQ_D1) | square_to_bitboard(SQ_C1) | square_to_bitboard(SQ_B1)) & all);
        can_castle = board.current_state->state & S_WHITE_CASTLE_Q;

        if (can_castle && !occupied) {
            attacked = can_black_attack_square(SQ_D1, all) ||
                       can_black_attack_square(SQ_C1, all);
            if (!attacked) {
                moves.push_back(create_queenside_castle_move(SQ_E1, SQ_C1, P_W_KING));
            }
        }
    }

    void MoveGen::add_black_castling_moves(u64 all) {
        if (can_white_attack_square(SQ_E8, all))
            return;

        bool occupied, attacked, can_castle;

        /*Kingside castling.*/
        occupied = ((square_to_bitboard(SQ_F8) | square_to_bitboard(SQ_G8)) & all);
        can_castle = board.current_state->state & S_BLACK_CASTLE_K;

        if (can_castle && !occupied) {
            attacked = can_white_attack_square(SQ_F8, all) ||
                       can_white_attack_square(SQ_G8, all);

            if (!attacked) {
                moves.push_back(create_kingside_castle_move(SQ_E8, SQ_G8, P_B_KING));
            }
        }
        /*Queenside castling.*/
        occupied = ((square_to_bitboard(SQ_D8) | square_to_bitboard(SQ_C8) | square_to_bitboard(SQ_B8)) & all);
        can_castle = board.current_state->state & S_BLACK_CASTLE_Q;

        if (can_castle && !occupied) {
            attacked = can_white_attack_square(SQ_D8, all) ||
                       can_white_attack_square(SQ_C8, all);
            if (!attacked) {
                moves.push_back(create_queenside_castle_move(SQ_E8, SQ_C8, P_B_KING));
            }
        }
    }

    bool MoveGen::is_in_check(Color color) {
        u64 king_bitboard;
        u64 all = board.current_position.all;

        if (color == C_WHITE) {
            king_bitboard = board.current_position.placement[P_W_KING];
            return king_bitboard != 0 && can_black_attack_square(popLsb(king_bitboard), all);
        } else {
            king_bitboard = board.current_position.placement[P_B_KING];
            return king_bitboard != 0 && can_white_attack_square(popLsb(king_bitboard), all);
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
        moves.clear();
        legal_moves.clear();
        Color current = board.color_to_play();

        if (current == C_BLACK) {
            add_black_king_moves(white, all);
            add_black_knight_moves(white, all);
            add_black_pawn_moves(white, all);
            add_black_rook_moves(white, all);
            add_black_bishop_moves(white, all);
            add_black_queen_moves(white, all);
            add_black_castling_moves(all);
        } else {
            add_white_king_moves(black, all);
            add_white_knight_moves(black, all);
            add_white_pawn_moves(black, all);
            add_white_rook_moves(black, all);
            add_white_bishop_moves(black, all);
            add_white_queen_moves(black, all);
            add_white_castling_moves(all);
        }

        /*Checking for legality.*/
        for (const Move &move:moves) {
            board.make_move(move);
            if (!is_in_check(current)) {
                legal_moves.push_back(move);
            }
            board.undo_move(move);
        }
        return legal_moves;
    }


    unsigned int MoveGen::perft(unsigned int depth) {
        auto generated = get_moves();
        if (depth == 1) {
            return generated.size();
        } else {
            unsigned int result = 0;
            for (const Move &move:generated) {
                board.make_move(move);
                result += perft(depth - 1);
                board.undo_move(move);
            }
            return result;
        }
    }
}