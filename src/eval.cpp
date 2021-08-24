//
// Created by gvisan on 26.07.2021.
//

#include "eval.h"

namespace engine {


    template<Color side>
    void score_king(const Board &board, int &middle_game, int &end_game) {
        u64 king = board.current_position.placement[get_piece(PT_KING, side)];
        u64 pawns = board.current_position.placement[get_piece(PT_PAWN, side)];
        Square king_square = popLsb(king);

        /*PST bonus.*/
        middle_game += PST[PT_KING][side][PH_MIDDLEGAME][king_square];
        end_game += PST[PT_KING][side][PH_ENDGAME][king_square];

        /*King safety bonus.*/
        int strong_pawns_count = popCount(pawns & STRONG_PAWN_SHIELD[king_square][side]);
        int weak_pawns_count = popCount(pawns & WEAK_PAWN_SHIELD[king_square][side]);

        middle_game += strong_pawns_count * C_STRONG_PAWN_SHIELD_BONUS[PH_MIDDLEGAME] +
                       weak_pawns_count * C_WEAK_PAWN_SHIELD_BONUS[PH_MIDDLEGAME];

        end_game += strong_pawns_count * C_STRONG_PAWN_SHIELD_BONUS[PH_ENDGAME] +
                    weak_pawns_count * C_WEAK_PAWN_SHIELD_BONUS[PH_ENDGAME];
    }

    template<Color side>
    void score_pawns(const Board &board, int &middle_game, int &end_game) {

        u64 pawns = board.current_position.placement[get_piece(PT_PAWN, side)];
        u64 allied_pawns = pawns;
        u64 enemy_pawns = board.current_position.placement[get_piece(PT_PAWN, get_opposite(side))];

        int advanced;

        Square pawn_square;
        while (pawns) {
            pawn_square = popLsb(pawns);

            /*Material bonus.*/
            middle_game += C_PAWN_VALUE;
            end_game += C_PAWN_VALUE;

            /*PST bonus.*/
            middle_game += PST[PT_PAWN][side][PH_MIDDLEGAME][pawn_square];
            end_game += PST[PT_PAWN][side][PH_ENDGAME][pawn_square];


            /*Pawn structure bonus.*/
            if (PAWN_FORWARD[pawn_square][side] & allied_pawns) {
                middle_game += C_DOUBLED_PAWN_PENALTY[PH_MIDDLEGAME];
                end_game += C_DOUBLED_PAWN_PENALTY[PH_ENDGAME];
            } else if ((PAWN_FRONT_SPANS[pawn_square][side] & enemy_pawns) == 0) {
                advanced = (side == C_WHITE) ? get_rank(pawn_square) : (7 - get_rank(pawn_square));
                middle_game += C_PASSED_PAWN_BONUS[PH_MIDDLEGAME][advanced];
                end_game += C_PASSED_PAWN_BONUS[PH_ENDGAME][advanced];
            }
        }
    }

    template<Color side>
    void score_knights(const Board &board, int &middle_game, int &end_game) {
        u64 knights = board.current_position.placement[get_piece(PT_KNIGHT, side)];
        Square knight_square;
        while (knights) {
            knight_square = popLsb(knights);

            /*Material bonus.*/
            middle_game += C_KNIGHT_VALUE;
            end_game += C_KNIGHT_VALUE;

            /*PST bonus.*/
            middle_game += PST[PT_KNIGHT][side][PH_MIDDLEGAME][knight_square];
            end_game += PST[PT_KNIGHT][side][PH_ENDGAME][knight_square];
        }
    }

    template<Color side>
    void score_bishops(const Board &board, int &middle_game, int &end_game) {
        u64 bishops = board.current_position.placement[get_piece(PT_BISHOP, side)], attacks;
        u64 all = board.current_position.all, allied_pieces = board.get_occupancy<side>();

        Square bishop_square;
        while (bishops) {
            bishop_square = popLsb(bishops);
            attacks = get_magic_bishop_attacks(bishop_square, all) & (~allied_pieces);

            /*Material bonus.*/
            middle_game += C_BISHOP_VALUE;
            end_game += C_BISHOP_VALUE;

            /*PST bonus.*/
            middle_game += PST[PT_BISHOP][side][PH_MIDDLEGAME][bishop_square];
            end_game += PST[PT_BISHOP][side][PH_ENDGAME][bishop_square];

            /*Mobility bonus.*/
            middle_game += popCount(attacks) * C_MOBILITY_MULTIPLIER_BISHOP;
            end_game += popCount(attacks) * C_MOBILITY_MULTIPLIER_BISHOP;
        }
    }

    template<Color side>
    void score_rooks(const Board &board, int &middle_game, int &end_game) {
        u64 rooks = board.current_position.placement[get_piece(PT_ROOK, side)], attacks;
        u64 all = board.current_position.all, allied_pieces = board.get_occupancy<side>();

        Square rook_square;
        while (rooks) {
            rook_square = popLsb(rooks);
            attacks = get_magic_rook_attacks(rook_square, all) & (~allied_pieces);

            /*Material bonus.*/
            middle_game += C_ROOK_VALUE;
            end_game += C_ROOK_VALUE;

            /*PST bonus.*/
            middle_game += PST[PT_ROOK][side][PH_MIDDLEGAME][rook_square];
            end_game += PST[PT_ROOK][side][PH_ENDGAME][rook_square];

            /*Mobility bonus.*/
            middle_game += popCount(attacks) * C_MOBILITY_MULTIPLIER_ROOK;
            end_game += popCount(attacks) * C_MOBILITY_MULTIPLIER_ROOK;
        }
    }

