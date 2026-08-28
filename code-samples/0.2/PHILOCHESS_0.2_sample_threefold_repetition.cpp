// PHILOCHESS 0.2 code sample
// Threefold repetition detection
// Derived from the development of PHILOCHESS 0.2-alpha-012
// Author: Robert M. Cuadros
//
// This is a deliberately reduced educational sample.
// It demonstrates:
//   1. which state fields define repetition identity;
//   2. why move counters are excluded;
//   3. why an en-passant square matters only when an en-passant capture
//      is actually legal;
//   4. how game history and the current search path can be counted together;
//   5. how push/pop keeps sibling search branches isolated.
//
// It does not reproduce PHILOCHESS move generation, UCI reconstruction,
// alpha-beta search, quiescence search, or the complete draw-rule system.
//
// C++17, self-contained and independently compilable.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

constexpr int THREEFOLD_REPETITION_COUNT = 3;

struct PositionState {
    // FEN-like piece-placement field.  This sample treats it as an opaque
    // description; a real engine would derive it from its board structure.
    string piecePlacement;

    char sideToMove = 'w';

    // Canonical form such as "KQkq", "Kq", or "-".
    string castlingRights = "-";

    // Raw en-passant target from the position description.
    string enPassantSquare = "-";

    // In a real engine this must be established from legal move generation.
    // A merely geometric or nominal en-passant target is not sufficient.
    bool legalEnPassantCaptureExists = false;

    // These counters are intentionally NOT part of repetition identity.
    int halfmoveClock = 0;
    int fullmoveNumber = 1;
};

string effectiveEnPassant(const PositionState& pos) {
    if (pos.enPassantSquare == "-")
        return "-";

    return pos.legalEnPassantCaptureExists
        ? pos.enPassantSquare
        : "-";
}

string repetitionKey(const PositionState& pos) {
    // Repetition identity uses:
    //   piece placement
    //   side to move
    //   castling rights
    //   effective en-passant availability
    //
    // The halfmove and fullmove counters are deliberately omitted.
    return pos.piecePlacement
        + "|" + string(1, pos.sideToMove)
        + "|" + pos.castlingRights
        + "|" + effectiveEnPassant(pos);
}

int countKey(const string& key, const vector<string>& keys) {
    return static_cast<int>(
        count(keys.begin(), keys.end(), key)
    );
}

struct RepetitionContext {
    // Positions that already occurred in the actual game.
    vector<string> gameHistory;

    // Positions added only along the branch currently being searched.
    vector<string> searchPath;
};

int occurrenceCount(
    const PositionState& pos,
    const RepetitionContext& ctx
) {
    const string key = repetitionKey(pos);

    return countKey(key, ctx.gameHistory)
        + countKey(key, ctx.searchPath);
}

bool isThreefold(
    const PositionState& pos,
    const RepetitionContext& ctx
) {
    return occurrenceCount(pos, ctx)
        >= THREEFOLD_REPETITION_COUNT;
}

void pushSearchPosition(
    RepetitionContext& ctx,
    const PositionState& pos
) {
    ctx.searchPath.push_back(repetitionKey(pos));
}

void popSearchPosition(RepetitionContext& ctx) {
    if (!ctx.searchPath.empty())
        ctx.searchPath.pop_back();
}

// A tiny stand-in for a search-node evaluation.
// A real engine would continue with alpha-beta or quiescence when the
// position is not a repetition.
int evaluateSearchNode(
    const PositionState& pos,
    const RepetitionContext& ctx,
    int ordinaryScore
) {
    if (isThreefold(pos, ctx))
        return 0; // draw score

    return ordinaryScore;
}

PositionState makePosition(
    const string& placement,
    char side,
    const string& castling,
    const string& ep,
    bool legalEp,
    int halfmove,
    int fullmove
) {
    PositionState pos;
    pos.piecePlacement = placement;
    pos.sideToMove = side;
    pos.castlingRights = castling;
    pos.enPassantSquare = ep;
    pos.legalEnPassantCaptureExists = legalEp;
    pos.halfmoveClock = halfmove;
    pos.fullmoveNumber = fullmove;
    return pos;
}

