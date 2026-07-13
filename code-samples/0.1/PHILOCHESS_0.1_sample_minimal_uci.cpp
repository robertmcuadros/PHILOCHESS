/*
 * PHILOCHESS 0.1 — Minimal UCI interface sample
 *
 * Educational sample derived from PHILOCHESS 0.1-alpha-001.
 * It demonstrates the minimum command loop needed to answer the
 * UCI identification and readiness handshakes.
 *
 * This file is not the complete PHILOCHESS source code and may differ
 * from the current private implementation.
 *
 * Copyright (c) 2026 Robert M. Cuadros. All rights reserved.
 */

#include <iostream>
#include <string>

namespace
{
    void sendUciIdentification()
    {
        std::cout << "id name PHILOCHESS 0.1-alpha-001 sample\n"
                  << "id author Robert M. Cuadros\n"
                  << "uciok\n"
                  << std::flush;
    }

    void sendReadyResponse()
    {
        std::cout << "readyok\n"
                  << std::flush;
    }
}

int main()
{
    std::string command;

    while (std::getline(std::cin, command))
    {
        if (command == "uci")
        {
            sendUciIdentification();
        }
        else if (command == "isready")
        {
            sendReadyResponse();
        }
        else if (command == "quit")
        {
            break;
        }

        // Other UCI commands are intentionally ignored in this minimal sample.
    }

    return 0;
}
