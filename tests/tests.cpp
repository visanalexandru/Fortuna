//
// Created by gvisan on 11.07.2021.
//

#define CATCH_CONFIG_MAIN

#include"movegen.h"
#include"catch.hpp"
#include "util.h"
#include"eval.h"
#include"search.h"
#include"ttable.h"

using namespace engine;

TEST_CASE("Squares", "[util]") {
    Square squares[8][8] = {
            SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
            SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
            SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
            SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
            SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
            SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
            SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
            SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,

    };
    std::string string_squares[8][8] = {
            "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
            "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
            "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
            "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
            "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
            "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
            "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
            "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",

    };

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            REQUIRE(position_to_square(file, rank) == squares[rank][file]);
            REQUIRE(notation_to_square(string_squares[rank][file]) == squares[rank][file]);
            REQUIRE(square_to_notation(position_to_square(file, rank)) == string_squares[rank][file]);
            REQUIRE(get_file(position_to_square(file, rank)) == file);
            REQUIRE(get_rank(position_to_square(file, rank)) == rank);
        }
    }
}

TEST_CASE("Pieces", "[util]") {
    REQUIRE(notation_to_piece('P') == Piece::P_W_PAWN);
    REQUIRE(notation_to_piece('N') == Piece::P_W_KNIGHT);
    REQUIRE(notation_to_piece('B') == Piece::P_W_BISHOP);
    REQUIRE(notation_to_piece('R') == Piece::P_W_ROOK);
    REQUIRE(notation_to_piece('Q') == Piece::P_W_QUEEN);
    REQUIRE(notation_to_piece('K') == Piece::P_W_KING);

    REQUIRE(notation_to_piece('p') == Piece::P_B_PAWN);
    REQUIRE(notation_to_piece('n') == Piece::P_B_KNIGHT);
    REQUIRE(notation_to_piece('b') == Piece::P_B_BISHOP);
    REQUIRE(notation_to_piece('r') == Piece::P_B_ROOK);
    REQUIRE(notation_to_piece('q') == Piece::P_B_QUEEN);
    REQUIRE(notation_to_piece('k') == Piece::P_B_KING);


    REQUIRE(piece_to_notation(P_W_PAWN) == 'P');
    REQUIRE(piece_to_notation(P_W_KNIGHT) == 'N');
    REQUIRE(piece_to_notation(P_W_BISHOP) == 'B');
    REQUIRE(piece_to_notation(P_W_ROOK) == 'R');
    REQUIRE(piece_to_notation(P_W_QUEEN) == 'Q');
    REQUIRE(piece_to_notation(P_W_KING) == 'K');

    REQUIRE(piece_to_notation(P_B_PAWN) == 'p');
    REQUIRE(piece_to_notation(P_B_KNIGHT) == 'n');
    REQUIRE(piece_to_notation(P_B_BISHOP) == 'b');
    REQUIRE(piece_to_notation(P_B_ROOK) == 'r');
    REQUIRE(piece_to_notation(P_B_QUEEN) == 'q');
    REQUIRE(piece_to_notation(P_B_KING) == 'k');

    REQUIRE(piece_to_notation(P_NONE) == ' ');


    REQUIRE(get_piece(PT_NONE, C_WHITE) == P_NONE);
    REQUIRE(get_piece(PT_NONE, C_BLACK) == P_NONE);

    REQUIRE(get_piece(PT_PAWN, C_WHITE) == P_W_PAWN);
    REQUIRE(get_piece(PT_PAWN, C_BLACK) == P_B_PAWN);

    REQUIRE(get_piece(PT_KNIGHT, C_WHITE) == P_W_KNIGHT);
    REQUIRE(get_piece(PT_KNIGHT, C_BLACK) == P_B_KNIGHT);

    REQUIRE(get_piece(PT_BISHOP, C_WHITE) == P_W_BISHOP);
    REQUIRE(get_piece(PT_BISHOP, C_BLACK) == P_B_BISHOP);

    REQUIRE(get_piece(PT_ROOK, C_WHITE) == P_W_ROOK);
    REQUIRE(get_piece(PT_ROOK, C_BLACK) == P_B_ROOK);

    REQUIRE(get_piece(PT_QUEEN, C_WHITE) == P_W_QUEEN);
    REQUIRE(get_piece(PT_QUEEN, C_BLACK) == P_B_QUEEN);

    REQUIRE(get_piece(PT_KING, C_WHITE) == P_W_KING);
    REQUIRE(get_piece(PT_KING, C_BLACK) == P_B_KING);

    REQUIRE(get_opposite(C_WHITE) == C_BLACK);
    REQUIRE(get_opposite(C_BLACK) == C_WHITE);


    REQUIRE(get_piece_type(P_W_PAWN) == PT_PAWN);
    REQUIRE(get_piece_type(P_B_PAWN) == PT_PAWN);

    REQUIRE(get_piece_type(P_W_KNIGHT) == PT_KNIGHT);
    REQUIRE(get_piece_type(P_B_KNIGHT) == PT_KNIGHT);

    REQUIRE(get_piece_type(P_W_BISHOP) == PT_BISHOP);
    REQUIRE(get_piece_type(P_B_BISHOP) == PT_BISHOP);

    REQUIRE(get_piece_type(P_W_ROOK) == PT_ROOK);
    REQUIRE(get_piece_type(P_B_ROOK) == PT_ROOK);

    REQUIRE(get_piece_type(P_W_QUEEN) == PT_QUEEN);
    REQUIRE(get_piece_type(P_B_QUEEN) == PT_QUEEN);

    REQUIRE(get_piece_type(P_W_KING) == PT_KING);
    REQUIRE(get_piece_type(P_W_KING) == PT_KING);

    REQUIRE(get_piece_type(P_NONE) == PT_NONE);

    REQUIRE(get_piece_value(PT_PAWN) == C_PAWN_VALUE);
    REQUIRE(get_piece_value(PT_KNIGHT) == C_KNIGHT_VALUE);
    REQUIRE(get_piece_value(PT_BISHOP) == C_BISHOP_VALUE);
    REQUIRE(get_piece_value(PT_ROOK) == C_ROOK_VALUE);
    REQUIRE(get_piece_value(PT_QUEEN) == C_QUEEN_VALUE);
    REQUIRE(get_piece_value(PT_KING) == C_KING_VALUE);
    REQUIRE(get_piece_value(PT_NONE) == 0);
}

