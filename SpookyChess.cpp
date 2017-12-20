/*
  Andrew Efimov - aefimov1 - aefimov1@jhu.edu
  Adam Polevoy - apolevo1 - adampolevoy@gmail.com
  Daniel Sohn - dsohn3 - dsohn45@gmail.com
  
  Final Project
  601.220
  12/8/17
*/

#include <cstdio>
#include <cstdlib>
#include "SpookyChess.h"

// Set up the chess board with standard initial pieces
SpookyChess::SpookyChess() : ChessGame() {

  _gametype = SPOOKY;
  _numgen = 0;

  // Initialize ghost piece
  add_factory(new PieceFactory<Ghost>(GHOST_ENUM));
  init_piece(GHOST_ENUM, NO_ONE, Position(0,4));
  ghost = Position(0,4);
  
}

// Set up the chess board with game state loaded from file
SpookyChess::SpookyChess(const std::string filename) {

  _gametype = SPOOKY;
  
  // Because the base class constructor Chess() gets called, we need
  // this for loop to clear the board of the default initialized pieces
  for(unsigned int i = 0; i < _width * _height; i++) {
    if (_pieces[i] != nullptr) delete _pieces[i];
    _pieces[i] = nullptr; 
  }
  
  add_factory(new PieceFactory<Ghost>(GHOST_ENUM));
  int status = load_game(filename);

  if(status == LOAD_FAILURE) throw std::exception();
}

// Perform a move from the start Position to the end Position                                
// The method returns an integer with the status                                             
// >= 0 is SUCCESS, < 0 is failure                                                           
int SpookyChess::make_move(Position start, Position end) {

  // Move other pieces first
  int status = Game::make_move(start, end);

  // Check, check mate
  if(status < 0) return status;
  if(status == MOVE_CHECKMATE || status == MOVE_STALEMATE) return status;

  // Move ghost piece
  int moveTo = rand() % 64;
  
  Piece* ghostP = _pieces[index(ghost)];
  _pieces[index(ghost)] = nullptr;
  
  _numgen++;

  // Ghost can't take king
  while(_pieces[moveTo] != nullptr && _pieces[moveTo]->piece_type() == KING_ENUM) {
    moveTo = rand() % 64;
    _numgen++;
  }

  // Update position
  ghost.x = moveTo % _width;
  ghost.y = moveTo / _width; 

  // Capture piece
  bool captured = false;
  if(_pieces[moveTo] != nullptr) {
    delete _pieces[moveTo]; 
    captured = true;
  }

  _pieces[moveTo] = ghostP;

  // Check, checkmate after ghost move but before next player goes
  if (check((_turn + 1) % 2)) return GHOST_CHECKMATE;
  else if (check(_turn % 2)) return checkmate((_turn + 1) % 2);
  else if (checkmate((_turn + 1) % 2) == MOVE_CHECKMATE) return MOVE_STALEMATE;

  if (captured == true) return MOVE_GHOSTED;
  
  return status;
}
