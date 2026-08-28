// PHILOCHESS 0.2 code sample
// Piece-square tables
// Derived from the development of PHILOCHESS 0.2-alpha-007
// Author: Robert M. Cuadros
//
// This is a deliberately reduced educational sample.
// It demonstrates board orientation, color reflection, piece-square
// bonuses, and phase-dependent king interpolation.  The tables below
// are illustrative and do not reproduce the complete PHILOCHESS PST set.
//
// C++17, self-contained and independently compilable.

#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

constexpr int GAME_PHASE_MAX = 24;

struct PieceOnSquare {
    char piece;      // Uppercase = White, lowercase = Black
    int square;      // a1 = 0, h8 = 63
};

int fileOf(int square) {
    return square & 7;
}

int rankOf(int square) {
    return square >> 3;
}

int squareFromString(const string& s) {
    if (s.size() != 2) return -1;
    if (s[0] < 'a' || s[0] > 'h') return -1;
    if (s[1] < '1' || s[1] > '8') return -1;

    return (s[1] - '1') * 8 + (s[0] - 'a');
}

bool isWhitePiece(char piece) {
    return piece >= 'A' && piece <= 'Z';
}

// The sample keeps only a few illustrative PSTs.
// Values are in centipawns and are intentionally simple.

constexpr array<int, 64> KNIGHT_PST = {{
    -30, -20, -10, -10, -10, -10, -20, -30,
    -20, -10,   0,   5,   5,   0, -10, -20,
    -10,   0,  10,  15,  15,  10,   0, -10,
    -10,   5,  15,  20,  20,  15,   5, -10,
    -10,   5,  15,  20,  20,  15,   5, -10,
    -10,   0,  10,  15,  15,  10,   0, -10,
    -20, -10,   0,   5,   5,   0, -10, -20,
    -30, -20, -10, -10, -10, -10, -20, -30
}};

constexpr array<int, 64> KING_MIDDLEGAME_PST = {{
     10,  15,   5,   0,   0,   5,  15,  10,
      0,   0,  -5, -10, -10,  -5,   0,   0,
     -5,  -5, -10, -15, -15, -10,  -5,  -5,
    -10, -10, -15, -20, -20, -15, -10, -10,
    -15, -15, -20, -25, -25, -20, -15, -15,
    -20, -20, -25, -30, -30, -25, -20, -20,
    -25, -25, -30, -30, -30, -30, -25, -25,
    -30, -30, -30, -30, -30, -30, -30, -30
}};

constexpr array<int, 64> KING_ENDGAME_PST = {{
    -20, -15, -10, -10, -10, -10, -15, -20,
    -15,  -5,   0,   5,   5,   0,  -5, -15,
    -10,   0,  10,  15,  15,  10,   0, -10,
    -10,   5,  15,  20,  20,  15,   5, -10,
    -10,   5,  15,  20,  20,  15,   5, -10,
    -10,   0,  10,  15,  15,  10,   0, -10,
    -15,  -5,   0,   5,   5,   0,  -5, -15,
    -20, -15, -10, -10, -10, -10, -15, -20
}};

// The tables are stored from White's point of view.
// For Black, a vertical reflection keeps the file and reverses the rank.
int orientedSquare(char piece, int square) {
    return isWhitePiece(piece) ? square : (square ^ 56);
}

int interpolatedKingValue(int square, int phase) {
    phase = max(0, min(phase, GAME_PHASE_MAX));

    const int middlegame = KING_MIDDLEGAME_PST[square];
    const int endgame = KING_ENDGAME_PST[square];

    return (middlegame * phase
          + endgame * (GAME_PHASE_MAX - phase))
          / GAME_PHASE_MAX;
}

int pieceSquareValue(char piece, int square, int phase) {
    const int sq = orientedSquare(piece, square);

    switch (tolower(static_cast<unsigned char>(piece))) {
    case 'n':
        return KNIGHT_PST[sq];

    case 'k':
        return interpolatedKingValue(sq, phase);

    default:
        // Other pieces are omitted deliberately in this educational sample.
        return 0;
    }
}

int pieceSquareEvaluation(const vector<PieceOnSquare>& position, int phase) {
    int score = 0;

    for (const PieceOnSquare& ps : position) {
        const int value = pieceSquareValue(ps.piece, ps.square, phase);

        if (isWhitePiece(ps.piece))
            score += value;
        else
            score -= value;
    }

    return score;
}

void showKnightExample() {
    const int g1 = squareFromString("g1");
    const int f3 = squareFromString("f3");
    const int g8 = squareFromString("g8");
    const int f6 = squareFromString("f6");

    cout << "Knight development example\n";
    cout << "  White Ng1: " << pieceSquareValue('N', g1, GAME_PHASE_MAX) << " cp\n";
    cout << "  White Nf3: " << pieceSquareValue('N', f3, GAME_PHASE_MAX) << " cp\n";
    cout << "  Black Ng8: " << pieceSquareValue('n', g8, GAME_PHASE_MAX) << " cp\n";
    cout << "  Black Nf6: " << pieceSquareValue('n', f6, GAME_PHASE_MAX) << " cp\n";

    cout << "  White delta Ng1-f3: "
         << pieceSquareValue('N', f3, GAME_PHASE_MAX)
          - pieceSquareValue('N', g1, GAME_PHASE_MAX)
         << " cp\n\n";
}

void showKingPhaseExample() {
    const int e1 = squareFromString("e1");
    const int d4 = squareFromString("d4");

    cout << "King phase interpolation\n";

    for (int phase : {24, 12, 0}) {
        cout << "  phase " << phase
             << ": Ke1 = " << pieceSquareValue('K', e1, phase)
             << " cp, Kd4 = " << pieceSquareValue('K', d4, phase)
             << " cp\n";
    }

    cout << '\n';
}

void showSymmetryExample() {
    vector<PieceOnSquare> symmetricPosition = {
        {'N', squareFromString("f3")},
        {'n', squareFromString("f6")},
        {'K', squareFromString("g1")},
        {'k', squareFromString("g8")}
    };

    const int score = pieceSquareEvaluation(symmetricPosition, GAME_PHASE_MAX);

    cout << "Color-reflection symmetry\n";
    cout << "  PST score of symmetric position: "
         << score << " cp\n";
}

int main() {
    cout << "PHILOCHESS 0.2 sample: piece-square tables\n\n";

    showKnightExample();
    showKingPhaseExample();
    showSymmetryExample();

    cout << "\nKey ideas:\n";
    cout << "  - Store PSTs from one color's point of view.\n";
    cout << "  - Reflect Black vertically before indexing the same table.\n";
    cout << "  - Add PST terms to the static evaluation with opposite signs.\n";
    cout << "  - Interpolate king placement between middlegame and endgame.\n";
    cout << "  - A production engine still needs its own complete table design.\n";

    return 0;
}
