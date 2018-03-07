#include<iostream>
#include<string>
#include<fstream>
using namespace std;

#include"Inventory.h"
#include"Book.h"

//dfault constructor
Inventory::Inventory() : Module() 
{
	bookptr = new Book[MAX_BOOKS];
	bookcount = 0;
	pos = 0;
}

//copy constructor
Inventory::Inventory(Inventory &temp)
{
	int num = temp.bookcount;	
	bookcount = num;
	bookptr = new Book[MAX_BOOKS];
	// copy bookcount to the new class object 
	for (int i = 0; i < num; i++)	// copy each object 
	{
		bookptr[i] = temp.bookptr[i];
	}
}

Inventory::~Inventory()
{
	delete[] bookptr;
}


void Inventory::setBookCount(int c)
{
	bookcount = c;
}
int Inventory::getBookCount()const
{
	return bookcount;
}

Book * Inventory::getBookPtr()
{
	return this->bookptr;
}

//push book objects to the book array
void Inventory::push(const Book &temp)
{
	if (this->bookcount < MAX_BOOKS)
	{
		(this->bookptr)[bookcount] = temp;
		(this->bookcount)++;
	}
	else
	{
		cerr << "can't push because array is full";
		exit(10);
	}
}

//print book screen for Cashier
void Inventory::outputForCashier()
{
	if (bookcount == 0)
	{
		cout << "\nNo Books in the inventory \n";
	}
	else
	{
		for (int i = 0; i < bookcount; i++)
		{
			cout << "\nItem Num " << i + 1; 
			(this->bookptr)[i].outputForCashier();
		}
	}
}


int Inventory::lookUp(string search)
{
	for (int n = 0; n < bookcount; n++)
	{
		if (bookptr[n].getIsbn() == search)
		{
			pos = n;
			return n;
		}
	}
	cout << "This book is not in the database." << endl;
	return -1;
}

void Inventory::addBook()
{
	string str = "";
	int intnum = 0;
	double floatnum = 0;

	cout << "ISBN: ";
	getline(cin, str);
	if (lookUp(str) != -1)
	{
		cout << "This book has already been in the database!" << endl;
	}
	else
	{
		bookptr[bookcount].setIsbn(str);
		cout << "Title: ";
		getline(cin, str);
		bookptr[bookcount].setTitle(str);
		cout << "Author: ";
		getline(cin, str);
		bookptr[bookcount].setAuthor(str);
		cout << "Publisher: ";
		getline(cin, str);
		bookptr[bookcount].setPublisher(str);

		//bookptr[bookcount].setDate(currentDate);
		bookptr[bookcount].setDate("01/01/2017");

		cout << "Quantity: ";
		cin >> intnum;
		cin.ignore();
		bookptr[bookcount].setQuantityOnHand(intnum);
		cout << "Wholesale Cost: ";
		cin >> floatnum;
		cin.ignore();
		bookptr[bookcount].setWholesale(floatnum);
		cout << "Retail Price: ";
		cin >> floatnum;
		cin.ignore();
		bookptr[bookcount].setRetail(floatnum);
		bookcount++;
	}
}


void Inventory::deleteBook()
{
	int input, number;
	cout << "Enter ISBN number or press 0 to see list of books" << endl;
	cin >> input;
	cin.ignore();
	if (input == 0)
	{
		for (int n = 0; n < bookcount; n++)
		{
			cout << n + 1 << ". " << bookptr[n].getIsbn() << endl;
			cout << bookptr[n].getTitle() << endl;
			cout << bookptr[n].getAuthor() << endl;
			cout << endl;
		}
		cout << "Indicate the book you want to delete by a number:" << endl;
		cin >> number;
		cin.ignore();
		bookptr[number - 1] = bookptr[bookcount - 1];
		bookcount--;
	}
	else if (lookUp(to_string(input).c_str()) != -1)
	{
		bookptr[pos] = bookptr[bookcount - 1];
		bookcount--;
	}
	cout << "Delete successful!" << endl;
}


void Inventory::changeInfo()
{
	int state = 0, num = 0;
	int count = 0;
	double floatnum = 0.0;
	string ISBN;
	string str;
	cout << "Please enter ISBN number:" << endl;
	cin >> ISBN;
	cin.ignore();
	if (lookUp(ISBN) != -1)
	{
		state = changeInfoMenu();
		count = state < 8 ? 7 : 0;
		while (7 >= count)
		{
			switch (state)
			{
			case 1:
				cout << "ISBN :";
				getline(cin, str);
				bookptr[pos].setIsbn(str);
				break;
			case 2:
				cout << "Title: ";
				getline(cin, str);
				bookptr[pos].setTitle(str);
				break;
			case 3:
				cout << "Author: ";
				getline(cin, str);
				bookptr[pos].setAuthor(str);
				break;
			case 4:
				cout << "Publisher: ";
				getline(cin, str);
				bookptr[pos].setPublisher(str);
				break;
			case 5:
				cout << "Quantity: ";
				cin >> num;
				cin.ignore();
				bookptr[pos].setQuantityOnHand(num);
				break;
			case 6:
				cout << "Wholesale Cost: ";
				cin >> floatnum;
				cin.ignore();
				bookptr[pos].setWholesale(floatnum);
				break;
			case 7:
				cout << "Retail Price: ";
				cin >> floatnum;
				cin.ignore();
				bookptr[pos].setRetail(floatnum);
				break;
			case 8:
				state = 0;
				break;
			}// switch statement
			state++;
			count++;
		}//while statement
	}//if statement
}


int Inventory::changeInfoMenu()
{
	int choice;
	cout << "1.Change ISBN" << endl;
	cout << "2.Change Title" << endl;
	cout << "3.Change Author" << endl;
	cout << "4.Change Publisher" << endl;
	cout << "5.Change Qty" << endl;
	cout << "6.Change Wholesale Cost" << endl;
	cout << "7.Change Retail Price" << endl;
	cout << "8.Change Everything" << endl;
	cout << "\n";
	do{
		cout << "Please enter a choice from 1 to 8" << endl;
		cin >> choice;
		cin.ignore();
	} while (choice < 0 || choice>8);
	cout << "Please enter new information:" << endl;
	return choice;
}


void Inventory::toFile(string outfile)
{
	ofstream out;
	out.open(outfile.c_str());
	for (int i = 0; i < bookcount; i++)
	{
		out << bookptr[i].getIsbn() << endl;
		out << bookptr[i].getTitle() << endl;
		out << bookptr[i].getAuthor() << endl;
		out << bookptr[i].getPublisher() << endl;
		out << bookptr[i].getDate() << endl;
		out << bookptr[i].getQuantityOnHand() << endl;
		out << bookptr[i].getWholesale() << endl;
		out << bookptr[i].getRetail() << endl;
	}
	out.close();
}


int Inventory::showMenu()
{
	//cout << "";
	return 0;
}