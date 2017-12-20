/*
  Andrew Efimov - aefimov1 - aefimov1@jhu.edu
  Adam Polevoy - apolevo1 - adampolevoy@gmail.com
  Daniel Sohn - dsohn3 - dsohn45@gmail.com
  
  Final Project
  601.220
  12/8/17
*/

#ifndef KING_OF_THE_HILL_H
#define KING_OF_THE_HILL_H

#include "ChessGame.h"

class KingOfTheHill : public ChessGame {

public:

 KingOfTheHill() : ChessGame(){_gametype = KOTH;}

 KingOfTheHill(std::string filename);

 virtual int make_move(Position start, Position end) override;

};

#endif
