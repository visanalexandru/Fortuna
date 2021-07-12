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
        Square king_square = popLsb(board.current_position.placement[WKing]);
        u64 attacks = king_attacks[king_square];
        u64 king_quiet_moves = attacks & (~all);
        u64 king_captures = attacks & black;
        add_quiet_moves(king_square, king_quiet_moves, WKing, moves);
        add_capture_moves(king_square, king_captures, WKing, moves);
    }

    void MoveGen::add_black_king_moves(u64 white, u64 all, std::vector<Move> &moves) {

        Square king_square = popLsb(board.current_position.placement[BKing]);
        u64 attacks = king_attacks[king_square];
        u64 king_quiet_moves = attacks & (~all);
        u64 king_captures = attacks & white;
        add_quiet_moves(king_square, king_quiet_moves, BKing, moves);
        add_capture_moves(king_square, king_captures, BKing, moves);
    }

    void MoveGen::add_white_knight_moves(u64 black, u64 all, std::vector<Move> &moves) {
        u64 knights = board.current_position.placement[WKnight], attacks, knight_quiet_moves, knight_captures;
        Square knight_square;
        while (knights) {
            knight_square = popLsb(knights);
            attacks = knight_attacks[knight_square];
            knight_quiet_moves = attacks & (~all);
            knight_captures = attacks & (black);
            add_quiet_moves(knight_square, knight_quiet_moves, WKnight, moves);
            add_capture_moves(knight_square, knight_captures, WKnight, moves);

        }
    }

    void MoveGen::add_black_knight_moves(u64 white, u64 all, std::vector<Move> &moves) {
        u64 knights = board.current_position.placement[BKnight], attacks, knight_quiet_moves, knight_captures;
        Square knight_square;
        while (knights) {
            knight_square = popLsb(knights);
            attacks = knight_attacks[knight_square];
            knight_quiet_moves = attacks & (~all);
            knight_captures = attacks & (white);
            add_quiet_moves(knight_square, knight_quiet_moves, BKnight, moves);
            add_capture_moves(knight_square, knight_captures, BKnight, moves);
        }
    }

    void MoveGen::add_white_pawn_pushes(u64 all, std::vector<Move> &moves) {
        u64 pawns = board.current_position.placement[WPawn];

        u64 single_step = (pawns << 8u) & (~all);
        u64 promotions = single_step & mask_rank[RANK_8];
        u64 double_push = ((single_step & mask_rank[RANK_3]) << 8u) & (~all);

        Square destination, origin;

        u64 single_push = single_step & clear_rank[RANK_8];

        while (single_push) {
            destination = popLsb(single_push);
            origin = (Square) (destination - 8);
            moves.push_back(create_quiet_move(origin, destination, WPawn));
        }

        while (double_push) {
            destination = popLsb(double_push);
            origin = (Square) (destination - 16);
            moves.push_back(create_double_pawn_push_move(origin, destination, WPawn));
        }

        while (promotions) {
            destination = popLsb(promotions);
            origin = (Square) (destination - 8);
            moves.push_back(create_promotion_move(origin, destination, WPawn, WKnight));
            moves.push_back(create_promotion_move(origin, destination, WPawn, WBishop));
            moves.push_back(create_promotion_move(origin, destination, WPawn, WRook));
            moves.push_back(create_promotion_move(origin, destination, WPawn, WQueen));
        }
    }

    void MoveGen::add_black_pawn_pushes(u64 all, std::vector<Move> &moves) {
        u64 pawns = board.current_position.placement[BPawn];

        u64 single_step = (pawns >> 8u) & (~all);
        u64 promotions = single_step & mask_rank[RANK_1];
        u64 double_push = ((single_step & mask_rank[RANK_6]) >> 8u) & (~all);

        Square destination, origin;

        u64 single_push = single_step & clear_rank[RANK_1];

        while (single_push) {
            destination = popLsb(single_push);
            origin = (Square) (destination + 8);
            moves.push_back(create_quiet_move(origin, destination, BPawn));
        }

        while (double_push) {
            destination = popLsb(double_push);
            origin = (Square) (destination + 16);
            moves.push_back(create_double_pawn_push_move(origin, destination, BPawn));
        }

        while (promotions) {
            destination = popLsb(promotions);
            origin = (Square) (destination + 8);
            moves.push_back(create_promotion_move(origin, destination, BPawn, BKnight));
            moves.push_back(create_promotion_move(origin, destination, BPawn, BBishop));
            moves.push_back(create_promotion_move(origin, destination, BPawn, BRook));
            moves.push_back(create_promotion_move(origin, destination, BPawn, BQueen));
        }
    }


    void MoveGen::add_white_pawn_captures(u64 black, std::vector<Move> &moves) {
        u64 pawns = board.current_position.placement[WPawn];

        u64 left_attacks = ((pawns & clear_file[FILE_A]) << 9u) & black;
        u64 right_attacks = ((pawns & clear_file[FILE_H]) << 7u) & black;

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
            moves.push_back(create_capture_move(origin, destination, WPawn, captured));
        }

        while (right_captures) {
            destination = popLsb(right_captures);
            origin = (Square) (destination - 7);
            captured = board.current_position.pieces[destination];
            moves.push_back(create_capture_move(origin, destination, WPawn, captured));
        }

        while (left_promotion_captures) {
            destination = popLsb(left_promotion_captures);
            origin = (Square) (destination - 9);
            captured = board.current_position.pieces[destination];
            moves.push_back(create_promotion_capture_move(origin, destination, WPawn, captured, WKnight));
            moves.push_back(create_promotion_capture_move(origin, destination, WPawn, captured, WBishop));
            moves.push_back(create_promotion_capture_move(origin, destination, WPawn, captured, WRook));
            moves.push_back(create_promotion_capture_move(origin, destination, WPawn, captured, WQueen));
        }

        while (right_promotion_captures) {
            destination = popLsb(right_promotion_captures);
            origin = (Square) (destination - 7);
            captured = board.current_position.pieces[destination];

            moves.push_back(create_promotion_capture_move(origin, destination, WPawn, captured, WKnight));
            moves.push_back(create_promotion_capture_move(origin, destination, WPawn, captured, WBishop));
            moves.push_back(create_promotion_capture_move(origin, destination, WPawn, captured, WRook));
            moves.push_back(create_promotion_capture_move(origin, destination, WPawn, captured, WQueen));
        }
    }

    void MoveGen::add_black_pawn_captures(u64 black, std::vector<Move> &moves) {
        u64 pawns = board.current_position.placement[BPawn];

        u64 left_attacks = ((pawns & clear_file[FILE_A]) >> 9u) & black;
        u64 right_attacks = ((pawns & clear_file[FILE_H]) >> 7u) & black;

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
            moves.push_back(create_capture_move(origin, destination, BPawn, captured));
        }

        while (right_captures) {
            destination = popLsb(right_captures);
            origin = (Square) (destination + 7);
            captured = board.current_position.pieces[destination];
            moves.push_back(create_capture_move(origin, destination, BPawn, captured));
        }

        while (left_promotion_captures) {
            destination = popLsb(left_promotion_captures);
            origin = (Square) (destination + 9);
            captured = board.current_position.pieces[destination];
            moves.push_back(create_promotion_capture_move(origin, destination, BPawn, captured, BKnight));
            moves.push_back(create_promotion_capture_move(origin, destination, BPawn, captured, BBishop));
            moves.push_back(create_promotion_capture_move(origin, destination, BPawn, captured, BRook));
            moves.push_back(create_promotion_capture_move(origin, destination, BPawn, captured, BQueen));
        }

        while (right_promotion_captures) {
            destination = popLsb(right_promotion_captures);
            origin = (Square) (destination + 7);
            captured = board.current_position.pieces[destination];

            moves.push_back(create_promotion_capture_move(origin, destination, BPawn, captured, BKnight));
            moves.push_back(create_promotion_capture_move(origin, destination, BPawn, captured, BBishop));
            moves.push_back(create_promotion_capture_move(origin, destination, BPawn, captured, BRook));
            moves.push_back(create_promotion_capture_move(origin, destination, BPawn, captured, BQueen));
        }
    }

    void MoveGen::add_white_pawn_moves(u64 white, u64 black, u64 all, std::vector<Move> &moves) {
        add_white_pawn_pushes(all, moves);
        add_white_pawn_captures(black, moves);
    }

    void MoveGen::add_black_pawn_moves(u64 white, u64 black, u64 all, std::vector<Move> &moves) {
        add_black_pawn_pushes(all, moves);
        add_black_pawn_captures(white, moves);
    }

    std::vector<Move> MoveGen::get_moves() {
        u64 white = board.current_position.placement[WPawn] |
                    board.current_position.placement[WKnight] |
                    board.current_position.placement[WBishop] |
                    board.current_position.placement[WRook] |
                    board.current_position.placement[WQueen] |
                    board.current_position.placement[WKing];

        u64 black = board.current_position.placement[BPawn] |
                    board.current_position.placement[BKnight] |
                    board.current_position.placement[BBishop] |
                    board.current_position.placement[BRook] |
                    board.current_position.placement[BQueen] |
                    board.current_position.placement[BKing];

        u64 all = white | black;
        std::vector<Move> result;

        if (board.current_state->state & State::SideToMove) {
            add_black_king_moves(white, all, result);
            add_black_knight_moves(white, all, result);
            add_black_pawn_moves(white, black, all, result);
        } else {
            add_white_king_moves(black, all, result);
            add_white_knight_moves(black, all, result);
            add_white_pawn_moves(white, black, all, result);
        }
        return result;
    }

}