std::map<std::pair<u64, int>, unsigned> memo;

unsigned perft_memoized(int depth, MoveGen &gen, Board &board) {
    u64 zkey = board.current_state->zobrist_key;
    unsigned result = 0;

    if (memo.find({zkey, depth}) != memo.end()) {
        return memo[{zkey, depth}];
    }

    auto moves = gen.get_moves<GT_NORMAL>();
    if (depth == 1) {
        result = moves.size();
    } else {
        for (auto &move:moves) {
            board.make_move(move);
            result += perft_memoized(depth - 1, gen, board);
            board.undo_move(move);
        }
    }
    memo[{zkey, depth}] = result;
    return result;
}


TEST_CASE("Perft", "[move-generation]") {
    engine::init_tables();
    engine::Board board;
    engine::MoveGen gen(board);


    std::vector<std::string> fen_strings = {
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ",
            "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ",
            "4k3/8/8/8/8/8/8/4K2R w K - 0 1 ",
            "4k3/8/8/8/8/8/8/R3K3 w Q - 0 1 ",
            "4k2r/8/8/8/8/8/8/4K3 w k - 0 1 ",
            "r3k3/8/8/8/8/8/8/4K3 w q - 0 1 ",
            "4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1 ",
            "r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1 ",
            "8/8/8/8/8/8/6k1/4K2R w K - 0 1 ",
            "8/8/8/8/8/8/1k6/R3K3 w Q - 0 1 ",
            "4k2r/6K1/8/8/8/8/8/8 w k - 0 1 ",
            "r3k3/1K6/8/8/8/8/8/8 w q - 0 1 ",
            "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1 ",
            "r3k2r/8/8/8/8/8/8/1R2K2R w Kkq - 0 1 ",
            "r3k2r/8/8/8/8/8/8/2R1K2R w Kkq - 0 1 ",
            "r3k2r/8/8/8/8/8/8/R3K1R1 w Qkq - 0 1 ",
            "1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1 ",
            "2r1k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1 ",
            "r3k1r1/8/8/8/8/8/8/R3K2R w KQq - 0 1 ",
            "4k3/8/8/8/8/8/8/4K2R b K - 0 1 ",
            "4k3/8/8/8/8/8/8/R3K3 b Q - 0 1 ",
            "4k2r/8/8/8/8/8/8/4K3 b k - 0 1 ",
            "r3k3/8/8/8/8/8/8/4K3 b q - 0 1 ",
            "4k3/8/8/8/8/8/8/R3K2R b KQ - 0 1 ",
            "r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1 ",
            "8/8/8/8/8/8/6k1/4K2R b K - 0 1 ",
            "8/8/8/8/8/8/1k6/R3K3 b Q - 0 1 ",
            "4k2r/6K1/8/8/8/8/8/8 b k - 0 1 ",
            "r3k3/1K6/8/8/8/8/8/8 b q - 0 1 ",
            "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1 ",
            "r3k2r/8/8/8/8/8/8/1R2K2R b Kkq - 0 1 ",
            "r3k2r/8/8/8/8/8/8/2R1K2R b Kkq - 0 1 ",
            "r3k2r/8/8/8/8/8/8/R3K1R1 b Qkq - 0 1 ",
            "1r2k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1 ",
            "2r1k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1 ",
            "r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1 ",
            "8/1n4N1/2k5/8/8/5K2/1N4n1/8 w - - 0 1 ",
            "8/1k6/8/5N2/8/4n3/8/2K5 w - - 0 1 ",
            "8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1 ",
            "K7/8/2n5/1n6/8/8/8/k6N w - - 0 1 ",
            "k7/8/2N5/1N6/8/8/8/K6n w - - 0 1 ",
            "8/1n4N1/2k5/8/8/5K2/1N4n1/8 b - - 0 1 ",
            "8/1k6/8/5N2/8/4n3/8/2K5 b - - 0 1 ",
            "8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1 ",
            "K7/8/2n5/1n6/8/8/8/k6N b - - 0 1 ",
            "k7/8/2N5/1N6/8/8/8/K6n b - - 0 1 ",
            "B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1 ",
            "8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1 ",
            "k7/B7/1B6/1B6/8/8/8/K6b w - - 0 1 ",
            "K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1 ",
            "B6b/8/8/8/2K5/5k2/8/b6B b - - 0 1 ",
            "8/8/1B6/7b/7k/8/2B1b3/7K b - - 0 1 ",
            "k7/B7/1B6/1B6/8/8/8/K6b b - - 0 1 ",
            "K7/b7/1b6/1b6/8/8/8/k6B b - - 0 1 ",
            "7k/RR6/8/8/8/8/rr6/7K w - - 0 1 ",
            "R6r/8/8/2K5/5k2/8/8/r6R w - - 0 1 ",
            "7k/RR6/8/8/8/8/rr6/7K b - - 0 1 ",
            "R6r/8/8/2K5/5k2/8/8/r6R b - - 0 1 ",
            "6kq/8/8/8/8/8/8/7K w - - 0 1 ",
            "6KQ/8/8/8/8/8/8/7k b - - 0 1 ",
            "K7/8/8/3Q4/4q3/8/8/7k w - - 0 1 ",
            "6qk/8/8/8/8/8/8/7K b - - 0 1 ",
            "6KQ/8/8/8/8/8/8/7k b - - 0 1 ",
            "K7/8/8/3Q4/4q3/8/8/7k b - - 0 1 ",
            "8/8/8/8/8/K7/P7/k7 w - - 0 1 ",
            "8/8/8/8/8/7K/7P/7k w - - 0 1 ",
            "K7/p7/k7/8/8/8/8/8 w - - 0 1 ",
            "7K/7p/7k/8/8/8/8/8 w - - 0 1 ",
            "8/2k1p3/3pP3/3P2K1/8/8/8/8 w - - 0 1 ",
            "8/8/8/8/8/K7/P7/k7 b - - 0 1 ",
            "8/8/8/8/8/7K/7P/7k b - - 0 1 ",
            "K7/p7/k7/8/8/8/8/8 b - - 0 1 ",
            "7K/7p/7k/8/8/8/8/8 b - - 0 1 ",
            "8/2k1p3/3pP3/3P2K1/8/8/8/8 b - - 0 1 ",
            "8/8/8/8/8/4k3/4P3/4K3 w - - 0 1 ",
            "4k3/4p3/4K3/8/8/8/8/8 b - - 0 1 ",
            "8/8/7k/7p/7P/7K/8/8 w - - 0 1 ",
            "8/8/k7/p7/P7/K7/8/8 w - - 0 1 ",
            "8/8/3k4/3p4/3P4/3K4/8/8 w - - 0 1 ",
            "8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 1 ",
            "8/8/3k4/3p4/8/3P4/3K4/8 w - - 0 1 ",
            "k7/8/3p4/8/3P4/8/8/7K w - - 0 1 ",
            "8/8/7k/7p/7P/7K/8/8 b - - 0 1 ",
            "8/8/k7/p7/P7/K7/8/8 b - - 0 1 ",
            "8/8/3k4/3p4/3P4/3K4/8/8 b - - 0 1 ",
            "8/3k4/3p4/8/3P4/3K4/8/8 b - - 0 1 ",
            "8/8/3k4/3p4/8/3P4/3K4/8 b - - 0 1 ",
            "k7/8/3p4/8/3P4/8/8/7K b - - 0 1 ",
            "7k/3p4/8/8/3P4/8/8/K7 w - - 0 1 ",
            "7k/8/8/3p4/8/8/3P4/K7 w - - 0 1 ",
            "k7/8/8/7p/6P1/8/8/K7 w - - 0 1 ",
            "k7/8/7p/8/8/6P1/8/K7 w - - 0 1 ",
            "k7/8/8/6p1/7P/8/8/K7 w - - 0 1 ",
            "k7/8/6p1/8/8/7P/8/K7 w - - 0 1 ",
            "k7/8/8/3p4/4p3/8/8/7K w - - 0 1 ",
            "k7/8/3p4/8/8/4P3/8/7K w - - 0 1 ",
            "7k/3p4/8/8/3P4/8/8/K7 b - - 0 1 ",
            "7k/8/8/3p4/8/8/3P4/K7 b - - 0 1 ",
            "k7/8/8/7p/6P1/8/8/K7 b - - 0 1 ",
            "k7/8/7p/8/8/6P1/8/K7 b - - 0 1 ",
            "k7/8/8/6p1/7P/8/8/K7 b - - 0 1 ",
            "k7/8/6p1/8/8/7P/8/K7 b - - 0 1 ",
            "k7/8/8/3p4/4p3/8/8/7K b - - 0 1 ",
            "k7/8/3p4/8/8/4P3/8/7K b - - 0 1 ",
            "7k/8/8/p7/1P6/8/8/7K w - - 0 1 ",
            "7k/8/p7/8/8/1P6/8/7K w - - 0 1 ",
            "7k/8/8/1p6/P7/8/8/7K w - - 0 1 ",
            "7k/8/1p6/8/8/P7/8/7K w - - 0 1 ",
            "k7/7p/8/8/8/8/6P1/K7 w - - 0 1 ",
            "k7/6p1/8/8/8/8/7P/K7 w - - 0 1 ",
            "3k4/3pp3/8/8/8/8/3PP3/3K4 w - - 0 1 ",
            "7k/8/8/p7/1P6/8/8/7K b - - 0 1 ",
            "7k/8/p7/8/8/1P6/8/7K b - - 0 1 ",
            "7k/8/8/1p6/P7/8/8/7K b - - 0 1 ",
            "7k/8/1p6/8/8/P7/8/7K b - - 0 1 ",
            "k7/7p/8/8/8/8/6P1/K7 b - - 0 1 ",
            "k7/6p1/8/8/8/8/7P/K7 b - - 0 1 ",
            "3k4/3pp3/8/8/8/8/3PP3/3K4 b - - 0 1 ",
            "8/Pk6/8/8/8/8/6Kp/8 w - - 0 1 ",
            "n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1 ",
            "8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1 ",
            "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1 ",
            "8/Pk6/8/8/8/8/6Kp/8 b - - 0 1 ",
            "n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1 ",
            "8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1 ",
            "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1 ",
            "7k/8/8/K1pP3r/8/8/8/8 w - c6 0 1",
            "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1",
            "8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1",
            "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1",
            "6k1/3p4/8/2P5/2K5/8/8/8 b - - 0 1"
    };
    std::vector<std::vector<int>> values = {
            {20, 400,  8902,  197281,  4865609,  119060324,},
            {48, 2039, 97862, 4085603, 193690690,},
            {15, 66,   1197,  7059,    133987,   764643,},
            {16, 71,   1287,  7626,    145232,   846648,},
            {5,  75,   459,   8290,    47635,    899442,},
            {5,  80,   493,   8897,    52710,    1001523,},
            {26, 112,  3189,  17945,   532933,   2788982,},
            {5,  130,  782,   22180,   118882,   3517770,},
            {12, 38,   564,   2219,    37735,    185867,},
            {15, 65,   1018,  4573,    80619,    413018,},
            {3,  32,   134,   2073,    10485,    179869,},
            {4,  49,   243,   3991,    20780,    367724,},
            {26, 568,  13744, 314346,  7594526,  179862938,},
            {25, 567,  14095, 328965,  8153719,  195629489,},
            {25, 548,  13502, 312835,  7736373,  184411439,},
            {25, 547,  13579, 316214,  7878456,  189224276,},
            {26, 583,  14252, 334705,  8198901,  198328929,},
            {25, 560,  13592, 317324,  7710115,  185959088,},
            {25, 560,  13607, 320792,  7848606,  190755813,},
            {5,  75,   459,   8290,    47635,    899442,},
            {5,  80,   493,   8897,    52710,    1001523,},
            {15, 66,   1197,  7059,    133987,   764643,},
            {16, 71,   1287,  7626,    145232,   846648,},
            {5,  130,  782,   22180,   118882,   3517770,},
            {26, 112,  3189,  17945,   532933,   2788982,},
            {3,  32,   134,   2073,    10485,    179869,},
            {4,  49,   243,   3991,    20780,    367724,},
            {12, 38,   564,   2219,    37735,    185867,},
            {15, 65,   1018,  4573,    80619,    413018,},
            {26, 568,  13744, 314346,  7594526,  179862938,},
            {26, 583,  14252, 334705,  8198901,  198328929,},
            {25, 560,  13592, 317324,  7710115,  185959088,},
            {25, 560,  13607, 320792,  7848606,  190755813,},
            {25, 567,  14095, 328965,  8153719,  195629489,},
            {25, 548,  13502, 312835,  7736373,  184411439,},
            {25, 547,  13579, 316214,  7878456,  189224276,},
            {14, 195,  2760,  38675,   570726,   8107539,},
            {11, 156,  1636,  20534,   223507,   2594412,},
            {19, 289,  4442,  73584,   1198299,  19870403,},
            {3,  51,   345,   5301,    38348,    588695,},
            {17, 54,   835,   5910,    92250,    688780,},
            {15, 193,  2816,  40039,   582642,   8503277,},
            {16, 180,  2290,  24640,   288141,   3147566,},
            {4,  68,   1118,  16199,   281190,   4405103,},
            {17, 54,   835,   5910,    92250,    688780,},
            {3,  51,   345,   5301,    38348,    588695,},
            {17, 278,  4607,  76778,   1320507,  22823890,},
            {21, 316,  5744,  93338,   1713368,  28861171,},
            {21, 144,  3242,  32955,   787524,   7881673,},
            {7,  143,  1416,  31787,   310862,   7382896,},
            {6,  106,  1829,  31151,   530585,   9250746,},
            {17, 309,  5133,  93603,   1591064,  29027891,},
            {7,  143,  1416,  31787,   310862,   7382896,},
            {21, 144,  3242,  32955,   787524,   7881673,},
            {19, 275,  5300,  104342,  2161211,  44956585,},
            {36, 1027, 29215, 771461,  20506480, 525169084,},
            {19, 275,  5300,  104342,  2161211,  44956585,},
            {36, 1027, 29227, 771368,  20521342, 524966748,},
            {2,  36,   143,   3637,    14893,    391507,},
            {2,  36,   143,   3637,    14893,    391507,},
            {6,  35,   495,   8349,    166741,   3370175,},
            {22, 43,   1015,  4167,    105749,   419369,},
            {2,  36,   143,   3637,    14893,    391507,},
            {6,  35,   495,   8349,    166741,   3370175,},
            {3,  7,    43,    199,     1347,     6249,},
            {3,  7,    43,    199,     1347,     6249,},
            {1,  3,    12,    80,      342,      2343,},
            {1,  3,    12,    80,      342,      2343,},
            {7,  35,   210,   1091,    7028,     34834,},
            {1,  3,    12,    80,      342,      2343,},
            {1,  3,    12,    80,      342,      2343,},
            {3,  7,    43,    199,     1347,     6249,},
            {3,  7,    43,    199,     1347,     6249,},
            {5,  35,   182,   1091,    5408,     34822,},
            {2,  8,    44,    282,     1814,     11848,},
            {2,  8,    44,    282,     1814,     11848,},
            {3,  9,    57,    360,     1969,     10724,},
            {3,  9,    57,    360,     1969,     10724,},
            {5,  25,   180,   1294,    8296,     53138,},
            {8,  61,   483,   3213,    23599,    157093,},
            {8,  61,   411,   3213,    21637,    158065,},
            {4,  15,   90,    534,     3450,     20960,},
            {3,  9,    57,    360,     1969,     10724,},
            {3,  9,    57,    360,     1969,     10724,},
            {5,  25,   180,   1294,    8296,     53138,},
            {8,  61,   411,   3213,    21637,    158065,},
            {8,  61,   483,   3213,    23599,    157093,},
            {4,  15,   89,    537,     3309,     21104,},
            {4,  19,   117,   720,     4661,     32191,},
            {5,  19,   116,   716,     4786,     30980,},
            {5,  22,   139,   877,     6112,     41874,},
            {4,  16,   101,   637,     4354,     29679,},
            {5,  22,   139,   877,     6112,     41874,},
            {4,  16,   101,   637,     4354,     29679,},
            {3,  15,   84,    573,     3013,     22886,},
            {4,  16,   101,   637,     4271,     28662,},
            {5,  19,   117,   720,     5014,     32167,},
            {4,  19,   117,   712,     4658,     30749,},
            {5,  22,   139,   877,     6112,     41874,},
            {4,  16,   101,   637,     4354,     29679,},
            {5,  22,   139,   877,     6112,     41874,},
            {4,  16,   101,   637,     4354,     29679,},
            {5,  15,   102,   569,     4337,     22579,},
            {4,  16,   101,   637,     4271,     28662,},
            {5,  22,   139,   877,     6112,     41874,},
            {4,  16,   101,   637,     4354,     29679,},
            {5,  22,   139,   877,     6112,     41874,},
            {4,  16,   101,   637,     4354,     29679,},
            {5,  25,   161,   1035,    7574,     55338,},
            {5,  25,   161,   1035,    7574,     55338,},
            {7,  49,   378,   2902,    24122,    199002,},
            {5,  22,   139,   877,     6112,     41874,},
            {4,  16,   101,   637,     4354,     29679,},
            {5,  22,   139,   877,     6112,     41874,},
            {4,  16,   101,   637,     4354,     29679,},
            {5,  25,   161,   1035,    7574,     55338,},
            {5,  25,   161,   1035,    7574,     55338,},
            {7,  49,   378,   2902,    24122,    199002,},
            {11, 97,   887,   8048,    90606,    1030499,},
            {24, 421,  7421,  124608,  2193768,  37665329,},
            {18, 270,  4699,  79355,   1533145,  28859283,},
            {24, 496,  9483,  182838,  3605103,  71179139,},
            {11, 97,   887,   8048,    90606,    1030499,},
            {24, 421,  7421,  124608,  2193768,  37665329,},
            {18, 270,  4699,  79355,   1533145,  28859283,},
            {24, 496,  9483,  182838,  3605103,  71179139,},
            {5,  70,   438,   7051,    44848,    755356, 4974982, 86145861},
            {18, 92,   1670,  10138,   185429,   1134888},
            {13, 102,  1266,  10276,   135655,   1015133},
            {15, 126,  1928,  13931,   206379,   1440467},
            {7,  57,   415,   3415,    25611,    209382}
    };
    init_zobrist();
    unsigned positions = fen_strings.size(), to_check, depth;
    for (int pos = 0; pos < positions; pos++) {
        to_check = values[pos].size();
        board.load_fen(fen_strings[pos]);
        for (int val = 0; val < to_check; val++) {
            depth = val + 1;

            float time_taken = clock();
            memo.clear();
            unsigned result = perft_memoized(depth, gen, board);
            time_taken = (clock() - time_taken) / CLOCKS_PER_SEC;
            REQUIRE(result == values[pos][val]);
            std::cout << "nodes: " << result << " time: " << time_taken << " nps: "
                      << (int) ((float) result / time_taken) << std::endl;
        }
        std::cout << "Ok " << pos + 1 << " perft positions " << std::endl;
    }
}

