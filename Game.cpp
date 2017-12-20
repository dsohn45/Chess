/*
  Andrew Efimov - aefimov1 - aefimov1@jhu.edu
  Adam Polevoy - apolevo1 - adampolevoy@gmail.com
  Daniel Sohn - dsohn3 - dsohn45@gmail.com
  
  Final Project
  601.220
  12/8/17
*/

#include <iostream>
#include <sstream>
#include <cassert>
#include <cctype>
#include <string>
#include <algorithm>

#include "Game.h"

#define ASCII   97

using std::transform;
using std::istringstream;
using std::cin;
using std::string;
using std::cout;
using std::endl;
using std::vector;


Game::~Game() {

    // Delete the factories used to generate pieces
    for (size_t i = 0; i < _registered_factories.size(); i++) {
        delete _registered_factories[i];
    }
    // Delete any other dynamically-allocated resources here
    for (unsigned int i = 0; i < _width * _height; i++) {
        delete _pieces[i];
    }
}

// Create a Piece on the board using the appropriate factory.
// Returns true if the piece was successfully placed on the board.
bool Game::init_piece(int piece_type, Player owner, Position pos) {
    Piece* piece = new_piece(piece_type, owner);
    if (!piece) return false;
    // Fail if the position is out of bounds
    if (!valid_position(pos)) {
        Prompts::out_of_bounds();
        return false;
    }
    // Fail if the position is occupied
    if (get_piece(pos)) {
        Prompts::blocked();
        return false;
    }
    _pieces[index(pos)] = piece;
    return true;
}

// Get the Piece at a specified Position.  Returns nullptr if no
// Piece at that Position or if Position is out of bounds.
Piece* Game::get_piece(Position pos) const {
    if (valid_position(pos))
        return _pieces[index(pos)];
    else {
        Prompts::out_of_bounds();
        return nullptr;
    }
}

//prints out board
void Game::board() const{
  
  cout << endl;    
  Terminal::color_all(true, Terminal::WHITE, Terminal::BLACK);
  //header letters
  cout << "    A  B  C  D  E  F  G  H" << endl;
  //goes through each row
  for (int i = 7; i >= 0; i--) {
    Terminal::color_all(true, Terminal::WHITE, Terminal::BLACK);
    //left side numbers
    cout << " " << i + 1 << " ";
    //for each position in row
    for (int j = 0; j < 8; j++) {
      //alternate between red and plue
      if((i + j) % 2 == 1) Terminal::color_bg(Terminal::RED);
      else Terminal::color_bg(Terminal::BLUE);
      
      //place the piece
      Piece* ptr = get_piece(Position(j,i));
      if (ptr == nullptr) cout << "   ";
      else{
        if(ptr->owner() == WHITE) Terminal::color_fg(false, Terminal::YELLOW);
        else if(ptr->owner() == NO_ONE) Terminal::color_fg(false, Terminal::CYAN);
        else Terminal::color_fg(false, Terminal::GREEN);  
        cout << " " << ptr->piece_type() << " ";
      }
    }
    Terminal::color_all(true, Terminal::WHITE, Terminal::BLACK);
    //right side numbers
    cout << " " << i + 1 << endl;
  }
  //bottom letters
  cout << "    A  B  C  D  E  F  G  H" << endl;
  Terminal::set_default();
}

