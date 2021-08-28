#include <iostream>
#include"util.h"
#include"board.h"
#include"movegen.h"
#include"attacks.h"
#include"ttable.h"
#include"uci.h"

using namespace engine;

int main() {
    init_tables();
    init_pst();
    init_zobrist();
    UCI protocol;
    protocol.start();

    return 0;
}
