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

## Testing the chess engine

This engine implements the UCI protocol. You can test it with Arena Chess: https://playwitharena.de/

1. Build the engine (as shown above: `./src/Fortuna` on Linux/Mac or `src\Fortuna.exe` on Windows).
2. In Arena: Engines → Install New Engine...; select the executable and choose the UCI protocol.
3. Play: Game → Play → Human vs Engine or use Tournament to run automated matches and export PGN.
  
