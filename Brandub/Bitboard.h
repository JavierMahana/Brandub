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
    const std::bitset<56> CENTER {"00000000000000000000000000010000000000000000000000000000"};
    const std::bitset<56> TOP_LEFT_CORNER {"10000000000000000000000000000000000000000000000000000000"};
    const std::bitset<56> TOP_RIGHT_CORNER {"00000010000000000000000000000000000000000000000000000000"};
    const std::bitset<56> BOTTOM_LEFT_CORNER {"00000000000000000000000000000000000000000000000010000000"};
    const std::bitset<56> BOTTOM_RIGHT_CORNER {"00000000000000000000000000000000000000000000000000000010"};
    const std::bitset<56> BORDER {"11111110"
                                      "10000010"
                                      "10000010"
                                      "10000010"
                                      "10000010"
                                      "10000010"
                                      "11111110"};

    const std::bitset<56> START_BLACK {"00010000000100000000000011000110000000000001000000010000"};
    const std::bitset<56> START_KING {"00000000000000000000000000010000000000000000000000000000"};
    const std::bitset<56> START_WHITE {"00000000000000000001000000101000000100000000000000000000"};

    const char BLACK_CHAR = 'B';
    const char WHITE_CHAR = 'W';
    const char KING_CHAR = 'K';
    const char EMPTY_CHAR = ' ';
    const char BLOCK_CHAR = 'X';
    const char CORNER_CHAR = 'O';


    const int DANGER_ALLY_ADYACENT = -1;
    const int DANGER_ENEMY_ADYACENT = 2;
    const int DANGER_ADYACENT_ENEMY_ONSIGHT = 1;
    const int DANGER_ENEMY_MAYEAT = 3;
    const int DANGER_KING_FACTOR = 3;

    const int FLEE_ENEMY_ONSIGHT = 1;
    const int FLEE_EMPTY_ADYACENT = 2;
    const int FLEE_ALLY_ONSIGHT = 3;
    const int FLEE_BLOCK_ONSIGHT = 3;
    const int KING_VICTORY_ON_BORDER = 3;
    const int PIECE_EVAL = 8;


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
    enum class CellType {
        BLACK,
        WHITE,
        KING,
        BLOCK,
        CORNER,
        EMPTY
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
    std::bitset<56> getAllCorners () const { return TOP_LEFT_CORNER | TOP_RIGHT_CORNER | BOTTOM_LEFT_CORNER | BOTTOM_RIGHT_CORNER;};
    std::bitset<56> getBlockedBits () const { return getAllCorners() | CENTER;};

    std::bitset<56> shiftDirection(std::bitset<56> bitset, Direction direction);
    std::bitset<56> shiftUp(std::bitset<56> bitset);
    std::bitset<56> shiftDown(std::bitset<56> bitset);
    std::bitset<56> shiftLeft(std::bitset<56> bitset);
    std::bitset<56> shiftRight(std::bitset<56> bitset);

    std::bitset<56> dilateBits(std::bitset<56> bitset, DirectionFlag dilateDirection);
    std::bitset<56> erodeBits(std::bitset<56> bitset, DirectionFlag erodeDirection);

    Direction getOpositeDirection(Direction direction);

    CellType getCellType(std::bitset<56> bit);

    void print();
    void print(std::bitset<56> bitsetToPrint);

    bool tryMove(Move move);
    CellType pieceOnSight(std::bitset<56> a, Direction direction);
    bool bitsOnSight(std::bitset<56> a, std::bitset<56> b);

    bool isAllyInCell(std::bitset<56> cellToCheck, bool isWhite);
    bool checkForMate();
    bool checkKingPosition(std::bitset<56> cellPosition){return (cellPosition & getBitsKing()).any();}
    void checkEat(Move move);

    bool bitGenerateCapture(std::bitset<56> bitToCheck, bool isWhite, Direction& captureDirection);
    bool bitMustBeEaten(std::bitset<56> bitToCheck, bool isWhite);

    int evaluateDangerInCell(bool isWhiteTurn, std::bitset<56> cell, bool isWhite);
    int evaluateBoard(bool isWhiteTurn);
    int evaluateKingPosition(bool isWhiteTurn);
    int evaluateKingFleeChances(bool isWhiteTurn);
    int evaluateKingVictory(bool isWhiteTurn);

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
    const char getEmptyChar(){return EMPTY_CHAR;}
    const char getBlockChar(){return BLOCK_CHAR;}
    const char getCornerChar(){return CORNER_CHAR;}

};


#endif //BRANDUB_BITBOARD_H
