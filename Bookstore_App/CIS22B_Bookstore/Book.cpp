/*
this is the implementation of class Book
*/

using namespace std;
#include "Book.h"


//default constructor
Book::Book()
{
	isbn = "";
	title = "";
	author = "";
	publisher = "";
	date = "01/01/2017";
	quantityOnHand = 0;
	wholesaleCost = 0.00;
	retailPrice = 0.00;
}

//override constructor
Book::Book(string isbn, string title, string author, string publisher,
	string date, int quantityOnHand, 
	double wholesaleCost, double retailPrice)
{
	this->isbn = isbn;
	this->title = title;
	this->author = author;
	this->publisher = publisher;
	
	this->date = date;
	this->quantityOnHand = quantityOnHand;
	this->wholesaleCost = wholesaleCost;
	this->retailPrice = retailPrice;
}

//copy constructor
Book::Book(const Book &in)
{
	this->isbn = in.isbn;
	this->title = in.title;
	this->author = in.author;
	this->publisher = in.publisher;
	this->date = in.date;
	this->wholesaleCost = in.wholesaleCost;
	this->retailPrice = in.retailPrice;
}

//destructor. Need to implement anything here??
Book::~Book(){}

//setters
void Book::setIsbn(string i)
{
	isbn = i;
}
void Book::setTitle(string t)
{
	title = t;
}
void Book::setAuthor(string a)
{
	author = a;
}
void Book::setPublisher(string p)
{
	publisher = p;
}

void Book::setDate(string d)
{
	date = d;
}
void Book::setQuantityOnHand(int q)
{
	quantityOnHand = q;
}
void Book::setWholesale(double w)
{
	wholesaleCost = w;
}
void Book::setRetail(double r)
{
	retailPrice = r;
}

//getters
string Book::getIsbn() const
{
	return isbn;
}
string Book::getTitle() const
{
	return title;
}
string Book::getAuthor() const{
	return author;
}
string Book::getPublisher() const
{
	return publisher;
}


string Book::getDate() const
{
	return date;
}
int Book::getQuantityOnHand()const
{
	return quantityOnHand;
}
double Book::getWholesale() const
{
	return wholesaleCost;
}
double Book::getRetail() const
{
	return retailPrice;
}


/* **************** Book::operator= *******************************
sets the values in the left hand object from the right hand object
*/
Book & Book::operator=(const Book & bookB)
{
	setup(bookB.isbn, bookB.title, bookB.author, bookB.publisher, bookB.date,
		bookB.quantityOnHand, bookB.wholesaleCost, bookB.retailPrice);

	return *this;
}

/* ************ Book::Friend Operator bool ********************
This function tests to see if the left and right Book are equal
size saved in calling function via reference parameter
*/
bool operator==(const Book &left, const Book &right)
{
	if (left.isbn == right.isbn)
	{
		return true;
	}
	else
	{
		return false;
	}
}



void Book::setup(string isbn, string title, string author, string publisher,
	string date, int quantityOnHand,
	double wholesaleCost, double retailPrice)
{
	this->isbn = isbn;
	this->title = title;
	this->author = author;
	this->publisher = publisher;
	this->date = date;
	this->quantityOnHand = quantityOnHand;
	this->wholesaleCost = wholesaleCost;
	this->retailPrice = retailPrice;
}


void Book::outputForCashier()
{
	cout << left << endl;
	cout << setprecision(2) << fixed << endl;
	cout << setw(15) << "ISBN:" << setw(15) << isbn << endl;
	cout << setw(15) << "Title:" << setw(15) << title << endl;
	cout << setw(15) << "Price:" << setw(3) << "$ " << retailPrice << endl;
	cout << setw(15) << "Qty:" << setw(15) << quantityOnHand << endl;
}