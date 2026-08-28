// PHILOCHESS 0.2 code sample
// Basic mobility evaluation
// Derived from the development of PHILOCHESS 0.2-alpha-008
// Author: Robert M. Cuadros
//
// This is a deliberately reduced educational sample.
// It demonstrates a simple pseudolegal mobility term for knights, bishops,
// rooks, and queens.  It does not reproduce the complete PHILOCHESS
// evaluation, parameter set, search, or move generator.
//
// C++17, self-contained and independently compilable.

#include <array>
#include <cctype>
#include <iostream>
#include <string>
#include <utility>

using namespace std;

constexpr int NO_SQUARE = -1;

struct Position {
    array<char, 64> board{};
};

int fileOf(int square) {
    return square & 7;
}

int rankOf(int square) {
    return square >> 3;
}

bool onBoard(int square) {
    return square >= 0 && square < 64;
}

bool isWhitePiece(char piece) {
    return piece >= 'A' && piece <= 'Z';
}

bool isBlackPiece(char piece) {
    return piece >= 'a' && piece <= 'z';
}

bool isEmpty(char piece) {
    return piece == '.';
}

bool sameColor(char a, char b) {
    if (isEmpty(a) || isEmpty(b)) return false;
    return (isWhitePiece(a) && isWhitePiece(b))
        || (isBlackPiece(a) && isBlackPiece(b));
}

int squareFromString(const string& s) {
    if (s.size() != 2) return NO_SQUARE;
    if (s[0] < 'a' || s[0] > 'h') return NO_SQUARE;
    if (s[1] < '1' || s[1] > '8') return NO_SQUARE;

    return (s[1] - '1') * 8 + (s[0] - 'a');
}

void clearBoard(Position& pos) {
    pos.board.fill('.');
}

void put(Position& pos, const string& square, char piece) {
    const int sq = squareFromString(square);
    if (onBoard(sq)) pos.board[sq] = piece;
}

bool isMobilityPiece(char piece) {
    const char type =
        char(tolower(static_cast<unsigned char>(piece)));

    return type == 'n'
        || type == 'b'
        || type == 'r'
        || type == 'q';
}

// In this educational model:
// - empty destinations count;
// - an enemy piece counts as the terminal destination;
// - a friendly piece blocks and does not count;
// - the enemy king blocks geometry but is not counted as a capturable square.
//
// This is geometric/pseudolegal mobility.  It does not test whether moving
// a piece would expose its own king.
bool destinationCounts(char movingPiece, char destinationPiece) {
    if (isEmpty(destinationPiece)) return true;
    if (sameColor(movingPiece, destinationPiece)) return false;

    return tolower(static_cast<unsigned char>(destinationPiece)) != 'k';
}

int knightMobility(const Position& pos, int from) {
    static constexpr int deltas[8][2] = {
        { 1,  2}, { 2,  1}, { 2, -1}, { 1, -2},
        {-1, -2}, {-2, -1}, {-2,  1}, {-1,  2}
    };

    const char piece = pos.board[from];
    const int f = fileOf(from);
    const int r = rankOf(from);

    int count = 0;

    for (const auto& delta : deltas) {
        const int nf = f + delta[0];
        const int nr = r + delta[1];

        if (nf < 0 || nf > 7 || nr < 0 || nr > 7)
            continue;

        if (destinationCounts(piece, pos.board[nr * 8 + nf]))
            ++count;
    }

    return count;
}

int slidingMobility(
    const Position& pos,
    int from,
    const pair<int, int>* directions,
    size_t directionCount
) {
    const char piece = pos.board[from];
    const int f = fileOf(from);
    const int r = rankOf(from);

    int count = 0;

    for (size_t i = 0; i < directionCount; ++i) {
        int nf = f + directions[i].first;
        int nr = r + directions[i].second;

        while (nf >= 0 && nf < 8 && nr >= 0 && nr < 8) {
            const char target = pos.board[nr * 8 + nf];

            if (isEmpty(target)) {
                ++count;
            }
            else {
                if (destinationCounts(piece, target))
                    ++count;

                break;
            }

            nf += directions[i].first;
            nr += directions[i].second;
        }
    }

    return count;
}

