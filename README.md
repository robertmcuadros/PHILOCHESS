# PHILOCHESS
<p align="center">
  <img src="assets/philochess-logo.png" alt="PHILOCHESS logo" width="420">
</p>

*A personal C++ chess engine project evolving from its foundations toward a classical understanding of chess.*

PHILOCHESS was born from a technical curiosity about how a chess engine generates moves, but also from a desire to express a chess philosophy through code. The project is being developed step by step, with the aim of building an engine guided by classical chess principles.

I am not a professional programmer. I am a chemical engineer, a chess coach, and a chess analyst, as well as a lifelong student of this beautiful art of the sixty-four squares. For me, PHILOCHESS is also a learning journey, an effort to translate chess logic, training ideas, and strategic concepts into a working engine. My goal is to evaluate chess positions through factors such as development, mobility, activity, coordination, structure, material, king safety, and initiative.

PHILOCHESS began as a simple engine, but I hope to develop it into a tool that reflects a human approach to understanding chess and, in time, becomes useful to students, coaches, and chess enthusiasts.

## About this repository

This public repository documents the evolution of PHILOCHESS through detailed development logs, structured test inventories, validation games, and selected code samples.

The complete source code is maintained in a private repository. The examples shared here have been adapted for educational and documentation purposes.

## Repository contents

- [`development-logs/`](development-logs/) — Versioned records of the project's progress
- [`games/`](games/) — External tests and validation tournaments
- [`test-inventories/`](test-inventories/) — Structured tests covering implemented engine features
- [`code-samples/`](code-samples/) — Educational code examples from completed development phases
- [`assets/`](assets/) — Visual resources for PHILOCHESS

The published C++ examples are self-contained and can be compiled independently using a C++17-compatible compiler.

## Current status

PHILOCHESS 0.3-alpha-002 completed the Engine Modularization and Interface Separation milestone. The original monolithic codebase has been reorganized into eight functional modules covering core infrastructure, chess logic, evaluation, rules, search, UCI communication, diagnostics, and testing.

The new architecture preserves the functionality established in alpha-001, including search decisions, evaluation, time controls, draw-rule handling, UCI contracts, asynchronous search control, and consistent behavior across Release and Debug builds.

Alpha-002 focused on improving the engine's internal organization rather than its playing strength. This work provides a more maintainable foundation for the remaining PHILOCHESS 0.3 milestones.

Development will continue with PHILOCHESS 0.3-alpha-003, Reproducible Benchmarking, Profiling, and Statistical Match Analysis.

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
| 0.2-alpha-010 | Conservative dead-position recognition throughout the search |
| 0.2-alpha-011 | Fifty-move rule recognition throughout the search |
| 0.2-alpha-012 | Threefold-repetition detection throughout the search |
| 0.2-alpha-013 | Unified draw-rule integration, cross-rule regression, and final 0.2 validation and closure |

## PHILOCHESS 0.3 development milestones

| Version | Milestone |
|---|---|
| 0.3-alpha-001 | Concurrent UCI infinite-analysis implementation, stop control, and iterative search reporting |
| 0.3-alpha-002 | Engine modularization, component separation, and explicit interface definition |

## Acknowledgments

I am grateful to the developers of chess engines, graphical interfaces, and other tools used in computer-chess research. I also wish to thank those who test engines, maintain rating lists, write technical documentation, and preserve historical versions and resources for the community. Much of their work goes unnoticed, despite its importance. Thanks to their efforts, I have been able to test PHILOCHESS against other engines, measure its progress, identify weaknesses, and continue learning from the knowledge and experience shared by the computer-chess community.

I am equally grateful to my students and their families. Their trust and commitment encourage me to keep improving as a chess coach, prepare better lessons, and find clearer ways to teach chess. What I have learned through these years of teaching has also influenced the way I think about chess and approach the development of PHILOCHESS.

## Licensing

Different parts of this repository are distributed under different terms.
See [`LICENSE.md`](LICENSE.md) for the complete licensing structure.

## Author

Robert M. Cuadros
