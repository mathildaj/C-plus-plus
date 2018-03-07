/* 
 objective: 
  This project demonstrates performance differences between 
   - simple integer generated hash numbers
   - simple string  generated hash numbers
   - and compares these with Collision avoidance methods:
    - Linear Probe (with same settings for demonstration)
	- Double Hash with a numbers Hash Function first then a string Hash Function
	- Double Hash with a numbers Hash Function first followed by a different numbers Hash Functions
	- Finally a Quadratic Hash Function
*/


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include <regex>
#include <stack>		
#include <queue>		
#include <bitset>		
#include <iomanip>
#include <sstream>		

using namespace std;

const string FILENAME = "Customer.csv";	// Filename... change it here if needed
double overFactor = 1.25;			// to set how large the hash table is 
bool simpleHash = true;
bool useNumbersForKey = true;			// set to true for Key from Numbers
bool useDoubleHash = false;
bool useDoubleHash2 = false;
bool useLinearProbe = false;
bool useQuadratic = false;
bool useDoubleDivisionHash = false;
//bool useNumbersForKey = false;			// set to false for StringsForKey
int collisions = 0;


//It has a prev and next pointers for implementing a doubly linked list.
// ***********************************************************************
class CustNode
{
public:
	string			data;		// plan to fill with the string from csv
	string		nameFirst;	// name
	string		nameLast;
	int			numFirst;
	int			numLast;
//public:
	CustNode *	prev;   // pointer to previous object
	CustNode *	next;	// new pointer to next object

	CustNode()				// default constructor
	{
		prev = NULL;
		next = NULL;	// new
		data = "$";
	}

	CustNode(string t)			// and another constructor
	{
		data = t;
		prev = NULL;
		next = NULL;
	}
	CustNode(string last, string first, int fstNum, int lstNum)			// and another constructor
	{
		//data = t;
		nameFirst = first;	// name
		nameLast	= last;
		numFirst	= fstNum;
		numLast		= lstNum;
		prev = NULL;
		next = NULL;
	}
	~CustNode() {}			// destructor
	
	void setName(string last, string first)
	{
		nameFirst = first;
		nameLast = last;
	}
	void setNumber(int first, int last)
	{
		numFirst = first;
		numLast = last;
	}
	void setData(string tempAll)
	{
		data = tempAll;
	}
	string getData()
	{
		return data;
	}
	string getName()
	{
		
		return nameLast + "," + nameFirst; 
	}
	string getNumber()
	{
		stringstream ss;
		stringstream ss2;
		ss << this->numFirst;
		ss2 << this->numLast;
		string stNumFirst = ss.str();
		string stNumLast = ss2.str();
		string answer = stNumFirst + "-" + stNumLast;
		return answer;
	}
};


class CustHashList 
{
private:
	CustNode *	head;
	CustNode *	tail;
	int			itemCount;
	int			numHash;
public:
	CustHashList() {}
	CustHashList(int hashNumber)		// default constructor
	{
		head = NULL;
		tail = NULL;
		itemCount = 0;
		numHash = hashNumber;
	}
	~CustHashList()					// destructor
	{
		clear();
	}
		

	void clear()	// clear this LinkedList
	{
		while ((this->head != NULL) && (this->tail != NULL))
		{
			CustNode * delPtr = this->tail;
			CustNode * prevPtr = this->tail->prev;
			this->tail->next = NULL;
			this->tail->prev = NULL;
			if (prevPtr != NULL) prevPtr->next = NULL;
			delete delPtr;
			this->tail = prevPtr;
			if (this->itemCount > 0) this->itemCount--;
			//cout << "\nInside CustHashList running clear().";
			//Be aware!!-- delete from the tail, NOT the head
		}
		if (tail == NULL) head = NULL;
		itemCount = 0;

		collisions = 0;
	}

	
bool isEmpty()			// define isEmpty:
	{
		return (itemCount == 0);
	}


	bool add(CustNode  newEntry)
	{
		CustNode *newPtr = new CustNode(newEntry);
		if ((head == NULL) && (tail == NULL))
		{
			head = newPtr;
			tail = newPtr;
			//itemCount++;
			
		}
		else
		{
			
			tail->next = newPtr;
			newPtr->prev = tail;
			tail = newPtr;

			collisions++;
			
		}
		itemCount++;
		
		return true;
	}
	//  Add at Tail... 
	
	bool addTail(string newEntry)
	{
		CustNode * newPtr = NULL;
		newPtr = new CustNode(newEntry);	// create new CustNode

		if ((head == NULL) && (tail == NULL))
		{
			head = newPtr;
			tail = newPtr;
		}
		else
		{
			newPtr->next = tail;
			newPtr->prev = NULL; // redunant predefined
			tail->prev = newPtr;
			tail = newPtr;
		}
		itemCount++;
		if (tail->data == newEntry) return true;
		else return false;
	}
	


	// Counts the number of times a given entry appears in list.
	// param anEntry The entry to be counted.
	// return The number of times anEntry appears in the list.
	int	 getFrequencyOfName(string anEntry)
	{
		CustNode * aPtr = head;
		int count = 0;
		while (aPtr != NULL)
		{
			if (aPtr->nameFirst == anEntry) count++;
			if (aPtr->nameLast == anEntry) count++;
			aPtr = aPtr->prev;
		}
		return count;
		// test this in main
	}
	// Tests whether this list contains a given entry.
	// param anEntry The entry to locate.
	// return True if list contains anEntry, or false otherwise.
	bool containsLastName(string anEntry)
	{
		CustNode * aPtr = head;
		bool found = false;
		while (aPtr != NULL)
		{
			if (aPtr->nameLast == anEntry) // added ->prev
			{
				found = true;
				cout << "  value is: " << aPtr->numFirst << "-" << aPtr->numLast ;
				return found;
			}
			aPtr = aPtr->next;
		}
		return found;
		// test this in main
	}
	bool containsNumber(int anEntry)
	{
		CustNode * aPtr = head;
		bool found = false;
		while (aPtr != NULL)  // aPtr... was aPtr
		{
			if ((aPtr->numLast == anEntry) || (aPtr->numFirst == anEntry))
			{
				found = true;
				
				cout << "\n   Lookup test found value is: " << aPtr->nameLast << "," << aPtr->nameFirst ;
				return found;
			}
			aPtr = aPtr->next;
		}
		return found;
		// test this in main
	}
	// how many are there?
	int getItemCount()
	{
		return itemCount;
	}