int mobilityDestinations(const Position& pos, int square) {
    if (!onBoard(square)) return 0;

    const char piece = pos.board[square];

    static constexpr array<pair<int, int>, 4> bishopDirections = {{
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    }};

    static constexpr array<pair<int, int>, 4> rookDirections = {{
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    }};

    static constexpr array<pair<int, int>, 8> queenDirections = {{
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    }};

    switch (tolower(static_cast<unsigned char>(piece))) {
    case 'n':
        return knightMobility(pos, square);

    case 'b':
        return slidingMobility(
            pos, square,
            bishopDirections.data(),
            bishopDirections.size()
        );

    case 'r':
        return slidingMobility(
            pos, square,
            rookDirections.data(),
            rookDirections.size()
        );

    case 'q':
        return slidingMobility(
            pos, square,
            queenDirections.data(),
            queenDirections.size()
        );

    default:
        return 0;
    }
}

// Illustrative parameters only.
// A real engine should tune its own references and weights.
int mobilityReference(char piece) {
    switch (tolower(static_cast<unsigned char>(piece))) {
    case 'n': return 4;
    case 'b': return 6;
    case 'r': return 7;
    case 'q': return 12;
    default:  return 0;
    }
}

int mobilityWeight(char piece) {
    switch (tolower(static_cast<unsigned char>(piece))) {
    case 'n': return 3;
    case 'b': return 2;
    case 'r': return 1;
    case 'q': return 1;
    default:  return 0;
    }
}

int mobilityAdjustment(char piece, int destinations) {
    if (!isMobilityPiece(piece)) return 0;

    return mobilityWeight(piece)
        * (destinations - mobilityReference(piece));
}

int mobilityEvaluation(const Position& pos) {
    int score = 0;

    for (int square = 0; square < 64; ++square) {
        const char piece = pos.board[square];

        if (!isMobilityPiece(piece))
            continue;

        const int destinations =
            mobilityDestinations(pos, square);

        const int adjustment =
            mobilityAdjustment(piece, destinations);

        score += isWhitePiece(piece)
            ? adjustment
            : -adjustment;
    }

    return score;
}

void printPieceMobility(
    const Position& pos,
    const string& square
) {
    const int sq = squareFromString(square);
    const char piece = pos.board[sq];
    const int destinations = mobilityDestinations(pos, sq);
    const int adjustment = mobilityAdjustment(piece, destinations);

    cout << "  " << piece << " on " << square
         << ": destinations=" << destinations
         << ", adjustment=" << adjustment << " cp\n";
}

int main() {
    cout << "PHILOCHESS 0.2 sample: basic mobility evaluation\n\n";

    // Example 1: an active knight in the center.
    Position centralKnight;
    clearBoard(centralKnight);
    put(centralKnight, "d4", 'N');
    put(centralKnight, "h1", 'K');
    put(centralKnight, "h8", 'k');

    cout << "1. Central knight\n";
    printPieceMobility(centralKnight, "d4");
    cout << "  mobility evaluation = "
         << mobilityEvaluation(centralKnight)
         << " cp\n\n";

    // Example 2: a bishop whose own pawns restrict its geometry.
    Position blockedBishop;
    clearBoard(blockedBishop);
    put(blockedBishop, "d4", 'B');
    put(blockedBishop, "c5", 'P');
    put(blockedBishop, "e5", 'P');
    put(blockedBishop, "c3", 'P');
    put(blockedBishop, "e3", 'P');
    put(blockedBishop, "h1", 'K');
    put(blockedBishop, "h8", 'k');

    cout << "2. Blocked bishop\n";
    printPieceMobility(blockedBishop, "d4");
    cout << "  mobility evaluation = "
         << mobilityEvaluation(blockedBishop)
         << " cp\n\n";

    // Example 3: symmetric mobility should cancel.
    Position symmetric;
    clearBoard(symmetric);
    put(symmetric, "d4", 'N');
    put(symmetric, "e5", 'n');
    put(symmetric, "h1", 'K');
    put(symmetric, "a8", 'k');

    cout << "3. Symmetric knights\n";
    printPieceMobility(symmetric, "d4");
    printPieceMobility(symmetric, "e5");
    cout << "  mobility evaluation = "
         << mobilityEvaluation(symmetric)
         << " cp\n\n";

    cout << "Key ideas:\n";
    cout << "  - Mobility measures available destinations, not material.\n";
    cout << "  - Friendly pieces restrict mobility; enemy pieces may terminate a ray.\n";
    cout << "  - Knights and sliding pieces require different counting logic.\n";
    cout << "  - White bonuses and Black bonuses enter with opposite signs.\n";
    cout << "  - This sample uses pseudolegal geometry, not full move legality.\n";
    cout << "  - The parameters are illustrative rather than PHILOCHESS production values.\n";

    return 0;
}
