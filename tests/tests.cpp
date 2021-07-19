//
// Created by gvisan on 11.07.2021.
//

#define CATCH_CONFIG_MAIN

#include"movegen.h"
#include"catch.hpp"
#include "util.h"

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

    REQUIRE(piece_to_notation(P_NONE) == '#');

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
    unsigned positions = fen_strings.size(), to_check, depth;
    for (int pos = 0; pos < positions; pos++) {
        to_check = values[pos].size();
        board.load_fen(fen_strings[pos]);
        for (int val = 0; val < to_check; val++) {
            depth = val + 1;

            float time_taken = clock();
            unsigned result = gen.perft(depth);
            time_taken = (clock() - time_taken) / CLOCKS_PER_SEC;
            REQUIRE(result == values[pos][val]);
            std::cout << "nodes: " << result << " time: " << time_taken << " nps: "
                      << (int) ((float) result / time_taken) << std::endl;
        }
        std::cout << "Ok " << pos + 1 << " perft positions " << std::endl;
    }
}

