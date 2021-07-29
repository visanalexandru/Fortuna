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

    /*Gets the current game phase, between 0 and C_MAX_PHASE, used for tapered eval.*/
    int get_phase(const Board &board);


    /*Statically evaluates the board, returning a heuristic score.*/
    int evaluate(const Board &board);

}


#endif //CHESSENGINE_EVAL_H
