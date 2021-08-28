//
// Created by gvisan on 13.08.2021.
//

#ifndef CHESSENGINE_TTABLE_H
#define CHESSENGINE_TTABLE_H

#include"defines.h"
#include"move.h"


namespace engine {

    enum TTEntryType : u8 {
        TT_NULL,
        TT_EXACT,
        TT_UPPERBOUND,
        TT_LOWERBOUND,
    };

    struct TTEntry {
        /*We hold the zobrist key to check if the entry is correct.*/
        u64 zobrist_key;

        /*The type of the entry.*/
        TTEntryType type;

        /*The best move in the position.*/
        Move move;

        /*The evaluation of the position.*/
        int score;

        /*How deep we have evaluated the position.*/
        u8 depth;
    };

    const int C_DEFAULT_HASH_SIZE = 128;
    const int C_MINIMUM_HASH_SIZE = 1;
    const int C_MAXIMUM_HASH_SIZE = 4096;

    /*This class is a big hash table, which holds information that can be reused when
     *we encounter a position that is already in the table.*/
    class TranspositionTable {
    private:
        /*How many entries are reserved in the table.*/
        unsigned int size;
        /*The array of entries.*/
        TTEntry *table;

        /*Returns an index in the table by applying modulo the number of entries.*/
        unsigned hash(u64 key) const;

    public:
        explicit TranspositionTable(unsigned int megabytes);

        /*Reserves enough entries to satisfy the given memory limit. This operation clears the table.*/
        void reserve(unsigned int megabytes);

        /*Clears the table. This operation does not free the allocated memory, just sets all entry types to null.*/
        void clear();

        /*Release the allocated memory.*/
        ~TranspositionTable();

        /*Returns the entry count.*/
        unsigned int get_size() const;

        /*Probes the table for the given key. If no entry with the given key is found, return nullptr.*/
        TTEntry *probe(u64 key);

        /*Saves a new entry in the table if the replacement scheme allows it.*/
        /*We only replace entries that have a lower evaluation depth that the given entry.*/
        void save(const TTEntry &entry);
    };

    extern TranspositionTable transposition_table;
}


#endif //CHESSENGINE_TTABLE_H