TEST_CASE("Smallest-Attacker", "[board]") {
    engine::init_tables();
    Board board;
    board.load_fen("2Q5/3B4/5q2/4b3/2N1k3/5p2/5K1R/8 w - - 0 1");
    Square attacker;

    REQUIRE(board.get_smallest_attacker(SQ_F3, C_WHITE, attacker) == PT_KING);
    REQUIRE(board.get_smallest_attacker(SQ_H1, C_WHITE, attacker) == PT_ROOK);
    REQUIRE(board.get_smallest_attacker(SQ_H3, C_WHITE, attacker) == PT_BISHOP);
    REQUIRE(board.get_smallest_attacker(SQ_G2, C_WHITE, attacker) == PT_ROOK);
    REQUIRE(board.get_smallest_attacker(SQ_E5, C_WHITE, attacker) == PT_KNIGHT);
    REQUIRE(board.get_smallest_attacker(SQ_G4, C_WHITE, attacker) == PT_BISHOP);
    REQUIRE(board.get_smallest_attacker(SQ_F3, C_BLACK, attacker) == PT_QUEEN);
    REQUIRE(board.get_smallest_attacker(SQ_D1, C_BLACK, attacker) == PT_NONE);
    REQUIRE(board.get_smallest_attacker(SQ_H2, C_BLACK, attacker) == PT_BISHOP);
    REQUIRE(board.get_smallest_attacker(SQ_F2, C_BLACK, attacker) == PT_NONE);
    REQUIRE(board.get_smallest_attacker(SQ_E2, C_BLACK, attacker) == PT_PAWN);
    REQUIRE(board.get_smallest_attacker(SQ_G2, C_BLACK, attacker) == PT_PAWN);
}


