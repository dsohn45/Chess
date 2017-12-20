/*
  Andrew Efimov - aefimov1 - aefimov1@jhu.edu
  Adam Polevoy - apolevo1 - adampolevoy@gmail.com
  Daniel Sohn - dsohn3 - dsohn45@gmail.com
  
  Final Project
  601.220
  12/8/17
*/

#include "ChessPiece.h"

int Pawn::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const {
	//start
  trajectory.push_back(start);
	int diff = start.x - end.x;
      
    //White player
	if(owner() == WHITE) {
    //can move one left, one right, one none
    if(diff == 0 || diff == -1 || diff == 1) {
      //must move forward one
      if(start.y + 1 == end.y) {
        //push mpve
        trajectory.push_back(Position(start.x+diff, end.y));
      return SUCCESS;
      }
      //if it hasn't moved yet, it can move two forward
      if(start.y == 1) {
        if(end.y == 3) {
            trajectory.push_back(Position(start.x+diff, end.y));
            return SUCCESS;
        }
      }
    }
	}

  //Black player (mostly the same, but moves backwards, starts other side
	if(owner() == BLACK) {
    if(diff > -2 && diff < 2) {
      if(start.y - 1 == end.y) {
        trajectory.push_back(Position(start.x+diff, end.y));
        return SUCCESS;
      }
      if(start.y == 6) {
        if(end.y == 4) {
          trajectory.push_back(Position(start.x+diff, end.y));
          return SUCCESS;
  	    }
      }
    }
 	 }  
	return MOVE_ERROR_ILLEGAL;
}

int Rook::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const{
  //start
  trajectory.push_back(start);
  int diffx = end.x - start.x;
  int diffy = end.y - start.y;
  
  //if it moves in x direction, can't move in y direction
  if (diffx != 0) {
    if (diffy == 0) {
      //fills trajectory depending on moving right or left
      if (diffx > 0) {
        //push every positiong on the path
	    for (int i = 1; i <= diffx; i++) {
	        trajectory.push_back(Position(start.x+i, start.y));
	    }
      } else if (diffx < 0) {
	    for (int i = -1; i >= diffx; i--) {
	        trajectory.push_back(Position(start.x+i, start.y));
	    }
      }
      return SUCCESS;
    }
  }
  
  //if it moves in y direction, can't move in x direction (mostly the same)
  else if (diffy != 0) {
    if (diffx == 0) {
      if (diffy > 0) {
	    for (int i = 1; i <= diffy; i++) {
	        trajectory.push_back(Position(start.x, start.y+i));
	    }
      } else if (diffy < 0) {
	    for (int i = -1; i >= diffy; i--) {
	        trajectory.push_back(Position(start.x, start.y+i));
	    }
      }
      return SUCCESS;
    }
  } 
  return MOVE_ERROR_ILLEGAL;
}

int Knight::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const{

  //start
  trajectory.push_back(start);
  int diffx = abs(end.x - start.x);
  int diffy = abs(end.y - start.y);

  if((diffx == 1 && diffy == 2) ||(diffx == 2 && diffy == 1)) {
    trajectory.push_back(end);
    return SUCCESS;
  }
  return MOVE_ERROR_ILLEGAL;
}


int Bishop::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const{
  
  //start
  trajectory.push_back(start);
  int diffx = end.x - start.x;
  int diffy = end.y - start.y;
  
  //if it moves diagonally with a positive slope
  if (diffx != 0 && diffx == diffy) {
    //fills trajectory depending on positive or negative motion
    if (diffx > 0) {
      //push all positions in move
      for (int i = 1; i <= diffx; i++) {
	    trajectory.push_back(Position(start.x+i, start.y+i));
      }
    } else if (diffx < 0) {
      for (int i = -1; i >= diffx; i--) {
	    trajectory.push_back(Position(start.x+i, start.y+i));
      }
    }
    return SUCCESS;
  }
  
  //if it moves diagonally with a negative slope (mostly the same)
  else if (diffx != 0 && diffx == -diffy) {
    //fills trajectory depending on positive or negative motion
    if (diffx > 0) {
      for (int i = 1; i <= diffx; i++) {
	    trajectory.push_back(Position(start.x+i, start.y-i));
      }
    } else if (diffx < 0) {
      for (int i = -1; i >= diffx; i--) {
	    trajectory.push_back(Position(start.x+i, start.y-i));
      }
    }
    return SUCCESS;
  } 
  return MOVE_ERROR_ILLEGAL;
}

int Queen::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const{
  
  //start
  trajectory.push_back(start);
  int diffx = end.x - start.x;
  int diffy = end.y - start.y;
  
  //note: I just copied and pasted the stuff from bishop and rook
  
  //bishop stuff
  if (diffx != 0 && diffx == diffy) {
    if (diffx > 0) {
      for (int i = 1; i <= diffx; i++) {
        trajectory.push_back(Position(start.x+i, start.y+i));
      }
    } else if (diffx < 0) {
      for (int i = -1; i >= diffx; i--) {
	    trajectory.push_back(Position(start.x+i, start.y+i));
      }
    }
    return SUCCESS;
  } else if (diffx != 0 && diffx == -diffy) {
    if (diffx > 0) {
      for (int i = 1; i <= diffx; i++) {
	    trajectory.push_back(Position(start.x+i, start.y-i));
      }
    } else if (diffx < 0) {
      for (int i = -1; i >= diffx; i--) {
	    trajectory.push_back(Position(start.x+i, start.y-i));
      }
    }
    return SUCCESS;
  }
  
  //rook stuff
  if (diffx != 0) {
    if (diffy == 0) {
      if (diffx > 0) {
	    for (int i = 1; i <= diffx; i++) {
	  trajectory.push_back(Position(start.x+i, start.y));
	}
      } else if (diffx < 0) {
	    for (int i = -1; i >= diffx; i--) {
	  trajectory.push_back(Position(start.x+i, start.y));
	}
      }
      return SUCCESS;
    }
  } else if (diffy != 0) {
    if (diffx == 0) {
      if (diffy > 0) {
	    for (int i = 1; i <= diffy; i++) {
	        trajectory.push_back(Position(start.x, start.y+i));
	    }
      } else if (diffy < 0) {
	    for (int i = -1; i >= diffy; i--) {
            trajectory.push_back(Position(start.x, start.y+i));
	    }
      }
      return SUCCESS;
    }
  }
  return MOVE_ERROR_ILLEGAL;
}

int King::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const{
  
  //start
  trajectory.push_back(start);
  int diffx = abs(end.x - start.x);
  int diffy = abs(end.y - start.y);
  
  //king had to move somewhere
  if ((diffx == 0) && (diffy == 0)) {
    return MOVE_ERROR_ILLEGAL;
  }

  //can move one in any direction
  if ((diffx == 0 || diffx == 1) && (diffy == 0 || diffy == 1)) {
    trajectory.push_back(end);
    return SUCCESS;
  }
  
  return MOVE_ERROR_ILLEGAL;
}


int Ghost::valid_move_shape(Position start, Position end, std::vector<Position>& trajectory) const{
  
  trajectory.push_back(start);
  trajectory.push_back(end);
  return MOVE_ERROR_ILLEGAL;
  
}
