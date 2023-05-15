//
// Created by Javier on 01-04-2023.
//
#include <iostream>
#include "Move.h"
#include "Game.h"


Move::Move(bool isWhiteTurnMove, const std::bitset<56> &fromMove, const std::bitset<56> &toMove) : whiteTurnMove(
        isWhiteTurnMove), fromMove(fromMove), toMove(toMove) {}

void Move::Print(Game game)
{
    std::cout << "Move From: " << game.getSquareNameOfFirstBit(fromMove) << " To: " << game.getSquareNameOfFirstBit(toMove) << std::endl;
}

const std::bitset<56> &Move::getFromMove() const {
    return fromMove;
}

const std::bitset<56> &Move::getToMove() const {
    return toMove;
}

bool Move::isWhiteTurnMove() const {
    return whiteTurnMove;
}
