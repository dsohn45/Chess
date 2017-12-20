/*
  Andrew Efimov - aefimov1 - aefimov1@jhu.edu
  Adam Polevoy - apolevo1 - adampolevoy@gmail.com
  Daniel Sohn - dsohn3 - dsohn45@gmail.com
  
  Final Project
  601.220
  12/8/17
*/

#include "KingOfTheHill.h"

KingOfTheHill::KingOfTheHill(const std::string filename) {
  
  _gametype = KOTH;

  // Because the base class constructor Chess() gets called, we need
  // this for loop to clear the board of the default initialized pieces
  for(unsigned int i = 0; i < _width * _height; i++) {
    if (_pieces[i] != nullptr) delete _pieces[i];
    _pieces[i] = nullptr; 
  }
  
  int status = KingOfTheHill::load_game(filename);
  if(status == LOAD_FAILURE) throw std::exception();

}

int KingOfTheHill::make_move(Position start, Position end) {
  
  int status = ChessGame::make_move(start, end);

  if(status == MOVE_CHECKMATE) return status;

  // If a king reaches the hill, it's game over
  if( ((wKing.x == 3 || wKing.x == 4) && (wKing.y == 3 || wKing.y == 4))
      || ((bKing.x == 3 || bKing.x == 4) && (bKing.y == 3 || bKing.y == 4)) )
    {
      return MOVE_CONQUERED;  
    }
  return status; 
}