TEST_CASE("SEE", "[board]") {
    engine::init_tables();
    Board board;

    board.load_fen("4R3/2r3p1/5bk1/1p1r3p/p2PR1P1/P1BK1P2/1P6/8 b - - 0 1");
    REQUIRE(board.see(create_capture_move(SQ_H5, SQ_G4, P_B_PAWN, P_W_PAWN), C_BLACK) == 0);
    board.load_fen("4R3/2r3p1/5bk1/1p1r1p1p/p2PR1P1/P1BK1P2/1P6/8 b - - 0 1");
    REQUIRE(board.see(create_capture_move(SQ_H5, SQ_G4, P_B_PAWN, P_W_PAWN), C_BLACK) == 0);
    board.load_fen("4r1k1/5pp1/nbp4p/1p2p2q/1P2P1b1/1BP2N1P/1B2QPPK/3R4 b - - 0 1");
    REQUIRE(board.see(create_capture_move(SQ_G4, SQ_F3, P_B_BISHOP, P_W_KNIGHT), C_BLACK) ==
            C_KNIGHT_VALUE - C_BISHOP_VALUE);
    board.load_fen("2r1r1k1/pp1bppbp/3p1np1/q3P3/2P2P2/1P2B3/P1N1B1PP/2RQ1RK1 b - - 0 1");
    REQUIRE(board.see(create_capture_move(SQ_D6, SQ_E5, P_B_PAWN, P_W_PAWN), C_BLACK) == C_PAWN_VALUE);
    board.load_fen("7r/5qpk/p1Qp1b1p/3r3n/BB3p2/5p2/P1P2P2/4RK1R w - -");
    REQUIRE(board.see(create_quiet_move(SQ_E1, SQ_E8, P_W_ROOK), C_WHITE) == 0);
    board.load_fen("6rr/6pk/p1Qp1b1p/2n5/1B3p2/5p2/P1P2P2/4RK1R w - - 0 1");
    REQUIRE(board.see(create_quiet_move(SQ_E1, SQ_E8, P_W_ROOK), C_WHITE) == -C_ROOK_VALUE);
    board.load_fen("7r/5qpk/2Qp1b1p/1N1r3n/BB3p2/5p2/P1P2P2/4RK1R w - -");
    REQUIRE(board.see(create_quiet_move(SQ_E1, SQ_E8, P_W_ROOK), C_WHITE) == -C_ROOK_VALUE);

    board.load_fen("8/4kp2/2npp3/1Nn5/1p2PQP1/7q/1PP1B3/4KR1r b - -");
    REQUIRE(board.see(create_capture_move(SQ_H1, SQ_F1, P_B_ROOK, P_W_ROOK), C_BLACK) == 0);

    board.load_fen("8/4kp2/2npp3/1Nn5/1p2P1P1/7q/1PP1B3/4KR1r b");
    REQUIRE(board.see(create_capture_move(SQ_H1, SQ_F1, P_B_ROOK, P_W_ROOK), C_BLACK) == 0);

    board.load_fen("2r2r1k/6bp/p7/2q2p1Q/3PpP2/1B6/P5PP/2RR3K b - -");
    REQUIRE(board.see(create_capture_move(SQ_C5, SQ_C1, P_B_QUEEN, P_W_ROOK), C_BLACK) ==
            2 * C_ROOK_VALUE - C_QUEEN_VALUE);

    board.load_fen("r2qk1nr/pp2ppbp/2b3p1/2p1p3/8/2N2N2/PPPP1PPP/R1BQR1K1 w kq - 0 1");
    REQUIRE(board.see(create_capture_move(SQ_F3, SQ_E5, P_W_KNIGHT, P_B_PAWN), C_WHITE) == C_PAWN_VALUE);

    board.load_fen("6r1/4kq2/b2p1p2/p1pPb3/p1P2B1Q/2P4P/2B1R1P1/6K1 w - - 0 1");
    REQUIRE(board.see(create_capture_move(SQ_F4, SQ_E5, P_W_BISHOP, P_B_BISHOP), C_WHITE) == 0);


    board.load_fen("k1b3b1/8/4p3/8/2B3B1/1Q6/B7/K7 w - - 0 1");
    REQUIRE(board.see(SQ_E6, C_WHITE) == C_PAWN_VALUE);
}

