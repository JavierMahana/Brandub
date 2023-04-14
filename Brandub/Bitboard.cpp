//
// Created by Javier on 01-04-2023.
//

#include <iostream>
#include <algorithm>
#include "Bitboard.h"
#include "Move.h"

Bitboard::Bitboard()
{
    setBitsWhite(Bitboard::START_WHITE);
    setBitsKing(Bitboard::START_KING);
    setBitsBlack(Bitboard::START_BLACK);
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

std::bitset<56> Bitboard::shiftUp(std::bitset<56> bitset) {
  return bitset << 8 & BOARD_MASK;
}

std::bitset<56> Bitboard::shiftDown(std::bitset<56> bitset) {
  return bitset >> 8 & BOARD_MASK;
}

std::bitset<56> Bitboard::shiftLeft(std::bitset<56> bitset) {
  return bitset << 1 & BOARD_MASK;
}

std::bitset<56> Bitboard::shiftRight(std::bitset<56> bitset) {
  return bitset >> 1 & BOARD_MASK;
}

std::bitset<56> Bitboard::shiftDirection(std::bitset<56> bitset, Bitboard::Direction direction) {
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

std::bitset<56> Bitboard::dilateBits(std::bitset<56> bitset, Bitboard::DirectionFlag dilateDirection) {

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

int Bitboard::evaluateBoard() {

    //evaluar balance de material
    //evaluar seguridad de piezas
    //evaluar seguridad del rey



    float materialBalance = getBitsWhite().count() * 2 - getBitsBlack().count();

    //evaluar posición del rey
    //revisa cuantos lados tiene libre para llegar a una esquina
    //si no tiene ninguna
    //uno revisa cuantos lados tiene libres para llegar a un borde.
    //si no tiene un borde
    float kingPosition;

    return 0;
}

//el peligro lo voy a cambiar para que sea primero un numero entero de rangos [-a, a]
int Bitboard::evaluateDangerInCell(std::bitset<56> bitset, bool isWhite) {

    int DangerScore = 0;
    bool mayBeEaten;

    for (int i = 0; i < 4; ++i){
        auto direction = static_cast<Bitboard::Direction>(i);
        std::bitset<56> shiftedCell = bitset;
        std::bitset<56> adyacentCell;
        mayBeEaten = false;

        while (true){
            shiftedCell = shiftDirection(shiftedCell, direction);

            // revisar si cuantos enemigos adyacentes (2)
            if((shiftedCell & getBitsBlack()).any() && isWhite){
                DangerScore += 1;
                mayBeEaten = true;
            }
            else if((shiftedCell & getAllWhiteBits()).any() && !isWhite){
                DangerScore += 1;
                mayBeEaten = true;
            }
            // else the adyacent cell is free. So we must check on the remaining directions of the adyacent cell
            else{
                adyacentCell = shiftedCell;
                //DangerScore += FUNCTION THAT CHECKS ENEMIES IN 4 DIRECTIONS(ADYACENT CELL, DIRECTION);
            }

            // revisar la opuesta a un enemigo (desocupada, ocupada por amigo)
            if(mayBeEaten){
                Bitboard::Direction oppositeDirection = direction;

                switch (direction) {
                    case Bitboard::Direction::LEFT:
                        oppositeDirection = Bitboard::Direction::RIGHT;
                        break;
                    case Bitboard::Direction::RIGHT:
                        oppositeDirection = Bitboard::Direction::LEFT;
                        break;
                    case Bitboard::Direction::UP:
                        oppositeDirection = Bitboard::Direction::DOWN;
                        break;
                    case Bitboard::Direction::DOWN:
                        oppositeDirection = Bitboard::Direction::UP;
                        break;
                    default: break;
                }

                shiftedCell = bitset;
                adyacentCell = shiftDirection(shiftedCell, oppositeDirection);

                if(isWhite){
                    if(isOnSight(shiftedCell, getAllWhiteBits(), oppositeDirection)){
                        DangerScore -= checkKingPosition(shiftedCell) ? 4 : 1;
                    }
                    if(isOnSight(shiftedCell, getBitsBlack(), oppositeDirection)){
                        DangerScore += checkKingPosition(shiftedCell) ? 4 : 1;
                    }
                }
                else{
                    if(isOnSight(shiftedCell, getBitsBlack(), oppositeDirection)){
                        DangerScore -= 1;
                    }
                    if(isOnSight(shiftedCell, getAllWhiteBits(), oppositeDirection)){
                        DangerScore += 1;
                    }
                }
            }
            break;
        }
    }
    return DangerScore;
}

int Bitboard::evaluateEatenDanger(std::bitset<56> bitset, Bitboard::Direction direction) {
    return 0;
}

int Bitboard::evaluateKingPosition(){

    auto kingPos = getBitsKing();
    return 0;
}

bool Bitboard::isOnSight(std::bitset<56> origin, std::bitset<56> target, Bitboard::Direction direction){
    //If the bidoboards see each other in the selected direction

    //bitboardA -> se mueve por los bits
    // 0O1 -> se mueve a la derecha y revisa si el otro bitset esta ahi
    // 000
    for (int i = 0; i < 7; ++i)
    {
        auto shiftedBits = shiftDirection(origin, direction);
        if((shiftedBits & target).any())
            return true;
    }
    return false;
}

bool Bitboard::checkForMate(){

    int counter = 0;
    std::bitset<56> shiftedCell = getBitsKing();

    //we check if the king is next to one of the blocked cells
    if(true){
        //caso de jaque en 4 lados en el centro del tablero
        for (int j = 0; j < 4; ++j){
            auto direction = static_cast<Bitboard::Direction>(j);
            shiftedCell = shiftDirection(shiftedCell, direction);
            //It checks if the shifted cell is in has an enemypiece.
            if((shiftedCell & getBitsBlack()).any() || (shiftedCell & getBlockedBits()).any()){
                counter++;
            }
        }
        return counter == 4;
    }
    // we check if it is eaten normally
    else{

    }


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
            std::cout << "ComeNegra" << std::endl;
        }
        else{
            setBitsWhite(getBitsWhite() xor enemyCell);
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
bool Bitboard::bitMustBeEaten(std::bitset<56> bitToCheck, bool isWhite)
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

Bitboard::Direction Bitboard::getOpositeDirection(Bitboard::Direction direction){

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
}


//
//int Bitboard::EvaluateDangerInCell(std::bitset<56> cell, bool isWhite) {
//    if(cell.count() == 0)
//    {
//        std::cout << "You can't use a cell that's empty" << std::endl;
//        return 0;
//    }
//    if(cell.count() > 1)
//    {
//        std::cout << "It may cause an error to have a cell of more than 1 cell" << std::endl;
//    }
//
//    int DangerScore = 0;
//    bool mayBeEaten;
//
//
//
//    for (int i = 0; i < 4; ++i){
//
//        auto direction = static_cast<Bitboard::Direction>(i);
//        auto neighborCell = shiftDirection(cell, direction);
//
//        mayBeEaten = false;
//
//        // revisar si cuantos enemigos adyacentes (2)
//        if((shiftedCell & getBitsBlack()).any() && isWhite){
//            DangerScore += 1;
//            mayBeEaten = true;
//        }
//        else if((shiftedCell & getAllWhiteBits()).any() && !isWhite){
//            DangerScore += 1;
//            mayBeEaten = true;
//        }
//            // else the adyacent cell is free. So we must check on the remaining directions of the adyacent cell
//        else{
//            adyacentCell = shiftedCell;
//            //DangerScore += FUNCTION THAT CHECKS ENEMIES IN 4 DIRECTIONS(ADYACENT CELL, DIRECTION);
//        }
//        // revisar la opuesta a un enemigo (desocupada, ocupada por amigo)
//        if(mayBeEaten){
//            Bitboard::Direction oppositeDirection = direction;
//
//            switch (direction) {
//                case Bitboard::Direction::LEFT:
//                    oppositeDirection = Bitboard::Direction::RIGHT;
//                    break;
//                case Bitboard::Direction::RIGHT:
//                    oppositeDirection = Bitboard::Direction::LEFT;
//                    break;
//                case Bitboard::Direction::UP:
//                    oppositeDirection = Bitboard::Direction::DOWN;
//                    break;
//                case Bitboard::Direction::DOWN:
//                    oppositeDirection = Bitboard::Direction::UP;
//                    break;
//                default: break;
//            }
//
//            shiftedCell = cellMask;
//            adyacentCell = shiftDirection(shiftedCell, oppositeDirection);
//            // si esta desocupada revisar si hay aliados o enemigos en las rectas
//            //DangerScore += FUNCTION THAT CHECKS ENEMIES IN 4 DIRECTIONS(ADYACENT CELL, DIRECTION);
//        }
//    }
//    return DangerScore;
//}



/*
// Funcion de evluación

* INSEGURIDAD

- Si hay una pieza enemiga alrededor revisar si hay un espacio contrario a esta disponible. Si el espacio existe
  ver si hay enemigos en las rectas.
- Ver si hay enemigos en las rectas de la posición actual de cada pieza.



*/

//  void Bitboard::setBitValueAtIndex(std::bitset<56>& bitset, bool value, int index)
//  {
//
//      if(index < 0 || index > 55)
//      {
//          //ERROR DE INPUT
//          return;
//      }
//      bitset[55-index] = value;
//
//  }
//
//  bool Bitboard::getBitValueAtIndex(std::bitset<56> bitset, int index) {
//
//      auto str = bitset.to_string();
//      std::reverse(str.begin(), str.end());
//      return str[index];
//  }






