/*
This is the header file for Module class
*/

#ifndef __MODULE_H__
#define __MODULE_H__

#include <iostream>
#include <string>

using std::string;

class Module
{
	
public:
	//mimic the default constructor
	Module();

	//destructor
	virtual ~Module();

	
	//other methods
	//this method to be overriden by child classes
	virtual int showMenu();
	
};
#endif