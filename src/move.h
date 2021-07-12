//
// Created by gvisan on 11.07.2021.
//

#ifndef CHESSENGINE_MOVE_H
#define CHESSENGINE_MOVE_H

#include"defines.h"
#include"util.h"

namespace engine {
    struct Move {
        MoveType type;

        Square origin;
        Square destination;

        Piece moved;
        Piece captured;
        Piece promotion;
    };

    inline Move create_quiet_move(Square origin, Square destination, Piece moved) {
        return {MoveType::M_QUIET, origin, destination, moved, Piece::P_NONE, Piece::P_NONE};
    }

    inline Move create_capture_move(Square origin, Square destination, Piece moved, Piece captured) {
        return {MoveType::M_CAPTURE, origin, destination, moved, captured, Piece::P_NONE};
    }

    inline Move create_double_pawn_push_move(Square origin, Square destination, Piece moved) {
        return {MoveType::M_DOUBLE_PAWN_PUSH, origin, destination, moved, Piece::P_NONE, Piece::P_NONE};
    }

    inline Move create_promotion_move(Square origin, Square destination, Piece moved, Piece promoted) {
        return {MoveType::M_PROMOTION, origin, destination, moved, Piece::P_NONE, promoted};
    }

    inline Move
    create_promotion_capture_move(Square origin, Square destination, Piece moved, Piece captured, Piece promoted) {
        return {MoveType::M_PROMOTION_CAPTURE, origin, destination, moved, captured, promoted};
    }

    inline std::string move_to_string(const Move &move) {
        std::string result = square_to_notation(move.origin);
        if (move.type == MoveType::M_CAPTURE || move.type == MoveType::M_PROMOTION_CAPTURE) {
            result += 'x';
        }
        result += square_to_notation(move.destination);
        if (move.type == MoveType::M_PROMOTION || move.type == MoveType::M_PROMOTION_CAPTURE) {
            result += "=";
            result += piece_to_notation(move.promotion);
        }
        return result;
    }

}


#endif //CHESSENGINE_MOVE_H