TEST_CASE("Material-Count", "[eval]") {
    engine::init_tables();
    engine::init_pst();
    Board board;
    board.load_fen("2b5/p2NBp1p/1bp1nPPr/3P4/2pRnr1P/Rk1B1Ppp/5pBP/Rq1N3K b - - 0 1");
    REQUIRE(score_material<C_BLACK>(board) ==
            8 * C_PAWN_VALUE + 2 * C_KNIGHT_VALUE + 2 * C_BISHOP_VALUE + 2 * C_ROOK_VALUE + C_QUEEN_VALUE);
    REQUIRE(score_material<C_WHITE>(board) ==
            6 * C_PAWN_VALUE + 2 * C_KNIGHT_VALUE + 3 * C_BISHOP_VALUE + 3 * C_ROOK_VALUE);

    board.load_fen("8/8/8/5pK1/1b2kP2/1P1p4/7P/2B5 b - - 1 54");
    REQUIRE(score_material<C_BLACK>(board) == 2 * C_PAWN_VALUE + C_BISHOP_VALUE);
    REQUIRE(score_material<C_WHITE>(board) == 3 * C_PAWN_VALUE + C_BISHOP_VALUE);

    board.load_fen("1r4rk/7p/3q4/ppp1p1B1/5p1P/3P1BP1/PP3P2/3RR1K1 w - - 0 29");
    REQUIRE(score_material<C_BLACK>(board) == 6 * C_PAWN_VALUE + 2 * C_ROOK_VALUE + C_QUEEN_VALUE);
    REQUIRE(score_material<C_WHITE>(board) == 6 * C_PAWN_VALUE + 2 * C_ROOK_VALUE + 2 * C_BISHOP_VALUE);
}


