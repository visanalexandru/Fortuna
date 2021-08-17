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

    template<Color side, GenType type>
    void MoveGen::add_king_moves(u64 opposite, u64 all) {
        u64 king_bitboard = board.current_position.placement[get_piece(PT_KING, side)];
        assert(king_bitboard != 0);
        Square king_square = popLsb(king_bitboard);
        u64 attacks = KING_ATTACKS[king_square];
        u64 king_quiet_moves = attacks & (~all);
        u64 king_captures = attacks & opposite;
        if (type == GT_NORMAL) {
            add_quiet_moves(king_square, king_quiet_moves, get_piece(PT_KING, side));
        }
        add_capture_moves(king_square, king_captures, get_piece(PT_KING, side));
    }

    template<Color side, GenType type>
    void MoveGen::add_knight_moves(u64 opposite, u64 all) {
        u64 knights = board.current_position.placement[get_piece(PT_KNIGHT, side)];
        u64 attacks, knight_quiet_moves, knight_captures;
        Square knight_square;
        while (knights) {
            knight_square = popLsb(knights);
            attacks = KNIGHT_ATTACKS[knight_square];
            knight_quiet_moves = attacks & (~all);
            knight_captures = attacks & (opposite);
            if (type == GT_NORMAL) {
                add_quiet_moves(knight_square, knight_quiet_moves, get_piece(PT_KNIGHT, side));
            }
            add_capture_moves(knight_square, knight_captures, get_piece(PT_KNIGHT, side));

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

    template<GenType type>
    void MoveGen::add_white_pawn_moves(u64 black, u64 all) {
        if (type == GT_NORMAL) {
            add_white_pawn_pushes(all);
        }
        add_white_pawn_captures(black);
        add_white_en_passant_moves();
    }

    template<GenType type>
    void MoveGen::add_black_pawn_moves(u64 white, u64 all) {
        if (type == GT_NORMAL) {
            add_black_pawn_pushes(all);
        }
        add_black_pawn_captures(white);
        add_black_en_passant_moves();
    }

    template<Color side, GenType type>
    void MoveGen::add_rook_moves(u64 opposite, u64 all) {
        u64 rooks = board.current_position.placement[get_piece(PT_ROOK, side)];
        u64 rook_attacks, rook_captures, rook_quiet_moves;
        Square rook_square;
        while (rooks) {
            rook_square = popLsb(rooks);
            rook_attacks = get_magic_rook_attacks(rook_square, all);

            rook_captures = rook_attacks & opposite;
            rook_quiet_moves = rook_attacks & (~all);

            if (type == GT_NORMAL) {
                add_quiet_moves(rook_square, rook_quiet_moves, get_piece(PT_ROOK, side));
            }
            add_capture_moves(rook_square, rook_captures, get_piece(PT_ROOK, side));
        }
    }

    template<Color side, GenType type>
    void MoveGen::add_bishop_moves(u64 opposite, u64 all) {
        u64 bishops = board.current_position.placement[get_piece(PT_BISHOP, side)];
        u64 bishop_attacks, bishop_captures, bishop_quiet_moves;
        Square bishop_square;
        while (bishops) {
            bishop_square = popLsb(bishops);
            bishop_attacks = get_magic_bishop_attacks(bishop_square, all);

            bishop_captures = bishop_attacks & opposite;
            bishop_quiet_moves = bishop_attacks & (~all);

            if (type == GT_NORMAL) {
                add_quiet_moves(bishop_square, bishop_quiet_moves, get_piece(PT_BISHOP, side));
            }
            add_capture_moves(bishop_square, bishop_captures, get_piece(PT_BISHOP, side));
        }
    }

    template<Color side, GenType type>
    void MoveGen::add_queen_moves(u64 opposite, u64 all) {
        u64 queens = board.current_position.placement[get_piece(PT_QUEEN, side)];
        u64 queen_attacks, queen_captures, queen_quiet_moves;
        Square queen_square;
        while (queens) {
            queen_square = popLsb(queens);
            queen_attacks = get_magic_bishop_attacks(queen_square, all) | get_magic_rook_attacks(queen_square, all);

            queen_captures = queen_attacks & opposite;
            queen_quiet_moves = queen_attacks & (~all);

            if (type == GT_NORMAL) {
                add_quiet_moves(queen_square, queen_quiet_moves, get_piece(PT_QUEEN, side));
            }
            add_capture_moves(queen_square, queen_captures, get_piece(PT_QUEEN, side));
        }
    }

    template<Color side>
    bool MoveGen::can_attack_square(Square square, u64 all) {
        u64 rooks = board.current_position.placement[get_piece(PT_ROOK, side)];
        u64 bishops = board.current_position.placement[get_piece(PT_BISHOP, side)];
        u64 queens = board.current_position.placement[get_piece(PT_QUEEN, side)];

        u64 bishop_attacks = 0, rook_attacks = 0;

        if (ROOK_ATTACKS[square] & (queens | rooks))
            rook_attacks = get_magic_rook_attacks(square, all);

        if (BISHOP_ATTACKS[square] & (queens | bishops))
            bishop_attacks = get_magic_bishop_attacks(square, all);

        if (bishop_attacks & bishops)
            return true;

        if (rook_attacks & rooks)
            return true;

        if ((bishop_attacks | rook_attacks) & queens) {
            return true;
        }

        if (KNIGHT_ATTACKS[square] & board.current_position.placement[get_piece(PT_KNIGHT, side)])
            return true;
        if (KING_ATTACKS[square] & board.current_position.placement[get_piece(PT_KING, side)])
            return true;

        /*We take the opposite color pawn attack to know if any white pawns attack this square.*/
        if (PAWN_ATTACKS[square][get_opposite(side)] & board.current_position.placement[get_piece(PT_PAWN, side)])
            return true;

        return false;
    }

    void MoveGen::add_white_castling_moves(u64 all) {
        if (can_attack_square<C_BLACK>(SQ_E1, all))
            return;

        bool occupied, attacked, can_castle;

        /*Kingside castling.*/
        occupied = ((square_to_bitboard(SQ_F1) | square_to_bitboard(SQ_G1)) & all);
        can_castle = board.current_state->state & S_WHITE_CASTLE_K;

        if (can_castle && !occupied) {
            attacked = can_attack_square<C_BLACK>(SQ_F1, all) ||
                       can_attack_square<C_BLACK>(SQ_G1, all);

            if (!attacked) {
                legal_moves.push_back(create_kingside_castle_move(SQ_E1, SQ_G1, P_W_KING));
            }
        }
        /*Queenside castling.*/
        occupied = ((square_to_bitboard(SQ_D1) | square_to_bitboard(SQ_C1) | square_to_bitboard(SQ_B1)) & all);
        can_castle = board.current_state->state & S_WHITE_CASTLE_Q;

        if (can_castle && !occupied) {
            attacked = can_attack_square<C_BLACK>(SQ_D1, all) ||
                       can_attack_square<C_BLACK>(SQ_C1, all);
            if (!attacked) {
                legal_moves.push_back(create_queenside_castle_move(SQ_E1, SQ_C1, P_W_KING));
            }
        }
    }

    void MoveGen::add_black_castling_moves(u64 all) {
        if (can_attack_square<C_WHITE>(SQ_E8, all))
            return;

        bool occupied, attacked, can_castle;

        /*Kingside castling.*/
        occupied = ((square_to_bitboard(SQ_F8) | square_to_bitboard(SQ_G8)) & all);
        can_castle = board.current_state->state & S_BLACK_CASTLE_K;

        if (can_castle && !occupied) {
            attacked = can_attack_square<C_WHITE>(SQ_F8, all) ||
                       can_attack_square<C_WHITE>(SQ_G8, all);

            if (!attacked) {
                legal_moves.push_back(create_kingside_castle_move(SQ_E8, SQ_G8, P_B_KING));
            }
        }
        /*Queenside castling.*/
        occupied = ((square_to_bitboard(SQ_D8) | square_to_bitboard(SQ_C8) | square_to_bitboard(SQ_B8)) & all);
        can_castle = board.current_state->state & S_BLACK_CASTLE_Q;

        if (can_castle && !occupied) {
            attacked = can_attack_square<C_WHITE>(SQ_D8, all) ||
                       can_attack_square<C_WHITE>(SQ_C8, all);
            if (!attacked) {
                legal_moves.push_back(create_queenside_castle_move(SQ_E8, SQ_C8, P_B_KING));
            }
        }
    }

    bool MoveGen::is_in_check(Color color) {
        u64 king_bitboard;
        u64 all = board.current_position.all;

        if (color == C_WHITE) {
            king_bitboard = board.current_position.placement[P_W_KING];
            assert(king_bitboard != 0);
            return can_attack_square<C_BLACK>(popLsb(king_bitboard), all);
        } else {
            king_bitboard = board.current_position.placement[P_B_KING];
            assert(king_bitboard != 0);
            return can_attack_square<C_WHITE>(popLsb(king_bitboard), all);
        }
    }

    bool MoveGen::is_legal(const Move &move, Color color) {
        bool legal;
        board.make_move(move);
        legal = !is_in_check(color);
        board.undo_move(move);
        return legal;
    }

    u64 MoveGen::get_pinned_pieces(Color color, u64 white, u64 black) {
        u64 pinned = 0, occupied = board.current_position.all;
        u64 king_bitboard, own, op_rq, op_bq;

        if (color == C_WHITE) {
            king_bitboard = board.current_position.placement[P_W_KING];
            assert(king_bitboard != 0);
            own = white;
            op_bq = board.current_position.placement[P_B_QUEEN] | board.current_position.placement[P_B_BISHOP];
            op_rq = board.current_position.placement[P_B_QUEEN] | board.current_position.placement[P_B_ROOK];
        } else {
            king_bitboard = board.current_position.placement[P_B_KING];
            assert(king_bitboard != 0);
            own = black;
            op_bq = board.current_position.placement[P_W_QUEEN] | board.current_position.placement[P_W_BISHOP];
            op_rq = board.current_position.placement[P_W_QUEEN] | board.current_position.placement[P_W_ROOK];
        }

        Square king_square = popLsb(king_bitboard), pinner_square;
        u64 pins = xray_rook_attacks(king_square, own, occupied) & op_rq;

        while (pins) {
            pinner_square = popLsb(pins);
            pinned |= IN_BETWEEN[king_square][pinner_square] & own;
        }

        pins = xray_bishop_attacks(king_square, own, occupied) & op_bq;
        while (pins) {
            pinner_square = popLsb(pins);
            pinned |= IN_BETWEEN[king_square][pinner_square] & own;
        }
        return pinned;
    }

    template<GenType type>
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

        /*Getting the king square.*/
        Square king_square;
        u64 king_bitboard = board.current_position.placement[get_piece(PT_KING, current)];
        assert(king_bitboard != 0);
        king_square = popLsb(king_bitboard);

        /*Generating all moves.*/
        if (current == C_BLACK) {
            add_king_moves<C_BLACK, type>(white, all);
            add_knight_moves<C_BLACK, type>(white, all);
            add_black_pawn_moves<type>(white, all);
            add_rook_moves<C_BLACK, type>(white, all);
            add_bishop_moves<C_BLACK, type>(white, all);
            add_queen_moves<C_BLACK, type>(white, all);
            if (type == GT_NORMAL) {
                add_black_castling_moves(all);
            }
        } else {
            add_king_moves<C_WHITE, type>(black, all);
            add_knight_moves<C_WHITE, type>(black, all);
            add_white_pawn_moves<type>(black, all);
            add_rook_moves<C_WHITE, type>(black, all);
            add_bishop_moves<C_WHITE, type>(black, all);
            add_queen_moves<C_WHITE, type>(black, all);
            if (type == GT_NORMAL) {
                add_white_castling_moves(all);
            }
        }

        bool in_check = is_in_check(current);
        u64 pinned_pieces = (in_check) ? 0 : get_pinned_pieces(current, white, black);
        u64 king_evasions = (!in_check) ? 0 : get_magic_rook_attacks(king_square, all) |
                                              get_magic_bishop_attacks(king_square, all) |
                                              KNIGHT_ATTACKS[king_square];

        /*Checking for legality.*/
        bool legal;
        for (const Move &move:moves) {
            legal = false;

            if (move.type == MoveType::M_EN_PASSANT) {
                legal = is_legal(move, current);
            } else if (move.origin == king_square) {
                legal = (current == C_WHITE) ? (!can_attack_square<C_BLACK>(move.destination,
                                                                            all ^ square_to_bitboard(king_square)))
                                             : (!can_attack_square<C_WHITE>(move.destination,
                                                                            all ^ square_to_bitboard(king_square)));
            } else if (!in_check) {
                if (square_to_bitboard(move.origin) & pinned_pieces) {
                    legal = LINE[move.origin][move.destination] & square_to_bitboard(king_square);
                } else legal = true;

            } else {
                if ((square_to_bitboard(move.destination)) & king_evasions) {
                    legal = is_legal(move, current);
                }
            }

            if (legal)
                legal_moves.push_back(move);
        }
        return legal_moves;
    }


    unsigned int MoveGen::perft(unsigned int depth) {
        auto generated = get_moves<GT_NORMAL>();
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