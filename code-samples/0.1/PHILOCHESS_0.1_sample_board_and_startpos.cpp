/*
 * PHILOCHESS 0.1 — Board representation and start position sample
 *
 * Educational sample derived from PHILOCHESS 0.1-alpha-003.
 * It demonstrates:
 *   - a 64-square board stored in std::array;
 *   - an explicit piece enumeration;
 *   - conversion from file/rank coordinates to an array index;
 *   - initialization and display of the standard chess starting position.
 *
 * This file is not the complete PHILOCHESS source code and may differ
 * from the current private implementation.
 *
 * Copyright (c) 2026 Robert M. Cuadros. All rights reserved.
 */

#include <array>
#include <iostream>

enum class Piece
{
    Empty,

    WhitePawn,
    WhiteKnight,
    WhiteBishop,
    WhiteRook,
    WhiteQueen,
    WhiteKing,

    BlackPawn,
    BlackKnight,
    BlackBishop,
    BlackRook,
    BlackQueen,
    BlackKing
};

char pieceToChar(Piece piece)
{
    switch (piece)
    {
    case Piece::WhitePawn:   return 'P';
    case Piece::WhiteKnight: return 'N';
    case Piece::WhiteBishop: return 'B';
    case Piece::WhiteRook:   return 'R';
    case Piece::WhiteQueen:  return 'Q';
    case Piece::WhiteKing:   return 'K';

    case Piece::BlackPawn:   return 'p';
    case Piece::BlackKnight: return 'n';
    case Piece::BlackBishop: return 'b';
    case Piece::BlackRook:   return 'r';
    case Piece::BlackQueen:  return 'q';
    case Piece::BlackKing:   return 'k';

    case Piece::Empty:
    default:
        return '.';
    }
}

class Board
{
public:
    Board()
    {
        clear();
    }

    void clear()
    {
        squares_.fill(Piece::Empty);
    }

    void setStartPosition()
    {
        clear();

        constexpr std::array<Piece, 8> whiteBackRank{
            Piece::WhiteRook,
            Piece::WhiteKnight,
            Piece::WhiteBishop,
            Piece::WhiteQueen,
            Piece::WhiteKing,
            Piece::WhiteBishop,
            Piece::WhiteKnight,
            Piece::WhiteRook
        };

        constexpr std::array<Piece, 8> blackBackRank{
            Piece::BlackRook,
            Piece::BlackKnight,
            Piece::BlackBishop,
            Piece::BlackQueen,
            Piece::BlackKing,
            Piece::BlackBishop,
            Piece::BlackKnight,
            Piece::BlackRook
        };

        for (int file = 0; file < 8; ++file)
        {
            squares_[index(file, 0)] = whiteBackRank[file];
            squares_[index(file, 1)] = Piece::WhitePawn;

            squares_[index(file, 6)] = Piece::BlackPawn;
            squares_[index(file, 7)] = blackBackRank[file];
        }
    }

    void print() const
    {
        std::cout << '\n';

        for (int rank = 7; rank >= 0; --rank)
        {
            std::cout << rank + 1 << "  ";

            for (int file = 0; file < 8; ++file)
            {
                std::cout << pieceToChar(squares_[index(file, rank)]) << ' ';
            }

            std::cout << '\n';
        }

        std::cout << "\n   a b c d e f g h\n\n";
    }

private:
    std::array<Piece, 64> squares_{};

    static constexpr int index(int file, int rank)
    {
        return rank * 8 + file;
    }
};

int main()
{
    Board board;
    board.setStartPosition();
    board.print();

    return 0;
}
