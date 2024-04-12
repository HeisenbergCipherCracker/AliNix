#ifndef ALINIX_KERNEL_APPS_CHESS_GAME_H
#define ALINIX_KERNEL_APPS_CHESS_GAME_H

namespace LIBHeisenKernel{
    namespace engine{
        
        /**
         * @brief Defining the constants here
        */
        const int ListSize = 256;

        /**
         * @brief Defining the list macro here
        */
        #define LIST_CLEAR(list)     ((list)->size=0)
        #define LIST_ADD(list,mv)    ((list)->move[(list)->size++]=(mv))

        #define LIST_IS_EMPTY(list)  ((list)->size==0)
        #define LIST_SIZE(list)      ((list)->size)

        #define LIST_MOVE(list,pos)  ((list)->move[pos])
        #define LIST_VALUE(list,pos) ((list)->value[pos])
    };
};

#endif /*ALINIX_KERNEL_APPS_CHESS_GAME_H*/