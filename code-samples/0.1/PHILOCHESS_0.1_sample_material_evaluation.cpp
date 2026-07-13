/*
 * PHILOCHESS 0.1 — Material evaluation sample
 *
 * Educational sample derived from PHILOCHESS 0.1-alpha-024.
 * It demonstrates:
 *   - conventional centipawn values for the chess pieces;
 *   - evaluation of material from White's point of view;
 *   - conversion of that score to the side-to-move point of view.
 *
 * Positive material scores favor White.
 * Negative material scores favor Black.
 *
 * This file is not the complete PHILOCHESS source code and may differ
 * from the current private implementation.
 *
 * Copyright (c) 2026 Robert M. Cuadros.
 * Licensed under the PHILOCHESS Educational Code Sample License.
 * See ../../LICENSES/PHILOCHESS-CODE-SAMPLES-LICENSE.txt.
 */

#include <array>
#include <cctype>
#include <iostream>

constexpr int VALUE_PAWN   = 100;
constexpr int VALUE_KNIGHT = 320;
constexpr int VALUE_BISHOP = 330;
constexpr int VALUE_ROOK   = 500;
constexpr int VALUE_QUEEN  = 900;
constexpr int VALUE_KING   = 0;

struct Position
{
    std::array<char, 64> board{};
    bool whiteToMove = true;
};

bool isWhitePiece(char piece)
{
    return piece >= 'A' && piece <= 'Z';
}

bool isBlackPiece(char piece)
{
    return piece >= 'a' && piece <= 'z';
}

int pieceValue(char piece)
{
    switch (std::tolower(static_cast<unsigned char>(piece)))
    {
    case 'p': return VALUE_PAWN;
    case 'n': return VALUE_KNIGHT;
    case 'b': return VALUE_BISHOP;
    case 'r': return VALUE_ROOK;
    case 'q': return VALUE_QUEEN;
    case 'k': return VALUE_KING;
    default:  return 0;
    }
}

int materialEvaluation(const Position& position)
{
    int score = 0;

    for (char piece : position.board)
    {
        if (isWhitePiece(piece))
        {
            score += pieceValue(piece);
        }
        else if (isBlackPiece(piece))
        {
            score -= pieceValue(piece);
        }
    }

    return score;
}

int sideToMoveEvaluation(const Position& position)
{
    const int score = materialEvaluation(position);
    return position.whiteToMove ? score : -score;
}

int main()
{
    Position position;
    position.board.fill('.');

    // Small demonstration position:
    // White: king, queen, rook, bishop, knight and two pawns.
    // Black: king, queen, rook, bishop and two pawns.
    // White therefore has one extra knight: +320 centipawns.
    position.board[4]  = 'K';
    position.board[3]  = 'Q';
    position.board[0]  = 'R';
    position.board[2]  = 'B';
    position.board[1]  = 'N';
    position.board[8]  = 'P';
    position.board[9]  = 'P';

    position.board[60] = 'k';
    position.board[59] = 'q';
    position.board[63] = 'r';
    position.board[58] = 'b';
    position.board[48] = 'p';
    position.board[49] = 'p';

    position.whiteToMove = true;

    std::cout << "Material values: "
              << "P=" << VALUE_PAWN << ' '
              << "N=" << VALUE_KNIGHT << ' '
              << "B=" << VALUE_BISHOP << ' '
              << "R=" << VALUE_ROOK << ' '
              << "Q=" << VALUE_QUEEN << ' '
              << "K=" << VALUE_KING << '\n';

    std::cout << "Material evaluation: "
              << materialEvaluation(position)
              << " cp (positive = White is ahead)\n";

    std::cout << "Side-to-move evaluation: "
              << sideToMoveEvaluation(position)
              << " cp\n";

    return 0;
}
