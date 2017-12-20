/*
  Andrew Efimov - aefimov1 - aefimov1@jhu.edu
  Adam Polevoy - apolevo1 - adampolevoy@gmail.com
  Daniel Sohn - dsohn3 - dsohn45@gmail.com
  
  Final Project
  601.220
  12/8/17
 */

#ifndef CHESS_GAME_H
#define CHESS_GAME_H
#define ASCII   97

#include <fstream>
#include <string>
#include <exception>

#include "Game.h"
#include "ChessPiece.h"

using std::ifstream;
using std::ofstream;

class ChessGame : public Game {

public:

    // Creates new game in standard start-of-game state 
    ChessGame();

    // Creates game with state indicated in specified file
    ChessGame(std::string filename);

    // Perform a move from the start Position to the end Position
    // The method returns an integer with the status
    // >= 0 is SUCCESS, < 0 is failure
    virtual int make_move(Position start, Position end) override;

    // Loads game from file or returns error code if unsuccessful
    virtual int load_game(std::string filename) override;

    // Saves game to file or returns error code if fails
    virtual int save_game(std::string filename) override;
    
protected:
    
    // Create all needed factories for the kinds of pieces
    // used in chess (doesn't make the actual pieces)
    virtual void initialize_factories();
    
    int _gametype;
};

#endif // CHESS_GAME_H
