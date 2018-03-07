#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

#include "Module.h"
#include "Book.h"
#include "Cashier.h"
#include "Inventory.h"
#include "Report.h"


//function prototype
void mainMenu();
void input(Inventory &in);


void mainMenu() //display mainMenu screen
{
	
	//to be filled
	cout << "Serendipity Book Sellers" << endl;
	cout << "\n\n\n";
	cout << "1. Cashier Module" << endl;
	//to be filled----------------other choices
	cout << "4. Exit" << endl;
}

void input(Inventory &in)
{
	string isbn;
	string title;
	string author;
	string publisher;
	string date;
	string strQty;
	string strWholesale;
	string strRetail;

	int quantityOnHand;
	double wholesaleCost;
	double retailPrice;

	ifstream inputFile;				// define and open the input text file
	inputFile.open("inventory.txt");
	if (!inputFile)					// if error use cerr and exit() 
	{
		cerr << "\nThere is a failure opening inventory file!\n";
		exit(100);
	}
	// ok, go on									
	while (inputFile.peek() != EOF)		// Loop to process all the information
	{
		getline(inputFile, isbn, '|');
		getline(inputFile, title, '|');		
		getline(inputFile, author, '|');
		getline(inputFile, publisher, '|');
		getline(inputFile, date, '|');		
		getline(inputFile, strQty, '|');
		getline(inputFile, strWholesale, '|');
		getline(inputFile, strRetail); //the last one does not have |

		quantityOnHand = atoi(strQty.c_str());
		wholesaleCost = atof(strWholesale.c_str());
		retailPrice = atof(strRetail.c_str());

		// ok, go on ... finally create the Book object named temp
		Book temp(isbn, title, author, publisher, date,
			quantityOnHand, wholesaleCost, retailPrice);
		in.push(temp);		// push temp into the Inventory
	}								// repeat until all objects have been read
	// all done... 
	inputFile.close();      // close the file
}

int main()
{
	//user interaction
	//to be filled, not completed----------------------------

	//create an Inventory object
	//get the book array from the input file
	Inventory newIn;
	input(newIn);
	Cashier c1;

	int choice = 0;
	int user_return = 0;
	
	mainMenu();
	cout << "\nEnter Your Choice: ";
	cin >> choice;
	cin.ignore();

	while (choice != 4)
	{
		switch (choice)
		{
			case 1: 
				newIn.outputForCashier();
				user_return = c1.cashierOutput(newIn);
				if (user_return == 0)
				{
					mainMenu();
					cout << "\nEnter Your Choice: ";
					cin >> choice;
					cin.ignore();
				}
				break;
			default:
				break;
			//other cases--to be filled----------
			
		}
	}

	system("pause");

	return 0;
}

