// PHILOCHESS 0.2 code sample
// PV move and MVV-LVA move ordering
// Derived from the development of PHILOCHESS 0.2-alpha-005
// Author: Robert M. Cuadros
//
// This is a deliberately reduced educational sample.
// It demonstrates the ordering idea only; it is not the complete
// PHILOCHESS move-ordering or search implementation.
//
// C++17, self-contained and independently compilable.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class Piece {
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

struct Move {
    string name;          // Human-readable move label for the demonstration.
    Piece attacker;
    Piece victim;         // None means a quiet move.
};

bool sameMove(const Move& a, const Move& b) {
    return a.name == b.name;
}

// MVV-LVA does not need the engine's full evaluation values.
// A compact rank is enough:
//   more valuable victim  -> searched earlier
//   cheaper attacker      -> preferred when victims are equal
int orderingRank(Piece p) {
    switch (p) {
    case Piece::Pawn:   return 1;
    case Piece::Knight: return 2;
    case Piece::Bishop: return 3;
    case Piece::Rook:   return 4;
    case Piece::Queen:  return 5;
    case Piece::King:   return 6;
    default:            return 0;
    }
}

bool isCapture(const Move& m) {
    return m.victim != Piece::None;
}

int mvvLvaScore(const Move& m) {
    if (!isCapture(m)) return 0;

    const int victimRank = orderingRank(m.victim);
    const int attackerRank = orderingRank(m.attacker);

    // Victim value dominates; attacker value breaks ties.
    return victimRank * 100 - attackerRank;
}

void orderMoves(vector<Move>& moves, const Move* pvMove = nullptr) {
    stable_sort(moves.begin(), moves.end(),
        [&](const Move& a, const Move& b) {
            const bool aIsPV = pvMove && sameMove(a, *pvMove);
            const bool bIsPV = pvMove && sameMove(b, *pvMove);

            // The PV move from the previous completed iteration has
            // absolute priority at this node.
            if (aIsPV != bIsPV)
                return aIsPV;

            // Then captures are ordered by MVV-LVA.
            const int aScore = mvvLvaScore(a);
            const int bScore = mvvLvaScore(b);

            if (aScore != bScore)
                return aScore > bScore;

            // stable_sort preserves the original order on ties.
            return false;
        });
}

void printMoves(const vector<Move>& moves, const Move* pvMove = nullptr) {
    for (const Move& m : moves) {
        cout << "  " << m.name;

        if (pvMove && sameMove(m, *pvMove))
            cout << "  [PV move]";

        if (isCapture(m))
            cout << "  MVV-LVA=" << mvvLvaScore(m);
        else
            cout << "  quiet";

        cout << '\n';
    }
}

int main() {
    // A small artificial move list is enough to demonstrate the concept.
    //
    // The exact board position is intentionally omitted.  A real chess
    // engine must generate legal moves, identify captured pieces and carry
    // a principal variation from one completed iteration to the next.

    vector<Move> moves = {
        {"a1b1", Piece::King,   Piece::None},   // quiet
        {"d5c7", Piece::Knight, Piece::Queen},  // N x Q
        {"e5f6", Piece::Pawn,   Piece::Rook},   // P x R
        {"d5f6", Piece::Knight, Piece::Rook},   // N x R
        {"d5b4", Piece::Knight, Piece::Bishop}, // N x B
        {"h2h3", Piece::Pawn,   Piece::None}    // quiet
    };

    cout << "PHILOCHESS 0.2 sample: PV move and MVV-LVA ordering\n\n";

    cout << "1. Ordering without a PV hint\n";
    vector<Move> ordinary = moves;
    orderMoves(ordinary);
    printMoves(ordinary);

    cout << "\n2. Ordering with a PV move from the previous iteration\n";
    const Move previousPVMove = moves.front(); // a1b1
    vector<Move> withPV = moves;
    orderMoves(withPV, &previousPVMove);
    printMoves(withPV, &previousPVMove);

    cout << "\nKey idea:\n";
    cout << "  - The previous PV move is searched first.\n";
    cout << "  - The remaining captures follow MVV-LVA.\n";
    cout << "  - Quiet moves remain behind tactical captures.\n";

    return 0;
}
