/*
this is the implementation of class Module
*/

using namespace std;
#include "Module.h"


//default constructor
Module::Module(){}

//destructor. Need to implement anything here??
Module::~Module(){}

//other methods
//this method will be overriden by the child classes
int Module::showMenu()
{
	return 0;
}

//void backToMain()
//{
//	//cout << "This is the menu: " << endl;
//	cout << "Serendipity Book Sellers" << endl;
//	cout << "\n\n\n";
//	cout << "1. Cashier Module" << endl;
//	//to be filled
//	cout << "4. Exit" << endl;
//}
