# PHILOCHESS
<p align="center">
  <img src="assets/philochess-logo.png" alt="PHILOCHESS logo" width="420">
</p>

*A personal C++ chess engine project growing from basic engine foundations toward a classical understanding of chess.*

PHILOCHESS was born from a technical curiosity about how a chess engine generates moves, but also from a personal desire to express a chess philosophy through code. The project is being built step by step, from board representation and move generation toward an engine guided by a classical understanding of chess.

I am not a professional programmer. I am a chemical engineer, a chess coach, and a chess analyst, as well as a lifelong student of this beautiful art of the sixty-four squares. For that reason, PHILOCHESS is also a learning journey: an attempt to translate chess logic, training ideas, and strategic concepts into a working engine. Its long-term ambition is to evaluate chess through factors such as development, mobility, activity, coordination, structure, material, king safety, and initiative.

PHILOCHESS starts as a simple engine, but it is intended to grow into a tool that reflects a human way of understanding chess and, eventually, may become useful for students, trainers, and chess lovers.

## About this repository

This public repository presents the evolution of PHILOCHESS through complete development logs, validation games, game analyses, and selected educational code samples.

The complete engine source code is maintained in a private development repository. Only selected code samples, adapted for educational and documentation purposes, are published here.

## Repository contents

- [`docs/`](docs/) — Development logs and project documentation
- [`games/`](games/) — External tests, validation tournaments and game analyses
- [`code-samples/`](code-samples/) — Selected educational code samples from completed phases
- [`assets/`](assets/) — PHILOCHESS visual resources

The published C++ samples are self-contained and can be compiled independently with a C++17-compatible compiler.

## Current status

The latest version, PHILOCHESS 0.2-alpha-006, passed its complete console regression suite and practical GUI validation. It converts larger time budgets into greater completed search depths, preserves the last fully completed iteration when a search is interrupted, and maintains legal and stable UCI operation without time losses or crashes. Its static evaluation remains material-only. Development will now continue with piece-square tables, basic mobility evaluation, basic pawn structure, basic king safety, a consolidated search and evaluation regression suite, and external match validation leading to the closure of PHILOCHESS 0.2.

## PHILOCHESS 0.1 development milestones

| Version | Milestone |
|---|---|
| 0.1-alpha-001–002 | Minimal UCI interface and first provisional bestmove |
| 0.1-alpha-003–005 | Board representation, square conversion and simple move application |
| 0.1-alpha-006–007 | Basic pawn and knight move generation, internal move representation and UCI parsing |
| 0.1-alpha-008–011 | Bishop, rook, queen and king move generation |
| 0.1-alpha-012 | Pseudo-legal move generation complete |
| 0.1-alpha-013–021 | Legal move validation, game status, FEN loading, special moves, perft and divide |
| 0.1-alpha-022–024 | Extended perft validation suite and material evaluation |
| 0.1-alpha-025–028 | Search preparation, basic negamax, expanded UCI integration and basic alpha-beta pruning |
| 0.1-alpha-029 | GUI compatibility and first external game validation |
| 0.1-alpha-030 | Tactical regression test from first external game |
| 0.1-alpha-031 | Basic move ordering for alpha-beta search |
| 0.1-alpha-032 | Final 0.1 validation and closure |

## PHILOCHESS 0.2 development milestones

| Version | Milestone |
|---|---|
| 0.2-alpha-001 | Basic quiescence search and refined MVV-LVA capture ordering |
| 0.2-alpha-002 | Tactical regression and quiescence correctness |
| 0.2-alpha-003 | Iterative deepening with preservation of the last completed iteration |
| 0.2-alpha-004 | Principal variation tracking and iterative UCI reporting |
| 0.2-alpha-005 | Improved move ordering with PV-move priority and MVV-LVA |
| 0.2-alpha-006 | Time-controlled iterative deepening with soft and hard time limits |

## Licensing

Different parts of this repository are distributed under different terms.
See [`LICENSE.md`](LICENSE.md) for the complete licensing structure.

## Author

Robert M. Cuadros