    template<Color side>
    void score_queens(const Board &board, int &middle_game, int &end_game) {
        u64 queens = board.current_position.placement[get_piece(PT_QUEEN, side)], attacks;
        u64 all = board.current_position.all, allied_pieces = board.get_occupancy<side>();

        Square queen_square;
        while (queens) {
            queen_square = popLsb(queens);
            attacks = (get_magic_rook_attacks(queen_square, all) | get_magic_bishop_attacks(queen_square, all)) &
                      (~allied_pieces);

            /*Material bonus.*/
            middle_game += C_QUEEN_VALUE;
            end_game += C_QUEEN_VALUE;

            /*PST bonus.*/
            middle_game += PST[PT_QUEEN][side][PH_MIDDLEGAME][queen_square];
            end_game += PST[PT_QUEEN][side][PH_ENDGAME][queen_square];

            /*Mobility bonus.*/
            middle_game += popCount(attacks) * C_MOBILITY_MULTIPLIER_QUEEN;
            end_game += popCount(attacks) * C_MOBILITY_MULTIPLIER_QUEEN;
        }
    }

    int get_phase(const Board &board) {
        int phase = C_PHASE_WEIGHT_SUM;
        u64 knights = board.current_position.placement[P_W_KNIGHT] | board.current_position.placement[P_B_KNIGHT];
        u64 bishops = board.current_position.placement[P_W_BISHOP] | board.current_position.placement[P_B_BISHOP];
        u64 rooks = board.current_position.placement[P_W_ROOK] | board.current_position.placement[P_B_ROOK];
        u64 queens = board.current_position.placement[P_W_QUEEN] | board.current_position.placement[P_B_QUEEN];

        phase -= popCount(knights) * C_KNIGHT_PHASE_WEIGHT;
        phase -= popCount(bishops) * C_BISHOP_PHASE_WEIGHT;
        phase -= popCount(rooks) * C_ROOK_PHASE_WEIGHT;
        phase -= popCount(queens) * C_QUEEN_PHASE_WEIGHT;
        phase = (phase * C_MAX_PHASE + (C_PHASE_WEIGHT_SUM / 2)) / C_PHASE_WEIGHT_SUM;
        return phase;
    }

    int evaluate(const Board &board) {
        int white_mg = 0, black_mg = 0, white_eg = 0, black_eg = 0;

        score_king<C_WHITE>(board, white_mg, white_eg);
        score_king<C_BLACK>(board, black_mg, black_eg);

        score_pawns<C_WHITE>(board, white_mg, white_eg);
        score_pawns<C_BLACK>(board, black_mg, black_eg);

        score_knights<C_WHITE>(board, white_mg, white_eg);
        score_knights<C_BLACK>(board, black_mg, black_eg);

        score_bishops<C_WHITE>(board, white_mg, white_eg);
        score_bishops<C_BLACK>(board, black_mg, black_eg);

        score_rooks<C_WHITE>(board, white_mg, white_eg);
        score_rooks<C_BLACK>(board, black_mg, black_eg);

        score_queens<C_WHITE>(board, white_mg, white_eg);
        score_queens<C_BLACK>(board, black_mg, black_eg);

        int opening = white_mg - black_mg;
        int end_game = white_eg - black_eg;

        int phase = get_phase(board);
        int eval = (opening * (C_MAX_PHASE - phase) + (end_game * phase)) / C_MAX_PHASE;
        return eval;
    }

    int evaluate_side(const Board &board, Color side) {
        int score = evaluate(board);
        return (side == C_WHITE) ? score : -score;
    }


    /*Individual evaluation feature functions, only used in testing.*/
    template<Color side>
    int score_material(const Board &board) {
        int score = 0;
        score += popCount(board.current_position.placement[get_piece(PT_PAWN, side)]) * C_PAWN_VALUE;
        score += popCount(board.current_position.placement[get_piece(PT_KNIGHT, side)]) * C_KNIGHT_VALUE;
        score += popCount(board.current_position.placement[get_piece(PT_BISHOP, side)]) * C_BISHOP_VALUE;
        score += popCount(board.current_position.placement[get_piece(PT_ROOK, side)]) * C_ROOK_VALUE;
        score += popCount(board.current_position.placement[get_piece(PT_QUEEN, side)]) * C_QUEEN_VALUE;
        return score;
    }

    template<Color side, PieceType piece_type>
    void score_piece_squares(const Board &board, int &middle_game, int &end_game) {
        u64 pieces = board.current_position.placement[get_piece(piece_type, side)];
        Square square;

        while (pieces) {
            square = popLsb(pieces);
            middle_game += PST[piece_type][side][PH_MIDDLEGAME][square];
            end_game += PST[piece_type][side][PH_ENDGAME][square];
        }
    }

