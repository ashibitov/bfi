//Brainfuck interpreter written in C++ by Anthony  Shibitov
//Read .bf code in one character at a time. Run appropriate
//input output (getchar,  cout). Use  tape  array  of  size
//30,000 for the tape (of type char).

//Syntax: consists of only ><+-.,[]
//Everything else is ignored.

//+ : Increments the value at the current cell by one.
//- : Decrements the value at the current cell by one.
//> : Moves the data pointer to the next cell (cell on the right).
//< : Moves the data pointer to the previous cell (cell on the left).
//. : Prints the ASCII value at the current cell (i.e. 65 = 'A').
//, : Reads a single input character into the current cell.
//[ : If the value at the current cell is zero, skips to the corresponding ] .
//    Otherwise, move to the next instruction.
//] : If the value at the current cell is zero, move to the next instruction.
//    Otherwise, move backwards in the instructions to the corresponding [ .

//[ and ] form a while loop. Obviously, they must be balanced.
//source: learnxinyminutes.com/docs/brainfuck

#include <iostream>
#include <fstream>
#include <string>

#define SUCCESS 0
#define FAILURE 1

#define FALSE false
#define TRUE true

#define CELLSIZE 30000

void usage(){
	std::cout << "usage: bfi inputfile.bf (-d) (0-29,999)" << std::endl;
	std::cout << "\t-d : show debug information\n";
	std::cout << "\t(0-29,999) : # of cells to display\n\n";
	return;
}

bool checkChar(char check){
	switch(check){
		case ',':
			return SUCCESS;
		case '.':
			return SUCCESS;
		case '<':
			return SUCCESS;
		case '>':
			return SUCCESS;
		case '-':
			return SUCCESS;
		case '+':
			return SUCCESS;
		case '[':
			return SUCCESS;
		case ']':
			return SUCCESS;
		default:
			return FAILURE;
	}
}

int main(int argc, char* argv[]){
	if(argc == 1){
		usage();
		return 0;
	}
	bool debug = FALSE;
	int CELL_PRINT = 5;
	if(argc == 3 || argc == 4){
		if(strcmp(argv[2], "d")){
			std::cout << "debug flag active . . .\n";
			debug = TRUE;
		}
		if(argc == 4){
			CELL_PRINT = atoi(argv[3]);
		}
	}
	std::ifstream inputBF;
	inputBF.open(argv[1]);
	if(!inputBF.is_open()){
		std::cout << "error: file could not be opened" << std::endl;
		return FAILURE;
	}
	std::string filebuffer;
	std::string fullbuffer;
	while(std::getline(inputBF, filebuffer)){
			fullbuffer = fullbuffer + filebuffer;
	}
	fullbuffer.erase(std::remove(fullbuffer.begin(), fullbuffer.end(), ' '), fullbuffer.end());
	std::string finalbuffer;
	for(int i = 0; i < fullbuffer.length(); i++){
		if(checkChar(fullbuffer[i]) == SUCCESS){
			finalbuffer = finalbuffer + fullbuffer[i];
		}
	}
	char * tape = new char[CELLSIZE];
	int tapePosition = 0;
	for(int i = 0; i < finalbuffer.length(); i++){
		if(debug == TRUE){
			std::cout << "\n\nON INSTRUCTION " << i << " = " << finalbuffer[i] << "\n";
			for(int j = 0; j < CELL_PRINT; j++){
				std::cout << j << "cell = " << (int)tape[j] << std::endl;
			}
			std::cout << "\n";
		}
		switch(finalbuffer[i]){
			case '>':
				tapePosition++;
				break;
			case '<':
				tapePosition--;
				break;
			case '+':
				tape[tapePosition]++;
				break;
			case '-':
				tape[tapePosition]--;
				break;
			case '.':
				std::cout << tape[tapePosition];
				break;
			case ',':
				std::cin.get(tape[tapePosition]);
				break;
			case '[':
				if(((int)tape[tapePosition]) == 0){
					int current = i;
					bool match = TRUE;
					int loopCount = 1;
					for(int x = i+1; x < finalbuffer.length(); x++){
						if(finalbuffer[x] == '['){
							loopCount++;
						}
						if(finalbuffer[x] == ']'){
							loopCount--;
						}
						if(loopCount == 0){
							i = x;
							break;
						}
					}
				}
				break;
			case ']':
				if(((int)tape[tapePosition]) != 0){
					int current = i;
					bool match = TRUE;
					int loopCount = 1;
					for(int x = i-1; x > 0; x--){
						if(finalbuffer[x] == ']'){
							loopCount++;
						}
						if(finalbuffer[x] == '['){
							loopCount--;
						}
						if(loopCount == 0){
							i = x;
							break;
						}
					}
				}
				break;
			default:
				break;
		}
	}
	delete tape;
}