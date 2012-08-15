////////////////////////////////////////////////////////////////////////
//                                                                   ///
// Program to encrypt and decrypt files for security using PIN number //
// Created by Michael Spelling 10/2009                                //
// v2.2                                                         //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#define ENCRYPT_HEADER "..._,-^*[Mike.Encryption.AlgorithmV2.2]*^-,_..."
#define HEAD "..M"
#define FOOT "EncAv2_2Pin.txt"

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <time.h>
#include <math.h>

using namespace std;

void encrypt();
void decrypt();
char encode(char c);
char decode(char c);

void main()
{
	char c = 1;

	// Prompt for action until 'q' or 'Q' pressed
	while(c != 0) {
		cout << "Mike's Encryption Algorithm v2.2\n\n[E]ncrypt, [D]ecrypt or [Q]uit?\n>>";
		c = getch();
		cout << c << "\n";

		switch(c) {
		case 69:
		case 101:
			{
				encrypt();
				break;
			}
		case 68:
		case 100:
			{
				decrypt();
				break;
			}
		case 81:
		case 113:
			{
				c = 0;	// Break loop to exit
				break;
			}
		default:
			{
				cout << "\nError> Invalid command\n\n";
			}
		}
	}
	return;
}

void encrypt()
{
	string inputFile;
	string outputFile;
	string check;
	string randPinFile;
	string inputFileEnc;
	string pin;
	char c;
	int random;
	int count;
	int x, y, z;
	int pinNo = 0;
	int stringCount = 0;

	// Get filename to encrypt
	cout << "\nEnter name of file to encrypt:\n>>";
	cin >> inputFile;

	// Open input file
	ifstream inFile(inputFile.c_str());	
	if(!inFile) {
		cout << "\nError> Cannot open file '" << inputFile << "'.\n\n"; return;}

	// Check if file is already encrypted by getting first line
	inFile >> check;
	if(check == ENCRYPT_HEADER) {
		cout << "Already encrypted\n";
		return;	// Quit if file already encrypted
	}	

	// Get user pin number
	cout << "Enter PIN:\n>> ";
	do {
		c = getch();
		if(c != 13) {
			pin.append(1,c);
			cout << "*";
		}

	} while(c != 13);
	z = pin.size()-1;
	for(x = 0; x < pin.size(); x++) {
		y = pow(10, z);
		pinNo = pinNo + (y * pin[x]);
		z--;
	}

	// Initialise random numbers
	srand(time(NULL));	

	// Create File for random pins
	randPinFile = "Pins/";
	inputFileEnc.resize(inputFile.size()-4);
	for(x = 0; x < inputFile.size()-4; x++)
		inputFileEnc[x] = inputFile[x]+111;
	randPinFile = randPinFile + HEAD + inputFileEnc + FOOT;
	remove(randPinFile.c_str());
	ofstream outPinFile(randPinFile.c_str());
	if(!outPinFile) {
		cout << "\nError> Could not create PIN file.\n\n"; return;}

	// Open output file
	outputFile = "~" + inputFile;
	ofstream outFile(outputFile.c_str());
	if(!outFile) {
		cout << "\nError> File could not be made.\n\n"; return;}

	// Create encrypted label in output file
	outFile << ENCRYPT_HEADER << "\n";

	// Make it read from beginning
	inFile.clear();
	inFile.seekg(0);

	// Get content, encrypt it, then write to output file
	while(!inFile.eof())
	{		
		random = rand() % 250;		
		c = inFile.get();
		if(inFile.eof())break; // Get character from source file
		c = encode(c);
		c = c + pin[stringCount] + pinNo + (random-11); // Add numbers from pin and random number	
		c = ~c; // Invert bits
		outFile << c; // Send character to destination file
		outPinFile << random << "\n"; // Save random in file
		stringCount++; // Increment position in pin and reset if past end
		if(pin.size() == stringCount) stringCount = 0;
	}

	// Output a random number of random numbers into file for decoy
	count = rand() % 50;
	for(y = 0; x < count; x++) {
		random = rand() % 250;
		outPinFile << random << "\n";
	}

	// Close files, remove old file, rename new one
	inFile.close();
	outFile.close();
	outPinFile.close();
	if(remove(inputFile.c_str()) != 0)
		cout << "Error> Cannot remove temp file\n";
	if(rename(outputFile.c_str(), inputFile.c_str()) != 0)
		cout << "Error> Cannot rename file\n";
	
	cout << "\n";
	return;
}