	CustNode * getHead()  // for STACK / LIFO operation
	{
		//if (itemCount > 0)
			return this->head;
		//else
		//	return NULL;
	}
	CustNode * getTail()  // for FIFO
	{
		if (itemCount > 0)
			return this->tail;
		else
			return NULL;
	}
	void prtTopDn()
	{
		if (itemCount == 0) return;
		cout << "\nPrinting out from top to bottom.";
		CustNode * whichItem = this->head;
		while (whichItem != NULL)
		{
			string st1 = whichItem->nameLast;
			string st2 = whichItem->nameFirst;
			string mid = ",";
			cout << "\n" << whichItem->data << st2 << mid << st1 << whichItem->numFirst << "-" << whichItem->numLast ;
			//whichItem = whichItem->prev;
			whichItem = whichItem->next;
		}
		cout << "\nThat is it... \n";
	}
	void prtBotUp()
	{
		if (itemCount == 0) return;
		cout << "\nPrinting from bottom to top.";
		CustNode * whichItem = tail;
		while (whichItem != NULL)
		{
			string st1 = whichItem->nameLast;
			string st2 = whichItem->nameFirst;
			string mid = ",";
			cout << "\n" << st2 << mid << st1 << whichItem->numFirst << "-" << whichItem->numLast;
			whichItem = whichItem->next;
		}
		cout << "\nThat is it... \n";
	}
};

// DynArrCustHash is modified it will make the hash table. 
//  each bin can contain a linked list
class DynArrCustHash
{
private:
	CustHashList	**	tPtr;		// points to where it is
	int		capacity;	// present capacity (total objects)
	int		size;		// present number of entries

public:
	DynArrCustHash(int c=1): capacity(c)
	{
		capacity = c;
		size = c;
		tPtr = new CustHashList*[capacity];
		//int hashSize = 1000;
		for (int i = 0; i < capacity; i++)
			tPtr[i] = new CustHashList();
	}
	~DynArrCustHash()				// a destructor
	{
		int howMany = this->size;
		for (int i = 0; i < howMany; i++)
		{
			this->tPtr[i]->~CustHashList();
			//cout << "\nDelete DynArrCustHash CustHashList [" << i << "]. ";
		}
		if (howMany == 0)	cout << "\nProblem in ~DynArrCustHash ERROR 555. ";
		else delete[] tPtr;
	}

	CustHashList  *operator[](int index)
	{
		return tPtr[index];
	}
	int getCurrentSize()	// how many are filled in
	{
		return size;
	}
	int getCapacity()		// total number of capacity containers
	{
		return capacity;
	}
	bool isEmpty()			// if empty isEmpty = 1
	{
		return (size == 0);
	}
	bool insert(CustHashList *newEntry)	// did it work ?
	{
		if (size < capacity)
		{
			tPtr[size] = newEntry;
			size++;
			return true;
		}
		else
		{
			//resize();
			tPtr[size] = newEntry;
			size++;
			return true;
		}
		return false;
	}
	bool remove(CustHashList *anEntry)	// look for "anEntry" 
	{
		int i = 0;
		bool stop = false;
		int temp = 0;
		while ((i < size) && !stop)
		{
			if (tPtr[i] == anEntry)
			{
				stop = true;
				temp = i;
			}
			i++;
		}
		int itsIndex = temp;
		for (itsIndex; itsIndex < size; itsIndex++)
		{
			tPtr[itsIndex] = tPtr[itsIndex + 1];
		}
		tPtr[size] = NULL;
		size--;
		return stop;
	}
	CustHashList *getValue(int index)		// what is in the one specified
	{
		return tPtr[index];
	}
	void clear()			// erase all the contents
	{
		for (int i = 0; i == size; i++)
		{
			tPtr[i] = 0;
		}
		size = 0;
	}
	
	
};

// DynArr will make the hash table. 
//  each bin can contain a linked list
/* ********* .... DynArr class is now a template ... ************* */
template <class T>
class DynArr
{
private:
	T	*	tPtr;		// points to where it is
	int		capacity;	// present capacity (total objects)
	int		size;		// present number of entries

public:
	DynArr()
	{
		capacity = 10;
		size = 0;
		tPtr = new T[capacity];
	}
	~DynArr()				// a destructor
	{
		if (size > 0)
		{
			for (int i = 0; i < size; i++)
				tPtr = NULL;
		}
		this->clear();
		delete[] tPtr;
	}
	int getCurrentSize()	// how many are filled in
	{
		return size;
	}
	int getCapacity()		// total number of capacity containers
	{
		return capacity;
	}
	bool isEmpty()			// if empty isEmpty = 1
	{
		return (size == 0);
	}
	bool insert(T newEntry)	// did it work ?
	{
		if (size < capacity)
		{
			tPtr[size] = newEntry;
			size++;
			return true;
		}
		else
		{
			resize();
			tPtr[size] = newEntry;
			size++;
			return true;
		}
		return false;
	}
	bool remove(T anEntry)	// look for "anEntry" 
	{
		int i = 0;
		bool stop = false;
		int temp = 0;
		while ((i < size) && !stop)
		{
			if (tPtr[i] == anEntry)
			{
				stop = true;
				temp = i;
			}
			i++;
		}
		int itsIndex = temp;
		for (itsIndex; itsIndex < size; itsIndex++)
		{
			tPtr[itsIndex] = tPtr[itsIndex + 1];
		}
		tPtr[size] = "";
		size--;
		return stop;
	}
	T getValue(int index)		// what is in the one specified
	{
		return tPtr[index];
	}
	void clear()			// erase all the contents
	{
		for (int i = size; i == 0; i--)
		{
			this->remove(tPtr[i]);
			// tPtr[i] = 0;
			cout << "\nClearing DynArr element [" << i << "]. ";
		}
		size = 0;
	}
	int resize()			// double it
	{
		//int newCapacity = capacity * 2;
		T * tPtr2;
		tPtr2 = new T[capacity * 2];
		for (int i = 0; i < size; i++)
		{
			tPtr2[i] = tPtr[i];
		}
		delete[] tPtr;
		tPtr = tPtr2;
		capacity = capacity * 2;
		// cout << "\n resize in progress ... ";	// for testing
		return capacity;
	}
};


