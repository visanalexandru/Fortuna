# Fortuna Chess Engine
This is a UCI compatible chess engine writen in C++14.

## Move generation
  - Bitboard-based move generation
  - Fast generation of sliding piece attacks using magic bitboards
  - Move legality checking using pinned piece generation
## Transposition table
  - Zobrist hashing
  - Depth-prefered replacement scheme
## Search
  - Alpha-Beta/Negamax
  - Iterative deepening
  - Quiescence search
  - Check extensions
  - Late move reductions
## Move Ordering
  - Static exchange evaluation (SEE)
  - Killer heuristic
  - MVV-LVA
  
## Evaluation
  - Material
  - Piece-square tables
  - King safety
  - Passed pawns
  - Doubled pawns
  - Piece mobility
   
### To compile and run the engine:
```
git clone https://github.com/visanalexandru/Fortuna.git 
cd Fortuna/
cmake .
make
./src/Fortuna
```
  
