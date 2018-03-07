/*
CIS29
Lab0
Due Date: 04/20/2017
Author: Ying Xie
Program Description: This program reads a text file, extract all the integers
from the file, then converts these integers (as degrees) to ASCII code

The file includes the following classes:
FileIO -- handling the file input, and parsing the text to integers.
(I decide to put the file parse function in this class, instead of creating
a new class to do the parsing. Because it feels natural to have FileIO class
to handle input/output from the file)
ConvertToAscii -- converting a vector of hex decimal numbers to Ascii code/string
DegreeToAscii -- this is a class that calls the methods from the above three classes,
and makes the conversion

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class FileIO
{
private:
	string fileName;
	vector<int> degrees;

public:
	//default constructor
	FileIO(){}

	//other constructor
	FileIO(string fileName)
	{
		this->fileName = fileName;
		extractInts();
	}

	//setter
	void setFileName(string fileName)
	{
		this->fileName = fileName;
	}
	//extract the integers from the file
	void extractInts()
	{
		int num;
		char c1;
		char c2;
		string s1 = "";
		ifstream inFile;
		inFile.open(this->fileName);
		//handle file open exception
		if (!inFile)
		{
			throw "\nCannot open the text file!\n";
		}

		//if opens ok, then go on until it reaches the eof
		while (inFile >> c1)
		{
			while (isdigit(c1))
			{
				c2 = c1 - '0';
				num = c2;
				s1 += to_string(num);
				c1 = NULL;
				inFile >> c1;
			}
			if (!s1.empty()) //add the numbers to the vector
				this->degrees.push_back(stoi(s1));
			s1 = "";
		}
		//close the file stream
		inFile.close();
	}

	//getter
	vector<int> getDegrees()
	{
		return this->degrees;
	}
	string getFileName()
	{
		return this->fileName;
	}
};



class ConvertToAscii
{
private:
	string ascii;

public:
	//default constructor
	ConvertToAscii(){}
	//other constructor
	ConvertToAscii(const vector<int> &hex)
	{
		setAscii(hex);
	}
	//setter
	//converting hex decimal numbers to ascii code/string
	void setAscii(const vector<int> &hex)
	{
		//declare return string
		string re = "";

		int i = 0; //counter
		int max = hex.size();
		char cAdd;
		int val_1;
		int val_2;

		//if max is an odd number, then leave the last number out
		if (max % 2 != 0)
			max -= 1;
		//loop through the vector, but increment by 2, as hex takes two numbers
		for (i = 0; i < max; i += 2)
		{
			val_1 = (int)(hex[i] / 22.5);
			val_2 = (int)(hex[i + 1] / 22.5);
			cAdd = 16 * val_1 + val_2;
			re += cAdd;
		}
		//set the instance variable
		this->ascii = re;
	}
	//getter
	string getAscii()
	{
		return this->ascii;
	}
};

class DegreeToAscii
{
private:
	string fileName;
public:
	//default constructor
	DegreeToAscii(){}
	//other constructor
	DegreeToAscii(string fileName)
	{
		this->fileName = fileName;
	}

	//setter
	void setFileName(string fileName)
	{
		this->fileName = fileName;
	}

	//getter
	string getFileName()
	{
		return this->fileName;
	}

	//other method
	//this is a wrapper method 
	string convert()
	{
		//read the file
		FileIO f1(this->getFileName());

		//first get the numbers from the input file
		vector<int> degrees = f1.getDegrees();
		//convert the degrees(as hex decimal values) to ascii characters
		ConvertToAscii a1(degrees);
		//display to the console
		return a1.getAscii();
	}


};


//to run the program
int main()
{
	try
	{
		DegreeToAscii dToA("AsciiDegrees.csv");
		//display to the console
		cout << "This is what is in this file: \n" << endl;
		cout << dToA.convert();
	}
	catch (char* e)
	{
		cerr << e << endl;
		cout << "Press any key to exit the program." << endl;
		exit(100);
	}

	system("pause");
	return 0;
}