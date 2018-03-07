#include <memory>
#include <cstdlib>
#include <list>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <stack>
#include <map>

using namespace std;


//this is Allocate class
template <typename T>
class Allocate
{
public:
	typedef T value_type;
	Allocate() {}
	template <typename T> Allocate(const Allocate<T>& other) {}
	T* allocate(size_t count)
	{
		return static_cast<T*>(malloc(count * sizeof(T)));
	}
	void deallocate(T* object, size_t n)
	{
		free(object);
	}
}; //end of Allocate class

//this is memory manager class
class MemoryManager
{
public:
	map<void*, size_t, less<void*>, Allocate<pair<void*, size_t>>> memoryMap;

}; //end of MemoryManager class


//global functions and variables
MemoryManager gManager;
size_t totalBytes = 0;

void* operator new (size_t st)
{
	totalBytes += st;
	void* ptr = malloc(st);
	gManager.memoryMap.insert(pair<void*, size_t>(ptr, st));
	return ptr;
}


void* operator new [](size_t st)
{
	totalBytes += st;
	void* ptr = malloc(st);
	gManager.memoryMap.insert(pair<void*, size_t>(ptr, st));
	return ptr;
}


void operator delete(void* p)
{
	totalBytes -= gManager.memoryMap[p];
	//reset the bytesize for that address/pointer to 0
	gManager.memoryMap[p] = 0;

	p = nullptr;
	free(p);
}


void operator delete[](void* p)
{
	totalBytes -= gManager.memoryMap[p];
	//reset the bytesize for that address/pointer to 0
	gManager.memoryMap[p] = 0;
	p = nullptr;
	free(p);
}




//-------------------------End of Global functions and variables--------------

class Student
{
	string* _firstname;
	string* _lastname;
	string* _course;
	double* _gpa;
public:
	Student()
	{
		
		_firstname = NULL;
		_lastname = NULL;
		_course = NULL;
		_gpa = NULL;
	}
	Student(char* pl, char* pf, char* pc, char* pg)
	{
		_lastname = new string(pl);
		_firstname = new string(pf);
		_course = new string(pc);
		_gpa = new double(atof(pg));
	}
	Student(string* pitems)
	{
		_lastname = new string(pitems[0]);
		_firstname = new string(pitems[1]);
		_course = new string(pitems[2]);
		_gpa = new double(atof(pitems[3].c_str()));
	}
	Student(const vector<string>& vs)
	{
		//if (_course)
		//{
			_firstname = new string(vs[1]);
			_lastname = new string(vs[0]);
			_course = new string(vs[2]);
			_gpa = new double(atof(vs[3].c_str()));
		//}
	}
	Student(const Student& s)
	{
		_course = NULL;
		_lastname = NULL;
		_firstname = NULL;
		_gpa = NULL;
		if (s._course)
		{
			_course = new string(*s._course);
			_lastname = new string(*s._lastname);
			_firstname = new string(*s._firstname);
			_gpa = new double(*s._gpa);
		}
	}
	Student(Student&& s)
	{
		_course = s._course; s._course = NULL;
		_lastname = s._lastname; s._lastname = NULL;
		_firstname = s._firstname; s._firstname = NULL;
		_gpa = s._gpa; s._gpa = NULL;
	}

	~Student()
	{
		if (_course) delete _course;
		if (_lastname) delete _lastname;
		if (_firstname) delete _firstname;
		if (_gpa) delete _gpa;

	}

	string GetFirstName() { return *_firstname; }
	string GetLastName() { return *_lastname; }
	string GetCourseName() { return *_course; }
	double GetGpa() { return *_gpa; }
	void operator = (const Student& right)
	{
		if (_course)
		{
			_course = new string(const_cast<Student&>(right).GetCourseName());
			_lastname = new string(const_cast<Student&>(right).GetLastName());
			_firstname = new string(const_cast<Student&>(right).GetFirstName());
			_gpa = new double(const_cast<Student&>(right).GetGpa());
		}
	}
	friend ostream& operator << (ostream& os, Student& s)
	{
		os << s.GetCourseName();
		os << '\t';
		os << s.GetLastName();
		os << '\t';
		os << s.GetFirstName();
		os << '\t';
		os << s.GetGpa();
		return os;
	}
};

//some other global helper functions-----------
template <typename T>
T RegexString(string text, string split)
{
	T container;
	sregex_token_iterator end; // size is ZERO when created
	regex pattern(split);
	for (sregex_token_iterator pos(text.begin(), text.end(), pattern); pos != end; ++pos)
	{
		if ((*pos).length() > 0)
		{
			if ((static_cast<string>(*pos))[0] != 0x20) // 0x20 = BLANK
				container.push_back(*pos);
		}
	}
	return container;
}

template <typename T>
int ReadFile(T& container, string sfilename, string delimiter)
{
	vector<string> vs;

	ifstream filein(sfilename);

	while (!filein.eof())
	{
		string buffer;
		getline(filein, buffer);
		vs = RegexString<vector<string>>(buffer, delimiter);
		
		if (vs.size() > 0)
			container.push_back(unique_ptr<Student>(new Student(vs)));
	
		//print out the memory size
		cout << "\nAfter adding a student, the total memory size is now : " << totalBytes << " bytes" << endl;
		vs.clear();
	}
	
	filein.close();

	return container.size();

}

//end of global helper functions


//a function to test the MemoryManager class
void rawPointer()
{
	totalBytes = 0;

	//create a Student instance using new
	cout << "\nCreating a Student object using raw pointer..." << endl;

	Student* spointer = new Student("Bob", "Smith", "CIS29", "4.0");

	//check the memory size
	cout << "\nThe memory size allocated for the Student object is: "
		<< gManager.memoryMap[spointer] << " bytes" << endl;

	cout << "\nThe total memory size is now: " << totalBytes << " bytes" << endl;

	//delete the Student instance
	cout << "\nDeleting the Student object created with a raw pointer..." << endl;
	delete spointer;

	//check the memory size after the delete
	cout << "\nThe memory size allocated for the Student object is: "
		<< gManager.memoryMap[spointer] << " bytes" << endl;

	cout << "\nThe total memory size is now: " << totalBytes << " bytes" << endl;
} //end of rawPointer



int  main()
{
	//start with totalBytes 0
	totalBytes = 0;


	/*
	We have to take care of in terms of memory cleaning:

	1. the Student object created with unique_ptr new when calling ReadFile function.

		We take care of this part by putting the ReadFile function in a local scope,
		So when it goes out of the scope, the Student object that is created by using
		unique_ptr will AUTOMATICALLY be deleted.
	
	*/

	//needs to put unique_ptr in a scope
	//make sure this { is here
	{
		vector<unique_ptr<Student>> vs;

		int size = ReadFile(vs, "Students.csv", "[^,]*");

		cout << "\nHere are the students added: " << endl;
		for (int x = 0; x < vs.size(); x++)
		{
			cout << x + 1 << ". " << vs[x]->GetFirstName() << '\t' << vs[x]->GetLastName() << endl;
		}


	} //make sure this } is here

	//check the final memory size
	cout << "\nThe final total memory size is now: " << totalBytes << " bytes" << endl;


	system("pause");
	return 0;
}
