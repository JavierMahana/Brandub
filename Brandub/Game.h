//
// Created by Javier on 02-04-2023.
//

#ifndef BRANDUB_GAME_H
#define BRANDUB_GAME_H


#include "Bitboard.h"

class Game {


public:
    Game();
    void gameLoop();
    bool hasWhiteWon() const;
    bool hasBlackWon() const;
    bool isCorrectColorPiece(int cellIndex) const;
    bool areLegalMoves(int cellIndex, std::bitset<56>& legalMoveMask);
    bool areLegalMoves(std::bitset<56> cellMask, std::bitset<56>& legalMoveMask);
    std::bitset<56> indexToBitset(int index);
    std::string getSquareNameOfFirstBit(const std::bitset<56>& bitset);
    bool validInput(const std::string& input, int& cellIndex);

    void MoveRandom();

private:

    Bitboard gameBoard;
    bool whiteTurn;

public:

//    enum Board : int {
//        a1 = 0, b1, c1, d1, e1, f1, g1,
//        a2 = 8, b2, c2, d2, e2, f2, g2,
//        a3 = 16, b3, c3, d3, e3, f3, g3,
//        a4 = 24, b4, c4, d4, e4, f4, g4,
//        a5 = 32, b5, c5, d5, e5, f5, g5,
//        a6 = 40, b6, c6, d6, e6, f6, g6,
//        a7 = 48, b7, c7, d7, e7, f7, g7,
//    };

//    const std::string boardMap[56]{
//            "a1","b1","c1","d1","e1","f1","g1","SBit",
//            "a2","b2","c2","d2","e2","f2","g2", "SBit",
//            "a3","b3","c3","d3","e3","f3","g3","SBit",
//            "a4","b4","c4","d4","e4","f4","g4","SBit",
//            "a5","b5","c5","d5","e5","f5","g5","SBit",
//            "a6","b6","c6","d6","e6","f6","g6","SBit",
//            "a7","b7","c7","d7","e7","f7","g7","SBit"
//    };



//it is reversed because the std::bitset stores the bits in the order of least important first (reverse)
     const std::string boardMap[56]{
            "SBit","g7","f7","e7","d7","c7","b7","a7",
            "SBit","g6","f6","e6","d6","c6","b6","a6",
            "SBit","g5","f5","e5","d5","c5","b5","a5",
            "SBit","g4","f4","e4","d4","c4","b4","a4",
            "SBit","g3","f3","e3","d3","c3","b3","a3",
            "SBit","g2","f2","e2","d2","c2","b2","a2",
            "SBit","g1","f1","e1","d1","c1","b1","a1"
    };

    enum Board : int {
        g7 = 1, f7, e7, d7, c7, b7, a7,
        g6 = 9, f6, e6, d6, c6, b6, a6,
        g5 = 17, f5, e5, d5, c5, b5, a5,
        g4 = 25, f4, e4, d4, c4, b4, a4,
        g3 = 33, f3, e3, d3, c3, b3, a3,
        g2 = 41, f2, e2, d2, c2, b2, a2,
        g1 = 49, f1, e1, d1, c1, b1, a1,
    };


    std::bitset<56> GetRandomCell();
};


#endif //BRANDUB_GAME_H
