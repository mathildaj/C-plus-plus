/*
This is the header file for Book class
*/

#ifndef __BOOK_H__
#define __BOOK_H__

#include <iostream>
#include <string>
#include <iomanip>

using std::string;

class Book
{
//instant variables
private:
	string isbn;
	string title;
	string author;
	string publisher;
	string date;
	int quantityOnHand;
	double wholesaleCost;
	double retailPrice;

public:
	//mimic the default constructor
	Book();
	
	//override constructor
	Book(string isbn, string title, string author, string publisher,
		string date, int quantityOnHand, 
		double wholesaleCost, double retailPrice);
	
	//copy constructor
	Book(const Book &in);
	
	//destructor
	~Book();

	//getters and setters
	void setIsbn(string i);
	void setTitle(string t);
	void setAuthor(string a);
	void setPublisher(string p);
	void setDate(string d);
	void setQuantityOnHand(int q);
	void setWholesale(double w);
	void setRetail(double r);
	


	string getIsbn() const;
	string getTitle() const;
	string getAuthor() const;
	string getPublisher() const;
	string getDate() const;

	int getQuantityOnHand()const;
	double getWholesale() const;
	double getRetail() const;

	// =operator overloading
	Book & Book::operator=(const Book & bookB);
	//friend function
	friend bool operator==(const Book &left, const Book &right);

	void setup(string isbn, string title, string author, string publisher,
			string date, int quantityOnHand,
			double wholesaleCost, double retailPrice);
	
	//print out book object to the screen
	void outputForCashier();
};
#endif