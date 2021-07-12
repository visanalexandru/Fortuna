//
// Created by gvisan on 10.07.2021.
//

#include "board.h"

namespace engine {
    Board::Board() : history(), current_position(), current_state(&history[0]) {
        load_fen(C_BASE_POSITION);
    }

    void Board::set_piece_at(Square square, Piece piece) {
        u64 pos = square_to_bitboard(square);
        current_position.placement[piece] |= pos;
        current_position.pieces[square] = piece;
    }

    void Board::remove_piece_at(Square square, Piece piece) {
        u64 pos = square_to_bitboard(square);
        u64 mask = c_all ^pos;
        current_position.placement[piece] &= mask;
        current_position.pieces[square] = Piece::None;
    }

    void Board::make_quiet_move(const Move &move) {
        set_piece_at(move.destination, move.moved);
        remove_piece_at(move.origin, move.moved);
    }

    void Board::undo_quiet_move(const Move &move) {
        remove_piece_at(move.destination, move.moved);
        set_piece_at(move.origin, move.moved);
    }

    void Board::make_move(const Move &move) {
        switch (move.type) {
            case MoveType::Quiet:
                make_quiet_move(move);
                break;
        }


    }

    void Board::undo_move(const Move &move) {
        switch (move.type) {
            case MoveType::Quiet:
                undo_quiet_move(move);
                break;
        }
    }

    void Board::load_fen(const std::string &fen) {
        history[0] = {};
        current_state = &history[0];
        current_position = {};

        for (int square = 0; square < C_NUM_SQUARES; square++) {
            current_position.pieces[square] = Piece::None;
        }

        std::stringstream stream;
        stream << fen;
        std::string placement, move, castling, ep_square;
        stream >> placement >> move >> castling >> ep_square;

        int rank = 7, file = 0;

        for (char token: placement) {
            Square here = position_to_square(file, rank);

            if (token == '/') {//New rank
                rank--;
                file = 0;

            } else if (isdigit(token)) { //We need to empty skip squares
                file += token - '0';
            } else if (isalpha(token)) { //We need to place a piece
                set_piece_at(here, notation_to_piece(token));
                file++;
            }
        }

        if (move == "b") {
            current_state->state |= State::SideToMove;
        }

        for (char token:castling) {
            if (token == 'K') {
                current_state->state |= State::WhiteCastleK;
            } else if (token == 'k') {
                current_state->state |= State::BlackCastleK;
            } else if (token == 'Q') {
                current_state->state |= State::WhiteCastleQ;
            } else if (token == 'q') {
                current_state->state |= State::BlackCastleQ;
            }
        }

        if (ep_square != "-") {
            current_state->ep_square = notation_to_square(ep_square);
        }

    }


    std::string Board::to_string() const {
        std::string result;

        for (int rank = 7; rank >= 0; rank--) {
            for (int file = 0; file < 8; file++) {
                Square here = position_to_square(file, rank);
                result += piece_to_notation(current_position.pieces[here]);
            }
            result += '\n';
        }
        result += '\n';
        return result;
    }
}