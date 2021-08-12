//
// Created by gvisan on 10.07.2021.
//

#include "board.h"

namespace engine {
    Board::Board() : history(), current_position(), current_state(&history[0]) {
        load_fen(C_BASE_POSITION);
    }

    Color Board::color_to_play() const {
        return (current_state->state & S_SIDE_TO_MOVE) ? C_BLACK : C_WHITE;
    }

    void Board::set_piece_at(Square square, Piece piece) {
        u64 pos = square_to_bitboard(square);
        current_position.placement[piece] |= pos;
        current_position.all |= pos;
        current_position.pieces[square] = piece;
        current_state->zobrist_key ^= ZOBRIST_PIECES[piece][square];
    }

    void Board::remove_piece_at(Square square, Piece piece) {
        u64 pos = square_to_bitboard(square);
        current_position.placement[piece] &= ~pos;
        current_position.all &= ~pos;
        current_position.pieces[square] = Piece::P_NONE;
        current_state->zobrist_key ^= ZOBRIST_PIECES[piece][square];
    }

    void Board::make_quiet_move(const Move &move) {
        set_piece_at(move.destination, move.moved);
        remove_piece_at(move.origin, move.moved);
    }

    void Board::undo_quiet_move(const Move &move) {
        remove_piece_at(move.destination, move.moved);
        set_piece_at(move.origin, move.moved);
    }

    void Board::make_capture_move(const Move &move) {
        remove_piece_at(move.destination, move.captured);
        remove_piece_at(move.origin, move.moved);
        set_piece_at(move.destination, move.moved);
    }

    void Board::undo_capture_move(const Move &move) {
        remove_piece_at(move.destination, move.moved);
        set_piece_at(move.destination, move.captured);
        set_piece_at(move.origin, move.moved);
    }

    void Board::make_promotion_move(const Move &move) {
        set_piece_at(move.destination, move.promotion);
        remove_piece_at(move.origin, move.moved);
    }

    void Board::undo_promotion_move(const Move &move) {
        remove_piece_at(move.destination, move.promotion);
        set_piece_at(move.origin, move.moved);
    }

    void Board::make_promotion_capture_move(const Move &move) {
        remove_piece_at(move.destination, move.captured);
        remove_piece_at(move.origin, move.moved);
        set_piece_at(move.destination, move.promotion);
    }


    void Board::undo_promotion_capture_move(const Move &move) {
        remove_piece_at(move.destination, move.promotion);
        set_piece_at(move.destination, move.captured);
        set_piece_at(move.origin, move.moved);
    }

    void Board::make_en_passant_move(const Move &move) {
        Square pawn_square;

        if (move.moved == P_W_PAWN) {
            pawn_square = (Square) (move.destination - 8);
        } else pawn_square = (Square) (move.destination + 8);

        remove_piece_at(pawn_square, move.captured);
        remove_piece_at(move.origin, move.moved);
        set_piece_at(move.destination, move.moved);
    }

    void Board::undo_en_passant_move(const Move &move) {
        Square pawn_square;
        if (move.moved == P_W_PAWN) {
            pawn_square = (Square) (move.destination - 8);
        } else pawn_square = (Square) (move.destination + 8);

        remove_piece_at(move.destination, move.moved);
        set_piece_at(pawn_square, move.captured);
        set_piece_at(move.origin, move.moved);
    }

    void Board::make_kingside_castle_move(const Move &move) {
        if (move.moved == P_W_KING) {
            remove_piece_at(SQ_H1, P_W_ROOK);
            remove_piece_at(SQ_E1, P_W_KING);
            set_piece_at(SQ_G1, P_W_KING);
            set_piece_at(SQ_F1, P_W_ROOK);
        } else {
            remove_piece_at(SQ_H8, P_B_ROOK);
            remove_piece_at(SQ_E8, P_B_KING);
            set_piece_at(SQ_G8, P_B_KING);
            set_piece_at(SQ_F8, P_B_ROOK);
        }
    }