// input numbers or strings and get a hash interger out
// designed for cis22cLab5 customer.csv customer format
// format: last, first, numFirst-numLast: lu,lihong,25674316-6058714
class HashFunks
{
private:
	int		numInFirst;	// designed for customer.csv file
	int		numInLast;
	int		numInHash;
	int		numInHashMinus;
	int		binSize;	// for use in modulus
	string	nameFirst;
	string	nameLast;
	int		numStrInHash;
	int		numDoubleHash;
	int		numDoubleHash2;
	int		myPrimeNum;	// for calculations
	//int		maxVal;		
	int makeNumbHash()
	{
		unsigned long tempLong = 0;
		tempLong = myPrimeNum * (numInFirst + numInLast);
		//tempLong = myPrimeNum * (numInFirst * numInLast);
		numInHash = tempLong % binSize;
		return numInHash;
	}
	int makeNumbHash2()
	{
		unsigned long tempLong = 0;
		tempLong = myPrimeNum * (numInFirst - numInLast);
		double temp1 = 1.0 * numInFirst / (numInLast * 1.0);
		if (useDoubleDivisionHash) tempLong = (int) myPrimeNum * temp1;
		numInHashMinus = tempLong % binSize;
		return numInHashMinus;
	}
	int makeStrgHash()
	{
		unsigned long tempLong = 0;
		for (int i = 0; i < nameFirst.length(); i++)
		{
			tempLong += nameFirst[i] * pow(26, i);	// base 26 because of alphabet
		}
		for (int i = 0; i < nameLast.length(); i++)
		{
			tempLong += nameLast[i] * pow(26, i);	// base 26 because of alphabet
		}
		numStrInHash = (myPrimeNum * tempLong) % binSize;
		return numStrInHash;
	}
	int makeDoubleHash()
	{
		numDoubleHash = (makeNumbHash() + makeStrgHash()) % binSize;
		return numDoubleHash;
	}
	int makeDoubleHash2()
	{
		numDoubleHash2 = (makeNumbHash() + makeNumbHash2()) % binSize;
		return numDoubleHash2;
	}
public:
	HashFunks()
	{
		numInFirst = 0;			// lame initial values...
		numInLast = 0;
		numInHash = 0;
		nameFirst = "";
		nameLast = "";
		numStrInHash = 0;
		binSize = 1000;		// just for starters. It gets reset when running.
		myPrimeNum = 941;	// a magic big prime number and my navy company number
	}
	// setters
	void setName(string last, string first)
	{
		nameFirst = first;
		nameLast = last;
	}
	void setNumber(int first, int last)
	{
		numInFirst = first;
		numInLast = last;
	}
	void setBinSize(int temp)
	{
		binSize = temp;
	}
	void setPrime(int prime)
	{
		myPrimeNum = prime;
	}
	int getNumHash(int firstNum, int lastNum)
	{
		numInFirst = firstNum;
		numInLast = lastNum;
		numInHash = makeNumbHash();
		return numInHash;
	}
	int getStrHash(string last, string first)
	{
		nameFirst = first;
		nameLast = last;
		numStrInHash = makeStrgHash();
		return numStrInHash;
	}
	int getDoubleHash(int firstNum, int lastNum, string last, string first)
	{
		numInFirst = firstNum;
		numInLast = lastNum;
		nameFirst = first;
		nameLast = last;
		return makeDoubleHash();
		// /*int numDoubleHash = makeDoubleHash();
	}
	int getDoubleHash2(int firstNum, int lastNum) //, string last, string first)
	{
		numInFirst = firstNum;
		numInLast = lastNum;
		//nameFirst = first;
		//nameLast = last;
		return makeDoubleHash2();
		// /*int numDoubleHash = makeDoubleHash();
	}
};

class readCustomerFileFirst
{
private:
	int	numberOfCustomers;
	DynArr<string>	custListDynArr;

public:
	readCustomerFileFirst()
	{
		ifstream inputFile;
		inputFile.open(FILENAME);
		if (!inputFile)						// Test file open
		{
			cout << "Error opening file." << endl;
			exit(99);
		}
		// cout << "\n The file opened ok. ";
		string input;
		//cout << "\n ... Reading Customer.csv file to load DynArr ... \n";
		while (inputFile.peek() != EOF)
		{
			inputFile >> input;
			if (input.length() > 20)
			{
				bool readOk = custListDynArr.insert(input);
				numberOfCustomers++;
			}
		}
		inputFile.close();
		// cout << "\nRead all customer entries. \n\tNumber of customers = " << numberOfCustomers << endl;
	}
	int getNumberOfCustomers()
	{
		return numberOfCustomers;
	}
	DynArr<string> getDynArr()
	{
		return custListDynArr;
	}

};

class readCustomerFileSecond
{
private:
	int	numberOfCustomers;
	DynArr<string>	custListDynArr;

public:
	readCustomerFileSecond()
	{
		ifstream inputFile;
		inputFile.open(FILENAME);
		if (!inputFile)						// Test file open
		{
			cout << "Error opening file." << endl;
			exit(99);
		}
		// cout << "\n The file opened ok. ";
		string input;
		//cout << "\n ... Reading Customer.csv file to load DynArr ... \n";
		while (inputFile.peek() != EOF)
		{
			inputFile >> input;
			if (input.length() > 20)
			{
				bool readOk = custListDynArr.insert(input);
				numberOfCustomers++;
			}
		}
		inputFile.close();
		// cout << "\nRead all customer entries. \n\tNumber of customers = " << numberOfCustomers << endl;
	}
	int getNumberOfCustomers()
	{
		return numberOfCustomers;
	}
	DynArr<string> getDynArr()
	{
		return custListDynArr;
	}

};


