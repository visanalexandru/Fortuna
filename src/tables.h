//
// Created by gvisan on 10.07.2021.
//

#ifndef CHESSENGINE_TABLES_H
#define CHESSENGINE_TABLES_H

#include"defines.h"

namespace engine {
    const u64 c_all = 0xffffffffffffffff;

    const u64 mask_rank[] = {0xFF,
                             0xFF00,
                             0xFF0000,
                             0xFF000000,
                             0xFF00000000,
                             0xFF0000000000,
                             0xFF000000000000,
                             0xFF00000000000000
    };

    const u64 mask_file[] = {0x0101010101010101,
                             0x202020202020202,
                             0x404040404040404,
                             0x808080808080808,
                             0x1010101010101010,
                             0x2020202020202020,
                             0x4040404040404040,
                             0x8080808080808080
    };

    const u64 clear_rank[] = {0xFFFFFFFFFFFFFF00,
                              0xFFFFFFFFFFFF00FF,
                              0xFFFFFFFFFF00FFFF,
                              0xFFFFFFFF00FFFFFF,
                              0xFFFFFF00FFFFFFFF,
                              0xFFFF00FFFFFFFFFF,
                              0xFF00FFFFFFFFFFFF,
                              0x00FFFFFFFFFFFFFF
    };

    const u64 clear_file[] = {0xFEFEFEFEFEFEFEFE,
                              0xFDFDFDFDFDFDFDFD,
                              0xFBFBFBFBFBFBFBFB,
                              0xF7F7F7F7F7F7F7F7,
                              0xEFEFEFEFEFEFEFEF,
                              0xDFDFDFDFDFDFDFDF,
                              0xBFBFBFBFBFBFBFBF,
                              0x7F7F7F7F7F7F7F7F
    };
}

#endif //CHESSENGINE_TABLES_H
