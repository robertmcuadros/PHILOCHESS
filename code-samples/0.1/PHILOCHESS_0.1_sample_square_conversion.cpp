/*
 * PHILOCHESS 0.1 — Square conversion sample
 *
 * Educational sample derived from PHILOCHESS 0.1-alpha-004.
 * It demonstrates:
 *   - validation of algebraic square names;
 *   - conversion from a square such as "e4" to an index from 0 to 63;
 *   - conversion from an index from 0 to 63 back to algebraic notation.
 *
 * Board convention:
 *   a1 = 0, b1 = 1, ..., h1 = 7
 *   a2 = 8, ..., h8 = 63
 *
 * This file is not the complete PHILOCHESS source code and may differ
 * from the current private implementation.
 *
 * Copyright (c) 2026 Robert M. Cuadros.
 * Licensed under the PHILOCHESS Educational Code Sample License.
 * See ../../LICENSES/PHILOCHESS-CODE-SAMPLES-LICENSE.txt.
 */

#include <iostream>
#include <string>

bool isValidSquare(const std::string& square)
{
    if (square.size() != 2)
    {
        return false;
    }

    const char file = square[0];
    const char rank = square[1];

    return file >= 'a' && file <= 'h'
        && rank >= '1' && rank <= '8';
}

int squareToIndex(const std::string& square)
{
    if (!isValidSquare(square))
    {
        return -1;
    }

    const int fileIndex = square[0] - 'a';
    const int rankIndex = square[1] - '1';

    return rankIndex * 8 + fileIndex;
}

std::string indexToSquare(int index)
{
    if (index < 0 || index > 63)
    {
        return "??";
    }

    const char file = static_cast<char>('a' + (index % 8));
    const char rank = static_cast<char>('1' + (index / 8));

    std::string square;
    square += file;
    square += rank;

    return square;
}

int main()
{
    const std::string squares[] = { "a1", "e4", "h8", "z9" };

    for (const std::string& square : squares)
    {
        const int index = squareToIndex(square);

        if (index == -1)
        {
            std::cout << square << " is invalid\n";
        }
        else
        {
            std::cout << square << " = " << index
                      << " = " << indexToSquare(index) << '\n';
        }
    }

    return 0;
}