//create a function with the code in main
void makeUseHashTable0(DynArr<string> rawCustDynArr)
{
	cout << "\nUsing makeUseHashTable0 does not support Double Hashing";
	int numbCustomers = rawCustDynArr.getCurrentSize();
	string valueStr = "$";					// $ is a nice test to tell it was updated
	int trySize = numbCustomers * overFactor;

	DynArrCustHash hashTable0(trySize);		// make empty hashTable0 

	int collisionCount = 0;		// print later incremented in cx.add() if collision
	int totalEntries = 0;		// print later incremented for each 

								// for each customer entry
								//  create new CustNode 
								//  calculate hash value
								//  insert the CustNode in the correct location
								//   * during insert, if not empty, increment collision count 

	HashFunks hsh;
	hsh.setBinSize(trySize);
	int hshFrmStr = 0;
	int hshFrmNum = 0;
	int doubleHash = 0;


	//for (int many = 0; many < 1; many++)		// for testing fixed size
	for (int many = 0; many < numbCustomers; many++)
	{
		// for each customer entry get first and last / name and numbers
		valueStr = rawCustDynArr.getValue(many);
		int strLg = valueStr.length();
		int fstCma = 0;
		int scdCma = 0;
		int minSn = 0;	// magic number 20 definition... to weed out blank lines
		if (strLg > 20)	//  Because customer number is 16 char long + 2 commas + 2 letters
		{
			for (int j = 0; j < strLg; j++)
			{
				if (fstCma == 0)
				{
					if (valueStr[j] == ',') fstCma = j;
				}
				else if (scdCma == 0)
				{
					if (isdigit(valueStr[j])) scdCma = j - 1;
					//cout << valueStr[j];
				}
				else if (minSn == 0)
				{
					if (valueStr[j] == '-') minSn = j;
				}
			}

			// extract name and number from string in input file
			string lstNm = valueStr.substr(0, fstCma);
			string fstNm = valueStr.substr(fstCma + 1, scdCma - fstCma - 1);
			int ftNum = stoi(valueStr.substr(scdCma + 1, (minSn - scdCma)));
			int sdNum = stoi(valueStr.substr(minSn + 1, strLg));

			//  create CustNode 
			CustNode cX(lstNm, fstNm, ftNum, sdNum);
			cX.setData(valueStr);
			//cout << "\n" << cX.getName() << " \t" << cX.getNumber(); // << " " << cX.getData() << " ";

																	 // calculate hash value
																	 // hsh.setName(lstNm, fstNm);
			hshFrmStr = hsh.getStrHash(lstNm, fstNm);	// calc both types of hashNumbers 
														// or 
			hshFrmNum = hsh.getNumHash(ftNum, sdNum);

			doubleHash = hsh.getDoubleHash(ftNum, sdNum, lstNm, fstNm);

			// fill hash table with add to correct CustHashList (from DynArrCustHash) 
			// based on bool useNumbersForKey
			if (useDoubleHash)
			{
				hashTable0[doubleHash]->add(cX);
				//cout << "\nUsing DblHsh";
			}
			else if (useNumbersForKey)	// keys from numbers
			{
				hashTable0[hshFrmNum]->add(cX);
			}
			else
			{
				hashTable0[hshFrmStr]->add(cX); // keys from strings
				cout << "\nUsing hshFrmStr";
			}												//cout << "\nAdded CustNode to LinkedList in hashTable0.";
			totalEntries++;
		}
	}
	//  all done putting stuff in... 
	cout << "\nTotal number of entries is: ... " << totalEntries;
	cout << "\nCustomer Hash List size: ...... " << trySize;
	cout << "\nNumber of collisions is: ...... " << collisions;
	cout << "\nThe collision percentage is: .. " << 100.0 * collisions / totalEntries << "%";
	//cout << "\nNumber of customers is: ....... " << numbCustomers;

	cout << "\n\nNow test retrieval.. with customer: virkar,phyllis,81845385-3453124 ";
	string lstNm = "virkar";
	string fstNm = "phyllis";
	int ftNum = 81845385;
	int sdNum = 3453124;
	// this is my equivalent of hashTable0 "get"
	if (useNumbersForKey)	// using numbers to make hashTable0 keys. so "get" is as follows:
	{
		hshFrmNum = hsh.getNumHash(ftNum, sdNum);
		cout << "\nFor number " << ftNum << "-" << sdNum << " key is : " << hshFrmNum << endl;
		cout << "Search key finds that ... ";
		if (hashTable0[hshFrmNum]->containsNumber(ftNum))
		{
			cout << " TAA DAA! Number Key worked..  \n";
			//cout << "The value is: " << lstNm << "," << fstNm << endl;
		}
		else cout << "\n\n .... <<< key = Number SEARCH FAILED >>> .... ";
	}
	else	// using string to make hashTable0 keys.. then "get" is as follows:
	{
		hshFrmStr = hsh.getStrHash(lstNm, fstNm);
		cout << "\nFor string " << lstNm << "," << fstNm << " key is : " << hshFrmStr << endl;
		cout << "Search key finds that ... ";
		if (hashTable0[hshFrmStr]->containsLastName(lstNm))
		{
			cout << " TAA DAA! String Key worked..  \n"; // cout << "\t TAA DAA! ";
			// cout << "\nFound entry ";
		}
		else cout << "\n\n .... <<< key = string SEARCH FAILED >>> .... ";
	}
	//delete[] hashTable0; //DynArrCustHash();
	// Now clean out the hashTable0
	for (int i = 0; i < trySize; i++)
	{
		hashTable0[i]->clear();
		
	}
	//hashTable0.clear();
	//hashTable0.~DynArrCustHash();
}

