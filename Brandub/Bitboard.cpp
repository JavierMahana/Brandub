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
    // Tablero Normal
    /*
      for (int i = 0; i < bits_str.size(); i++) {

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
     */
  }


  bool Bitboard::TryMove(Move move)
  {
      bool validMove;

      if(move.isWhiteTurnMove())
      {
          validMove = ((move.getFromMove() & getAllWhiteBits()).any() && (move.getToMove() & getEmpty()).any());
      }
      else
      {
          validMove = ((move.getFromMove() & getBitsBlack()).any() && (move.getToMove() & getEmpty()).any());
      }

      if(!validMove)
      {
          return false;
      }

      if(move.isWhiteTurnMove())
      {
          //check if the move is a king move
          if((move.getFromMove() & getBitsKing()).any())
          {
              setBitsKing(move.getToMove());
          }
          else
          {
              setBitsWhite((getBitsWhite() xor move.getFromMove()) | move.getToMove());
          }
      }
      else
      {
          setBitsBlack((getBitsBlack() xor move.getFromMove()) | move.getToMove());
      }

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






