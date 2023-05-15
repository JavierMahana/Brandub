//
// Created by Javier on 01-04-2023.
//

#include <iostream>
#include <algorithm>
#include "Bitboard.h"
#include "Move.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"

Bitboard::Bitboard()
{
    setBitsWhite(Bitboard::START_WHITE);
    setBitsKing(Bitboard::START_KING);
    setBitsBlack(Bitboard::START_BLACK);
}

Bitboard::Bitboard(std::bitset<56> bitsWhite, std::bitset<56> bitsBlack, std::bitset<56> bitsKing)//, std::stack<Bitboard *> moveHistory)
{
    setBitsWhite(bitsWhite);
    setBitsKing(bitsBlack);
    setBitsBlack(bitsKing);
    //setHistory(moveHistory);
}

Bitboard::~Bitboard() {
    // Free the memory pointed to by the pointers in the move history
    while (!moveHistory.empty()) {
        delete moveHistory.top();
        moveHistory.pop();
    }
}



//TODO: Print the square names.
//   A B C D
//   _ _ _ _
// 1|
// 2|
// 3|
void Bitboard::print() {
    // bitsets are usually printed according to the way in which the byte values are stored in memory
    // therefore to print our visual representation we turn it into a string
    std::string bits_str = getFull().to_string();

    std::cout << "   A B C D E F G" <<std::endl;
    std::cout << "   _ _ _ _ _ _ _" <<std::endl;
    int row = 1;
    int j = 0;

    // Tablero Reverso
    for (int i = 55; i >= 0; --i) {

        if(j==0){
            std::cout<< row << "|";
        }
        if(i%8 != 0){

            std::cout << " ";

            if(getFull()[i] != 0 && bitsWhite[i] != 0){
                std::cout << getWhiteChar();
            }
            else if(getFull()[i] != 0 && bitsBlack[i] != 0){
                std::cout << getBlackChar();
            }
            else if(getFull()[i] != 0 && bitsKing[i] != 0){
                std::cout << getKingChar();
            }
            else if(getFull()[i] != 0 && getAllCorners() != 0){
                std::cout << getCornerChar();
            }
            else if(getFull()[i] != 0 && getBlockedBits()[i] != 0 && getBlockedBits()[i] != bitsKing[i]){
                std::cout << getBlockChar();
            }
            else{
                std::cout << getEmptyChar();
            }
        }

        j++;
        if(j==8){
            j = 0;
            row++;
            std::cout<<std::endl;
        }
    }
  }

void Bitboard::print(std::bitset<56> bitsetToPrint) {
    // bitsets are usually printed according to the way in which the byte values are stored in memory
    // therefore to print our visual representation we turn it into a string
    std::string bits_str = bitsetToPrint.to_string();

    std::cout << "   A B C D E F G" <<std::endl;
    std::cout << "   _ _ _ _ _ _ _" <<std::endl;
    int row = 1;
    int j = 0;

    // Tablero Reverso
    for (int i = 55; i >= 0; --i) {

        if(j==0){
            std::cout<< row << "|";
        }
        if(i%8 != 0){
            std::cout << " ";
            std::cout << bitsetToPrint[i];
        }

        j++;
        if(j==8){
            j = 0;
            row++;
            std::cout<<std::endl;
        }

    }
}

bool Bitboard::tryMove(Move move){

  bool validMove;
  auto fromMove = move.getFromMove();
  auto toMove = move.getToMove();

  if(move.isWhiteTurnMove()){
      if(checkKingPosition(fromMove)){
          validMove = ((fromMove & getAllWhiteBits()).any() && (toMove & getEmpty()).any());
      }
      else{
          validMove = ((fromMove & getAllWhiteBits()).any() && (toMove & getEmpty()).any()
                       && !(toMove & getBlockedBits()).any());
      }
  }
  else{
      validMove = ((fromMove & getBitsBlack()).any() && (toMove & getEmpty()).any()
              && !(toMove & getBlockedBits()).any());
  }

  if(!validMove){
      return false;
  }

  if(move.isWhiteTurnMove()){
      //check if the move is a king move
      if((fromMove & getBitsKing()).any()){
          // makes the move
          setBitsKing(toMove);
      }
      else{
          // makes the move
          setBitsWhite((getBitsWhite() xor fromMove) | toMove);
      }
  }
  else{
      // makes the move
      setBitsBlack((getBitsBlack() xor fromMove) | move.getToMove());
  }

  //check if the piece is eats something
  checkEat(move);

  return validMove;
}