    void Board::undo_kingside_castle_move(const Move &move) {
        if (move.moved == P_W_KING) {
            remove_piece_at(SQ_F1, P_W_ROOK);
            remove_piece_at(SQ_G1, P_W_KING);
            set_piece_at(SQ_E1, P_W_KING);
            set_piece_at(SQ_H1, P_W_ROOK);
        } else {
            remove_piece_at(SQ_F8, P_B_ROOK);
            remove_piece_at(SQ_G8, P_B_KING);
            set_piece_at(SQ_E8, P_B_KING);
            set_piece_at(SQ_H8, P_B_ROOK);
        }
    }

    void Board::make_queenside_castle_move(const Move &move) {
        if (move.moved == P_W_KING) {
            remove_piece_at(SQ_A1, P_W_ROOK);
            remove_piece_at(SQ_E1, P_W_KING);
            set_piece_at(SQ_C1, P_W_KING);
            set_piece_at(SQ_D1, P_W_ROOK);
        } else {
            remove_piece_at(SQ_A8, P_B_ROOK);
            remove_piece_at(SQ_E8, P_B_KING);
            set_piece_at(SQ_C8, P_B_KING);
            set_piece_at(SQ_D8, P_B_ROOK);
        }
    }


    void Board::undo_queenside_castle_move(const Move &move) {
        if (move.moved == P_W_KING) {
            remove_piece_at(SQ_D1, P_W_ROOK);
            remove_piece_at(SQ_C1, P_W_KING);
            set_piece_at(SQ_E1, P_W_KING);
            set_piece_at(SQ_A1, P_W_ROOK);
        } else {
            remove_piece_at(SQ_D8, P_B_ROOK);
            remove_piece_at(SQ_C8, P_B_KING);
            set_piece_at(SQ_E8, P_B_KING);
            set_piece_at(SQ_A8, P_B_ROOK);
        }
    }

    void Board::update_en_passant_status(const Move &move) {
        if (current_state->state & S_EN_PASSANT) {
            current_state->state &= ~S_EN_PASSANT;
            /*Reset en passant square zobrist key.*/
            current_state->zobrist_key ^= ZOBRIST_EN_PASSANT[get_file(current_state->ep_square)];
        }

        if (move.type == MoveType::M_DOUBLE_PAWN_PUSH) {
            current_state->state |= S_EN_PASSANT;
            if (move.moved == P_W_PAWN) {
                current_state->ep_square = (Square) (move.destination - 8);
            } else current_state->ep_square = (Square) (move.destination + 8);

            /*Toggle en passant square zobrist key.*/
            current_state->zobrist_key ^= ZOBRIST_EN_PASSANT[get_file(current_state->ep_square)];
        }
    }

    void Board::update_castling_status(const Move &move) {
        u8 modified = current_state->state;
        if (move.type == MoveType::M_CAPTURE || move.type == MoveType::M_PROMOTION_CAPTURE)
            switch (move.destination) {
                case SQ_A1:
                    current_state->state &= ~S_WHITE_CASTLE_Q;
                    break;
                case SQ_H1:
                    current_state->state &= ~S_WHITE_CASTLE_K;
                    break;
                case SQ_A8:
                    current_state->state &= ~S_BLACK_CASTLE_Q;
                    break;
                case SQ_H8:
                    current_state->state &= ~S_BLACK_CASTLE_K;
                    break;
                default:
                    break;
            }

        switch (move.origin) {
            case SQ_E1:
                current_state->state &= ~S_WHITE_CASTLE_K;
                current_state->state &= ~S_WHITE_CASTLE_Q;
                break;
            case SQ_E8:
                current_state->state &= ~S_BLACK_CASTLE_K;
                current_state->state &= ~S_BLACK_CASTLE_Q;
                break;
            case SQ_A1:
                current_state->state &= ~S_WHITE_CASTLE_Q;
                break;
            case SQ_H1:
                current_state->state &= ~S_WHITE_CASTLE_K;
                break;
            case SQ_A8:
                current_state->state &= ~S_BLACK_CASTLE_Q;
                break;
            case SQ_H8:
                current_state->state &= ~S_BLACK_CASTLE_K;
                break;
            default:
                break;
        }
        modified ^= current_state->state;

        if (modified & S_WHITE_CASTLE_K) {
            current_state->zobrist_key ^= ZOBRIST_CASTLING[ZOBRIST_WHITE_CASTLE_KINGSIDE];
        }
        if (modified & S_WHITE_CASTLE_Q) {
            current_state->zobrist_key ^= ZOBRIST_CASTLING[ZOBRIST_WHITE_CASTLE_QUEENSIDE];
        }
        if (modified & S_BLACK_CASTLE_K) {
            current_state->zobrist_key ^= ZOBRIST_CASTLING[ZOBRIST_BLACK_CASTLE_KINGSIDE];
        }
        if (modified & S_BLACK_CASTLE_Q) {
            current_state->zobrist_key ^= ZOBRIST_CASTLING[ZOBRIST_BLACK_CASTLE_QUEENSIDE];
        }
    }

