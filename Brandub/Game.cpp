//
// Created by Javier on 02-04-2023.
//

#include <iostream>
#include "Game.h"

Game::Game() {
    whiteTurn = true;
}


bool Game::hasWhiteWon() const{
    return false;
}

bool Game::hasBlackWon() const{
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


//hay que testear esta funcion!!! acá pueden surgir varios problemas
//it checks if a cell index have any legal moves available and returns them in a bitset-mask
bool Game::areLegalMoves(int cellIndex, std::bitset<56> &legalMoveMask) {

    std::bitset<56> cellMask;
    cellMask[cellIndex] = 1;

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
    gameBoard.Print();
    
    while(!hasBlackWon() && !hasWhiteWon())
    {
        std::string colorTurnString = whiteTurn ? "white" : "black";

        std::string input;

        //variables that are filled in the code
        int inputCellIndex;
        std::bitset<56> legalMoveMask;

        //Selection of the piece to move
        do {
            std::cout << "Enter the square of a " << colorTurnString << " piece that you want to move: ";
            std::cin >> input;

            if(!validInput(input, inputCellIndex))
            {
                std::cout << "Invalid input! it must follow the format of a letter then a number. Like:\"a1\"" << std::endl;
                continue;
            }

            if(!isCorrectColorPiece(inputCellIndex))
            {
                std::cout << "Invalid input! you must select square containing a " << colorTurnString << " piece" << std::endl;
                continue;
            }

            if(!areLegalMoves(inputCellIndex, legalMoveMask))
            {
                std::cout << "Invalid input! you must select square containing a piece that have legal moves" << std::endl;
                continue;
            }

            //The input is valid, so we leave the while loop.
            break;
            
        } while (true);


        std::cout << "You selected the: \"" << input << "\" square" << std::endl;
        std::cout << std::endl;
        std::cout << "Now enter the square that you want to move to, or \"q\" to select another piece." << std::endl;
        //do the game loop
        std::cout << "Leaving... temporal..." << std::endl;
        break;
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




