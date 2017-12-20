/*
  Andrew Efimov - aefimov1 - aefimov1@jhu.edu
  Adam Polevoy - apolevo1 - adampolevoy@gmail.com
  Daniel Sohn - dsohn3 - dsohn45@gmail.com
  
  Final Project
  601.220
  12/8/17
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "ChessGame.h"
#include "SpookyChess.h"

using std::cout;
using std::endl;

// Set up the chess board with standard initial pieces
ChessGame::ChessGame(): Game() {

  _gametype = CHESS;
  initialize_factories();

  std::vector<int> pieces {
        ROOK_ENUM, KNIGHT_ENUM, BISHOP_ENUM, QUEEN_ENUM,
        KING_ENUM, BISHOP_ENUM, KNIGHT_ENUM, ROOK_ENUM
    };

  for (size_t i = 0; i < pieces.size(); ++i) {
        init_piece(PAWN_ENUM, WHITE, Position(i, 1));
        init_piece(pieces[i], WHITE, Position(i, 0));
        init_piece(pieces[i], BLACK, Position(i, 7));
        init_piece(PAWN_ENUM, BLACK, Position(i, 6));
    }

  wKing = Position(4,0);  
  bKing = Position(5,7);
}


// Set up the chess board with game state loaded from file
ChessGame::ChessGame(const std::string filename) : Game() {

  _gametype = CHESS;

  int status = load_game(filename);
  if(status == LOAD_FAILURE) throw std::exception();
}

// Perform a move from the start Position to the end Position                                
// The method returns an integer with the status                                             
// >= 0 is SUCCESS, < 0 is failure                                                           
int ChessGame::make_move(Position start, Position end) {

    int status = Game::make_move(start, end);
    return status;
}

// Prepare the game to create pieces to put on the board
void ChessGame::initialize_factories() {

  // Add all factories needed to create Piece subclasses
    add_factory(new PieceFactory<Pawn>(PAWN_ENUM));
    add_factory(new PieceFactory<Rook>(ROOK_ENUM));
    add_factory(new PieceFactory<Knight>(KNIGHT_ENUM));
    add_factory(new PieceFactory<Bishop>(BISHOP_ENUM));
    add_factory(new PieceFactory<Queen>(QUEEN_ENUM));
    add_factory(new PieceFactory<King>(KING_ENUM));
}

// Load a game from save file. If load is unsuccessful, return error code 
int ChessGame::load_game(std::string filename){

  if(_gametype == CHESS) initialize_factories(); // KOTH and Spooky loading calls Chess(), which does this usually, but Chess(filename) requires initialization of factories

  //scan file
  ifstream in;
  in.open(filename);
  
  if(!in){
    return LOAD_FAILURE;
  }

  std::string game_type;
  int turns;
  
  in >> game_type;
  in >> turns;

  SpookyChess* spooky_this; // for use in assigning ghost position
  if(_gametype == SPOOKY) {
      spooky_this = (SpookyChess*) this;
      in >> _numgen;
      for(int i = 0; i < _numgen; i++) rand();
  }
  
  _turn = turns + 1; // because save file saves the last complete turn
  
  if((game_type == "chess" && _gametype == CHESS) || (game_type == "king" && _gametype == KOTH)
     || (game_type == "spooky" && _gametype == SPOOKY)){

    std::vector<int> pieces {PAWN_ENUM, ROOK_ENUM, KNIGHT_ENUM, BISHOP_ENUM, QUEEN_ENUM, KING_ENUM, GHOST_ENUM};
    std::vector<Player> owners{WHITE, BLACK, NO_ONE};
   
    int owner, piece_type, pos_int;
    char pos_char;
    
    while(in >> owner){
      
      in >> pos_char;
      in >> pos_int;
      in >> piece_type;
      Position pos;

      pos.x = tolower(pos_char) - ASCII; // ASCII offset
      pos.y = pos_int - 1;

      init_piece( pieces[piece_type], owners[owner], pos);  
    }

    in.close();
 
    // record white/black king positions, and ghost piece position upon load
    std::vector<Piece*>::iterator it;
    for(it = _pieces.begin(); it != _pieces.end(); it++){

      if( *it != nullptr && (*it)->piece_type() == 5 && (*it)->owner() == WHITE) {
        wKing = piecePosition(*it);
      }

      if( *it != nullptr && (*it)->piece_type() == 5 && (*it)->owner() == BLACK) {
        bKing = piecePosition(*it);
      }

      if ( *it != nullptr && (*it)->piece_type() == 6) {
        spooky_this->ghost = piecePosition(*it);
      }
    }    
    return SUCCESS;    
  }  
  return LOAD_FAILURE;
}

int ChessGame::save_game(std::string filename){

  ofstream out;
  out.open(filename);
  if(!out) return SAVE_FAILURE;

  if(_gametype == CHESS) out << "chess"<< endl;
  else if(_gametype == KOTH) out << "king" <<endl;
  else if(_gametype == SPOOKY) out << "spooky" << endl;
  else return LOAD_FAILURE;
  
  out << _turn - 1 << endl;
  if(_gametype == SPOOKY) out << _numgen << endl;

  // Iterate thru pieces and write to save file
  for(unsigned int i = 0; i < _pieces.size(); i++){

    if( _pieces[i] != nullptr){
      std::string pos;
      char pos_x;
      unsigned int pos_y;

      pos_x = (char)( i % _width + ASCII);
      pos_y = i / _width + 1;

      pos += pos_x;
      pos += std::to_string(pos_y);
      
      out << (_pieces[i])->owner() << " " << pos << " "	<< (_pieces[i])->piece_type() << endl;      
    }   

  }
  
  out.close();
  
  return SUCCESS;
}