// Execute the main gameplay loop.
void Game::run() {

  bool show_board = false; 
  bool quit = false;
  Player pl;
  Player opponent;
  string input;

  getline(cin, input);
  
  while(!quit) {

    if(show_board) board();

    //who is the current player vs. opponent
    if(_turn % 2 == 1) {
       pl = WHITE;
       opponent = BLACK;
    } else {
       pl = BLACK;
       opponent = WHITE;
    }

    Prompts:: player_prompt(pl, _turn);

    getline(cin, input);
    //this makes it case insensitive
    transform(input.begin(), input.end(), input.begin(), ::tolower);  
    //quit
    if(input == "q") quit = true;
    //show board
    else if(input == "board") show_board = !show_board;
    //save
    else if(input == "save"){
      Prompts::save_game();
      getline(cin, input);
      transform(input.begin(), input.end(), input.begin(), ::tolower);
      int status = save_game(input);
      if (status < 0) Prompts::save_failure();
    }
    //forfeit
    else if(input == "forfeit") {
    	Player wins;
    	if(pl == WHITE) wins = BLACK;
    	else wins = WHITE;
    	Prompts::win(wins, _turn);
    	Prompts::game_over();
    	quit = true;
    }    
    //moving a piece input
    else {
      
      istringstream is(input);     
      Position start, end;
      char s_letter = 'z', e_letter = 'z';
      int s_int = 0, e_int = 0;

      //makes sure proper coordinates (char int char int)    
      is >> s_letter;
      is >> s_int;
      is >> e_letter;
      is >> e_int;
      
      //transforms that into coordinates
      start.x = tolower(s_letter) - ASCII; // ASCII offset
      start.y = s_int - 1;
      end.x = tolower(e_letter) - ASCII;
      end.y = e_int - 1;
      
      //makes the move
      int status = make_move(start, end);
	
      //depending on status from make move ...     
      if (s_int == 0 || e_int == 0 || !isalpha(s_letter) || !isalpha(e_letter)) Prompts::parse_error();
      else if(status == MOVE_ERROR_OUT_OF_BOUNDS) Prompts::out_of_bounds();
      else if(status == MOVE_ERROR_NO_PIECE) Prompts::no_piece();
      else if(status == MOVE_ERROR_ILLEGAL) Prompts::illegal_move();
      else if(status == MOVE_ERROR_BLOCKED) Prompts::blocked();
      else if(status == MOVE_ERROR_CANT_EXPOSE_CHECK) Prompts::cannot_expose_check();
      else if(status == MOVE_ERROR_MUST_HANDLE_CHECK) Prompts::must_handle_check();
      else if(status == MOVE_ERROR_CANT_CASTLE) Prompts::cannot_castle();
      else if(status == MOVE_CHECKMATE){
				Prompts::checkmate(pl);
				Prompts::win(pl, _turn);
				Prompts::game_over();
				quit = true;		
      }
      else if(status == MOVE_CONQUERED){
      	Prompts::conquered(pl);
      	Prompts::win(pl, _turn);
      	Prompts::game_over();
      	quit = true;
      }
      else if(status == MOVE_GHOSTED) Prompts::ghost_capture();
      else if(status == MOVE_STALEMATE) {
      	Prompts::stalemate();
      	Prompts::game_over();
      	quit = true;
      }
      else if(status == MOVE_CHECK) Prompts::check(pl);
      else if(status == MOVE_CAPTURE) Prompts::capture(pl);
      else if(status == GHOST_CHECKMATE) {
        Prompts::checkmate(opponent);
        Prompts::win(opponent, _turn);
        Prompts::game_over();
        quit = true;
      }
      if(status > 0 && quit == false) {
        _turn++;
      }
    }
  }
}

//moves the piece
int Game::make_move(Position start, Position end) {

  //useful note: (_turn + 1) % 2 is the current player
	//if already checked
  bool already_checked = false;
  if(check((_turn + 1) % 2)) already_checked = true;
  Piece* toMove = get_piece(start);
  Piece* toTake = get_piece(end); 

	if(!valid_position(start) || !valid_position(end)) return MOVE_ERROR_OUT_OF_BOUNDS;
  if(toMove->owner() != ((_turn + 1)% 2)) return MOVE_ERROR_NO_PIECE;

  int status = valid_move(start, end);
  if(status < 0) return status;

  _pieces[index(end)] = toMove;
  _pieces[index(start)] = nullptr;
  //If you moved the king, update the king positions
  if (toMove->piece_type() == 5) {
    if (toMove->owner() == BLACK) bKing = end;
    if (toMove->owner() == WHITE) wKing = end;
	}

  if(check((_turn + 1) % 2)) {
	  _pieces[index(start)] = toMove;
	  _pieces[index(end)] = toTake;
	  //move king if it got move
	  if (toMove->piece_type() == 5) {
	    if (toMove->owner() == BLACK) bKing = start;
	    if (toMove->owner() == WHITE) wKing = start;
	  }
	  if(already_checked) return MOVE_ERROR_MUST_HANDLE_CHECK;
	  else return MOVE_ERROR_CANT_EXPOSE_CHECK;
	}

	if(toMove->piece_type() == 0){   
		if(end.y == 7) {
			delete toMove;
			_pieces[index(end)] = nullptr;
			init_piece(QUEEN_ENUM, WHITE, end);
		} 
		else if(end.y == 0) {
			delete toMove;
			_pieces[index(end)] = nullptr;
			init_piece(QUEEN_ENUM, BLACK, end);
			} 
	}
	//delete piece that was captured
	if(toTake != nullptr) {
		delete toTake;
		status = MOVE_CAPTURE;
	}

	if (check(_turn % 2)) return checkmate((_turn + 1) % 2);
	if (checkmate((_turn + 1) % 2) == MOVE_CHECKMATE) return MOVE_STALEMATE;
	else return status;
}

// Search the factories to find a factory that can translate
//`piece_type' into a Piece, and use it to create the Piece.
// Returns nullptr if factory not found.
Piece* Game::new_piece(int piece_type, Player owner) {

  PieceGenMap::iterator it = _registered_factories.find(piece_type);

  if (it == _registered_factories.end()) { // not found
      cout << "Piece type " << piece_type << " has no generator\n";
      return nullptr;
  } else {
      return it->second->new_piece(owner);
  }
}

// Add a factory to the Board to enable producing
// a certain type of piece. Returns whether factory
// was successfully added or not.
bool Game::add_factory(AbstractPieceFactory* piece_gen) {
    // Temporary piece to get the ID
    Piece* p = piece_gen->new_piece(WHITE);
    int piece_type = p->piece_type();
    delete p;

    PieceGenMap::iterator it = _registered_factories.find(piece_type);
    if (it == _registered_factories.end()) { // not found, so add it
        _registered_factories[piece_type] = piece_gen;
        return true;
    } else {
        cout << "Piece type " << piece_type << " already has a generator\n";
        return false;
    }
}

