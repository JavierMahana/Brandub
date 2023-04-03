//
// Created by Javier on 01-04-2023.
//

#ifndef BRANDUB_BITBOARD_H
#define BRANDUB_BITBOARD_H


#include <bitset>
#include <string>

class Move;

class Bitboard {

public:

    //Proportions 7*7 + 7 bit separation to the right
    const int BOARD_BIT_SIZE = 56;


    const std::bitset<56> BOARD_MASK {"11111110111111101111111011111110111111101111111011111110"};
    const std::bitset<56> TOP_LEFT_CORNER {"10000000000000000000000000000000000000000000000000000000"};
    const std::bitset<56> TOP_RIGHT_CORNER {"00000010000000000000000000000000000000000000000000000000"};
    const std::bitset<56> BOTTOM_LEFT_CORNER {"00000000000000000000000000000000000000000000000010000000"};
    const std::bitset<56> BOTTOM_RIGHT_CORNER {"00000000000000000000000000000000000000000000000000000010"};

    const std::bitset<56> START_BLACK {"00010000000100000000000011000110000000000001000000010000"};
    const std::bitset<56> START_KING {"00000000000000000000000000010000000000000000000000000000"};
    const std::bitset<56> START_WHITE {"00000000000000000001000000101000000100000000000000000000"};

    const char BLACK_CHAR = '+';
    const char WHITE_CHAR = '-';
    const char KING_CHAR = '@';

    enum class DirectionFlag : unsigned int {
        LEFT = 1 << 0,
        RIGHT = 1 << 1,
        UP = 1 << 2,
        DOWN = 1 << 3,
        ALL_DIRECTIONS = LEFT | RIGHT | UP| DOWN
    };
    enum class Direction {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };


private:

    std::bitset<56> bitsWhite;
    std::bitset<56> bitsBlack;
    std::bitset<56> bitsKing;

public:

    Bitboard();

    std::bitset<56> getFull () const { return bitsBlack | bitsKing | bitsWhite;};
    std::bitset<56> getEmpty () const {return getFull().flip() & BOARD_MASK;};
    std::bitset<56> getAllWhiteBits () const { return bitsKing | bitsWhite;};

    std::bitset<56> shiftDirection(std::bitset<56> bitset, Direction direction);
    std::bitset<56> shiftUp(std::bitset<56> bitset);
    std::bitset<56> shiftDown(std::bitset<56> bitset);
    std::bitset<56> shiftLeft(std::bitset<56> bitset);
    std::bitset<56> shiftRight(std::bitset<56> bitset);

    std::bitset<56> dilateBits(std::bitset<56> bitset, DirectionFlag dilateDirection);
    std::bitset<56> erodeBits(std::bitset<56> bitset, DirectionFlag erodeDirection);

    //Getter and setter of bits
    const std::bitset<56> &getBitsWhite() const;
    void setBitsWhite(const std::bitset<56> &bitsWhite);
    const std::bitset<56> &getBitsBlack() const;
    void setBitsBlack(const std::bitset<56> &bitsBlack);
    const std::bitset<56> &getBitsKing() const;
    void setBitsKing(const std::bitset<56> &bitsKing);

    //Getter of pieces chars
    const char getBlackChar(){return BLACK_CHAR;}
    const char getWhiteChar(){return WHITE_CHAR;}
    const char getKingChar(){return KING_CHAR;}

    void Print();

    bool TryMove(Move move);


public:






};


#endif //BRANDUB_BITBOARD_H
