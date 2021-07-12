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
        return {MoveType::Quiet, origin, destination, moved, Piece::None, Piece::None};
    }

    inline Move create_capture_move(Square origin, Square destination, Piece moved, Piece captured) {
        return {MoveType::Capture, origin, destination, moved, captured, Piece::None};
    }

    inline Move create_double_pawn_push_move(Square origin, Square destination, Piece moved) {
        return {MoveType::DoublePawnPush, origin, destination, moved, Piece::None, Piece::None};
    }

    inline Move create_promotion_move(Square origin, Square destination, Piece moved, Piece promoted) {
        return {MoveType::Promotion, origin, destination, moved, Piece::None, promoted};
    }

    inline Move
    create_promotion_capture_move(Square origin, Square destination, Piece moved, Piece captured, Piece promoted) {
        return {MoveType::PromotionCapture, origin, destination, moved, captured, promoted};
    }

    inline std::string move_to_string(const Move &move) {
        std::string result = square_to_notation(move.origin);
        if (move.type == MoveType::Capture || move.type==MoveType::PromotionCapture) {
            result += 'x';
        }
        result += square_to_notation(move.destination);
        if (move.type == MoveType::Promotion || move.type==MoveType::PromotionCapture) {
            result += "=";
            result += piece_to_notation(move.promotion);
        }
        return result;
    }

}


#endif //CHESSENGINE_MOVE_H
