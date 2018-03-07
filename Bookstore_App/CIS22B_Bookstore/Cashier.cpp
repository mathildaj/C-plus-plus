/*
this is the implementation of class Testbook
*/

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

#include "Cashier.h"

// default constructor
Cashier::Cashier() : Module() 
{
	buyerBookCount = 0;
}


//destructor //need to implement anything??
Cashier::~Cashier()
{
	//release the memory
	//delete[] bookBuyerPtr;
}

void Cashier::setBuyerBookCount(int c)
{
	buyerBookCount = c;
}
int Cashier::getBuyerBookCount()const
{
	return buyerBookCount;
}


//this shows the selection of the books and interact with user
int Cashier::cashierOutput(Inventory &in)
{
	int return_value = 1; //to use to return to main()

	int itemNum = 0;
	int qty = 0;
	int inBookQty = 0;
	int bookInventoryCount = in.getBookCount();

	buyerBookCount = 0;

	//ask user to choose 
	cout << "\nEnter item num to buy, enter -1 to checkout, or enter 0 to return to main menu:" << endl;
	cin >> itemNum;
	cin.ignore();
	
	while (itemNum != -1 && itemNum != 0)
	{
		//ask user to choose item num
		while (itemNum > bookInventoryCount || itemNum < 1)
		{
			cout << "\nThe item num you entered is invalid. Try again:" << endl;
			cin >> itemNum;
			cin.ignore();
			
		}
		
		//now ask for quantity of the book
		cout << "\nPlease enter the quantity of the book you want:" << endl;
		cin >> qty;
		cin.ignore();
		//get the existing inventory quantity
		inBookQty = in.getBookPtr()[itemNum - 1].getQuantityOnHand();
		while (qty < 0 || qty > inBookQty)
		{
			cout << "The quantity you entered is invalid. Try again:" << endl;
			cin >> qty;
			cin.ignore();
		}

		//reset the quantity of the book in inventory
		in.getBookPtr()[itemNum - 1].setQuantityOnHand(inBookQty - qty);
		//add the new book to the buyer books array
		Book temp(in.getBookPtr()[itemNum - 1].getIsbn(),
				in.getBookPtr()[itemNum - 1].getTitle(),
				in.getBookPtr()[itemNum - 1].getAuthor(),
				in.getBookPtr()[itemNum - 1].getPublisher(),
				in.getBookPtr()[itemNum - 1].getDate(),
				//in.getBookPtr()[itemNum - 1].getQuantityOnHand(),
				qty,
				in.getBookPtr()[itemNum - 1].getWholesale(),
				in.getBookPtr()[itemNum - 1].getRetail()
			);
		
		bookBuyerPtr[buyerBookCount] = temp;
		buyerBookCount++;
		this->setBuyerBookCount(buyerBookCount);
		
		//if after purchasing, quantityOnHand ==0, delete the book??
		//method to be written in Inventory module??
		//---------------------------------

		//buy more books
		cout << "\nTo buy another book, enter item num, or enter -1 to checkout:" << endl;
		cin >> itemNum;
		cin.ignore();
	}
	
	//go to checkout
	if (itemNum == -1)
	{
		//update the inventory file
		//need to be filled---inventory write to file method
		return_value = showMenu();
		
	}
	else if (itemNum == 0)
	{
		//return to main menu
		return_value = 0;
	}

	return return_value;
} //end of showMenu()


//this is the method to show the user the final purchase screen
int Cashier::showMenu()
{
	int return_value = 1; //use to return to main()

	double subTotal = 0.00;
	double tax = 0.00;
	double runningTotal = 0.00;
	double grandTotal = 0.00;

	int choice = 0;
		
	cout << "\nSerendipity Book Sellers" << endl;
	cout << "\n\n\n";
	cout << "Date: " << "12/12/2017" << "\n\n";
	cout << "Qty\tISBN\t\tTitle\t\t\t\tPrice\t\tTotal\n";
	cout <<
		"__________________________________________________________________________________________________";
	cout << "\n\n\n";

	for (int i = 0; i < buyerBookCount; i++)
	{
		subTotal = bookBuyerPtr[i].getQuantityOnHand() * bookBuyerPtr[i].getRetail();
		tax += subTotal * TAX_RATE;
		runningTotal += subTotal;
		grandTotal = tax + runningTotal;

		//display
		cout << bookBuyerPtr[i].getQuantityOnHand() << "\t";
		cout << left << setw(14) << bookBuyerPtr[i].getIsbn() << "\t";
		cout << left << setw(26) << bookBuyerPtr[i].getTitle() << "\t$ ";
		//cout << left << setw(30) << bookBuyerPtr[i].getTitle() << "\t$ ";
		cout << fixed << showpoint << right << setprecision(2);
		cout << setw(6) << bookBuyerPtr[i].getRetail() << "\t$ ";
		cout << setw(6) << subTotal << "\n\n\n";
	}

	cout << "\t\tSubtotal\t\t\t\t\t\t$ ";
	cout << setw(6) << runningTotal << "\n";
	cout << "\t\tTax\t\t\t\t\t\t\t$ ";
	cout << setw(6) << tax << "\n";
	cout << "\t\tTotal\t\t\t\t\t\t\t$ ";
	cout << setw(6) << grandTotal << "\n\n";

	cout << "\n\nThank you for shopping at Serendipity!\n";

	cout << "\n\nNow enter 0 to go back to main menu, or -1 to exit: \n" << endl;
	cin >> choice;
	cin.ignore();

	while (choice != -1 && choice != 0)
	{
		cout << "\n\nPlease enter 0 to go back to main menu, or -1 to exit: \n" << endl;
		cin >> choice;
		cin.ignore();
	}

	if (choice == 0)
	{
		return_value = 0;
	}
	else if (choice == -1)
	{
		exit(0); //exit the program
	}

	return return_value;
}
