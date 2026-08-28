/*
 * PHILOCHESS 0.2 — Iterative deepening sample
 *
 * Educational sample derived from PHILOCHESS 0.2-alpha-003.
 * It demonstrates:
 *   - searching successively at depths 1, 2, 3, ...;
 *   - committing a result only after an iteration is complete;
 *   - carrying the previous iteration's best move to the front of the
 *     next root search;
 *   - using a small alpha-beta/negamax search underneath the iteration loop.
 *
 * The game tree below is deliberately handcrafted. Its purpose is to make
 * the iterative-deepening mechanism visible without publishing PHILOCHESS's
 * board representation, move generator, quiescence search, UCI layer, test
 * inventory, or full search framework.
 *
 * This file is not the complete PHILOCHESS source code and may differ
 * from the current private implementation.
 *
 * Copyright (c) 2026 Robert M. Cuadros. All rights reserved.
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

constexpr int INF_SCORE = 1000000000;

// Every position in this sample is a node in a tiny handcrafted game tree.
// Scores are always expressed from the point of view of the side to move.
enum class PositionId
{
    Root,

    AfterNf3,
    AfterE4,
    AfterD4,

    Nf3_D5,
    Nf3_Nf6,
    E4_C5,
    E4_E5,
    D4_Nf6,
    D4_D5,

    Nf3_D5_C4,
    Nf3_D5_E3,
    Nf3_Nf6_C4,
    Nf3_Nf6_E3,

    E4_C5_Nf3,
    E4_C5_D5,
    E4_E5_Nf3,
    E4_E5_Bc4,

    D4_Nf6_C4,
    D4_Nf6_Nf3,
    D4_D5_C4,
    D4_D5_Nf3
};

struct Move
{
    std::string name;
    PositionId next;
};

struct IterationResult
{
    bool complete = false;
    std::string bestMove;
    int score = -INF_SCORE;
    std::uint64_t nodes = 0;
};

struct SearchResult
{
    std::string bestMove;
    int score = 0;
    int completedDepth = 0;
    std::uint64_t totalNodes = 0;
};

std::vector<Move> generateMoves(PositionId position)
{
    switch (position)
    {
    case PositionId::Root:
        return {
            {"Nf3", PositionId::AfterNf3},
            {"e4",  PositionId::AfterE4},
            {"d4",  PositionId::AfterD4}
        };

    case PositionId::AfterNf3:
        return {
            {"...d5",  PositionId::Nf3_D5},
            {"...Nf6", PositionId::Nf3_Nf6}
        };

    case PositionId::AfterE4:
        return {
            {"...c5", PositionId::E4_C5},
            {"...e5", PositionId::E4_E5}
        };

    case PositionId::AfterD4:
        return {
            {"...Nf6", PositionId::D4_Nf6},
            {"...d5",  PositionId::D4_D5}
        };

    case PositionId::Nf3_D5:
        return {
            {"c4", PositionId::Nf3_D5_C4},
            {"e3", PositionId::Nf3_D5_E3}
        };

    case PositionId::Nf3_Nf6:
        return {
            {"c4", PositionId::Nf3_Nf6_C4},
            {"e3", PositionId::Nf3_Nf6_E3}
        };

    case PositionId::E4_C5:
        return {
            {"Nf3", PositionId::E4_C5_Nf3},
            {"d5",  PositionId::E4_C5_D5}
        };

    case PositionId::E4_E5:
        return {
            {"Nf3", PositionId::E4_E5_Nf3},
            {"Bc4", PositionId::E4_E5_Bc4}
        };

    case PositionId::D4_Nf6:
        return {
            {"c4",  PositionId::D4_Nf6_C4},
            {"Nf3", PositionId::D4_Nf6_Nf3}
        };

    case PositionId::D4_D5:
        return {
            {"c4",  PositionId::D4_D5_C4},
            {"Nf3", PositionId::D4_D5_Nf3}
        };

    default:
        return {};
    }
}

int staticEvaluation(PositionId position)
{
    // Depth-1 frontier: e4 looks best at first.
    switch (position)
    {
    case PositionId::AfterNf3: return -20;
    case PositionId::AfterE4:  return -40;
    case PositionId::AfterD4:  return -30;

    // Depth-2 frontier: Black has a strong reply to e4, while d4 remains sound.
    case PositionId::Nf3_D5:  return 15;
    case PositionId::Nf3_Nf6: return 10;
    case PositionId::E4_C5:    return -60;
    case PositionId::E4_E5:    return 25;
    case PositionId::D4_Nf6:   return 35;
    case PositionId::D4_D5:    return 30;

    // Depth-3 frontier: Nf3 reveals the strongest continuation.
    case PositionId::Nf3_D5_C4:  return -70;
    case PositionId::Nf3_D5_E3:  return -25;
    case PositionId::Nf3_Nf6_C4: return -60;
    case PositionId::Nf3_Nf6_E3: return -20;

    case PositionId::E4_C5_Nf3: return 10;
    case PositionId::E4_C5_D5:  return 20;
    case PositionId::E4_E5_Nf3: return -30;
    case PositionId::E4_E5_Bc4: return -20;

    case PositionId::D4_Nf6_C4:  return -45;
    case PositionId::D4_Nf6_Nf3: return -35;
    case PositionId::D4_D5_C4:   return -40;
    case PositionId::D4_D5_Nf3:  return -30;

    case PositionId::Root:
        return 0;
    }

    return 0;
}

int negamax(PositionId position, int depth, int alpha, int beta,
            std::uint64_t& nodes)
{
    ++nodes;

    const std::vector<Move> moves = generateMoves(position);
    if (depth == 0 || moves.empty())
    {
        return staticEvaluation(position);
    }

    int bestScore = -INF_SCORE;

    for (const Move& move : moves)
    {
        const int score = -negamax(move.next, depth - 1, -beta, -alpha, nodes);
        bestScore = std::max(bestScore, score);
        alpha = std::max(alpha, score);

        if (alpha >= beta)
        {
            break;
        }
    }

    return bestScore;
}

void movePreviousBestFirst(std::vector<Move>& rootMoves,
                           const std::string& previousBestMove)
{
    if (previousBestMove.empty())
    {
        return;
    }

    const auto preferred = std::find_if(
        rootMoves.begin(), rootMoves.end(),
        [&](const Move& move) { return move.name == previousBestMove; });

    if (preferred != rootMoves.end())
    {
        std::iter_swap(rootMoves.begin(), preferred);
    }
}

void printRootOrder(const std::vector<Move>& rootMoves)
{
    std::cout << "Root order:";
    for (const Move& move : rootMoves)
    {
        std::cout << ' ' << move.name;
    }
    std::cout << '\n';
}

IterationResult searchIteration(int depth, const std::string& previousBestMove)
{
    std::vector<Move> rootMoves = generateMoves(PositionId::Root);

    // The best move from the previous completed iteration is searched first.
    movePreviousBestFirst(rootMoves, previousBestMove);
    printRootOrder(rootMoves);

    IterationResult result;
    result.complete = true;

    int alpha = -INF_SCORE;
    const int beta = INF_SCORE;

    for (const Move& move : rootMoves)
    {
        std::uint64_t moveNodes = 0;
        const int score = -negamax(move.next, depth - 1,
                                   -beta, -alpha, moveNodes);

        result.nodes += moveNodes;
        std::cout << "  " << move.name << ": " << score
                  << " cp  (" << moveNodes << " nodes)\n";

        if (score > result.score)
        {
            result.score = score;
            result.bestMove = move.name;
        }

        alpha = std::max(alpha, score);
    }

    return result;
}

SearchResult iterativeDeepening(int requestedDepth)
{
    SearchResult result;
    std::string previousBestMove;

    for (int currentDepth = 1; currentDepth <= requestedDepth; ++currentDepth)
    {
        std::cout << "\nDepth " << currentDepth << "\n";

        const IterationResult iteration =
            searchIteration(currentDepth, previousBestMove);

        result.totalNodes += iteration.nodes;

        // A result becomes official only after the whole iteration finishes.
        // If a real engine is interrupted during a later iteration, it can
        // still return the best move from the last completed depth.
        if (!iteration.complete)
        {
            break;
        }

        result.bestMove = iteration.bestMove;
        result.score = iteration.score;
        result.completedDepth = currentDepth;
        previousBestMove = iteration.bestMove;

        std::cout << "Completed depth " << currentDepth
                  << ": best move " << result.bestMove
                  << ", score " << result.score << " cp\n";
    }

    return result;
}

int main()
{
    std::cout << "PHILOCHESS 0.2 — Iterative deepening sample\n";
    std::cout << "The same root position is searched repeatedly at increasing depths.\n";
    std::cout << "The previous completed iteration's best move is tried first next time.\n";

    const SearchResult result = iterativeDeepening(3);

    std::cout << "\nFinal result\n";
    std::cout << "Completed depth: " << result.completedDepth << '\n';
    std::cout << "Best move: " << result.bestMove << '\n';
    std::cout << "Score: " << result.score << " cp\n";
    std::cout << "Total searched nodes across all iterations: "
              << result.totalNodes << '\n';

    std::cout << "\nThe preferred move changes as more of the tree becomes visible:\n";
    std::cout << "depth 1 -> e4, depth 2 -> d4, depth 3 -> Nf3.\n";

    return 0;
}