const std::bitset<56> &Bitboard::getBitsWhite() const {
  return bitsWhite;
}

void Bitboard::setBitsWhite(const std::bitset<56> &bitsWhite) {
  Bitboard::bitsWhite = bitsWhite;
}

const std::bitset<56> &Bitboard::getBitsBlack() const {
  return bitsBlack;
}

void Bitboard::setBitsBlack(const std::bitset<56> &bitsBlack) {
  Bitboard::bitsBlack = bitsBlack;
}

const std::bitset<56> &Bitboard::getBitsKing() const {
  return bitsKing;
}

void Bitboard::setBitsKing(const std::bitset<56> &bitsKing) {
  Bitboard::bitsKing = bitsKing;
}

const std::stack<Bitboard*> &Bitboard::getHistory() const {
    return moveHistory;
}

void Bitboard::setHistory(std::stack<Bitboard*> newHistory){
    moveHistory = newHistory;
}

std::bitset<56> Bitboard::shiftUp(std::bitset<56> bitset) const {
  return bitset << 8 & BOARD_MASK;
}

std::bitset<56> Bitboard::shiftDown(std::bitset<56> bitset) const {
  return bitset >> 8 & BOARD_MASK;
}

std::bitset<56> Bitboard::shiftLeft(std::bitset<56> bitset) const {
  return bitset << 1 & BOARD_MASK;
}

std::bitset<56> Bitboard::shiftRight(std::bitset<56> bitset) const {
  return bitset >> 1 & BOARD_MASK;
}

std::bitset<56> Bitboard::shiftDirection(std::bitset<56> bitset, Bitboard::Direction direction) const {
  switch (direction)
  {
      case Direction::LEFT:
          return shiftLeft(bitset);
      case Direction::RIGHT:
          return shiftRight(bitset);
      case Direction::UP:
          return shiftUp(bitset);
      case Direction::DOWN:
          return shiftDown(bitset);
      default:
          std::bitset<56> empty;
          return empty;
  }

}

std::bitset<56> Bitboard::dilateBits(std::bitset<56> bitset, Bitboard::DirectionFlag dilateDirection) const {

  std::bitset<56> result = bitset;

  if(static_cast<unsigned int>(dilateDirection) & static_cast<unsigned int>(DirectionFlag::RIGHT))
  {
      result |= shiftRight(bitset);
  }

  if(static_cast<unsigned int>(dilateDirection) & static_cast<unsigned int>(DirectionFlag::LEFT))
  {
      result |= shiftLeft(bitset);
  }

  if(static_cast<unsigned int>(dilateDirection) & static_cast<unsigned int>(DirectionFlag::UP))
  {
      result |= shiftUp(bitset);
  }

  if(static_cast<unsigned int>(dilateDirection) & static_cast<unsigned int>(DirectionFlag::DOWN))
  {
      result |= shiftDown(bitset);
  }

  return result;
}

std::bitset<56> Bitboard::erodeBits(std::bitset<56> bitset, Bitboard::DirectionFlag erodeDirection) {

  std::bitset<56> result = bitset;

  if(static_cast<unsigned int>(erodeDirection) & static_cast<unsigned int>(DirectionFlag::RIGHT))
  {
      result &= shiftRight(bitset);
  }

  if(static_cast<unsigned int>(erodeDirection) & static_cast<unsigned int>(DirectionFlag::LEFT))
  {
      result &= shiftLeft(bitset);
  }

  if(static_cast<unsigned int>(erodeDirection) & static_cast<unsigned int>(DirectionFlag::UP))
  {
      result &= shiftUp(bitset);
  }

  if(static_cast<unsigned int>(erodeDirection) & static_cast<unsigned int>(DirectionFlag::DOWN))
  {
      result &= shiftDown(bitset);
  }

  return result;
}