    void Board::update_status(const Move &move) {
        update_en_passant_status(move);
        update_castling_status(move);
        current_state->state ^= S_SIDE_TO_MOVE;
        current_state->zobrist_key ^= ZOBRIST_SIDE_TO_MOVE;
    }

    void Board::make_move(const Move &move) {

        /*We copy the current state into the next state.*/
        *(current_state + 1) = *current_state;
        /*We increment the state pointer.*/
        current_state++;
        /*We update the state flags.*/
        update_status(move);

        switch (move.type) {
            case MoveType::M_QUIET:
                make_quiet_move(move);
                break;

            case MoveType::M_CAPTURE:
                make_capture_move(move);
                break;

            case MoveType::M_DOUBLE_PAWN_PUSH:
                make_quiet_move(move);
                break;

            case MoveType::M_KINGSIDE_CASTLE:
                make_kingside_castle_move(move);
                break;

            case MoveType::M_QUEENSIDE_CASTLE:
                make_queenside_castle_move(move);
                break;

            case MoveType::M_PROMOTION:
                make_promotion_move(move);
                break;

            case MoveType::M_PROMOTION_CAPTURE:
                make_promotion_capture_move(move);
                break;

            case MoveType::M_EN_PASSANT:
                make_en_passant_move(move);
                break;
            case MoveType::M_NONE:
                break;
        }

    }

    void Board::undo_move(const Move &move) {
        switch (move.type) {
            case MoveType::M_QUIET:
                undo_quiet_move(move);
                break;

            case MoveType::M_CAPTURE:
                undo_capture_move(move);
                break;

            case MoveType::M_DOUBLE_PAWN_PUSH:
                undo_quiet_move(move);
                break;

            case MoveType::M_KINGSIDE_CASTLE:
                undo_kingside_castle_move(move);
                break;

            case MoveType::M_QUEENSIDE_CASTLE:
                undo_queenside_castle_move(move);
                break;

            case MoveType::M_PROMOTION:
                undo_promotion_move(move);
                break;

            case MoveType::M_PROMOTION_CAPTURE:
                undo_promotion_capture_move(move);
                break;

            case MoveType::M_EN_PASSANT:
                undo_en_passant_move(move);
                break;
            case MoveType::M_NONE:
                break;
        }
        /*We decrement the state pointer.*/
        current_state--;
    }

