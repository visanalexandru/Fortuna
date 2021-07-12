//
// Created by gvisan on 11.07.2021.
//

#define CATCH_CONFIG_MAIN
#include"catch.hpp"
#include "util.h"
using namespace engine;

TEST_CASE("Squares","[util]"){
    Square squares[8][8]={
            SQ_A1,SQ_B1,SQ_C1,SQ_D1,SQ_E1,SQ_F1,SQ_G1,SQ_H1,
            SQ_A2,SQ_B2,SQ_C2,SQ_D2,SQ_E2,SQ_F2,SQ_G2,SQ_H2,
            SQ_A3,SQ_B3,SQ_C3,SQ_D3,SQ_E3,SQ_F3,SQ_G3,SQ_H3,
            SQ_A4,SQ_B4,SQ_C4,SQ_D4,SQ_E4,SQ_F4,SQ_G4,SQ_H4,
            SQ_A5,SQ_B5,SQ_C5,SQ_D5,SQ_E5,SQ_F5,SQ_G5,SQ_H5,
            SQ_A6,SQ_B6,SQ_C6,SQ_D6,SQ_E6,SQ_F6,SQ_G6,SQ_H6,
            SQ_A7,SQ_B7,SQ_C7,SQ_D7,SQ_E7,SQ_F7,SQ_G7,SQ_H7,
            SQ_A8,SQ_B8,SQ_C8,SQ_D8,SQ_E8,SQ_F8,SQ_G8,SQ_H8,

    };
    std::string string_squares[8][8]={
            "a1","b1","c1","d1","e1","f1","g1","h1",
            "a2","b2","c2","d2","e2","f2","g2","h2",
            "a3","b3","c3","d3","e3","f3","g3","h3",
            "a4","b4","c4","d4","e4","f4","g4","h4",
            "a5","b5","c5","d5","e5","f5","g5","h5",
            "a6","b6","c6","d6","e6","f6","g6","h6",
            "a7","b7","c7","d7","e7","f7","g7","h7",
            "a8","b8","c8","d8","e8","f8","g8","h8",

    };

    for(int rank=0;rank<8;rank++){
        for(int file=0;file<8;file++){
            REQUIRE(position_to_square(file,rank)==squares[rank][file]);
            REQUIRE(notation_to_square(string_squares[rank][file])==squares[rank][file]);
            REQUIRE(square_to_notation(position_to_square(file,rank))==string_squares[rank][file]);
        }
    }
}
TEST_CASE("Pieces", "[util]"){
    REQUIRE(notation_to_piece('P')==Piece::WPawn);
    REQUIRE(notation_to_piece('N')==Piece::WKnight);
    REQUIRE(notation_to_piece('B')==Piece::WBishop);
    REQUIRE(notation_to_piece('R')==Piece::WRook);
    REQUIRE(notation_to_piece('Q')==Piece::WQueen);
    REQUIRE(notation_to_piece('K')==Piece::WKing);

    REQUIRE(notation_to_piece('p')==Piece::BPawn);
    REQUIRE(notation_to_piece('n')==Piece::BKnight);
    REQUIRE(notation_to_piece('b')==Piece::BBishop);
    REQUIRE(notation_to_piece('r')==Piece::BRook);
    REQUIRE(notation_to_piece('q')==Piece::BQueen);
    REQUIRE(notation_to_piece('k')==Piece::BKing);



    REQUIRE(piece_to_notation(WPawn)=='P');
    REQUIRE(piece_to_notation(WKnight)=='N');
    REQUIRE(piece_to_notation(WBishop)=='B');
    REQUIRE(piece_to_notation(WRook)=='R');
    REQUIRE(piece_to_notation(WQueen)=='Q');
    REQUIRE(piece_to_notation(WKing)=='K');

    REQUIRE(piece_to_notation(BPawn)=='p');
    REQUIRE(piece_to_notation(BKnight)=='n');
    REQUIRE(piece_to_notation(BBishop)=='b');
    REQUIRE(piece_to_notation(BRook)=='r');
    REQUIRE(piece_to_notation(BQueen)=='q');
    REQUIRE(piece_to_notation(BKing)=='k');

    REQUIRE(piece_to_notation(None)=='#');

}

