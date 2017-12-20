/*
  Andrew Efimov - aefimov1 - aefimov1@jhu.edu
  Adam Polevoy - apolevo1 - adampolevoy@gmail.com
  Daniel Sohn - dsohn3 - dsohn45@gmail.com
  
  Final Project
  601.220
  12/8/17
*/

#ifndef SPOOKYCHESS_GAME_H
#define SPOOKYCHESS_GAME_H

#include "ChessGame.h"

class SpookyChess : public ChessGame {
  
 public:

  // Creates new game in standard start-of-game state 
  SpookyChess();
  
  // Creates game with state indicated in specified file
  SpookyChess(const std::string filename);
  
  // Perform a move from the start Position to the end Position
  // The method returns an integer with the status
  // >= 0 is SUCCESS, < 0 is failure
  virtual int make_move(Position start, Position end) override;
  
  // Tracks ghost position. Public because we call assign this value in ChessGame.cpp loading game
  Position ghost;    
};

#endif // SPOOKYCHESS_GAME_H
