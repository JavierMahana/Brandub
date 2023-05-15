//
// Created by Javier on 01-04-2023.
//

#ifndef BRANDUB_MOVE_H
#define BRANDUB_MOVE_H

#include <bitset>


class Move {


public:

    Move(bool isWhiteTurnMove, const std::bitset<56> &fromMove, const std::bitset<56> &toMove);

    void Print(class Game game);

    const std::bitset<56> &getFromMove() const;

    const std::bitset<56> &getToMove() const;

    bool isWhiteTurnMove() const;

private:

    bool whiteTurnMove;


private:
    std::bitset<56> fromMove;
    std::bitset<56> toMove;
};


#endif //BRANDUB_MOVE_H