void makeUseHashTable1(DynArr<string> rawCustDynArr)
{
	cout << "\nUsing makeUseHashTable1. Supports: Linear Probing, Quadratic, and Double Hashing, ";
	int numbCustomers = rawCustDynArr.getCurrentSize();
	string valueStr = "$";					// $ is a nice test to tell it was updated
	int trySize = numbCustomers * overFactor;

	DynArrCustHash hashTable1(trySize);		// make empty hashTable1 

	int collisionCount = 0;		// print later incremented in cx.add() if collision
	int totalEntries = 0;		// print later incremented for each 

	//int collisions = 0;		
								// for each customer entry
								//  create new CustNode 
								//  calculate hash value
								//  insert the CustNode in the correct location
								//   * during insert, if not empty, increment collision count 

	HashFunks hsh;
	hsh.setBinSize(trySize);
	int hshFrmStr = 0;
	int hshFrmNum = 0;
	int doubleHash = 0;
	int doubleHash2 = 0;

	int binsUsed = 0;
	int maxNumTries = 1;

	if (useDoubleHash || useDoubleHash2 || useLinearProbe || useQuadratic) 
		simpleHash = false;
	else simpleHash = true;

	//for (int many = 0; many < 1; many++)		// for testing fixed size
	for (int many = 0; many < numbCustomers; many++)
	{
		// for each customer entry get first and last / name and numbers
		valueStr = rawCustDynArr.getValue(many);
		int strLg = valueStr.length();
		int fstCma = 0;
		int scdCma = 0;
		int simpleTries = 1;
		int minSn = 0;	// magic number 20 definition... to weed out blank lines
		if (strLg > 20)	//  Because customer number is 16 char long + 2 commas + 2 letters
		{
			for (int j = 0; j < strLg; j++)
			{
				if (fstCma == 0)
				{
					if (valueStr[j] == ',') fstCma = j;
				}
				else if (scdCma == 0)
				{
					if (isdigit(valueStr[j])) scdCma = j - 1;
					//cout << valueStr[j];
				}
				else if (minSn == 0)
				{
					if (valueStr[j] == '-') minSn = j;
				}
			}

			// extract name and number from string in input file
			string lstNm = valueStr.substr(0, fstCma);
			string fstNm = valueStr.substr(fstCma + 1, scdCma - fstCma - 1);
			int ftNum = stoi(valueStr.substr(scdCma + 1, (minSn - scdCma)));
			int sdNum = stoi(valueStr.substr(minSn + 1, strLg));

			//  create CustNode 
			CustNode cX(lstNm, fstNm, ftNum, sdNum);
			cX.setData(valueStr);
			//cout << "\n" << cX.getName() << " \t" << cX.getNumber(); // << " " << cX.getData() << " ";

			// calculate hash value
			// hsh.setName(lstNm, fstNm);
			hshFrmStr = hsh.getStrHash(lstNm, fstNm);	// calc both types of hashNumbers 
														// or 
			hshFrmNum = hsh.getNumHash(ftNum, sdNum);

			doubleHash = hsh.getDoubleHash(ftNum, sdNum, lstNm, fstNm);

			doubleHash2 = hsh.getDoubleHash2(ftNum, sdNum); //, lstNm, fstNm);

			// fill hash table with add to correct CustHashList (from DynArrCustHash) 
			// based on bool useNumbersForKey
			if (useDoubleHash)
			{
				int numTries = 1; 
				if (hashTable1[hshFrmNum]->isEmpty())
				{
					//cout << "\nUsing DoubleHash linkedList is not empty. ";
					binsUsed++;
					hashTable1[hshFrmNum]->add(cX);
				}
				else if (hashTable1[doubleHash]->isEmpty())
				{
					hashTable1[doubleHash]->add(cX);
					binsUsed++;
					numTries++;
				}
				else hashTable1[doubleHash]->add(cX);
				numTries++;
				if (numTries > maxNumTries) maxNumTries = numTries;
				//cout << "\nUsing DblHsh";
			}
			else if (useDoubleHash2)
			{
				int numTries = 1; 
				if (hashTable1[hshFrmNum]->isEmpty())
				{
					//cout << "\nUsing DoubleHash linkedList is not empty. ";
					binsUsed++;
					hashTable1[hshFrmNum]->add(cX);
				}
				else if (hashTable1[doubleHash2]->isEmpty())
				{
					hashTable1[doubleHash2]->add(cX);
					binsUsed++;
					numTries++;
				}
				else
				{
					hashTable1[doubleHash2]->add(cX);
					numTries++;
					if (numTries > maxNumTries) maxNumTries = numTries;
				}
				//cout << "\nUsing DblHsh";
			}
			else if (useLinearProbe)	// also uses keys from numbers
			{
				bool success = false;
				int numTries = 1;
				while (success == false)
				{
					if (hashTable1[hshFrmNum]->isEmpty())
					{
						hashTable1[hshFrmNum]->add(cX);
						binsUsed++;
						success = true;
						//if (numTries != 1)
						//	cout << "\nDuring LinearProbe add number of tries = " << numTries;
					}
					hshFrmNum = (hshFrmNum + 1) % trySize;
					numTries++;
					if (numTries > maxNumTries) maxNumTries = numTries;
				}
			}
			else if (useQuadratic)	// also uses keys from numbers
			{
				bool success = false;
				int numTries = 1;
				double c1 = 0.5;
				double c2 = 0.5;
				int x = 1;
				while (success == false)
				{
					if (hashTable1[hshFrmNum]->isEmpty())
					{
						hashTable1[hshFrmNum]->add(cX);
						binsUsed++;
						success = true;
						//if (numTries != 1)
						//	cout << "\nDuring Quadratic insert number of tries = " << numTries;
					}
					int delta = (int) (c1 * x + c2 * x * x);
					hshFrmNum = (hshFrmNum + delta) % trySize;
					numTries++;
					if (numTries > maxNumTries) maxNumTries = numTries;
					if (numTries > trySize)
					{
						return;
						cout << "\nERROR in QUADRATIC Insert... ";
					}
					x++;
				}
			}
			else if (simpleHash && useNumbersForKey)	// keys from numbers
			{
				simpleTries++;
				if (hashTable1[hshFrmNum]->isEmpty())
				{
					binsUsed++;
					simpleTries--;
				}
				hashTable1[hshFrmNum]->add(cX);
				if (simpleTries > maxNumTries) maxNumTries = simpleTries;
			}
														
			
			else if (simpleHash && !useNumbersForKey)
			{
				if (hashTable1[hshFrmStr]->isEmpty())
					binsUsed++;
				hashTable1[hshFrmStr]->add(cX);					// keys from strings
			}
			
			totalEntries++;
		}
	}
	int thickestLinkedList = 0;
	int tempest = 0;
	for (int i = 0; i < trySize; i++)
	{
		tempest = hashTable1[i]->getItemCount();
		if (tempest > thickestLinkedList) thickestLinkedList = tempest;
	}

	//  all done putting stuff in... 
	cout << "\nTotal number of entries is: ... " << totalEntries;
	cout << "\nThe number of containers: ..... " << trySize;
	cout << "\nThe number of contianers used:  " << binsUsed;
	if (binsUsed == 0) binsUsed++;
	cout << "\nEffieciency of bins uses is: .. " << 100.0 * ( binsUsed ) / trySize << '%';
	cout << "\nMax number of tries was: ...... " << maxNumTries;
	cout << "\nMax LinkedList Size was: ...... " << thickestLinkedList;
	cout << "\nNumber of collisions is: ...... " << collisions;
	cout << "\nThe collision percentage is: .. " << 100.0 * collisions / totalEntries << "%";
	//cout << "\nNumber of customers is: ....... " << numbCustomers;

	cout << "\n\nNow test retrieval.. with customer: virkar,phyllis,81845385-3453124 ";
	string lstNm = "virkar";
	string fstNm = "phyllis";
	int ftNum = 81845385;
	int sdNum = 3453124;
	// this is my equivalent of hashTable1 "get"
	

	// fill hash table with add to correct CustHashList (from DynArrCustHash) 
	// based on bool useNumbersForKey
	// now do the correct look up and seek.
	if (useDoubleHash)	
	{
		bool success = false;
		hshFrmNum = hsh.getNumHash(ftNum, sdNum);
		doubleHash = hsh.getDoubleHash(ftNum, sdNum, lstNm, fstNm);
		int numTries = 1;
		while (success == false)
		{
			cout << "\n  First: for number " << ftNum << "-" << sdNum << " key is : " << hshFrmNum << endl;
			if ((hashTable1[hshFrmNum]->containsNumber(ftNum))
				&& (hashTable1[hshFrmNum]->containsNumber(sdNum)))
			{
				cout << "\n      TAA DAA! Double Hash worked..  number of tries = " << numTries << "\n";
				success = true;
				cout << "\nResult is: " << hashTable1[hshFrmNum]->getHead()->getData();
				//cout << "The value is: " << lstNm << "," << fstNm << endl;
			}
			else
			{
				if ((hashTable1[doubleHash]->containsNumber(ftNum))
					&& (hashTable1[doubleHash]->containsNumber(sdNum)))
				{
					numTries += hashTable1[doubleHash]->getItemCount();
					success = true;
					cout << "\n     TAA DAA Double Hash look up success. " <<
						 "but needed to search/walk " << numTries << " times.\n";
				}
				else cout << "\nLOOKUP FAILED... error 4444"; 
			}	
		}
	}
	else if (useDoubleHash2)
	{
		bool success = false;
		hshFrmNum = hsh.getNumHash(ftNum, sdNum);
		doubleHash2 = hsh.getDoubleHash2(ftNum, sdNum); // , lstNm, fstNm);
		int numTries = 1;
		while (success == false)
		{
			cout << "\n  First: for number " << ftNum << "-" << sdNum << " key is : " << hshFrmNum << endl;
			if ((hashTable1[hshFrmNum]->containsNumber(ftNum))
				&& (hashTable1[hshFrmNum]->containsNumber(sdNum)))
			{
				cout << "\n      TAA DAA! Double Hash worked..  number of tries = " << numTries << "\n";
				success = true;
				cout << "\nResult is: " << hashTable1[hshFrmNum]->getHead()->getData();
				//cout << "The value is: " << lstNm << "," << fstNm << endl;
			}
			else
			{
				if ((hashTable1[doubleHash2]->containsNumber(ftNum))
					&& (hashTable1[doubleHash2]->containsNumber(sdNum)))
				{
					numTries += hashTable1[doubleHash2]->getItemCount();
					success = true;
					cout << "\n     TAA DAA Double Hash '2' look up success. " <<
						"but needed to search/walk " << numTries << " times.\n";
				}
				else cout << "\nLOOKUP FAILED... error 4444";
			}

		}
	}
	else if (useQuadratic)	// using numbers to make hashTable1 keys. so "get" is as follows:
	{
		hshFrmNum = hsh.getNumHash(ftNum, sdNum);
		bool success = false;
		int numTries = 1;
		double c1 = 0.5;
		double c2 = 0.5;
		int x = 1;
		while (success == false)
		{
			cout << "\nFor number " << ftNum << "-" << sdNum << " key is : " << hshFrmNum;
			if ((hashTable1[hshFrmNum]->containsNumber(ftNum))
				&& (hashTable1[hshFrmNum]->containsNumber(sdNum)))
			{
				cout << "\n TAA DAA! Quadratic worked.. but number of tries = " << numTries << "\n";
				success = true;
				cout << "\nResult is: " << hashTable1[hshFrmNum]->getHead()->getData() << endl;
				//cout << "The value is: " << lstNm << "," << fstNm << endl;
			}
			else
			{
				int delta = (int)(c1 * x + c2 * x * x);
				hshFrmNum = (hshFrmNum + delta) % trySize;
				cout << " ... Quadratic hashnumber now: " << hshFrmNum;
				numTries++;
				if (numTries > trySize)
				{
					return;
					cout << "\nERROR in QUADRATIC Insert... ";
				}
				x++;
			}
		}
	}
	else if (useLinearProbe)	// using numbers to make hashTable1 keys. so "get" is as follows:
	{
		hshFrmNum = hsh.getNumHash(ftNum, sdNum);
		bool success = false;
		int numTries = 1;
		while (success == false)
		{
			cout << "\nFor number " << ftNum << "-" << sdNum << " key is : " << hshFrmNum ;
			if ((hashTable1[hshFrmNum]->containsNumber(ftNum)) 
				&& (hashTable1[hshFrmNum]->containsNumber(sdNum)))
				{
					cout << "\n TAA DAA! LinearProbe worked.. but number of tries = " << numTries << "\n";
					success = true;
					cout << "\nResult is: " << hashTable1[hshFrmNum]->getHead()->getData();
				//cout << "The value is: " << lstNm << "," << fstNm << endl;
				}
			else
			{
				numTries++;
				hshFrmNum = (hshFrmNum + 1) % trySize;
			}
		}
	}
	else if (simpleHash && useNumbersForKey)	// using numbers to make hashTable1 keys. so "get" is as follows:
	{
		int numTries = 0;
		hshFrmNum = hsh.getNumHash(ftNum, sdNum);
		cout << "\nFor number " << ftNum << "-" << sdNum << " key is : " << hshFrmNum << endl;
		cout << "Search key finds that ... ";
		if (hashTable1[hshFrmNum]->containsNumber(ftNum))
		{
			numTries = hashTable1[hshFrmNum]->getItemCount(); 
			cout << " TAA DAA! Number Key could take " << numTries << " tries to find Phyllis.. \n";
			//cout << "The value is: " << lstNm << "," << fstNm << endl;
		}
		else cout << "\n\n .... <<< key = Number SEARCH FAILED >>> .... ";
	}
	else if (simpleHash && !useNumbersForKey)	// using string to make hashTable1 keys.. then "get" is as follows:
	{
		int numTries = 0;
		hshFrmStr = hsh.getStrHash(lstNm, fstNm);
		cout << "\nFor string " << lstNm << "," << fstNm << " key is : " << hshFrmStr << endl;
		cout << "Search key finds that ... ";
		if (hashTable1[hshFrmStr]->containsLastName(lstNm))
		{
			numTries = hashTable1[hshFrmStr]->getItemCount(); 
			cout << " TAA DAA! String Key could take " << numTries << " tries to find Phyllis.. \n";
														 // cout << "\nFound entry ";
		}
		else cout << "\n\n .... <<< key = string SEARCH FAILED >>> .... ";
	}
	
	for (int i = 0; i < trySize; i++)
	{
		hashTable1[i]->clear();

	}
	
}








