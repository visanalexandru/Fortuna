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
        u16 score;
    };

    inline bool operator==(const Move &a, const Move &b) {
        return a.type == b.type &&
               a.origin == b.origin &&
               a.destination == b.destination &&
               a.moved == b.moved &&
               a.captured == b.captured &&
               a.promotion == b.promotion;
    }

    inline bool operator!=(const Move &a, const Move &b) {
        return a.type != b.type ||
               a.origin != b.origin ||
               a.destination != b.destination ||
               a.moved != b.moved ||
               a.captured != b.captured ||
               a.promotion != b.promotion;
    }

    inline bool is_tactical(const Move &move) {
        return move.type == M_PROMOTION_CAPTURE || move.type == M_CAPTURE || move.type == M_EN_PASSANT ||
               move.type == M_PROMOTION;
    }

    inline bool is_irreversible(const Move &move) {
        return move.type == M_CAPTURE || get_piece_type(move.moved) == PT_PAWN;
    }

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

    inline Move create_en_passant_move(Square origin, Square destination, Piece moved, Piece captured) {
        return {MoveType::M_EN_PASSANT, origin, destination, moved, captured, Piece::P_NONE};
    }

    inline Move create_kingside_castle_move(Square origin, Square destination, Piece moved) {
        return {MoveType::M_KINGSIDE_CASTLE, origin, destination, moved};
    }

    inline Move create_queenside_castle_move(Square origin, Square destination, Piece moved) {
        return {MoveType::M_QUEENSIDE_CASTLE, origin, destination, moved};
    }

    inline Move create_empty_move() {
        return {MoveType::M_NONE, SQ_NONE, SQ_NONE, P_NONE, P_NONE, P_NONE};
    }

    inline std::string move_to_string(const Move &move) {
        //Return a null move.
        if (move.type == M_NONE) {
            return "0000";
        }
        std::string result = square_to_notation(move.origin);
        result += square_to_notation(move.destination);
        if (move.type == MoveType::M_PROMOTION || move.type == MoveType::M_PROMOTION_CAPTURE) {
            result += tolower(piece_to_notation(move.promotion));
        }
        return result;
    }

}


#endif //CHESSENGINE_MOVE_H