float Bitboard::evaluateBoard(bool isWhiteTurn) const {

    //the material of a piece is the main comparative when evaluating
    float materialBalance = BASE_PIECE_VALUE * (getBitsWhite().count() * WHITE_PIECES_FACTOR - getBitsBlack().count());

    //the king safety value is the sum of 2 values
    //FleeChances (Positive) + Position (positive) + KingPositionDanger (Negative)
    float kingValue = evaluateKingMobility(isWhiteTurn) + evaluateKingDanger(isWhiteTurn) + evaluateKingPosition(isWhiteTurn);

    //it sums the values of the danger of the positions
    float piecesPositionEval = evaluatePiecesDanger(isWhiteTurn);

    return materialBalance + kingValue + piecesPositionEval;
}

float Bitboard::evaluatePiecesDanger(bool isWhiteTurn) const {
    float piecesPositionEval = 0;

    //white adds negative value because the danger is a bad evaluation.

    auto whiteBits = getIndividualBitsOfBitset(getBitsWhite());
    while(!whiteBits.empty())
    {
        auto bit = whiteBits.top();
        whiteBits.pop();
        piecesPositionEval -= evaluateDangerInCell(isWhiteTurn, bit, true);
    }

    piecesPositionEval *=WHITE_PIECES_FACTOR;
    auto blackBits = getIndividualBitsOfBitset(getBitsBlack());
    while(!blackBits.empty())
    {
        auto bit = blackBits.top();
        blackBits.pop();
        piecesPositionEval += evaluateDangerInCell(isWhiteTurn, bit, false);
    }

    return piecesPositionEval;
}

//Evaluates the king position. It is always a negative value
float Bitboard::evaluateKingDanger(bool isWhiteTurn) const {

    auto kingPos = getBitsKing();

    //If the king is in the center
    //or next to center
    if((dilateBits(kingPos, DirectionFlag::ALL_DIRECTIONS) & getBlockedBits()).any())
    {
        return - evaluateDangerInCellKing(isWhiteTurn,kingPos, true);
    }
    //is in other case
    else
    {
        int cellDanger = evaluateDangerInCell(isWhiteTurn,kingPos, true);
        if(cellDanger == DANGER_ENEMY_MAY_EAT)
        {
            if(!isWhiteTurn)
                return - INT_MAX;
            else
                return - INT_MAX/2;
        }
        else
        {
            return - DANGER_KING_FACTOR * cellDanger;
        }
    }
}
//
float Bitboard::evaluateKingMobility(bool isWhiteTurn) const {
    auto kingPos = getBitsKing();
    std::bitset<56> shiftedCell;
    std::bitset<56> adyacentCell;

    float MovilityScore = 0;

    // We check on all four directions
    for (int i = 0; i < 4; ++i){
        auto direction = static_cast<Bitboard::Direction>(i);
        shiftedCell = kingPos;

        shiftedCell = shiftDirection(shiftedCell, direction);
        auto cellType = getCellType(shiftedCell);
        // Check if adyacent is empty
        if(cellType == CellType::EMPTY){
            MovilityScore += KING_MOBILITY_EMPTY_ADYACENT;
        }
    }

    return MovilityScore;

}