void printIdentityCheck(
    const string& label,
    const PositionState& a,
    const PositionState& b
) {
    cout << "  " << label << ": "
         << (repetitionKey(a) == repetitionKey(b)
             ? "same identity"
             : "different identity")
         << '\n';
}

int main() {
    cout << "PHILOCHESS 0.2 sample: threefold repetition detection\n\n";

    const string placement =
        "7k/8/8/8/8/5N2/8/7K";

    PositionState base = makePosition(
        placement, 'w', "-", "-", false, 4, 12
    );

    cout << "1. Position identity\n";

    PositionState differentCounters = base;
    differentCounters.halfmoveClock = 87;
    differentCounters.fullmoveNumber = 53;

    printIdentityCheck(
        "different FEN move counters",
        base,
        differentCounters
    );

    PositionState nominalEp = base;
    nominalEp.enPassantSquare = "e3";
    nominalEp.legalEnPassantCaptureExists = false;

    printIdentityCheck(
        "nominal but unusable en-passant square",
        base,
        nominalEp
    );

    PositionState effectiveEp = base;
    effectiveEp.enPassantSquare = "e3";
    effectiveEp.legalEnPassantCaptureExists = true;

    printIdentityCheck(
        "legally usable en-passant square",
        base,
        effectiveEp
    );

    PositionState differentCastling = base;
    differentCastling.castlingRights = "K";

    printIdentityCheck(
        "different castling rights",
        base,
        differentCastling
    );

    PositionState differentSide = base;
    differentSide.sideToMove = 'b';

    printIdentityCheck(
        "different side to move",
        base,
        differentSide
    );

    cout << "\n2. Third occurrence inside a search branch\n";

    PositionState alternate = makePosition(
        "7k/8/8/8/8/8/6N1/7K",
        'b', "-", "-", false, 5, 12
    );

    RepetitionContext ctx;

    // Imagine that the real game has already reached 'base' twice.
    ctx.gameHistory.push_back(repetitionKey(base));
    ctx.gameHistory.push_back(repetitionKey(alternate));
    ctx.gameHistory.push_back(repetitionKey(base));

    cout << "  occurrences before search child: "
         << occurrenceCount(base, ctx) << '\n';

    // The search reaches the same position once more.
    pushSearchPosition(ctx, base);

    cout << "  occurrences in this branch: "
         << occurrenceCount(base, ctx) << '\n';

    cout << "  search score: "
         << evaluateSearchNode(base, ctx, +240)
         << " cp (0 means draw)\n";

    popSearchPosition(ctx);

    cout << "\n3. Branch isolation\n";

    cout << "  search-path depth after returning: "
         << ctx.searchPath.size() << '\n';

    PositionState sibling = makePosition(
        "7k/8/8/8/4N3/8/8/7K",
        'w', "-", "-", false, 6, 13
    );

    pushSearchPosition(ctx, sibling);

    cout << "  sibling occurrences: "
         << occurrenceCount(sibling, ctx) << '\n';

    cout << "  sibling score: "
         << evaluateSearchNode(sibling, ctx, +90)
         << " cp\n";

    popSearchPosition(ctx);

    cout << "  final search-path depth: "
         << ctx.searchPath.size() << '\n';

    cout << "\nKey ideas:\n";
    cout << "  - Repetition is about position identity, not FEN move counters.\n";
    cout << "  - Side to move and castling rights are part of that identity.\n";
    cout << "  - En passant belongs to the identity only when the capture is legal.\n";
    cout << "  - Historical positions and the current search branch are counted together.\n";
    cout << "  - A third occurrence is evaluated as a draw.\n";
    cout << "  - Push/pop keeps temporary search history local to each branch.\n";

    return 0;
}
