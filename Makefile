CXX = g++-5
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = watan
OBJECTS = main.o Player.o Database.o Watan.o Board.o InitialBoard.o RandomGenerator.o Dice.o fair_dice.o load_dice.o FileLoad.o Subject.o AI.o 
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}

