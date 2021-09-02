#include <iostream>
#include"util.h"
#include"board.h"
#include"movegen.h"
#include"attacks.h"
#include"ttable.h"
#include"uci.h"
#include"nnue_eval.h"

using namespace engine;

int main() {
    init_tables();
    init_pst();
    init_zobrist();
    init_nnue("net.nnue");
    UCI protocol;
    protocol.start();

    return 0;
}
