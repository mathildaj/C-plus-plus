/*
Program Description: Convert a binary file made from morse code to human readable text
*/

#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

//this class set the morse code map
class Table
{
private:
	unordered_map<int, char> morseCodeMap;
public:

	//constructor
	Table()
	{
		setMorseCodeMap();
	}

	//setter
		
	/* this method will set the morseCodeMap. 
		It uses the decimal values of the binary representation
		of the Dots and Dashes of the Morse Code as index/keys, 
		and the characters as the values.
		Dot(10), and Dash(01)
		For this particular problem, we use 46 characters as provided by the requirement
	*/
	void setMorseCodeMap()
	{
		//For this particular problem, we use 46 characters as provided by the requirement
		const int SIZE = 46;
		char morseChars[SIZE] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
			'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', '0',
			'1', '2', '3', '4', '5', '6', '7', '8', '9',
			'\'', '@', ':', ',', '$', '=', '!', '.', '?', '"' };

		string morseCode[SIZE] = { ".-",
								"-...",
								"-.-.",
								"-..",
								".",
								"..-.",
								"--.",
								"....",
								"..",
								".---",
								"-.-",
								".-..",
								"--",
								"-.",
								"---",
								".--.",
								"--.-",
								".-.",
								"...",
								"-",
								"..-",
								"...-",
								".--",
								"-..-",
								"-.--",
								"--..",
								"-----",
								".----",
								"..---",
								"...--",
								"....-",
								".....",
								"-....",
								"--...",
								"---..",
								"----.",
								".----.",
								".--.-.",
								"---...",
								"--..--",
								"...-..-",
								"-...-",
								"-.-.--",
								".-.-.-",
								"..--..",
								".-..-." };

		string morseString[SIZE];
		bitset<16> morseBinary[SIZE];

		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < morseCode[i].length(); j++)
			{
				if (morseCode[i][j] == '.')
				{
					morseString[i] += "10";
				}
				else
				{
					morseString[i] += "01";
				}
			}
			//cout << "morseString[i]: " << morseString[i] << endl;
			bitset<16> bits(morseString[i]);
			morseBinary[i] = bits;
			//cout << "morseBinary[i]: " << morseBinary[i] << endl;
		}

		//calculate the keys, and make/set the map		
		int morseKeys[SIZE];
		for (int m = 0; m < SIZE; m++)
		{
			morseKeys[m] = (int)(morseBinary[m].to_ulong());
			//key-value pair: int-char
			morseCodeMap[morseKeys[m]] = morseChars[m];
		}
		
	} //end of setMorseCodeMap method

	//getters
	unordered_map<int, char> getMorseCodeMap()
	{
		return this->morseCodeMap;
	}

}; //the end of class Table

//this class handles the interaction with the binary file
class FileIO
{
private:
	//instance variables
	string fileName;
public:
	//default constructor
	FileIO(){}
	FileIO(string fn)
	{
		this->fileName = fn;
	}

	//getters
	string getFileName()
	{
		return this->fileName;
	}

	/* This method will read in the binary file, one char at a time.
	   Then check every two bits, until it reaches 00.
	   Then check the bitset against the MorseCodeMap I made, to find the human
	   readable characters.
	   Then return the text.
	*/
	string  readFile()
	{
		ifstream inFile;
		//open a binary file
		inFile.open(this->fileName, ios::in | ios::binary);
		//handle file open exception
		if (!inFile)
		{
			throw "\Cannot open the file!\n";
		}

		//if opens ok, read one char a time, and append to the return string
		//get the morseCodeMap
		Table t1;
		//vector<char> map = t1.getMorseCodeMap();
		unordered_map<int, char> map = t1.getMorseCodeMap();

		string res = "";
		unsigned char c1;
		
		//each char is one 8 bits set, but the result set could be larger than 8 bits, 
		//so I use 16 bistset
		bitset<16> readSet; //the bitset holds the char read in
		bitset<16> tempSet; //the bitset holds the two bits every time reading in two bits
		bitset<16> holdSet; //the bitset holds all the bits until it reaches to the end of letter
		//create a check bit to check two bits at a time
		bitset<16> check_bit(string("0000000011000000"));
		
		//for an 8 bit char, extracting 2 bits at a time from the most significant bit
		//to the least significant,
		//the starting position for the 2nd bit is 6, then 4, then 2, then 0
		int bitPos = 6; 
		int lookupKey = 0; //the decimal values of the bitset
		int shiftLeftCount = 0; //each time the holdBitset needs to shift left
				
		//while not end of file
		while (inFile >> c1)
		{
			//reset check bit to original
			check_bit = 0xC0; //"0000000011000000"
			//cout << "check_bit_original: " << check_bit << endl;
			//reset the bit position when starting to read a new char
			bitPos = 6;
			//cout << "c1: " << c1 << endl;
			
			//readSet = c1;
			//cout << "readSet original: " << readSet << endl;
			while (bitPos >= 0)
			{
				//reset readSet to original
				readSet = c1;
				//extract two bits
				readSet &= check_bit;
				//cout << "readSet: " << readSet << endl;
				
				//if reach the end of the letter
				if (readSet == 0)
				{
					//cout << "Final holdSet: " << holdSet << endl;
					//cout << "\n\n" << endl;
					//converting to the decimal value and find it in the code map
					lookupKey = (int)(holdSet.to_ulong());
					if (map[lookupKey] != NULL)
					{
						//find the letter from the map, and concatenate to the return string
						res += map[lookupKey]; 
					}
					//cout << "res= " << res <<  endl;
					//reset
					lookupKey = 0;
					tempSet = 0;
					holdSet = 0;

					shiftLeftCount = 0;
				}
				else if (readSet == check_bit)  //word space 11
				{
					//reset some values
					res += " "; //add a space between the words
					tempSet = 0;
					lookupKey = 0;
					holdSet = 0;

					shiftLeftCount = 0;
				}
				else //not letter space or word space
				{
					//now shift the readset to the right, to a 16 bit set
					tempSet = readSet >> bitPos;
					//when it is not the first two bits, then shift left by 2
					if (shiftLeftCount != 0)
						holdSet = holdSet << 2;
					shiftLeftCount++;
					//concatenate the tempSet to holdSet
					holdSet |= tempSet;
					//cout << "tempSet: " << tempSet << endl;
					//cout << "holdSet: " << holdSet << endl;
				}

				//shift the check bit to the appropriate bit position in the char
				check_bit = check_bit >> 2;
				bitPos -= 2;
				
			} //the end of inner while loop for reaching each new char
			
			
			
		}//the end of outer while loop

		//close the file stream
		inFile.close();

		return res;

	} //the end of readFile() method
}; //the end of class FileIO

//this class calling the methods to do the conversion
class Converter
{
private: string fileName;

public:
	//default constructor
	Converter(){}
	//other constructor
	Converter(string fileName)
	{
		this->fileName = fileName;
	}

	//getters
	string getFileName()
	{
		return this->fileName;
	}

	//other method
	//get the text from the binary file that made from MorseCode
	string getTextFromCode()
	{
		FileIO f1(this->fileName);
		return f1.readFile();
	}

}; //the end of class MorseToText


int main()
{
	Converter c1("Morse.bin");
	string out = c1.getTextFromCode();
	cout << "This is the translated text: " << endl;
	cout << "\n\n" << endl;
	cout << out << endl;
	system("pause");
	return 0;
}