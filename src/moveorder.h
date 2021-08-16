//
// Created by gvisan on 30.07.2021.
//

#ifndef CHESSENGINE_MOVEORDER_H
#define CHESSENGINE_MOVEORDER_H

#include"board.h"
#include"move.h"
#include"ttable.h"
#include<vector>
#include<algorithm>

namespace engine {
    /*This class is responsible for ordering moves using simple heuristics like mvv-lva.*/
    const int C_HASH_MOVE_BONUS = 6000;
    const int C_PROMOTION_BONUS = 3000;
    const int C_CAPTURE_BONUS = 1000;
    const int C_FIRST_KILLER_BONUS = 800;
    const int C_SECOND_KILLER_BONUS = 400;

    class MoveOrder {
    private:
        /*The internal board.*/
        Board &board;

        /*MVV-LVA table.*/
        int mvv_lva[5][6];

        /*Returns a heuristic score of the move, higher is better.*/
        int score_move(const Move &move, TTEntry *entry, int ply);

        /*First killer move.*/
        Move first_killer[C_MAX_SEARCH_DEPTH];

        /*Second killer move.*/
        Move second_killer[C_MAX_SEARCH_DEPTH];

    public:
        explicit MoveOrder(Board &internal_board);

        /*Orders moves by their heuristic score.*/
        void order_moves(std::vector<Move> &moves, TTEntry *entry, int ply);

        /*Sets a new killer, when a quiet move causes a cutoff.*/
        void set_killer(const Move &move, int ply);

        /*Clears the killer tables, usually between searches.*/
        void clear_killers();
    };

    /*Move comparator, used to sort moves.
     * If move a has a better score, it will appear before b in the move list.*/
    inline bool compare_moves(const Move &a, const Move &b) {
        return a.score > b.score;
    }

}


#endif //CHESSENGINE_MOVEORDER_H