void decrypt()
{
	string inputFile;
	string outputFile;
	string inputFileEnc;
	string check;	
	string randPinFile;	
	string pin;
	char c;	
	int random;
	int x, y, z;
	int pinNo = 0;
	int stringCount = 0;

	// Get filename to decrypt
	cout << "\nEnter name of file to decrypt:\n>>";
	cin >> inputFile;	

	// Open input file
	ifstream inFile(inputFile.c_str());	
	if(!inFile){
		cout << "\nError> Cannot open file '" << inputFile << "'.\n\n"; return;}

	// Check if file is encrypted
	inFile >> check;
	if(check != ENCRYPT_HEADER) {
		cout << "File is not encrypted\n";
		return; // Quit if file is not encrypted
	}

	// Get file of random pins
	randPinFile = "Pins/";
	inputFileEnc.resize(inputFile.size()-4);
	for(x = 0; x < inputFile.size()-4; x++)
		inputFileEnc[x] = inputFile[x]+111;
	randPinFile = randPinFile + HEAD + inputFileEnc + FOOT;
	ifstream inPinFile(randPinFile.c_str());
	if(!inPinFile) {
		cout << "\nError> Could not find PIN.\n\n"; return;}

	// Get user pin number as string and convert to integer
	cout << "Enter PIN:\n>> ";
	do {
		c = getch();
		if(c != 13) {
			pin.append(1,c);
			cout << "*";
		}

	} while(c != 13);
	z = pin.size()-1;
	for(x = 0; x < pin.size(); x++) {
		y = pow(10, z);
		pinNo = pinNo + (y * pin[x]);
		z--;
	}

	// Create output file
	outputFile = "~" + inputFile;
	ofstream outFile(outputFile.c_str());
	if(!outFile){
		cout << "\nError> File could not be made.\n\n"; return;};   

	// Get content, decrypt it, then write to output file
	inFile.get();	// Make it read from correct place
	while(!inFile.eof()) {		
		inPinFile >> random;
		c = inFile.get();
		if(inFile.eof())break;
		c = ~c;		
		c = c - pin[stringCount] - pinNo - (random-11);
		c = decode(c);
		outFile << c;
		stringCount++;
		if(pin.size() == stringCount) stringCount = 0;		
	}

	// Close files
	inFile.close();
	outFile.close();
	inPinFile.close();

	// Display output file
	ifstream checkFile(outputFile.c_str());
	cout << "\nFile contents:\n";
	while(!checkFile.eof()) {
		c = checkFile.get();
		if(checkFile.eof())break;
		cout << c;
	}
	checkFile.close();

	c = 0;
	while(c != 89 && c != 121 && c != 78 && c != 110) {
		cout << "\n\nCorrect? [Y]es/[N]o?\n";
		c = getch();
	}

	// If PIN was wrong keep old file, delete new one
	if(c == 'n') {
		if(remove(outputFile.c_str()) != 0)
			cout << "Error> Cannot remove temporary file";
		else
			cout << "Old file restored\n";
	}
	else {
		// Else remove old file, rename new one
		if(remove(inputFile.c_str()) != 0)
			cout << "Error> Cannot remove temp file\n";
		if(rename(outputFile.c_str(), inputFile.c_str()) != 0)
			cout << "Error> Cannot rename file\n";
		//Remove random Pin
		if(remove(randPinFile.c_str()) != 0)
			cout << "Error> Cannot remove PIN file\n";
	}
	
	cout << "\n";
	return;
}