TEST_CASE("Pawn-structure", "[eval]") {
    engine::init_tables();
    Board board;
    int middle_game = 0, end_game = 0;

    board.load_fen("r1bq1knr/p2p1ppp/3p4/1P6/1P6/5N2/1P1PP1PP/RNBQKB1R b KQ - 0 8");

    score_pawn_structure<C_WHITE>(board, middle_game, end_game);
    REQUIRE(middle_game == C_DOUBLED_PAWN_PENALTY[PH_MIDDLEGAME] * 2);
    REQUIRE(end_game == C_DOUBLED_PAWN_PENALTY[PH_ENDGAME] * 2);

    middle_game = 0, end_game = 0;
    score_pawn_structure<C_BLACK>(board, middle_game, end_game);
    REQUIRE(middle_game == C_DOUBLED_PAWN_PENALTY[PH_MIDDLEGAME]);
    REQUIRE(end_game == C_DOUBLED_PAWN_PENALTY[PH_ENDGAME]);
    board.load_fen("3rr1k1/1ppq1pp1/p4n2/3P1nB1/2Q1p3/2N3P1/PP2PPKP/2RR4 b - - 4 22");

    middle_game = 0, end_game = 0;
    score_pawn_structure<C_WHITE>(board, middle_game, end_game);
    REQUIRE(middle_game == 0);
    REQUIRE(end_game == 0);

    middle_game = 0, end_game = 0;
    score_pawn_structure<C_BLACK>(board, middle_game, end_game);
    REQUIRE(middle_game == 0);
    REQUIRE(end_game == 0);


    board.load_fen("8/2pkp2p/2p1p3/4p1P1/1P1P1PP1/1P1P1P2/1K6/8 w - - 0 1");

    middle_game = 0, end_game = 0;
    score_pawn_structure<C_WHITE>(board, middle_game, end_game);
    REQUIRE(middle_game == C_DOUBLED_PAWN_PENALTY[PH_MIDDLEGAME] * 4);
    REQUIRE(end_game == C_DOUBLED_PAWN_PENALTY[PH_ENDGAME] * 4);

    middle_game = 0, end_game = 0;
    score_pawn_structure<C_BLACK>(board, middle_game, end_game);
    REQUIRE(middle_game == C_DOUBLED_PAWN_PENALTY[PH_MIDDLEGAME] * 3);
    REQUIRE(end_game == C_DOUBLED_PAWN_PENALTY[PH_ENDGAME] * 3);

    board.load_fen("8/2k2p2/p5p1/p7/8/3PK2P/2P5/8 w - - 0 1");
    middle_game = 0, end_game = 0;
    score_pawn_structure<C_WHITE>(board, middle_game, end_game);
    REQUIRE(middle_game == C_PASSED_PAWN_BONUS[PH_MIDDLEGAME][1] + C_PASSED_PAWN_BONUS[PH_MIDDLEGAME][2]);
    REQUIRE(end_game == C_PASSED_PAWN_BONUS[PH_ENDGAME][1] + C_PASSED_PAWN_BONUS[PH_ENDGAME][2]);

    middle_game = 0, end_game = 0;
    score_pawn_structure<C_BLACK>(board, middle_game, end_game);
    REQUIRE(middle_game == C_PASSED_PAWN_BONUS[PH_MIDDLEGAME][3] + C_PASSED_PAWN_BONUS[PH_MIDDLEGAME][1] +
                           C_DOUBLED_PAWN_PENALTY[PH_MIDDLEGAME]);


    board.load_fen("1k6/8/8/6P1/1pP3P1/8/P6p/5K2 w - - 0 1");
    middle_game = 0, end_game = 0;
    score_pawn_structure<C_WHITE>(board, middle_game, end_game);
    REQUIRE(middle_game==C_PASSED_PAWN_BONUS[PH_MIDDLEGAME][3]+C_PASSED_PAWN_BONUS[PH_MIDDLEGAME][4]+C_DOUBLED_PAWN_PENALTY[PH_MIDDLEGAME]);
    REQUIRE(end_game==C_PASSED_PAWN_BONUS[PH_ENDGAME][3]+C_PASSED_PAWN_BONUS[PH_ENDGAME][4]+C_DOUBLED_PAWN_PENALTY[PH_ENDGAME]);

    middle_game = 0, end_game = 0;
    score_pawn_structure<C_BLACK>(board, middle_game, end_game);
    REQUIRE(middle_game==C_PASSED_PAWN_BONUS[PH_MIDDLEGAME][6]);
    REQUIRE(end_game==C_PASSED_PAWN_BONUS[PH_ENDGAME][6]);

}

