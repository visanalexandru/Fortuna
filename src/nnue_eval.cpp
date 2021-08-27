//
// Created by gvisan on 26.08.2021.
//

#include "nnue_eval.h"

namespace engine {
    int nnue_pieces[C_NUM_SQUARES];

    int nnue_squares[C_NUM_SQUARES];

    void init_nnue(const std::string &filename) {
        nnue_init(filename.c_str());
    }

    int evaluate_nnue(const Board &board) {
        int cnt = 0;

        /*We must first feed the king positions, and then other pieces in any order.*/
        board.get_piece_info(P_W_KING, nnue_pieces, nnue_squares, cnt);
        board.get_piece_info(P_B_KING, nnue_pieces, nnue_squares, cnt);

        board.get_piece_info(P_W_PAWN, nnue_pieces, nnue_squares, cnt);
        board.get_piece_info(P_B_PAWN, nnue_pieces, nnue_squares, cnt);

        board.get_piece_info(P_W_BISHOP, nnue_pieces, nnue_squares, cnt);
        board.get_piece_info(P_B_BISHOP, nnue_pieces, nnue_squares, cnt);

        board.get_piece_info(P_W_KNIGHT, nnue_pieces, nnue_squares, cnt);
        board.get_piece_info(P_B_KNIGHT, nnue_pieces, nnue_squares, cnt);

        board.get_piece_info(P_W_ROOK, nnue_pieces, nnue_squares, cnt);
        board.get_piece_info(P_B_ROOK, nnue_pieces, nnue_squares, cnt);

        board.get_piece_info(P_W_QUEEN, nnue_pieces, nnue_squares, cnt);
        board.get_piece_info(P_B_QUEEN, nnue_pieces, nnue_squares, cnt);

        /*Mark the end of the array.*/
        nnue_pieces[cnt] = 0;

        return nnue_evaluate(board.color_to_play(), nnue_pieces, nnue_squares);
    }

}