char encode(char c)
{
	switch(c) {
	case 'a':
		{
			c = 'z';
			break;
		}
	case 'b':
		{
			c = 'm';
			break;
		}
	case 'c':
		{
			c = 'y';
			break;
		}
	case 'd':
		{
			c = 'l';
			break;
		}
	case 'e':
		{
			c = 'x';
			break;
		}
	case 'f':
		{
			c = 'k';
			break;
		}
	case 'g':
		{
			c = 'w';
			break;
		}
	case 'h':
		{
			c = 'j';
			break;
		}
	case 'i':
		{
			c = 'v';
			break;
		}
	case 'j':
		{
			c = 'i';
			break;
		}
	case 'k':
		{
			c = 'u';
			break;
		}
	case 'l':
		{
			c = 'h';
			break;
		}
	case 'm':
		{
			c = 't';
			break;
		}
	case 'n':
		{
			c = 'g';
			break;
		}
	case 'o':
		{
			c = 's';
			break;
		}
	case 'p':
		{
			c = 'f';
			break;
		}
	case 'q':
		{
			c = 'r';
			break;
		}
	case 'r':
		{
			c = 'e';
			break;
		}
	case 's':
		{
			c = 'q';
			break;
		}
	case 't':
		{
			c = 'd';
			break;
		}
	case 'u':
		{
			c = 'p';
			break;
		}
	case 'v':
		{
			c = 'c';
			break;
		}
	case 'w':
		{
			c = 'o';
			break;
		}
	case 'x':
		{
			c = 'b';
			break;
		}
	case 'y':
		{
			c = 'n';
			break;
		}
	case 'z':
		{
			c = 'a';
			break;
		}
	case 'A':
		{
			c = 'Z';
			break;
		}
	case 'B':
		{
			c = 'M';
			break;
		}
	case 'C':
		{
			c = 'Y';
			break;
		}
	case 'D':
		{
			c = 'L';
			break;
		}
	case 'E':
		{
			c = 'X';
			break;
		}
	case 'F':
		{
			c = 'K';
			break;
		}
	case 'G':
		{
			c = 'W';
			break;
		}
	case 'H':
		{
			c = 'J';
			break;
		}
	case 'I':
		{
			c = 'V';
			break;
		}
	case 'J':
		{
			c = 'I';
			break;
		}
	case 'K':
		{
			c = 'U';
			break;
		}
	case 'L':
		{
			c = 'H';
			break;
		}
	case 'M':
		{
			c = 'T';
			break;
		}
	case 'N':
		{
			c = 'G';
			break;
		}
	case 'O':
		{
			c = 'S';
			break;
		}
	case 'P':
		{
			c = 'F';
			break;
		}
	case 'Q':
		{
			c = 'R';
			break;
		}
	case 'R':
		{
			c = 'E';
			break;
		}
	case 'S':
		{
			c = 'Q';
			break;
		}
	case 'T':
		{
			c = 'D';
			break;
		}
	case 'U':
		{
			c = 'P';
			break;
		}
	case 'V':
		{
			c = 'C';
			break;
		}
	case 'W':
		{
			c = 'O';
			break;
		}
	case 'X':
		{
			c = 'B';
			break;
		}
	case 'Y':
		{
			c = 'N';
			break;
		}
	case 'Z':
		{
			c = 'A';
			break;
		}
	case '0':
		{
			c = '9';
			break;
		}
	case '1':
		{
			c = '4';
			break;
		}
	case '2':
		{
			c = '8';
			break;
		}
	case '3':
		{
			c = '6';
			break;
		}
	case '4':
		{
			c = '7';
			break;
		}
	case '5':
		{
			c = '2';
			break;
		}
	case '6':
		{
			c = '3';
			break;
		}
	case '7':
		{
			c = '1';
			break;
		}
	case '8':
		{
			c = '5';
			break;
		}
	case '9':
		{
			c = '0';
			break;
		}
	default:
		{
			c = c;
		}
	}
	return(c);
}

