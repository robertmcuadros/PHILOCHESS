/*
 * PHILOCHESS 0.2 — Basic quiescence search sample
 *
 * Educational sample derived from PHILOCHESS 0.2-alpha-001.
 * It demonstrates:
 *   - replacing a static frontier evaluation with quiescence search;
 *   - stand-pat evaluation when the side to move is not in check;
 *   - continuation through captures and promotions;
 *   - searching every legal evasion when the side to move is in check.
 *
 * The small position graph below is deliberately handcrafted so that the
 * quiescence algorithm can be studied without publishing PHILOCHESS's full
 * board representation, move generator, search framework, or UCI layer.
 *
 * This file is not the complete PHILOCHESS source code and may differ
 * from the current private implementation.
 *
 * Copyright (c) 2026 Robert M. Cuadros. All rights reserved.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

constexpr int INF_SCORE = 1000000000;

enum class PositionId
{
    Root,
    AfterSafeMove,
    AfterQxd8,
    AfterKxd8
};

struct Position
{
    PositionId id = PositionId::Root;
};

struct Move
{
    std::string name;
    PositionId next;
    bool capture = false;
    bool promotion = false;
};

// Scores are expressed from the point of view of the side to move.
//
// The handcrafted example represents the essential horizon problem from
// PHILOCHESS 0.2-alpha-001:
//
//   White has Q vs R, so a safe continuation preserves about +400 cp.
//   Qxd8+ appears to win the rook and looks like +900 cp at a shallow frontier,
//   but Black is in check and can answer Kxd8, after which the material is equal.
int staticEvaluation(const Position& position)
{
    switch (position.id)
    {
    case PositionId::Root:
        return 400;   // White to move: Q vs R.
    case PositionId::AfterSafeMove:
        return -400;  // Black to move: White still has Q vs R.
    case PositionId::AfterQxd8:
        return -900;  // Black to move: White queen appears to have won the rook.
    case PositionId::AfterKxd8:
        return 0;     // White to move: queen and rook are both gone.
    }

    return 0;
}

bool inCheck(const Position& position)
{
    return position.id == PositionId::AfterQxd8;
}

std::vector<Move> generateLegalMoves(const Position& position)
{
    switch (position.id)
    {
    case PositionId::Root:
        return {
            {"Qe2",  PositionId::AfterSafeMove, false, false},
            {"Qxd8+", PositionId::AfterQxd8,    true,  false}
        };

    case PositionId::AfterQxd8:
        // Because Black is in check, this quiet king recapture is a legal
        // evasion and must be searched even though quiescence normally limits
        // itself to tactical continuations.
        return {
            {"Kxd8", PositionId::AfterKxd8, true, false}
        };

    case PositionId::AfterSafeMove:
    case PositionId::AfterKxd8:
        return {};
    }

    return {};
}

Position makeMove(const Move& move)
{
    return Position{move.next};
}

bool isQuiescenceMove(const Move& move)
{
    return move.capture || move.promotion;
}

int quiescence(const Position& position, int alpha, int beta)
{
    const std::vector<Move> legalMoves = generateLegalMoves(position);
    const bool sideInCheck = inCheck(position);

    if (!sideInCheck)
    {
        // Stand pat: if the current static position is already good enough to
        // fail high, there is no need to examine further tactical moves.
        const int standPat = staticEvaluation(position);

        if (standPat >= beta)
        {
            return beta;
        }

        alpha = std::max(alpha, standPat);
    }

    for (const Move& move : legalMoves)
    {
        // Outside check, this basic quiescence version follows only captures
        // and promotions. When in check, every legal evasion must be searched.
        if (!sideInCheck && !isQuiescenceMove(move))
        {
            continue;
        }

        const Position child = makeMove(move);
        const int score = -quiescence(child, -beta, -alpha);

        if (score >= beta)
        {
            return beta;
        }

        alpha = std::max(alpha, score);
    }

    return alpha;
}

struct SearchChoice
{
    std::string move;
    int score = -INF_SCORE;
};

SearchChoice searchOnePly(bool useQuiescence)
{
    const Position root{PositionId::Root};
    SearchChoice best;

    for (const Move& move : generateLegalMoves(root))
    {
        const Position child = makeMove(move);

        const int score = useQuiescence
            ? -quiescence(child, -INF_SCORE, INF_SCORE)
            : -staticEvaluation(child);

        std::cout << "  " << move.name << ": " << score << " cp\n";

        if (score > best.score)
        {
            best.move = move.name;
            best.score = score;
        }
    }

    return best;
}

int main()
{
    std::cout << "PHILOCHESS 0.2 — Basic quiescence search sample\n\n";

    std::cout << "One-ply search with static frontier evaluation:\n";
    const SearchChoice staticChoice = searchOnePly(false);
    std::cout << "Best move: " << staticChoice.move
              << " (" << staticChoice.score << " cp)\n\n";

    std::cout << "One-ply search with quiescence at the frontier:\n";
    const SearchChoice quiescentChoice = searchOnePly(true);
    std::cout << "Best move: " << quiescentChoice.move
              << " (" << quiescentChoice.score << " cp)\n\n";

    std::cout << "The shallow static search prefers Qxd8+ because it stops before Kxd8.\n";
    std::cout << "Quiescence searches the forced recapture and preserves the safer Qe2.\n";

    return 0;
}
