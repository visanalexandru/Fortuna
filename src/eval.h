//
// Created by gvisan on 26.07.2021.
//

#ifndef CHESSENGINE_EVAL_H
#define CHESSENGINE_EVAL_H

#include"movegen.h"
#include"defines.h"

namespace engine {
    /*This header contains functions to statically evaluate a position.*/

    /*Counts the material value for the given side.*/
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

    /*Adds the piece placement bonus for the given side and piece type to the game phases.*/
    template<Color side, PieceType piece_type>
    void score_piece(const Board &board, int &middle_game, int &end_game) {
        u64 pieces = board.current_position.placement[get_piece(piece_type, side)];
        Square square;

        while (pieces) {
            square = popLsb(pieces);
            middle_game += PST[piece_type][side][PH_MIDDLEGAME][square];
            end_game += PST[piece_type][side][PH_ENDGAME][square];
        }
    }

    /*Adds the total piece placement bonus for the given side to the game phases .*/
    template<Color side>
    void score_piece_placement(const Board &board, int &middle_game, int &end_game) {
        score_piece<side, PT_PAWN>(board, middle_game, end_game);
        score_piece<side, PT_KNIGHT>(board, middle_game, end_game);
        score_piece<side, PT_BISHOP>(board, middle_game, end_game);
        score_piece<side, PT_ROOK>(board, middle_game, end_game);
        score_piece<side, PT_QUEEN>(board, middle_game, end_game);
        score_piece<side, PT_KING>(board, middle_game, end_game);
    }

    /*Adds the pawn structure score for the given side to the game phases.*/
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

    /*Adds the king safety score for the given side to the game phases.*/
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

    /*Adds the queen, bishop and rook mobility score to the game phases.*/
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

    /*Gets the current game phase, between 0 and C_MAX_PHASE, used for tapered eval.*/
    int get_phase(const Board &board);


    /*Statically evaluates the board, returning a heuristic score.*/
    int evaluate(const Board &board);

    /*Returns the static evaluation of the board, from the perspective of the given side.*/
    int evaluate_side(const Board &board, Color side);

}


#endif //CHESSENGINE_EVAL_H
