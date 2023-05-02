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
void Bitboard::Print() {
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

void Bitboard::Print(std::bitset<56> bitsetToPrint) {
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

/// CHECK KING CHECK
void Bitboard::CheckEat(Move move)  {
std::bitset<56> cellMask = move.getToMove();

    for (int i = 0; i < 4; ++i)
    {
        auto direction = static_cast<Bitboard::Direction>(i);
        std::bitset<56> shiftedCell = cellMask;
        while (true){

            std::bitset<56> enemyPiece;
            shiftedCell = shiftDirection(shiftedCell, direction);
            //It checks if the shifted cell is in has an enemypiece.
            if((shiftedCell & getBitsBlack()).any() && move.isWhiteTurnMove()){
                enemyPiece = shiftedCell;
            }
            // Here check for the King because he belongs to the White Pieces
            else if((shiftedCell & getAllWhiteBits()).any() && !move.isWhiteTurnMove())
                enemyPiece = shiftedCell;

            if(enemyPiece.none()){
                break;
            }

            // checks if there is an ally in the same direction
            if(Bitboard::CheckAlly(enemyPiece, direction, move.isWhiteTurnMove())){

                if(move.isWhiteTurnMove()){
                    setBitsBlack(getBitsBlack() xor enemyPiece );
                    std::cout << "Blanca come Negra" << std::endl;
                }
                else{
                    setBitsWhite(getBitsWhite() xor enemyPiece );
                    std::cout << "Negra come Blanca" << std::endl;
                }
                break;
            }
        }
    }
}

bool Bitboard::CheckAlly(std::bitset<56> bitset, Bitboard::Direction direction, bool isWhite){

    std::bitset<56> shiftedCell = shiftDirection(bitset, direction);
    if(isWhite){
        if((shiftedCell & getAllWhiteBits()).any()){
            return true;
        }
    }
    else{
        if((shiftedCell & getBitsBlack()).any()){
            return true;
        }
    }

    return false;
}

  bool Bitboard::TryMove(Move move)
  {
      bool validMove;

      if(move.isWhiteTurnMove()){
          validMove = ((move.getFromMove() & getAllWhiteBits()).any() && (move.getToMove() & getEmpty()).any());
      }
      else{
          validMove = ((move.getFromMove() & getBitsBlack()).any() && (move.getToMove() & getEmpty()).any());
      }

      if(!validMove){
          return false;
      }

      if(move.isWhiteTurnMove()){
          //check if the move is a king move
          if((move.getFromMove() & getBitsKing()).any()){
              // makes the move
              setBitsKing(move.getToMove());
          }
          else{
              // makes the move
              setBitsWhite((getBitsWhite() xor move.getFromMove()) | move.getToMove());
          }
          move.getToMove();
      }
      else{
          // makes the move
          setBitsBlack((getBitsBlack() xor move.getFromMove()) | move.getToMove());
          move.getToMove();
      }

      //check if the piece is eats something
      CheckEat(move);

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

float Bitboard::EvaluateBoard() {

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

float Bitboard::EvaluateDanger(std::bitset<56> bitset, bool isWhite) {

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

        while (true){
            shiftedCell = shiftDirection(shiftedCell, direction);

            // Check if adyacent is empty
            if((shiftedCell & getEmpty()).any()){
                adyacentCell = shiftedCell;
                for (int j = 0; j < 4; ++j){
                    auto adyacentDirection = static_cast<Bitboard::Direction>(j);
                    DangerScore += PieceOnSight(adyacentCell, adyacentDirection, isWhite);
                }
            }
            // Check for oponent in adyacent cells
            else{
                if((shiftedCell & getBitsBlack()).any() && isWhite){
                    DangerScore += 1;
                    mayBeEaten = true;
                }
                else if((shiftedCell & getAllWhiteBits()).any() && !isWhite){
                    DangerScore += 1;
                    mayBeEaten = true;
                }
            }

            // We check on the opposite direction for a piece
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

                shiftedCell = shiftDirection(cellMask, oppositeDirection);
                for (int j = 0; j < 4; ++j){
                    auto newDirection = static_cast<Bitboard::Direction>(j);
                    if(newDirection != oppositeDirection){
                        DangerScore += PieceOnSight(shiftedCell, newDirection, isWhite);
                    }
                }
            }
            break;
        }
    }
    std::cout <<" ** Danger Evaluated: " << DangerScore << std::endl;
    return DangerScore;
}

float Bitboard::EvaluateEatenDanger(std::bitset<56> bitset, Bitboard::Direction direction) {
    return 0;
}

float Bitboard::EvaluateKingPosition(){

    auto kingPos = getBitsKing();
    return 0;
}

float Bitboard::PieceOnSight(std::bitset<56> cellPosition, Bitboard::Direction direction, bool isWhite){

    std::bitset<56> shiftedCell = cellPosition;
    for (int i = 0; i < 7; ++i)
    {
        shiftedCell = shiftDirection(shiftedCell, direction);

        if(isWhite){
            if((shiftedCell & getBitsBlack()).any()){
                return 1;
            }

            else if((shiftedCell & getBitsWhite()).any()) return 0;
        }
        else{
            if((shiftedCell & getBitsWhite()).any()){
                return 1;
            }
            else if((shiftedCell & getBitsBlack()).any()) return 0;
        }
    }

    return 0;
}

bool Bitboard::CheckForMate(){

    std::bitset<56> cellMask = getBitsKing();
    int counter = 0;

    for(int i = 0; i < 56; i++){
        if(cellMask.test(i)){
            std::bitset<56> shiftedCell = cellMask.test(i);
            for (int j = 0; j < 4; ++j){
                auto direction = static_cast<Bitboard::Direction>(j);
                shiftedCell = shiftDirection(shiftedCell, direction);
                //It checks if the shifted cell is in has an enemypiece.
                if((shiftedCell & getBitsBlack()).any()){
                    counter++;
                }
            }
        }
    }
    return counter == 4;
}



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






