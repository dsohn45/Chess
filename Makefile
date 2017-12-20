#  Andrew Efimov - aefimov1 - aefimov1@jhu.edu
#  Adam Polevoy - apolevo1 - adampolevoy@gmail.com
#  Daniel Sohn - dsohn3 - dsohn45@gmail.com
  
#  Final Project
#  601.220
#  12/8/17

CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -g

play: Play.o Game.o ChessGame.o KingOfTheHill.o SpookyChess.o ChessPiece.o
	$(CXX) Play.o Game.o ChessGame.o KingOfTheHill.o SpookyChess.o ChessPiece.o -o play

Play.o: Play.cpp Game.h ChessGame.h Prompts.h KingOfTheHill.h SpookyChess.h
	$(CXX) $(CXXFLAGS) -c Play.cpp

Game.o: Game.cpp Game.h Piece.h Prompts.h Enumerations.h Terminal.h 
	$(CXX) $(CXXFLAGS) -c Game.cpp

ChessGame.o: ChessGame.cpp Game.h ChessGame.h ChessPiece.h 
	$(CXX) $(CXXFLAGS) -c ChessGame.cpp

KingOfTheHill.o: KingOfTheHill.cpp ChessGame.h KingOfTheHill.h
	$(CXX) $(CXXFLAGS) -c KingOfTheHill.cpp

SpookyChess.o: SpookyChess.cpp ChessGame.h SpookyChess.h 
	$(CXX) $(CXXFLAGS) -c SpookyChess.cpp

ChessPiece.o: ChessPiece.cpp Enumerations.h Game.h Piece.h ChessPiece.h
	$(CXX) $(CXXFLAGS) -c ChessPiece.cpp

clean:
	rm *.o play 
