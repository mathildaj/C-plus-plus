/*
This is the header file for Cashier class, it inherits Module class
*/

#ifndef __CASHIER_H__
#define __CASHIER_H__

#include <iostream>
#include <string>


using std::string;

#include "Module.h"
#include "Book.h"
#include "Inventory.h"

class Cashier : public Module
{
private:
	const double TAX_RATE = 0.0875; //using the current Cupertino sales tax rate
	const int MAX_BOOKS = 25; //arbitary numbers, the max numbers a user can buy
	int buyerBookCount; //the actual count of books the buyer bought. Should be <= max_books
	Book bookBuyerPtr[25]; //book array from the buyer, max 25 books
	
public:
	//default constructor
	Cashier();
	
	//destructor
	~Cashier();

	//getters and setters
	//getters and setters
	void setBuyerBookCount(int);
	int getBuyerBookCount()const;

	//other functions
	int cashierOutput(Inventory &in); //print out the output for all the books user buying
	//has its own version of showMenu for Cashier Module
	int showMenu(); //the buyer's final purchase screen
	
};


#endif