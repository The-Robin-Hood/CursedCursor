# set compiler
CC = g++
# include files
INCLUDE = -I ./include -I ./external/include
#compilers flags for compiling object files
CFLAGSO = -std=c++14 -Wall -m64 -O3 -c ${INCLUDE}
# libraries
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
#compilers flags for compiling binary file
CFLAGSB = -s -L external/libs ${LIBS}

default: objCompile
	mkdir -p ./dist/bin
	${CC} ./dist/build/*.o -o ./dist/bin/main ${CFLAGSB}
	cp -r ./res ./dist/bin/res

objCompile: 
	mkdir -p ./dist/build
	${CC} ./src/*.cpp ${CFLAGSO}
	# laymans way to move object files to dist/build folder 
	mv *.o ./dist/build

windows: winObjCompile
	if not exist dist\bin mkdir dist\bin
	${CC} dist/build/*.o -o dist\bin\main ${CFLAGSB} -mwindows
	xcopy /E /I /Y res dist\bin\res
	xcopy /I /Y external\dlls dist\bin\

winObjCompile:
	if not exist dist\build mkdir dist\build
	${CC} ./src/*.cpp ${CFLAGSO} -mwindows
	move *.o dist\build