    template<Color side>
    void score_piece_placement(const Board &board, int &middle_game, int &end_game) {
        score_piece_squares<side, PT_PAWN>(board, middle_game, end_game);
        score_piece_squares<side, PT_KNIGHT>(board, middle_game, end_game);
        score_piece_squares<side, PT_BISHOP>(board, middle_game, end_game);
        score_piece_squares<side, PT_ROOK>(board, middle_game, end_game);
        score_piece_squares<side, PT_QUEEN>(board, middle_game, end_game);
        score_piece_squares<side, PT_KING>(board, middle_game, end_game);
    }

    template<Color side>
    void score_pawn_structure(const Board &board, int &middle_game, int &end_game) {
        u64 pawns = board.current_position.placement[get_piece(PT_PAWN, side)];
        u64 allied_pawns = pawns;
        u64 enemy_pawns = board.current_position.placement[get_piece(PT_PAWN, get_opposite(side))];
        int advanced;

        Square pawn_square;
        while (pawns) {
            pawn_square = popLsb(pawns);
            if (PAWN_FORWARD[pawn_square][side] & allied_pawns) {
                middle_game += C_DOUBLED_PAWN_PENALTY[PH_MIDDLEGAME];
                end_game += C_DOUBLED_PAWN_PENALTY[PH_ENDGAME];
            } else if ((PAWN_FRONT_SPANS[pawn_square][side] & enemy_pawns) == 0) {
                advanced = (side == C_WHITE) ? get_rank(pawn_square) : (7 - get_rank(pawn_square));
                middle_game += C_PASSED_PAWN_BONUS[PH_MIDDLEGAME][advanced];
                end_game += C_PASSED_PAWN_BONUS[PH_ENDGAME][advanced];
            }
        }
    }

    template<Color side>
    void score_king_safety(const Board &board, int &middle_game, int &end_game) {
        u64 king = board.current_position.placement[get_piece(PT_KING, side)];
        u64 pawns = board.current_position.placement[get_piece(PT_PAWN, side)];
        Square king_square = popLsb(king);

        int strong_pawns_count = popCount(pawns & STRONG_PAWN_SHIELD[king_square][side]);
        int weak_pawns_count = popCount(pawns & WEAK_PAWN_SHIELD[king_square][side]);

        middle_game += strong_pawns_count * C_STRONG_PAWN_SHIELD_BONUS[PH_MIDDLEGAME] +
                       weak_pawns_count * C_WEAK_PAWN_SHIELD_BONUS[PH_MIDDLEGAME];

        end_game += strong_pawns_count * C_STRONG_PAWN_SHIELD_BONUS[PH_ENDGAME] +
                    weak_pawns_count * C_WEAK_PAWN_SHIELD_BONUS[PH_ENDGAME];
    }

    template<Color side>
    void score_mobility(const Board &board, int &middle_game, int &end_game) {
        int bonus = 0;
        u64 allied_pieces = board.get_occupancy<side>(), all = board.current_position.all;
        u64 bishops = board.current_position.placement[get_piece(PT_BISHOP, side)];
        u64 rooks = board.current_position.placement[get_piece(PT_ROOK, side)];
        u64 queens = board.current_position.placement[get_piece(PT_QUEEN, side)];
        u64 attacks;

        Square square;
        while (bishops) {
            square = popLsb(bishops);
            attacks = get_magic_bishop_attacks(square, all) & (~allied_pieces);
            bonus += popCount(attacks) * C_MOBILITY_MULTIPLIER_BISHOP;
        }

        while (rooks) {
            square = popLsb(rooks);
            attacks = get_magic_rook_attacks(square, all) & (~allied_pieces);
            bonus += popCount(attacks) * C_MOBILITY_MULTIPLIER_ROOK;
        }

        while (queens) {
            square = popLsb(queens);
            attacks = (get_magic_rook_attacks(square, all) | get_magic_bishop_attacks(square, all)) & (~allied_pieces);
            bonus += popCount(attacks) * C_MOBILITY_MULTIPLIER_QUEEN;
        }

        middle_game += bonus;
        end_game += bonus;
    }

    /*Explicit template instantiation of the test functions.*/
    template int score_material<C_WHITE>(const Board &board);

    template int score_material<C_BLACK>(const Board &board);

    template void score_piece_placement<C_WHITE>(const Board &board, int &middle_game, int &end_game);

    template void score_piece_placement<C_BLACK>(const Board &board, int &middle_game, int &end_game);

    template void score_pawn_structure<C_WHITE>(const Board &board, int &middle_game, int &end_game);

    template void score_pawn_structure<C_BLACK>(const Board &board, int &middle_game, int &end_game);

    template void score_king_safety<C_WHITE>(const Board &board, int &middle_game, int &end_game);

    template void score_king_safety<C_BLACK>(const Board &board, int &middle_game, int &end_game);

    template void score_mobility<C_WHITE>(const Board &board, int &middle_game, int &end_game);

    template void score_mobility<C_BLACK>(const Board &board, int &middle_game, int &end_game);
}