TEST_CASE("King-safety", "[eval]") {
    engine::init_tables();
    int middle_game = 0, end_game = 0;
    Board board;
    board.load_fen("1k6/ppp3b1/8/8/5N2/1P6/P7/1K6 w - - 0 1");

    score_king_safety<C_WHITE>(board, middle_game, end_game);
    REQUIRE(middle_game==C_WEAK_PAWN_SHIELD_BONUS[PH_MIDDLEGAME]+C_STRONG_PAWN_SHIELD_BONUS[PH_MIDDLEGAME]);
    REQUIRE(end_game==C_WEAK_PAWN_SHIELD_BONUS[PH_ENDGAME]+C_STRONG_PAWN_SHIELD_BONUS[PH_ENDGAME]);

    middle_game=0,end_game=0;
    score_king_safety<C_BLACK>(board, middle_game, end_game);
    REQUIRE(middle_game==C_STRONG_PAWN_SHIELD_BONUS[PH_MIDDLEGAME]*3);
    REQUIRE(end_game==C_STRONG_PAWN_SHIELD_BONUS[PH_ENDGAME]*3);

    board.load_fen("r7/2k5/2p5/8/4PP2/6P1/1Q3K2/8 w - - 0 1");
    middle_game=0,end_game=0;
    score_king_safety<C_WHITE>(board, middle_game, end_game);
    REQUIRE(middle_game==C_WEAK_PAWN_SHIELD_BONUS[PH_MIDDLEGAME]*2+C_STRONG_PAWN_SHIELD_BONUS[PH_MIDDLEGAME]);
    REQUIRE(end_game==C_WEAK_PAWN_SHIELD_BONUS[PH_ENDGAME]*2+C_STRONG_PAWN_SHIELD_BONUS[PH_ENDGAME]);

    middle_game=0,end_game=0;
    score_king_safety<C_BLACK>(board, middle_game, end_game);
    REQUIRE(middle_game==C_STRONG_PAWN_SHIELD_BONUS[PH_MIDDLEGAME]);
    REQUIRE(end_game==C_STRONG_PAWN_SHIELD_BONUS[PH_ENDGAME]);
}


