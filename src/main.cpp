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
    board.load_fen("8/pp1p3p/p1B1p1Kp/NNNNNBNN/p6p/NNpNNNNN/p5p1/1R4RR w - - 0 1");
    MoveGen gen(board);
    auto x=gen.get_moves();
    for(auto a:x){
        std::cout<<move_to_string(a)<<std::endl;
    }

    std::cout<<board.to_string()<<std::endl;
    return 0;
}
