/*
This is the header file for Report class, it inherits Module class
*/

#ifndef __REPORT_H__
#define __REPORT_H__

#include <iostream>
#include <string>

using namespace std;

#include "Module.h"

class Report: public Module
{
private:
	Inventory inv; //the inventory object report needs 
	
public:
	//default constructor
	Report();
	
	void listInventory();
	void listWholesaleValue();
	void listRetailValue();
	void listByQty();
	void listByCost();
	void listByAge();

	//has its own version of showMenu for Cashier Module
	int showMenu();

};


#endif