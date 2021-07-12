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
    MoveGen gen(board);
    auto x=gen.get_moves();
    for(auto a:x){
        std::cout<<move_to_string(a)<<std::endl;
    }

    std::cout<<board.to_string()<<std::endl;
    return 0;
}
