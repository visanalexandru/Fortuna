#include <iostream>
#include"util.h"
#include"tables.h"
#include"board.h"
#include"movegen.h"
#include"attacks.h"
#include<cassert>

using namespace engine;

int main() {

    init_attack_tables();

    Board board;
    board.load_fen("rnbqkbnr/pppppp1p/8/8/6pP/4PP2/PPPP2P1/RNBQKBNR b KQkq h3 0 3");

    for (int sq = 0; sq < 64; sq++) {

        u64 queen_attacks = 0;
        for (int k = 1; k < C_NUM_DIRECTIONS; k += 2) {
            queen_attacks |= RAY_ATTACKS[sq][k];
        }
        std::cout << dump_bitboard(queen_attacks) << std::endl;
    }

    MoveGen gen(board);
    auto x = gen.get_moves();
    for (auto a:x) {
        std::cout << move_to_string(a) << std::endl;
    }

    std::cout << board.to_string() << std::endl;
    return 0;
}
