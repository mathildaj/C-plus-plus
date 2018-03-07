/*
Program Description: Write a program to decrypt the old punch card code into plain text

*/

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;


//this is PunchCardMap class
class PunchCardMap
{
//instance variables
private:
	map<string, char> charMap;
	vector<vector<char>> charTable;
	const int COL = 16;
	const int ROW = 4;
public:
	//default constructor
	PunchCardMap()
	{
		readChars();
		setCharMap();
	}
			
	//getters and setters
	void readChars()
	{
		//all the characters in the order of row->col
		//make sure the first space char is not removed, and the escape chars not removed
		string allChars = " 123456789:#@\'=\"&ABCDEFGHI[.<(+!-JKLMNOPQR]$*);^0/STUVWXYZ\\,%_>?";
		int count = 0;
		
		for (int i = 0; i < ROW; i++)
		{
			this->charTable.push_back(vector<char>());
			for (int j = 0; j < COL; j++)
			{
				this->charTable[i].push_back(allChars[count]);
				//cout << "row= " << i << endl;
				//cout << "col= " << j << endl;
				//cout << "charTable[i][j] " << charTable[i][j] << endl;
				//cout << "count: " << count << endl;
				count++;
			}
		}
	} //end of readChars

	void setCharMap()

	{
		vector<string> hKeys;
		hKeys.push_back("-");
		hKeys.push_back("1");
		hKeys.push_back("2");
		hKeys.push_back("3");
		hKeys.push_back("4");
		hKeys.push_back("5");
		hKeys.push_back("6");
		hKeys.push_back("7");
		hKeys.push_back("8");
		hKeys.push_back("9");
		//hKeys.push_back("2-8");
		//hKeys.push_back("3-8");
		//hKeys.push_back("4-8");
		//hKeys.push_back("5-8");
		//hKeys.push_back("6-8");
		//hKeys.push_back("7-8");
		hKeys.push_back("28");
		hKeys.push_back("38");
		hKeys.push_back("48");
		hKeys.push_back("58");
		hKeys.push_back("68");
		hKeys.push_back("78");

		vector<char> vKeys;
		vKeys.push_back('-');
		vKeys.push_back('Y');
		vKeys.push_back('X');
		vKeys.push_back('0');

		string key;
		char value;

		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				//only add if not '-'
				if (vKeys[i] != '-' && hKeys[j] != "-")
					key = vKeys[i] + hKeys[j];
				else if (vKeys[i] != '-')
					key = vKeys[i];
				else
					key = hKeys[j];

				value = this->charTable[i][j];
				//this->charMap.insert(make_pair(key, value));
				this->charMap[key] = value;
			}
		}

	}

	//get the map
	map<string, char> getCharMap()
	{
		return this->charMap;
	}

	//print
	void printTable()
	{
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				cout << charTable[i][j] << std::endl;

			}
		}
	}

	void printMap()
	{
		for (pair<string, char> p : this->charMap)
			cout << p.first << '\t' << p.second << endl;
	}

}; //the end of PunchCard class


//this is FileIO class
class FileIO
{
private:
	const int ROW = 12;
	//vector<string> card;
	vector <vector<string>> allCards;
public:
	//default constructor
	FileIO(){}
	FileIO(string fileName)
	{
		setCard(fileName);
	}

	void setCard(string fileName)
	{
		//open the file
		ifstream inFile;
		inFile.open(fileName);
		//file opening exception
		if (!inFile)
			throw "Cannot open the XML file!";

		string line = "";
		int row = 0;
	
		while (getline(inFile, line))
		{
			//reset to 0 every time after reading one card
			row = 0;
			//if it is separating line
			if (line[0] == '-')
			{
				vector<string> card;
				//start reading the card 
				while (row < ROW) //read 12 lines
				{
					getline(inFile, line);
					card.push_back(line);
					row++;
				}
				if (!card.empty())
					this->allCards.push_back(card);
			}

		} //end of while loop
		
		//close the file
		inFile.close();

	} //end of setCard

	vector<vector<string>> getAllCards()
	{
		return this->allCards;
	}

}; //this is the end of FileIO class


//this is the Rotation class
class Rotation
{
private:
	const int COL = 80;
	const int ROW = 12;
	vector<vector<int>> rotatedCard;
	
public:
	//default constructor
	Rotation(){}
	//other constructor
	Rotation(vector<string> card)
	{
		setRotatedCard(card);
	}

	//void setRotatedCard(vector<vector<int>> card)
	void setRotatedCard(vector<string> card)
	{
		//needs to clear it out, otherwise, will concatenate the last card
		this->rotatedCard.clear();

		for (int i = 0; i < COL; i++)
		{
			this->rotatedCard.push_back(vector<int>());
			for (int j = 0; j < ROW; j++)
			{
				this->rotatedCard[i].push_back((int)(card[j][i] - '0'));
			}
		}
	}

	vector<vector<int>> getRotatedCard()
	{
		return this->rotatedCard;
	}



}; //end of class Rotation


//this is the CardParsr class
class CardParser
{
private:
	const int COL = 12;
	const int ROW = 80;
	string parsedChars;
public:
	//default constructor
	CardParser(){}
	//other constructor
	CardParser(vector<vector<int>> card)
	{
		setParsedChars(card);
	}

	void setParsedChars(vector<vector<int>> card)
	{
		//clear out the vector, otherwise will concatenate the last result
		this->parsedChars.clear();

		string rowOrders = "YX0123456789"; //12 chars
		//get the map
		PunchCardMap p1;
		map<string, char> charMap = p1.getCharMap();

		for (int i = 0; i < ROW; i++)
		{
			string key = "";
			for (int j = 0; j < COL; j++)
			{
				if (card[i][j] == 1)
					key += rowOrders[j];
			}
			if (charMap[key] != NULL)
				this->parsedChars += charMap[key];
		}
	}

	string getParsedChars()
	{
		return this->parsedChars;
	}


}; //this is the end of CardParser


//this is the Converter class
class Converter
{
private:
	string fileName;
public:
	//default constructor
	Converter(){}
	//other constructor
	Converter(string fileName)
	{
		this->fileName = fileName;

	}

	void outputResults()
	{
		//FileIO f1("PunchCards.txt");
		FileIO f1(this->fileName);
		vector<vector<string>> allCards = f1.getAllCards();
		Rotation r1;
		CardParser c1;
		
		//loop through all the cards in the file
		for (auto it = allCards.begin(); it != allCards.end(); it++)
		{
			r1.setRotatedCard(*it);
			vector<vector<int>> rotatedCard = r1.getRotatedCard();
			c1.setParsedChars(rotatedCard);
			string result = c1.getParsedChars();
			cout << result << endl;
			cout << "\n" << endl;
			}
	
	}


}; //this is the end of the Converter class


//main
int main()
{
	Converter c1("PunchCards.txt");
	c1.outputResults();

	system("pause");
	return 0;
}