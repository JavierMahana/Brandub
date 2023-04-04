//
// Created by Javier on 02-04-2023.
//

#include <iostream>
#include <limits>
#include "Game.h"
#include "Move.h"
#include <bitset>

Game::Game() {
    whiteTurn = true;
}


bool Game::hasWhiteWon() const{
    return false;
}

bool Game::hasBlackWon() const{
    return false;
}

void Game::gameLoop() {

    std::cout << "WELCOME TO BRANDUB" << std::endl;
    std::cout << "The white player has the objective of putting the king in one of the 4 corners of the board" << std::endl;
    std::cout << "And the black player has the objective of trapping the king around 4 of his pieces" << std::endl;
    std::cout << "To capture a piece you must place 2 of your own pieces in each side" << std::endl;
    std::cout << "Glosary" << std::endl;
    std::cout << "White piece:" << gameBoard.getWhiteChar() << std::endl;
    std::cout << "Black piece:" << gameBoard.getBlackChar() << std::endl;
    std::cout << "King:" << gameBoard.getKingChar() << std::endl;
    std::cout << std::endl;

    
    while(!hasBlackWon() && !hasWhiteWon())
    {
        gameBoard.Print();
        bool repeatLoop = false;

        std::string colorTurnString = whiteTurn ? "white" : "black";

        std::string input;

        //variables that are filled in the code
        int inputCellIndex;
        std::bitset<56> legalMovesMask;

        std::bitset<56> pieceToMoveMask;
        std::bitset<56> destinationSquareMask;

        //Selection of the piece to move
        while (true){
            std::cout << "Enter the square of a " << colorTurnString << " piece that you want to move: ";
            std::cin >> input;

            if(!validInput(input, inputCellIndex))
            {
                std::cout << "Invalid input! it must follow the format of a letter then a number. Like:\"a1\"" << std::endl;
                continue;
            }

            std::cout << "Input index: "<< inputCellIndex << std::endl;


            if(!isCorrectColorPiece(inputCellIndex))
            {
                std::cout << "Invalid input! you must select square containing a " << colorTurnString << " piece" << std::endl;
                continue;
            }

            if(!areLegalMoves(inputCellIndex, legalMovesMask))
            {
                std::cout << "Invalid input! you must select square containing a piece that have legal moves" << std::endl;
                continue;
            }

            std::cin.clear();

            //The input is valid, so we set the pieceToMoveMask and leave the while loop.

            pieceToMoveMask = indexToBitset(inputCellIndex);
            break;
        }


        std::cout << "You selected the: \"" << input << "\" square" << std::endl;
        std::cout << std::endl;



        while (true)
        {
            std::cout << "Now enter the square that you want to move to, or \"q\" to select another piece." << std::endl;
            std::cin >> input;

            if(input == "q" ||input == "Q")
            {
                repeatLoop = true;
                break;
            }

            if(!validInput(input, inputCellIndex))
            {
                std::cout << "Invalid input! it must follow the format of a letter then a number. Like:\"a1\". Or \"q\"" << std::endl;
                continue;
            }

            destinationSquareMask = indexToBitset(inputCellIndex);
            if(!(destinationSquareMask & legalMovesMask).any())
            {
                std::cout << "Invalid input! you must select square that have a legal move" << std::endl;
                continue;
            }

            //Here we know that we have a legal move!!!
            break;

        }
        if(repeatLoop)
            continue;

        Move turnMove(whiteTurn, pieceToMoveMask, destinationSquareMask);
        turnMove.Print(*this);

        gameBoard.TryMove(turnMove);

        //change the turn
        whiteTurn = !whiteTurn;

        //do the game loop
        //std::cout << "Leaving... temporal..." << std::endl;
        //break;
    }
}

//it checks if the string input is in the correct format and also returns the cell index that the string references.
bool Game::validInput(const std::string& input, int& cellIndex) {
    cellIndex = -1;

    //exception case
    if(input == "SBit")
        return false;

    for (int i = 0; i < 56; i++) {
        if (input == boardMap[i])
        {
            cellIndex = i;
            return true;
        }
    }
    return false;
}

bool Game::isCorrectColorPiece(int cellIndex) const {
    if(whiteTurn && gameBoard.getAllWhiteBits()[cellIndex])
    {
        return true;
    }
    else if(!whiteTurn && gameBoard.getBitsBlack()[cellIndex])
    {
        return true;
    }
    return false;
}



//it checks if a cell index have any legal moves available and returns them in a bitset-mask
bool Game::areLegalMoves(int cellIndex, std::bitset<56> &legalMoveMask) {

    std::bitset<56> cellMask = indexToBitset(cellIndex);
    //cellMask[cellIndex] = 1;
    //Bitboard::setBitValueAtIndex(cellMask, 1, cellIndex);

    for (int i = 0; i < 4; ++i)
    {
        Bitboard::Direction direction = static_cast<Bitboard::Direction>(i);
        std::bitset<56> shiftedCell = cellMask;
        while (true)
        {
            shiftedCell = gameBoard.shiftDirection(shiftedCell, direction);
            //It checks if the shifted cell is in an empty space.
            auto legalMove = shiftedCell & gameBoard.getEmpty();
            if(legalMove.any())
                legalMoveMask |= legalMove;
            else
                break;
        }

    }
    return legalMoveMask.any();
}

std::bitset<56> Game::indexToBitset(int index) {
    if(index < 0 || index >=56)
    {
        std::cout << "Index out of range!"<< std::endl;
        return std::bitset<56>();
    }
    std::bitset<56> bitset;
    bitset[index] = 1;
    return bitset;
}

std::string Game::getSquareNameOfFirstBit(const std::bitset<56> &bitset) {

    int firstSetBit = -1;
    for (int i = 0; i < bitset.size(); i++) {
        if (bitset.test(i)) {
            firstSetBit = i;
            break;
        }
    }

    if(firstSetBit != -1)
        return boardMap[firstSetBit];
    else
        return "EMPTY";
}