float Bitboard::evaluateKingPosition(bool isWhiteTurn) const {
    auto kingPos = getBitsKing();

    bool isOnBorder = (kingPos & BORDER).any();
    int kingVictoryScore = 0;

    if(isOnBorder)
    {
        kingVictoryScore = KING_POSITION_ON_BORDER;
    }

    // Check chance to reach all 4 corners
    auto shiftedCell = kingPos;
    for (int i = 0; i < 4; ++i){
        auto direction = static_cast<Bitboard::Direction>(i);
        if(pieceOnSight(kingPos, direction) == CellType::CORNER)
        {

            kingVictoryScore = KING_POSITION_ON_SIGHT;
            //White/King on turn may achieve victory
            if(isWhiteTurn){
                return kingVictoryScore = INT_MAX - 1;
            }

            //Black on turn must stop victory from being achieved
            else{
                //we find blacks from all the empty spaces between the king and the corner
                while(true){
                    auto shiftedCell = shiftDirection(kingPos,direction);
                    if(getCellType(shiftedCell) != CellType::EMPTY){
                        break;
                    }
                    // we check on the other directions for a black piece
                    for (int j = 0; j < 4; ++j){
                        auto newDirection = static_cast<Bitboard::Direction>(j);
                        if(direction != newDirection){
                            // there is black piece that may block the king's advance
                            if(pieceOnSight(shiftedCell, direction) == CellType::BLACK){
                                return kingVictoryScore = BLACK_CAN_BLOCK_KING;
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}

//this function is to evaluate the special king cases: in the center square or adyacent to it.
//It weighted with the correct values buy in positive.
float Bitboard::evaluateDangerInCellKing(bool isWhiteTurn, std::bitset<56> bitset, bool isWhite) const {
    std::bitset<56> cellMask = bitset;
    std::bitset<56> shiftedCell;
    std::bitset<56> adyacentCell;


    float DangerScore = 0;

    int sidesWithDanger = 0;

    // We check on all four directions
    for (int i = 0; i < 4; ++i){
        auto direction = static_cast<Bitboard::Direction>(i);
        shiftedCell = cellMask;
        shiftedCell = shiftDirection(shiftedCell, direction);

        auto neighborCellType = getCellType(shiftedCell);

        //if the neighbor cell is an enemy/corner/block we mark that we may be eaten and add a danger score.
        if(neighborCellType == CellType::EMPTY)
            continue;
        else if(neighborCellType == CellType::CORNER || neighborCellType == CellType::BLOCK){
            DangerScore += DANGER_ENEMY_ADYACENT;
            sidesWithDanger++;
        }
        else
        {
            if(isWhite && neighborCellType == CellType::BLACK){
                DangerScore += DANGER_ENEMY_ADYACENT;
                sidesWithDanger++;
            }
            else if(isWhite && neighborCellType == CellType::WHITE){
                DangerScore += DANGER_ALLY_ADYACENT;
            }
            else if (!isWhite && neighborCellType == CellType::WHITE){
                DangerScore += DANGER_ENEMY_ADYACENT;
                sidesWithDanger++;
            }
        }
    }

    if(sidesWithDanger >= 3)
    {
        for (int i = 0; i < 4; ++i) {
            auto direction = static_cast<Bitboard::Direction>(i);
            shiftedCell = cellMask;
            shiftedCell = shiftDirection(shiftedCell, direction);

            auto neighborCellType = getCellType(shiftedCell);

            //if the opposite direction of the enemy is not empty there is no danger to be eaten.
            if(neighborCellType == CellType::EMPTY){
                for (int j = 0; j < 4; ++j){
                    auto newDirection = static_cast<Bitboard::Direction>(j);

                    auto otherPiece = pieceOnSight(shiftedCell, newDirection);
                    if(isWhite && otherPiece == CellType::BLACK){
                        std::cout << " ** Danger Evaluated: " << DANGER_ENEMY_MAY_EAT_KING << std::endl;
                        if(!isWhiteTurn)
                            return DANGER_ENEMY_MAY_EAT_KING;
                        else
                            return DANGER_ENEMY_MAY_EAT_KING/2;
                    }
                    else if(!isWhite && (otherPiece == CellType::WHITE || otherPiece == CellType::KING)){
                        std::cout << " ** Danger Evaluated: " << DANGER_ENEMY_MAY_EAT_KING << std::endl;
                        return DANGER_ENEMY_MAY_EAT_KING;
                    }
                }
            }
        }
    }

    std::cout <<" ** Danger Evaluated: " << DangerScore << std::endl;
    return DangerScore * DANGER_KING_FACTOR;
}

//el peligro lo voy a cambiar para que sea primero un numero entero de rangos [-a, a]
float Bitboard::evaluateDangerInCell(bool isWhiteTurn, std::bitset<56> bitset, bool isWhite) const {
    std::bitset<56> cellMask = bitset;
    std::bitset<56> shiftedCell;
    std::bitset<56> adyacentCell;


    float DangerScore = 0;
    bool mayBeEaten;

    // We check on all four directions
    for (int i = 0; i < 4; ++i){
        auto direction = static_cast<Bitboard::Direction>(i);
        shiftedCell = cellMask;
        adyacentCell = cellMask;
        mayBeEaten = false;

        shiftedCell = shiftDirection(shiftedCell, direction);

        auto neighborCellType = getCellType(shiftedCell);

        //if the neighbor cell is an enemy/corner/block we mark that we may be eaten and add a danger score.
        if(neighborCellType == CellType::EMPTY)
            continue;
        else if(neighborCellType == CellType::CORNER || neighborCellType == CellType::BLOCK){
            DangerScore += DANGER_ENEMY_ADYACENT;
            mayBeEaten = true;
        }
        else
        {
            if(isWhite && neighborCellType == CellType::BLACK){
                DangerScore += DANGER_ENEMY_ADYACENT;
                mayBeEaten = true;
            }
            else if (!isWhite && neighborCellType == CellType::WHITE || neighborCellType == CellType::KING ){
                DangerScore += DANGER_ENEMY_ADYACENT;
                mayBeEaten = true;
            }
        }


        //We check on the opposite direction for a piece
        if(mayBeEaten){

            Bitboard::Direction oppositeDirection = getOpositeDirection(direction);
            adyacentCell = shiftDirection(cellMask, oppositeDirection);

            auto adyacentCellType = getCellType(adyacentCell);
            //if the opposite direction of the enemy is not empty there is no danger to be eaten.
            if(adyacentCellType != CellType::EMPTY){
                DangerScore -= DANGER_ENEMY_ADYACENT;
                continue;
            }

            //if it is empty we check to see if the empty cell has an enemy on sight.
            //If so we return the max danger of the cell that is equal to the const of DANGER_ENEMY_MAY_EAT
            for (int j = 0; j < 4; ++j){
                auto newDirection = static_cast<Bitboard::Direction>(j);

                auto otherPiece = pieceOnSight(adyacentCell, newDirection);
                if(isWhite && otherPiece == CellType::BLACK){
                    std::cout << " ** Danger Evaluated: " << DANGER_ENEMY_MAY_EAT << std::endl;
                    return DANGER_ENEMY_MAY_EAT;
                }
                else if(!isWhite && (otherPiece == CellType::WHITE || otherPiece == CellType::KING)){
                    std::cout << " ** Danger Evaluated: " << DANGER_ENEMY_MAY_EAT << std::endl;
                    return DANGER_ENEMY_MAY_EAT;
                }
            }
        }
    }
    std::cout <<" ** Danger Evaluated: " << DangerScore << std::endl;
    return DangerScore;
}

//fucnión que revisa que es lo primero que hay en una dirección
Bitboard::CellType Bitboard::pieceOnSight(std::bitset<56> origin, Bitboard::Direction direction) const {
    //If the bidoboards see each other in the selected direction

    //bitboardA -> se mueve por los bits
    // 0O1 -> se mueve a la derecha y revisa si el otro bitset esta ahi
    // 000
    for (int i = 0; i < 7; ++i)
    {
        auto shiftedBits = shiftDirection(origin, direction);

        if((shiftedBits & BOARD_MASK).none())
            return CellType::EMPTY;

        auto cellType = getCellType(shiftedBits);
        if(cellType != CellType::EMPTY) {
            return cellType;
        }
    }
    return CellType::EMPTY;
}

void Bitboard::checkEat(Move move)  {
    std::bitset<56> toMove = move.getToMove();
    std::bitset<56> enemyCell;
    bool isWhite = move.isWhiteTurnMove();

    Direction directionToCapture;

    if(bitGenerateCapture(toMove, isWhite, directionToCapture))
    {
        enemyCell = shiftDirection(toMove, directionToCapture);
        std::cout << "Se debe comer una pieza!" << std::endl;

        if(isWhite){
            setBitsBlack(getBitsBlack() xor enemyCell);
            clearMoveHistory();
            std::cout << "ComeNegra" << std::endl;
        }
        else{
            setBitsWhite(getBitsWhite() xor enemyCell);
            clearMoveHistory();
            std::cout << "ComeBlanca" << std::endl;
        }
    }
}

bool Bitboard::isAllyInCell(std::bitset<56> cellToCheck, bool isWhite)
{
    if(isWhite){
        if((cellToCheck & getAllWhiteBits()).any()){
            return true;
        }
    }
    else{
        if((cellToCheck & getBitsBlack()).any()){
            return true;
        }
    }

    return false;
}

bool Bitboard::bitGenerateCapture(std::bitset<56> bitToCheck, bool isWhite, Direction& captureDirection)
{
    for (int i = 0; i < 4; ++i)
    {
        auto direction = static_cast<Bitboard::Direction>(i);
        std::bitset<56> shiftedBit = bitToCheck;
        while (true) {

            shiftedBit = shiftDirection(shiftedBit, direction);

            //We set the enemyPiece.
            std::bitset<56> enemyPiece;

            if (isWhite && (shiftedBit & getBitsBlack()).any())
                enemyPiece = shiftedBit;
            else if (!isWhite && (shiftedBit & getAllWhiteBits()).any())
                enemyPiece = shiftedBit;

            if (enemyPiece.none())
                break;

            shiftedBit = shiftDirection(shiftedBit, direction);

            if(isAllyInCell(shiftedBit, isWhite) || (shiftedBit & getBlockedBits()).any())
            {
                captureDirection = direction;
                return true;
            }
        }
    }

    captureDirection = static_cast<Bitboard::Direction>(0);
    return false;
}

//Yo lo que queria hacer era que revisas el bit y ves si tiene a los 2 lados un enemigo.
bool Bitboard::bitMustBeEaten(std::bitset<56> bitToCheck, bool isWhite) const
{
    for (int i = 0; i < 4; ++i)
    {
        auto direction = static_cast<Bitboard::Direction>(i);
        std::bitset<56> shiftedBit = bitToCheck;
        while (true) {

            shiftedBit = shiftDirection(shiftedBit, direction);

            //We set the enemyPiece.
            std::bitset<56> enemyPiece;

            if (isWhite && (shiftedBit & getBitsBlack()).any())
                enemyPiece = shiftedBit;
            else if (!isWhite && (shiftedBit & getAllWhiteBits()).any())
                enemyPiece = shiftedBit;

            if (enemyPiece.none())
                break;

            //there is an enemy
            //we check the opposite direction

            shiftedBit = shiftDirection(bitToCheck, getOpositeDirection(direction));

            if (isWhite && (shiftedBit & getBitsBlack()).any())
                enemyPiece = shiftedBit;
            else if (!isWhite && (shiftedBit & getAllWhiteBits()).any())
                enemyPiece = shiftedBit;

            if (enemyPiece.none())
                break;

            return true;
        }
    }
    return false;
}

Bitboard::Direction Bitboard::getOpositeDirection(Bitboard::Direction direction) const {

    switch (direction) {
        case Direction::LEFT:
            return Direction::RIGHT;
        case Direction::RIGHT:
            return Direction::LEFT;
        case Direction::UP:
            return Direction::DOWN;
        case Direction::DOWN:
            return Direction::UP;
    }
    return Direction::UP;
}

//estan 2 bits a la vista sin otra pieza entre medio?
bool Bitboard::bitsOnSight(std::bitset<56> a, std::bitset<56> b) {
    for (int i = 0; i < 4; ++i){
        auto direction = static_cast<Bitboard::Direction>(i);

        for (int i = 0; i < 7; ++i)
        {
            auto shiftedBits = shiftDirection(a, direction);
            if((shiftedBits & BOARD_MASK).none())
                break;

            auto cellType = getCellType(shiftedBits);
            if(cellType != CellType::EMPTY)
                break;

            if((shiftedBits & b).any())
                return  true;

        }
    }
    return false;
}

Bitboard::CellType Bitboard::getCellType(std::bitset<56> bit) const {

    if(bit.count() != 1)
    {
        std::cout << "WARNING: you are getting the cell type of multiple bits at the same time. It can generate errors!" <<std::endl;
    }

    if((bit & BOARD_MASK).none())
        return CellType::EMPTY;

    if((bit & getAllCorners()).any()){
        return CellType::CORNER;
    }

    if((bit & getFull()).any()){
        if((bit & getBitsKing()).any()){
            return CellType::KING;
        }
        if((bit & getBitsBlack()).any()){
            return CellType::BLACK;
        }
        if((bit & getBitsWhite()).any()){
            return CellType::WHITE;
        }
    }
    if((bit & getBlockedBits()).any()){
        return CellType::BLOCK;
    }


    return Bitboard::CellType::EMPTY;
}

std::stack<Bitboard> Bitboard::allLegalMoves(bool isWhite) const {
    // Stack that will contain the stacks of each piece's legal moves
    std::stack<Bitboard> stack;
    std::stack<std::bitset<56>> colorPlayer;

    if(isWhite)
        colorPlayer = getIndividualBitsOfBitset(getAllWhiteBits());
    else
        colorPlayer = getIndividualBitsOfBitset(getBitsBlack());

    while(!colorPlayer.empty()){
        auto topPiece = colorPlayer.top();
        colorPlayer.pop();

        auto pieceGet = getLegalMoves(topPiece, isWhite);

        while(!pieceGet.empty()) {
            auto topMove = pieceGet.top();
            pieceGet.pop();
            stack.push(topMove);
        }
    }

    return stack;
}

std::stack<Bitboard> Bitboard::getLegalMoves(std::bitset<56> piece, bool isWhite) const {
    // stack for all the legal moves of the piece
    std::stack<Bitboard> stack;
    std::bitset<56> shiftedCell;


    PieceType pieceType;
    if(isWhite){
        if((piece & getBitsKing()).any())
            pieceType = PieceType::KING;
        else
            pieceType = PieceType::WHITE;
    }
    else
        pieceType = PieceType::BLACK;


    // we check on all 4 directions for empty cells
    for (int i = 0; i < 4; ++i){
        auto direction = static_cast<Bitboard::Direction>(i);
        shiftedCell = piece;
        for (int j = 0; j < 7; ++j)
        {
            // if it is valid and empty we add it to the stack,
            // else if it is occupied we stop searching that direction
            shiftedCell = shiftDirection(shiftedCell, direction);
            if((shiftedCell & BOARD_MASK).any())
            {
                auto cellType = getCellType(shiftedCell);
                if(cellType == CellType::EMPTY){

                    std::bitset<56> copyBitset;
                    Bitboard boardWithMove;
                                        //It must create a new Bitboard with the move generated.
                    switch (pieceType) {
                        case PieceType::WHITE:
                            copyBitset = getBitsWhite();
                            //we remove the piece bit from the copy bitset
                            copyBitset ^= piece;
                            copyBitset |= shiftedCell;
                            boardWithMove = Bitboard(copyBitset, std::bitset<56>(), std::bitset<56>());//, moveHistory());

                            break;
                        case PieceType::BLACK:
                            copyBitset = getBitsBlack();
                            //we remove the piece bit from the copy bitset
                            copyBitset ^= piece;
                            copyBitset |= shiftedCell;
                            boardWithMove = Bitboard(std::bitset<56>(), copyBitset, std::bitset<56>());//, moveHistory);

                            break;
                        case PieceType::KING:
                            copyBitset = getBitsKing();
                            //we remove the piece bit from the copy bitset
                            copyBitset ^= piece;
                            copyBitset |= shiftedCell;
                            boardWithMove = Bitboard(std::bitset<56>(), std::bitset<56>(), copyBitset);//, moveHistory);

                            break;
                    }
                    // And add it to the LEGAL MOVES!
                    stack.push(boardWithMove);
                }
                else{
                    break;
                }
            }
            else{
                break;
            }
        }
    }

    return stack;

}

bool Bitboard::isDraw() const {

    auto stackCopy = moveHistory;

    while (!stackCopy.empty()) {
        if(stackCopy.top() == this){
            return true;
        }
        stackCopy.pop();
    }
    return false;
}

void Bitboard::updateMoveHistory(){
    auto latestBoard = this;
    moveHistory.push(this);
}

void Bitboard::clearMoveHistory(){
    std::stack<Bitboard*> emptyStack;
    // we swap a new empty stack for the current moveHistory - optimal way to clear a stack
    emptyStack.swap(moveHistory);
}

std::stack<std::bitset<56>> Bitboard::getIndividualBitsOfBitset(std::bitset<56> bitset) const {

    std::stack<std::bitset<56>> arrayOfIndividualBits;

    for(int i = 0; i < 56; i++){
        if(bitset[i]){
            std::bitset<56> cellMask;
            cellMask.set(i,true);
            arrayOfIndividualBits.push(cellMask);
        }
    }
    return arrayOfIndividualBits;
}

Bitboard::CellType Bitboard::getPossibleWinner(){
    // if king is dead
    if(getBitsKing().any()){
        return CellType::BLACK;
    }
    else if((getBitsKing() & getAllCorners()).any()){
        return CellType::WHITE;
    }

    return CellType::EMPTY;
}

Bitboard::BoardState Bitboard::getCurrentBoardState() const{
    // if king is dead
    if(getBitsKing().none()){
        return Bitboard::BoardState::BLACK_WON;
    }
    if((getBitsKing() & getAllCorners()).any()){
        return Bitboard::BoardState::WHITE_WON;
    }
    else if(isDraw())
        return Bitboard::BoardState::DRAW;

    return Bitboard::BoardState::PLAYING;
}












#pragma clang diagnostic pop