int main()
{

	
	cout << "\n\n Welcome to the ... Hashed Potatoes Project ... \n";
	
	{
		readCustomerFileFirst rFirst;
		DynArr<string> fred = rFirst.getDynArr();

		// /*
		cout << "\nNow NUMBER Keys w/ hash table 1.25x > than number of entries.";
		overFactor = 1.25;
		useDoubleHash2 = false;
		useDoubleHash = false;
		useLinearProbe = false;
		useQuadratic = false;
		useNumbersForKey = true;
		makeUseHashTable1(fred);
		// */
	}
	
	cout << "\n\n Hashed Potatoes step 2 ... \n";

	{
		readCustomerFileSecond rSecond;
		DynArr<string> bob = rSecond.getDynArr();
		//collisionCount = 0;
		/* First using 1.25x and Strings for keys: */

		cout << "\nNow STRING Keys w/ hash table 1.25x > than number of entries.";
		overFactor = 1.25;
		useDoubleHash2 = false;
		useDoubleHash = false;
		useLinearProbe = false;
		useQuadratic = false;
		useNumbersForKey = false;
		makeUseHashTable1(bob);
		// */
	}
	
	readCustomerFileFirst rFirst;
	DynArr<string> fred = rFirst.getDynArr();

	cout << "\n\n Hashed Potatoes step 3 ... \n";
	cout << "\nNow using Linear Probe with Number Keys w/ hash table 1.25x > than number of entries.";
	overFactor = 1.25;
	useDoubleHash = false;
	useLinearProbe = true;
	useQuadratic = false;
	useNumbersForKey = false;
	makeUseHashTable1(fred);
	

	cout << "\n\n Hashed Potatoes step 4 ... \n";
	cout << "\nNow using Double Hash: numbers first, then if collision combine string hash .. " << 
			"\n ..... Keys w/ hash table 1.25x > than number of entries.";
	overFactor = 1.25;
	useDoubleHash = true;
	useLinearProbe = false;
	useQuadratic = false;
	useNumbersForKey = false;
	makeUseHashTable1(fred);

	cout << "\n\n Hashed Potatoes step 5 ... \n";
	cout << "\nNow using Double Hash2: numbers first, then if collision combine subtraction number hash .. " <<
		"\n ..... Keys w/ hash table 1.25x > than number of entries.";
	overFactor = 1.25;
	useDoubleHash2 = true;
	useDoubleHash = false;
	useLinearProbe = false;
	useQuadratic = false;
	useNumbersForKey = false;
	makeUseHashTable1(fred);
	
	cout << "\n\n Hashed Potatoes step 6 ... \n";
	cout << "\nNow using Double Hash2: numbers first, then if collision division number hash .. " <<
		"\n ..... Keys w/ hash table 1.25x > than number of entries.";
	overFactor = 1.25;
	useDoubleHash2 = true;
	useDoubleDivisionHash = true;
	useDoubleHash = false;
	useLinearProbe = false;
	useQuadratic = false;
	useNumbersForKey = false;
	makeUseHashTable1(fred);

	cout << "\n\n Hashed Potatoes step 7 ... \n";
	cout << "\nNow using Quadratic: numbers first, then (hashnum +( c1*x + c2*x^2)  .. " <<
		"\n ..... Keys w/ hash table 1.25x > than number of entries.";
	overFactor = 1.25;
	useDoubleHash2 = false;
	useDoubleDivisionHash = false;
	useDoubleHash = false;
	useLinearProbe = false;
	useQuadratic = true;
	useNumbersForKey = false;
	makeUseHashTable1(fred);
	

	system("pause");

	return 0;
}

