# PHILOCHESS
<p align="center">
  <img src="assets/philochess-logo.png" alt="PHILOCHESS logo" width="420">
</p>

*A personal C++ chess engine project growing from basic engine foundations toward a classical understanding of chess.*

PHILOCHESS was born from a technical curiosity about how a chess engine generates moves, but also from a personal desire to express a chess philosophy through code. The project is being built step by step, from board representation and move generation toward an engine guided by a classical understanding of chess.

I am not a professional programmer. I am a chemical engineer, a chess coach, and a chess analyst, as well as a lifelong student of this beautiful art of the sixty-four squares. For that reason, PHILOCHESS is also a learning journey: an attempt to translate chess logic, training ideas, and strategic concepts into a working engine. Its long-term ambition is to evaluate chess through factors such as development, mobility, activity, coordination, structure, material, king safety, and initiative.

PHILOCHESS starts as a simple engine, but it is intended to grow into a tool that reflects a human way of understanding chess and, eventually, may become useful for students, trainers, and chess lovers.

## About this repository

This public repository documents the evolution of PHILOCHESS through complete development logs, structured test inventories, validation games and analyses, and selected code samples.

The engine’s full source code is maintained in a private repository. The samples published here are adapted specifically for educational and documentation purposes.

## Repository contents

- [`development-logs/`](development-logs/) — Versioned project logs and documentation
- [`games/`](games/) — External tests, validation tournaments, and game analyses
- [`test-inventories/`](test-inventories/) — Structured test inventories for implemented engine features
- [`code-samples/`](code-samples/) — Selected educational code samples from completed project phases
- [`assets/`](assets/) — PHILOCHESS visual resources

The published C++ samples are self-contained and can be compiled independently with a C++17-compatible compiler.

## Current status

The latest version, PHILOCHESS 0.2-alpha-009, introduces stalemate recognition throughout the search. The engine now identifies positions in which the side to move is not in check but has no legal moves and consistently evaluates them as draws at the root, in alpha-beta search, and in quiescence search. The implementation was validated against a stalemate test inventory and the complete regression suite. Development will continue with PHILOCHESS 0.2-alpha-010: Dead-Position Recognition in Search.

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
| 0.1-alpha-029 | GUI compatibility and validation in the first GUI game |
| 0.1-alpha-030 | Tactical regression test derived from the first GUI game |
| 0.1-alpha-031 | Basic move ordering for alpha-beta search |
| 0.1-alpha-032 | Final 0.1 validation and closure |

## PHILOCHESS 0.2 development milestones

| Version | Milestone |
|---|---|
| 0.2-alpha-001 | Basic quiescence search and refined MVV-LVA capture ordering |
| 0.2-alpha-002 | Tactical regression testing and quiescence validation following the first tournament |
| 0.2-alpha-003 | Iterative deepening with preservation of the last completed iteration |
| 0.2-alpha-004 | Principal variation tracking and iterative UCI reporting |
| 0.2-alpha-005 | Improved move ordering with PV-move priority and MVV-LVA |
| 0.2-alpha-006 | Time-controlled iterative deepening with soft and hard time limits |
| 0.2-alpha-007 | Piece-square tables with phase-dependent king evaluation |
| 0.2-alpha-008 | Basic knight, bishop, rook and queen mobility evaluation |
| 0.2-alpha-009 | Stalemate recognition throughout the search |

## Acknowledgments

My sincere gratitude goes to the developers of chess engines, graphical interfaces, and other tools for computer-chess research, as well as to those who test chess engines, produce and maintain rating lists, write technical documentation, and preserve, organize, and make valuable resources and historical versions available to the community. Much of this work is carried out quietly and receives far less recognition than it deserves. Their contributions have made it possible to test PHILOCHESS against other engines, measure its progress, identify and correct its weaknesses, and guide its continued development through the accumulated knowledge and experience of the computer-chess community.

I am equally grateful to my students and their families. Their trust, commitment, and expectations continually inspire me to grow as a chess coach, refine my methods, prepare better lessons, and find clearer and more effective ways to share chess knowledge. What I have learned through this shared journey has shaped the care, rigor, and educational perspective with which PHILOCHESS is conceived and developed.

## Licensing

Different parts of this repository are distributed under different terms.
See [`LICENSE.md`](LICENSE.md) for the complete licensing structure.

## Author

Robert M. Cuadros