TEST_CASE("Transposition-Table", "[ttable]") {
    TranspositionTable table(1);
    unsigned bytes_per_mb = 1000000;
    REQUIRE(table.get_size() == bytes_per_mb / sizeof(TTEntry));
    table.reserve(5);
    REQUIRE(table.get_size() == 5 * bytes_per_mb / sizeof(TTEntry));

    u64 first_key = 1237918123123273;
    u64 second_key = 4809012380129831;
    REQUIRE(table.probe(first_key) == nullptr);
    REQUIRE(table.probe(second_key) == nullptr);

    TTEntry first_entry{first_key, TT_EXACT, create_empty_move(), 400, 20};
    table.save(first_entry);
    REQUIRE(table.probe(first_key)->score == first_entry.score);
    REQUIRE(table.probe(second_key) == nullptr);

    TTEntry second_entry{first_key, TT_LOWERBOUND, create_empty_move(), -100, 21};
    table.save(second_entry);
    REQUIRE(table.probe(first_key)->score == second_entry.score);
    REQUIRE(table.probe(second_key) == nullptr);

    TTEntry third_entry{second_key, TT_UPPERBOUND, create_empty_move(), 30, 4};
    table.save(third_entry);
    REQUIRE(table.probe(first_key)->score == second_entry.score);
    REQUIRE(table.probe(second_key)->score == third_entry.score);

    TTEntry fourth_entry{second_key, TT_EXACT, create_empty_move(), -233, 2};
    table.save(fourth_entry);
    REQUIRE(table.probe(second_key)->score == third_entry.score);

    table.reserve(2);
    REQUIRE(table.probe(first_key) == nullptr);
    REQUIRE(table.probe(second_key) == nullptr);

}

TEST_CASE("Tactics", "[search]") {
    engine::init_tables();
    engine::init_pst();
    engine::init_zobrist();
    Board board;
    Search search(board);
    search.limits.allotted_time = 1000.0;

    SECTION("Checkmates") {
        search.limits.maximum_depth = 4;
        board.load_fen("8/2k5/8/2K5/2B5/2b5/2q2N2/8 b - - 0 1");
        REQUIRE(move_to_string(search.iterative_deepening()) == "c2a4");

        board.load_fen("8/6b1/1pp5/k1pqN3/N6Q/PP2P3/3P4/4KB2 w - - 0 1");
        REQUIRE(move_to_string(search.iterative_deepening()) == "h4d8");


        search.limits.maximum_depth = 6;
        board.load_fen("8/p7/p6p/1p3Kpk/p7/PRP2pPp/pPP2P1P/8 w - - 0 1");
        REQUIRE(move_to_string(search.iterative_deepening()) == "b3b4");
        board.load_fen("6bk/5P1p/7K/8/8/8/8/1B6 w - - 0 1");
        REQUIRE(move_to_string(search.iterative_deepening()) == "f7f8b");
        board.load_fen("3r2k1/2p2ppp/4p3/pPK5/2P3P1/3q4/8/3R1R2 b - - 0 1");
        REQUIRE(move_to_string(search.iterative_deepening()) == "d3e3");

        search.limits.maximum_depth = 8;
        board.load_fen("r4rk1/2p2pp1/p1q2n1p/4p3/2Qn2b1/PP1P3N/2P2PPP/R1B2RK1 b - - 0 1");
        REQUIRE(move_to_string(search.iterative_deepening()) == "c6g2");
        board.load_fen("1rb3k1/p1q2p1p/Pp4p1/1P1Q1nb1/3P2P1/B7/5P1P/4R1K1 w - - 0 1");
        REQUIRE(move_to_string(search.iterative_deepening()) == "e1e8");
    }

}