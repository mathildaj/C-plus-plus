/*
This is the header file for Inventory class, it inherits Module class
*/

#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include <iostream>
#include <string>
#include<fstream>

using namespace std;

#include "Module.h"
#include"Book.h"

class Inventory: public Module
{
private:
	const int MAX_BOOKS = 25; //arbitary numbers, the max numbers in the inventory
	int bookcount; //the actual count of books in the inventory. Should be <= max_books
	Book *bookptr = 0; //pointer to the array of Books
	int pos;//position of the book in array
public:
	//default constructor
	Inventory();
	//copy constructor
	Inventory(Inventory &);

	//destructor
	~Inventory();
	
	//getters and setters
	void setBookCount(int);
	int getBookCount()const;
	Book * getBookPtr();
		
	//helper functions
	void push(const Book &temp);
	void outputForCashier();
	//Book pop();
	
	
	int lookUp(string);
	void addBook();
	void deleteBook();
	void changeInfo();
	int changeInfoMenu();
	//write data to file, call this function before ending program
	void toFile(string);

	
	
	//override showMenu
	int showMenu();
	
};


#endif