//returns position struct of a piece
Position Game::piecePosition(Piece* p) {

  //iterate thru pieces
  for(unsigned int i = 0; i < _pieces.size(); i++){
    if( _pieces[i] == p){

      string pos;
      char pos_x;
      unsigned int pos_y;

      pos_x = ( i % _width);
      pos_y = i / _width;

      return Position(pos_x,pos_y);
    }
  }
  cout << "Not a valid piece" << endl;
  return Position(1000,1000);
}

//checks to see if piece can make the move
//LOOKS LIKE A GOOD AMOUNT OF REPEATED CODE, LET'S LOOK LATER
int Game::valid_move(Position start, Position end){
  
  if(!valid_position(start) || !valid_position(end)) return MOVE_ERROR_OUT_OF_BOUNDS;

  Piece* toMove = get_piece(start);
  Piece* toTake = get_piece(end);
  //No piece
  if(toMove == nullptr) return MOVE_ERROR_NO_PIECE;
  //Invalid Move
  vector<Position> trajectory; 
  int status = toMove->valid_move_shape(start, end, trajectory);
  if(status < 0) return status;  
  //Attempt to take own piece
  if((toTake != nullptr) && (toTake->owner() == toMove->owner() 
      || toTake->owner() == NO_ONE)) return MOVE_ERROR_BLOCKED;
  //Make sure path is not blocked
  for(unsigned int i = 1; i < trajectory.size() - 1; i++) {
    if(get_piece(trajectory[i]) != nullptr) return MOVE_ERROR_BLOCKED;  
  } 
  //IMPORTANT TO HANDLE PAWNS (they are special)
  if(toMove->piece_type() == 0) {
 		if(start.x != end.x && toTake == nullptr) return MOVE_ERROR_ILLEGAL;		
		else if(start.x == end.x && toTake != nullptr) return MOVE_ERROR_BLOCKED;
  }
  return SUCCESS;  
}

//Note: Check sees if the player you gave it is in check
//      Checkmate sees if the player you did not give it is in checkmate

//checks if owner (player you gave it) is in check
bool Game::check(int owner){

  //opponent is not the player given
  int opponent = (owner + 1) % 2;
  int status = -1;
  Position king = Position(1000, 1000);

  if(owner == BLACK){
    king = bKing;
  }
  else if(owner == WHITE){   
    king = wKing;
  }
  // checks for check
  vector<Piece*>::iterator it;
  //iterate through all pieces
  for(it = _pieces.begin(); it != _pieces.end(); it++){
    //if it is one of the opponent's pieces 
    if( *it != nullptr && (*it)->owner() == opponent){ 
      //if it can make a valid move to owner's king, owner is in check
      status = valid_move(piecePosition( *it), king);      
      if(status > 0) return true;      
    }   
  }
  return false;  
}

//cehcks to see if opponent (not the player you gave it) is in checkmate or in check
int Game::checkmate(int owner){

  //opponent is not the player given
  int opponent = (owner + 1) % 2;

  vector<Piece*>::iterator it;
  //iterate through all pieces
  for(it = _pieces.begin(); it != _pieces.end(); it++){
    //if it is one of the opponent's pieces
    if(*it != nullptr && (*it)->owner() == opponent) {
      Position start = piecePosition(*it);
      Piece* toMove = get_piece(start);
      //try to move that piece to all possible positions
      for(unsigned int i = 0; i < _width; i++){
	      for(unsigned int j = 0; j < _height; j++){
            //if it is valid
    	    if (valid_move(start, Position(i,j)) > 0){

                //make the move
    	        Position end = Position(i,j);				  				    
	            Piece* toTake = get_piece(end);
                _pieces[index(start)] = nullptr;
		        _pieces[index(end)] = toMove;

                //if you're moving the king, update king position
		        if (toMove->piece_type() == 5) {
      		        if (opponent == WHITE) wKing = end;
            	    else bKing = end;
        	    }

                //if the opponent is still in check
	    		if(check(opponent)){
                    //undo everything you did
				    _pieces[index(start)] = toMove;
				    _pieces[index(end)] = toTake;
	          	    if(toMove->piece_type() == 5) {
	          		    if (opponent == WHITE) wKing = start;
    	      		    else bKing = start;
    	      	    }
                    //continue in for loop and try another piece
    	        }
                //if the opponent successfully got out of check
				else{
                    //undo everything you did
				    _pieces[index(start)] = toMove;
				    _pieces[index(end)] = toTake;
	          	    if(toMove->piece_type() == 5) {
	          		    if (opponent == WHITE) wKing = start;
    	      		    else bKing = start;
    	      	    }
                    //opponent is not in checkmate, so return check
	      			return MOVE_CHECK;
	    	    }  
	  		}
		}
      }
    }
  }
  return MOVE_CHECKMATE;
}
