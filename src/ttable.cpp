//
// Created by gvisan on 13.08.2021.
//

#include "ttable.h"

namespace engine {
    TranspositionTable transposition_table(C_DEFAULT_HASH_SIZE);

    TranspositionTable::TranspositionTable(unsigned megabytes) : size(0), table(nullptr) {
        reserve(megabytes);
    }

    void TranspositionTable::reserve(unsigned int megabytes) {
        delete[]table;
        size = (megabytes * 1000000) / sizeof(TTEntry);
        table = new TTEntry[size];
        clear();
    }

    void TranspositionTable::clear() {
        for (unsigned index = 0; index < size; index++) {
            table[index].type = TT_NULL;
        }
    }

    unsigned int TranspositionTable::hash(u64 key) const {
        return key % size;
    }

    unsigned int TranspositionTable::get_size() const {
        return size;
    }

    TTEntry *TranspositionTable::probe(u64 key) {
        unsigned index = hash(key);
        TTEntry *entry = &table[index];
        if (entry->type == TT_NULL || entry->zobrist_key != key)
            entry = nullptr;
        return entry;
    }

    void TranspositionTable::save(const TTEntry &entry) {
        unsigned index = hash(entry.zobrist_key);
        if (table[index].type == TT_NULL || table[index].depth <= entry.depth) {
            table[index] = entry;
        }
    }


    TranspositionTable::~TranspositionTable() {
        delete[]table;
    }

}