//
// Created by gvisan on 26.07.2021.
//

#ifndef CHESSENGINE_EVAL_H
#define CHESSENGINE_EVAL_H

#include"movegen.h"
#include"defines.h"

namespace engine {
    /*This header contains functions to statically evaluate a position.*/

    /*Evaluates the king. King PST and king safety*/
    template<Color side>
    void score_king(const Board &board, int &middle_game, int &end_game);

    /*Evaluates the pawns. Pawn PST, pawn structure and material.*/
    template<Color side>
    void score_pawns(const Board &board, int &middle_game, int &end_game);

    /*Evaluates the knights. Knight PST and material.*/
    template<Color side>
    void score_knights(const Board &board, int &middle_game, int &end_game);

    /*Evaluates the bishops. Bishop PST, material and mobility bonus.*/
    template<Color side>
    void score_bishops(const Board &board, int &middle_game, int &end_game);

    /*Evaluates the rooks. Rook PST, material and mobility bonus.*/
    template<Color side>
    void score_rooks(const Board &board, int &middle_game, int &end_game);

    /*Evaluates the queens. Queen PST, material and mobility bonus.*/
    template<Color side>
    void score_queens(const Board &board, int &middle_game, int &end_game);

    /*Gets the current game phase, between 0 and C_MAX_PHASE, used for tapered eval.*/
    int get_phase(const Board &board);

    /*Statically evaluates the board, returning a heuristic score.*/
    int evaluate(const Board &board);

    /*Returns the static evaluation of the board, from the perspective of the given side.*/
    int evaluate_side(const Board &board, Color side);


    /*The following functions are only used for unit testing because the score is normally calculated per piece.*/
    /*So we can test individual features of the evaluation like king safety and mobility.*/

    /* Counts the material value for the given side.*/
    template<Color side>
    int score_material(const Board &board);

    /*Adds the piece placement bonus for the given side and piece type to the game phases.*/
    template<Color side, PieceType piece_type>
    void score_piece_squares(const Board &board, int &middle_game, int &end_game);

    /*Adds the total piece placement bonus for the given side to the game phases .*/
    template<Color side>
    void score_piece_placement(const Board &board, int &middle_game, int &end_game);

    /*Adds the pawn structure score for the given side to the game phases.*/
    template<Color side>
    void score_pawn_structure(const Board &board, int &middle_game, int &end_game);

    /*Adds the king safety score for the given side to the game phases.*/
    template<Color side>
    void score_king_safety(const Board &board, int &middle_game, int &end_game);

    /*Adds the queen, bishop and rook mobility score to the game phases.*/
    template<Color side>
    void score_mobility(const Board &board, int &middle_game, int &end_game);
}


#endif //CHESSENGINE_EVAL_H
