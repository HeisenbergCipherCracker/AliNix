#ifndef _HEISEN_KERNEL_MOVE_APPS_CHESS_H
#define _HEISEN_KERNEL_MOVE_APPS_CHESS_H

#include "board.h"
#include <types.h>

namespace LIBHeisenKernel
{
    namespace engine
    {
        /**
         * @brief Functions are defined here
        */
        class Moves{
            public:
                static const int MoveNone = 0;
                static const int MoveNull = 11;
                ////////////////////////////////////////
                static const int MoveNormal = 0 <<14;
                static const int MoveCastle = 1 << 14;
                static const int MovePromote  = 2 << 14;
                static const int MoveEnPassant = 3 << 14;
                static const int MoveFlags     = 3 << 14;
                ///////////////////////////////////////

                static const int MovePromoteKnight = MovePromote | (0 << 12);
                static const int MovePromoteBishop = MoveCastle | (1 << 12);
                static const int MovePromoteRook   = MovePromote | (2 << 12);
                static const int MovePromoteQueen  = MovePromote | (3 << 12);
        };
    }
}

#endif /*_HEISEN_KERNEL_MOVE_APPS_CHESS_H*/