    PieceType Board::get_smallest_attacker(Square square, Color side, Square &attacker) const {
        u64 pawns = current_position.placement[get_piece(PT_PAWN, side)];
        u64 knights = current_position.placement[get_piece(PT_KNIGHT, side)];
        u64 bishops = current_position.placement[get_piece(PT_BISHOP, side)];
        u64 rooks = current_position.placement[get_piece(PT_ROOK, side)];
        u64 queens = current_position.placement[get_piece(PT_QUEEN, side)];
        u64 king = current_position.placement[get_piece(PT_KING, side)];

        u64 all = current_position.all;

        if (PAWN_ATTACKS[square][get_opposite(side)] & pawns) {
            pawns &= PAWN_ATTACKS[square][get_opposite(side)];
            attacker = popLsb(pawns);
            return PT_PAWN;
        }

        if (KNIGHT_ATTACKS[square] & knights) {
            knights &= KNIGHT_ATTACKS[square];
            attacker = popLsb(knights);
            return PT_KNIGHT;
        }

        u64 bishop_attacks = (BISHOP_ATTACKS[square] & (bishops | queens)) ? get_magic_bishop_attacks(square, all) : 0;
        u64 rook_attacks = (ROOK_ATTACKS[square] & (rooks | queens)) ? get_magic_rook_attacks(square, all) : 0;

        if (bishop_attacks & bishops) {
            bishops &= bishop_attacks;
            attacker = popLsb(bishops);
            return PT_BISHOP;
        }

        if (rook_attacks & rooks) {
            rooks &= rook_attacks;
            attacker = popLsb(rooks);
            return PT_ROOK;
        }
        u64 queen_attacks = bishop_attacks | rook_attacks;

        if (queen_attacks & queens) {
            queens &= (queen_attacks);
            attacker = popLsb(queens);
            return PT_QUEEN;
        }

        if (KING_ATTACKS[square] & king) {
            attacker = popLsb(king);
            return PT_KING;
        }

        /*No piece attacks the given square.*/
        attacker = SQ_NONE;
        return PT_NONE;
    }

    int Board::see(Square square, Color side) {
        Square attacker_square;
        PieceType attacker = get_smallest_attacker(square, side, attacker_square);
        if (attacker == PT_NONE) {
            return 0;
        }

        Piece captured = current_position.pieces[square];

        int score;
        Move capture = create_capture_move(attacker_square, square, get_piece(attacker, side), captured);

        make_move(capture);
        score = get_piece_value(get_piece_type(captured)) - see(square, get_opposite(side));
        score = std::max(0, score);
        undo_move(capture);

        return score;
    }

    int Board::see(const Move &capture, Color side) {
        make_move(capture);
        int score = get_piece_value(get_piece_type(capture.captured)) - see(capture.destination, get_opposite(side));
        undo_move(capture);
        return score;
    }

    void Board::load_fen(const std::string &fen) {
        history[0] = {};
        current_state = &history[0];
        current_position = {};

        for (int square = 0; square < C_NUM_SQUARES; square++) {
            current_position.pieces[square] = Piece::P_NONE;
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
            current_state->state |= State::S_SIDE_TO_MOVE;
            current_state->zobrist_key ^= ZOBRIST_SIDE_TO_MOVE;
        }

        for (char token:castling) {
            if (token == 'K') {
                current_state->state |= State::S_WHITE_CASTLE_K;
                current_state->zobrist_key ^= ZOBRIST_CASTLING[ZOBRIST_WHITE_CASTLE_KINGSIDE];
            } else if (token == 'k') {
                current_state->state |= State::S_BLACK_CASTLE_K;
                current_state->zobrist_key ^= ZOBRIST_CASTLING[ZOBRIST_BLACK_CASTLE_KINGSIDE];
            } else if (token == 'Q') {
                current_state->state |= State::S_WHITE_CASTLE_Q;
                current_state->zobrist_key ^= ZOBRIST_CASTLING[ZOBRIST_WHITE_CASTLE_QUEENSIDE];
            } else if (token == 'q') {
                current_state->state |= State::S_BLACK_CASTLE_Q;
                current_state->zobrist_key ^= ZOBRIST_CASTLING[ZOBRIST_BLACK_CASTLE_QUEENSIDE];
            }
        }

        if (ep_square != "-") {
            Square ep = notation_to_square(ep_square);
            current_state->ep_square = ep;
            current_state->state |= State::S_EN_PASSANT;
            current_state->zobrist_key ^= ZOBRIST_EN_PASSANT[get_file(ep)];
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