/* ********************  interesting results  ********************************* 
  Welcome to the ... Hashed Potatoes Project ...

Now NUMBER Keys w/ hash table 1.25x > than number of entries.
Using makeUseHashTable1. Supports: Linear Probing, Quadratic, and Double Hashing,
Total number of entries is: ... 512
The number of containers: ..... 640
The number of contianers used:  361
Effieciency of bins uses is: .. 56.4063%
Max number of tries was: ...... 2
Max LinkedList Size was: ...... 4
Number of collisions is: ...... 151
The collision percentage is: .. 29.4922%

Now test retrieval.. with customer: virkar,phyllis,81845385-3453124
For number 81845385-3453124 key is : 521
Search key finds that ...
   Lookup test found value is: virkar,phyllis TAA DAA! Number Key could take 3 tries to find Phyllis..


 Hashed Potatoes step 2 ...

Now STRING Keys w/ hash table 1.25x > than number of entries.
Using makeUseHashTable1. Supports: Linear Probing, Quadratic, and Double Hashing,
Total number of entries is: ... 512
The number of containers: ..... 640
The number of contianers used:  332
Effieciency of bins uses is: .. 51.875%
Max number of tries was: ...... 1
Max LinkedList Size was: ...... 8
Number of collisions is: ...... 180
The collision percentage is: .. 35.1563%

Now test retrieval.. with customer: virkar,phyllis,81845385-3453124
For string virkar,phyllis key is : 164
Search key finds that ...   value is: 81845385-3453124 TAA DAA! String Key could take 1 tries to find Phyllis..


 Hashed Potatoes step 3 ...

Now using Linear Probe with Number Keys w/ hash table 1.25x > than number of entries.
Using makeUseHashTable1. Supports: Linear Probing, Quadratic, and Double Hashing,
Total number of entries is: ... 512
The number of containers: ..... 640
The number of contianers used:  512
Effieciency of bins uses is: .. 80%
Max number of tries was: ...... 35
Max LinkedList Size was: ...... 1
Number of collisions is: ...... 0
The collision percentage is: .. 0%

Now test retrieval.. with customer: virkar,phyllis,81845385-3453124
For number 81845385-3453124 key is : 521
For number 81845385-3453124 key is : 522
For number 81845385-3453124 key is : 523
For number 81845385-3453124 key is : 524
For number 81845385-3453124 key is : 525
For number 81845385-3453124 key is : 526
   Lookup test found value is: virkar,phyllis
   Lookup test found value is: virkar,phyllis
 TAA DAA! LinearProbe worked.. but number of tries = 6

Result is: virkar,phyllis,81845385-3453124

 Hashed Potatoes step 4 ...

Now using Double Hash: numbers first, then if collision combine string hash ..
 ..... Keys w/ hash table 1.25x > than number of entries.
Using makeUseHashTable1. Supports: Linear Probing, Quadratic, and Double Hashing,
Total number of entries is: ... 512
The number of containers: ..... 640
The number of contianers used:  428
Effieciency of bins uses is: .. 66.875%
Max number of tries was: ...... 3
Max LinkedList Size was: ...... 3
Number of collisions is: ...... 84
The collision percentage is: .. 16.4063%

Now test retrieval.. with customer: virkar,phyllis,81845385-3453124
  First: for number 81845385-3453124 key is : 521

   Lookup test found value is: virkar,phyllis
   Lookup test found value is: virkar,phyllis
     TAA DAA Double Hash look up success. but needed to search/walk 2 times.


 Hashed Potatoes step 5 ...

Now using Double Hash2: numbers first, then if collision combine subtraction number hash ..
 ..... Keys w/ hash table 1.25x > than number of entries.
Using makeUseHashTable1. Supports: Linear Probing, Quadratic, and Double Hashing,
Total number of entries is: ... 512
The number of containers: ..... 640
The number of contianers used:  413
Effieciency of bins uses is: .. 64.5313%
Max number of tries was: ...... 2
Max LinkedList Size was: ...... 4
Number of collisions is: ...... 99
The collision percentage is: .. 19.3359%

Now test retrieval.. with customer: virkar,phyllis,81845385-3453124
  First: for number 81845385-3453124 key is : 521

   Lookup test found value is: virkar,phyllis
   Lookup test found value is: virkar,phyllis
     TAA DAA Double Hash '2' look up success. but needed to search/walk 4 times.


 Hashed Potatoes step 6 ...

Now using Double Hash2: numbers first, then if collision division number hash ..
 ..... Keys w/ hash table 1.25x > than number of entries.
Using makeUseHashTable1. Supports: Linear Probing, Quadratic, and Double Hashing,
Total number of entries is: ... 512
The number of containers: ..... 640
The number of contianers used:  429
Effieciency of bins uses is: .. 67.0313%
Max number of tries was: ...... 2
Max LinkedList Size was: ...... 4
Number of collisions is: ...... 83
The collision percentage is: .. 16.2109%

Now test retrieval.. with customer: virkar,phyllis,81845385-3453124
  First: for number 81845385-3453124 key is : 521

   Lookup test found value is: virkar,phyllis
   Lookup test found value is: virkar,phyllis
     TAA DAA Double Hash '2' look up success. but needed to search/walk 2 times.


 Hashed Potatoes step 7 ...

Now using Quadratic: numbers first, then (hashnum +( c1*x + c2*x^2)  ..
 ..... Keys w/ hash table 1.25x > than number of entries.
Using makeUseHashTable1. Supports: Linear Probing, Quadratic, and Double Hashing,
Total number of entries is: ... 512
The number of containers: ..... 640
The number of contianers used:  512
Effieciency of bins uses is: .. 80%
Max number of tries was: ...... 18
Max LinkedList Size was: ...... 1
Number of collisions is: ...... 0
The collision percentage is: .. 0%

Now test retrieval.. with customer: virkar,phyllis,81845385-3453124
For number 81845385-3453124 key is : 521 ... Quadratic hashnumber now: 522
For number 81845385-3453124 key is : 522 ... Quadratic hashnumber now: 525
For number 81845385-3453124 key is : 525 ... Quadratic hashnumber now: 531
For number 81845385-3453124 key is : 531 ... Quadratic hashnumber now: 541
For number 81845385-3453124 key is : 541 ... Quadratic hashnumber now: 556
For number 81845385-3453124 key is : 556 ... Quadratic hashnumber now: 577
For number 81845385-3453124 key is : 577 ... Quadratic hashnumber now: 605
For number 81845385-3453124 key is : 605 ... Quadratic hashnumber now: 1
For number 81845385-3453124 key is : 1 ... Quadratic hashnumber now: 46
For number 81845385-3453124 key is : 46 ... Quadratic hashnumber now: 101
For number 81845385-3453124 key is : 101 ... Quadratic hashnumber now: 167
For number 81845385-3453124 key is : 167 ... Quadratic hashnumber now: 245
For number 81845385-3453124 key is : 245 ... Quadratic hashnumber now: 336
For number 81845385-3453124 key is : 336
   Lookup test found value is: virkar,phyllis
   Lookup test found value is: virkar,phyllis
 TAA DAA! Quadratic worked.. but number of tries = 14

Result is: virkar,phyllis,81845385-3453124
Press any key to continue . . .
 
*/







