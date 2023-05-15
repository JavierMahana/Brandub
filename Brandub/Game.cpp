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
    srand(time(NULL));
}

bool Game::hasWhiteWon() const{
    return whiteWon;
}

bool Game::hasBlackWon() const{
    return blackWon;
}

void Game::gameLoop() {

    whiteWon = false;
    blackWon = false;

    bool blackIA = true;
    bool whiteIA = true;

    std::cout << "WELCOME TO BRANDUB" << std::endl;
    std::cout << "The white player has the objective of putting the king in one of the 4 corners of the board" << std::endl;
    std::cout << "And the black player has the objective of trapping the king around 4 of his pieces" << std::endl;
    std::cout << "To capture a piece you must place 2 of your own pieces in each side" << std::endl;
    std::cout << "Glosary" << std::endl;
    std::cout << "White piece:" << gameBoard.getWhiteChar() << std::endl;
    std::cout << "Black piece:" << gameBoard.getBlackChar() << std::endl;
    std::cout << "King:" << gameBoard.getKingChar() << std::endl;
    std::cout << std::endl;


    //se agrega la primera movida.
    //gameBoard.updateMoveHistory();
    
    while(!hasBlackWon() && !hasWhiteWon())
    {
        gameBoard.print();
        bool repeatLoop = false;

        std::string colorTurnString = whiteTurn ? "white" : "black";

        //IA Turn
       std::string input;

        //variables that are filled in the code
        int inputCellIndex;
        std::bitset<56> legalMovesMask;

        std::bitset<56> pieceToMoveMask;
        std::bitset<56> destinationSquareMask;

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

        //Try to move the piece
        gameBoard.tryMove(turnMove);

        /// AI FUNCTIONS DISABLED
        //Bitboard bestMoveBoard;
        //auto generatedMove = negamax(gameBoard,whiteTurn,2);
        //gameBoard = generatedMove.move;
        ///

        //Updates the moveHistory to check for draws
        gameBoard.updateMoveHistory();

        //change the turn
        whiteTurn = !whiteTurn;

        std::cout << whiteWon <<std::endl;

        //Check for a winner
        if(gameBoard.getPossibleWinner() == Bitboard::CellType::WHITE)
            whiteWon = true;
        else if(gameBoard.getPossibleWinner() == Bitboard::CellType::BLACK)
            blackWon = true;
        else if(gameBoard.isDraw()){
            break;
        }
    }


    std::cout << "END OF THE MATCH" << std::endl;
    gameBoard.print();
    if(whiteWon)
        std::cout << "WHITE WINS" << std::endl;
    else if(blackWon)
        std::cout << "BLACK WINS" << std::endl;
    else if(!blackWon && !whiteWon)
        std::cout << "DRAW!" << std::endl;
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

bool Game::areLegalMoves(std::bitset<56> cellMask, std::bitset<56> &legalMoveMask)
{
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
//it checks if a cell index have any legal moves available and returns them in a bitset-mask
bool Game::areLegalMoves(int cellIndex, std::bitset<56> &legalMoveMask) {

    std::bitset<56> cellMask = indexToBitset(cellIndex);

    return areLegalMoves(cellMask, legalMoveMask);
}

Game::searchMove Game::negamax(const Bitboard& board, bool isWhiteTurn, int depth) {

    Bitboard bestMove;
    Game::searchMove bestResult;

    // Check if the maximum depth has been reached or if the game is over
    auto boardState = board.getCurrentBoardState();
    if (depth == 0 || boardState != Bitboard::BoardState::PLAYING) {
        switch (boardState) {
            case Bitboard::BoardState::WHITE_WON:
                bestResult.score = INT_MAX;
                return bestResult;
            case Bitboard::BoardState::BLACK_WON:
                bestResult.score -INT_MAX;
                return bestResult;
            case Bitboard::BoardState::DRAW:
                bestResult.score = 0;
                return bestResult;
            case Bitboard::BoardState::PLAYING:
                bestResult.score = board.evaluateBoard(isWhiteTurn);
                return bestResult;
        }
    }

    float best_value = std::numeric_limits<float>::min();

    // Generate all possible moves for the current player
    auto moves = board.allLegalMoves(isWhiteTurn);

    while (!moves.empty()) {
        auto newBoard = moves.top();
        moves.pop();
        //newBoard.updateMoveHistory();
        // Make the bestMove and evaluate the resulting game state

        Game::searchMove newMove = negamax(newBoard, !isWhiteTurn, depth - 1);
        newMove.score *= -1; // equivalent of negative negamax

        // Update the best value and move if necessary
        if(best_value < newMove.score)
        {
            bestMove = newBoard;
            best_value = newMove.score;
        }

    }

    bestResult.score = best_value;
    bestResult.move = bestMove;

    return bestResult;
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

void Game::MoveRandom()
{
    int tryNumber = 0;
    int maxRandomTries = 9999;

    while (tryNumber <= maxRandomTries)
    {
        tryNumber++;

        //First we generate a random from move.
        std::bitset<56> randomFromMove = GetRandomCell();

        //Now we validate that there is a piece of the correct color there
        if(whiteTurn)
        {
            if((randomFromMove & gameBoard.getAllWhiteBits()).none())
            {
                continue;
            }
        }
        else
        {
            if((randomFromMove & gameBoard.getBitsBlack()).none())
            {
                continue;
            }
        }

        //and then if it has any legal moves.
        std::bitset<56> legalMovesMask;
        if(!areLegalMoves(randomFromMove, legalMovesMask))
        {
            continue;
        }

        //Now we generate a random To Move and we validate it
        std::bitset<56> randomToMove;
        do {
            randomToMove = GetRandomCell();
        } while ((randomToMove & legalMovesMask).none());

        Move randomMove(whiteTurn, randomFromMove, randomToMove);
        //Here we make the move!
        if(!gameBoard.tryMove(randomMove))
        {
            std::cout << "There was a problem with the random Move!" << std::endl;
        }
        randomMove.Print(*this);

        break;
    }
    if(tryNumber >= maxRandomTries)
    {
        std::cout << "There was a problem with the random Move! MAX tries reached" << std::endl;
    }
}

std::bitset<56> Game::GetRandomCell() {
    std::bitset<56> randomCell;

    int random_number = -1;  // Initialize to a value that is not a multiple of 8
    while (random_number % 8 == 0 || random_number == -1) {
        random_number = rand() % 56;  // Generate a random number between 0 and 56
    }

    randomCell = indexToBitset(random_number);
    return randomCell;
}