char decode(char c)
{
	switch(c) {
	case 'a':
		{
			c = 'z';
			break;
		}
	case 'b':
		{
			c = 'x';
			break;
		}
	case 'c':
		{
			c = 'v';
			break;
		}
	case 'd':
		{
			c = 't';
			break;
		}
	case 'e':
		{
			c = 'r';
			break;
		}
	case 'f':
		{
			c = 'p';
			break;
		}
	case 'g':
		{
			c = 'n';
			break;
		}
	case 'h':
		{
			c = 'l';
			break;
		}
	case 'i':
		{
			c = 'j';
			break;
		}
	case 'j':
		{
			c = 'h';
			break;
		}
	case 'k':
		{
			c = 'f';
			break;
		}
	case 'l':
		{
			c = 'd';
			break;
		}
	case 'm':
		{
			c = 'b';
			break;
		}
	case 'n':
		{
			c = 'y';
			break;
		}
	case 'o':
		{
			c = 'w';
			break;
		}
	case 'p':
		{
			c = 'u';
			break;
		}
	case 'q':
		{
			c = 's';
			break;
		}
	case 'r':
		{
			c = 'q';
			break;
		}
	case 's':
		{
			c = 'o';
			break;
		}
	case 't':
		{
			c = 'm';
			break;
		}
	case 'u':
		{
			c = 'k';
			break;
		}
	case 'v':
		{
			c = 'i';
			break;
		}
	case 'w':
		{
			c = 'g';
			break;
		}
	case 'x':
		{
			c = 'e';
			break;
		}
	case 'y':
		{
			c = 'c';
			break;
		}
	case 'z':
		{
			c = 'a';
			break;
		}
	case 'A':
		{
			c = 'Z';
			break;
		}
	case 'B':
		{
			c = 'X';
			break;
		}
	case 'C':
		{
			c = 'V';
			break;
		}
	case 'D':
		{
			c = 'T';
			break;
		}
	case 'E':
		{
			c = 'R';
			break;
		}
	case 'F':
		{
			c = 'P';
			break;
		}
	case 'G':
		{
			c = 'N';
			break;
		}
	case 'H':
		{
			c = 'L';
			break;
		}
	case 'I':
		{
			c = 'J';
			break;
		}
	case 'J':
		{
			c = 'H';
			break;
		}
	case 'K':
		{
			c = 'F';
			break;
		}
	case 'L':
		{
			c = 'D';
			break;
		}
	case 'M':
		{
			c = 'B';
			break;
		}
	case 'N':
		{
			c = 'Y';
			break;
		}
	case 'O':
		{
			c = 'W';
			break;
		}
	case 'P':
		{
			c = 'U';
			break;
		}
	case 'Q':
		{
			c = 'S';
			break;
		}
	case 'R':
		{
			c = 'Q';
			break;
		}
	case 'S':
		{
			c = 'O';
			break;
		}
	case 'T':
		{
			c = 'M';
			break;
		}
	case 'U':
		{
			c = 'K';
			break;
		}
	case 'V':
		{
			c = 'I';
			break;
		}
	case 'W':
		{
			c = 'G';
			break;
		}
	case 'X':
		{
			c = 'E';
			break;
		}
	case 'Y':
		{
			c = 'C';
			break;
		}
	case 'Z':
		{
			c = 'A';
			break;
		}
	case '0':
		{
			c = '9';
			break;
		}
	case '1':
		{
			c = '7';
			break;
		}
	case '2':
		{
			c = '5';
			break;
		}
	case '3':
		{
			c = '6';
			break;
		}
	case '4':
		{
			c = '1';
			break;
		}
	case '5':
		{
			c = '8';
			break;
		}
	case '6':
		{
			c = '3';
			break;
		}
	case '7':
		{
			c = '4';
			break;
		}
	case '8':
		{
			c = '2';
			break;
		}
	case '9':
		{
			c = '0';
			break;
		}
	default:
		{
			c = c;
		}
	}
	return(c);
}