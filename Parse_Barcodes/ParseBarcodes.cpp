/*
CIS_29 Lab2
Author: Ying Xie
Program Description: Write a generic XML processor, and use it to process the Barcode3of9.XML file.  
					Use Regular expressions to parse the data.
					Process/Decrypt the Carts.csv 
					and finding the match product in ProductPrice.csv files using regular expressions.
					Output each cart with its product list and price

*/

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <stack>
#include <queue>
#include <list>
#include <bitset>
#include <iomanip>

using namespace std;

//------------this is the helper class for parsing text files
class ParserHelper
{
public:
	//this method iterate the matched strings from regex 
	vector<string> getMatchResults(string text, string split)
	{
		vector<string> matchedResults;
		sregex_token_iterator end;
		regex pattern(split);
		for (sregex_token_iterator iter(text.begin(), text.end(), pattern); iter != end; ++iter)
		{
			if ((*iter).length() > 0)
			{
				if ((static_cast<string>(*iter))[0] != 0x20)
				{
					//cout << *iter << "\t";
					matchedResults.push_back(*iter);
				}
			}
		}
		//cout << endl;
		return matchedResults;
	}
		
	//convert string to upper
	string toUpper(string input)
	{
		transform(input.begin(), input.end(), input.begin(), ::toupper);
		return input;
	}

	//convert Hex string to binary string
	string getBinaryFromHex(string sHex)
	{
		string sReturn = "";
		for (int i = 0; i < sHex.length(); ++i)
		{
			switch (sHex[i])
			{
				case 'A': sReturn.append("1010"); break;
				case 'B': sReturn.append("1011"); break;
				case 'C': sReturn.append("1100"); break;
				case 'D': sReturn.append("1101"); break;
				case 'E': sReturn.append("1110"); break;
				case 'F': sReturn.append("1111"); break;
				case '0': sReturn.append("0000"); break;
				case '1': sReturn.append("0001"); break;
				case '2': sReturn.append("0010"); break;
				case '3': sReturn.append("0011"); break;
				case '4': sReturn.append("0100"); break;
				case '5': sReturn.append("0101"); break;
				case '6': sReturn.append("0110"); break;
				case '7': sReturn.append("0111"); break;
				case '8': sReturn.append("1000"); break;
				case '9': sReturn.append("1001"); break;
				default: break;
			}
		}
		return sReturn;
	}
	
	//binary search
	//this uses a function template, can search for string or int or char
	template<class T>
	int binarySearch(vector<T> &aList, T item, int size)
	{
		int begin = 0;
		int end = size - 1;
		int mid = (begin + end) / 2;
		int position = -1;

		bool found = false;

		while (!found && begin <= end)
		{
			mid = (begin + end) / 2;     // Calculate mid point
			if (aList[mid] == item)      // If value is found at mid
			{
				found = true;
				position = mid;
			}
			else if (aList[mid] > item)  // If value is in lower half
				end = mid - 1;
			else
				begin = mid + 1;           // If value is in upper half
		}
		return position;
	}

}; //--------the end of helper class

//------------this is the class for XMLElement, contains the actual data we want
class XMLElement
{
private:
	string tagName;
	string data;
public:
	//default constructor
	XMLElement()
	{
		tagName = "";
		data = "";
	}

	//other constructor
	XMLElement(string tagName, string data)
	{
		this->tagName = tagName;
		this->data = data;
	}

	//getters and setters
	string getTagName()
	{
		return this->tagName;
	}
	string getData()
	{
		return this->data;
	}
	void setTagName(string name)
	{
		this->tagName = name;
	}
	void setData(string data)
	{
		this->data = data;
	}

}; //----------end of XMLElement class


//------------this is the XMLNode class
//------------I am trying to make it generic enough to handle more than this
//------------program's Barcode.xml
//------------Note: it does NOT handle attributes
class XMLNode
{
private:
	string nodeName;
	list<XMLNode> childNodes;
	list<XMLElement> elements;

public:
	//default constructor
	XMLNode(){}
	//other constructor
	XMLNode(string nodeName, list<XMLNode> childNodes,  list<XMLElement> elements)
	{
		this->nodeName = nodeName;
		this->childNodes = childNodes;
		this->elements = elements;
	}

	//getters and setters
	string getNodeName()
	{
		return this->nodeName;
	}
	list<XMLNode> getChildNodes()
	{
		return this->childNodes;
	}
	list<XMLElement> getElements()
	{
		return this->elements;
	}
	void setNodeName(string name)
	{
		this->nodeName = name;
	}
	void setChildNodes(list<XMLNode> nodes)
	{
		this->childNodes = nodes;
	}
	void setElements(list<XMLElement> elements)
	{
		this->elements = elements;
	}

	//other methods
	void appendChildNode(const XMLNode &node)
	{
		this->childNodes.push_back(node);
	}
	
	void appendElement(const XMLElement &element)
	{
		this->elements.push_back(element);
	}
	
}; //------------end of XMLNode class

//--------------this is XMLParser class
//------------I am trying to make it generic enough to handle more than this
//------------program's Barcode.xml
class XMLParser
{
private:
	list<XMLNode> xmlNodeList;
	string fileName;
	string masterNodeTag;
public:
	//default constructor
	XMLParser(){}
	//other constructor
	XMLParser(string fileName)
	{
		this->fileName = fileName;
		this->parseFile();
	}

	//getter and setters
	string getMasterNodeTag()
	{
		return this->masterNodeTag;
	}
	void setMasterNodeTag(string tag)
	{
		this->masterNodeTag = tag;
	}
	list<XMLNode> getXMLNodeList()
	{
		return this->xmlNodeList;
	}

	//other methods

	//parse xml file method
	//this method assumes that the XML file is properly formatted,
	//with xml header as first line, and master node at the second line
	//A full XML parser is no trivial work, will then use builtin xml parsing library
	void parseFile()
	{
		//open the file
		ifstream inFile;
		inFile.open(this->fileName);
		//file opening exception
		if (!inFile)
			throw "Cannot open the XML file!";

		//now start to process---------------------------------------------

		//declare a helper class object for parsing
		ParserHelper h1;

		//need stack and list
		stack<XMLNode> nodeStack;
		list<XMLElement> elementList;
		

		//regular expression match patterns
		string pattern = "[^<^>]*";
		//regex pattern("(.*)<\\w*>");
		
		string line = "";
		
		getline(inFile, line); //read in the first line, xml header, and ignore it, not processing it
		getline(inFile, line); //second line is the master node
			
		//process the second line: master node
		vector<string> matchedResults;
		matchedResults = h1.getMatchResults(line, pattern);
		if (matchedResults.size() == 1)
		{
			string strMaster = matchedResults[0];
			//cout << strMaster << endl;
			this->setMasterNodeTag(strMaster);
		}

		//start processing from line three: what we what for this program
		while (getline(inFile, line))
		{
			//find match results
			matchedResults = h1.getMatchResults(line, pattern);
			//the closing tag for the master node, do nothing and ignore
			if (matchedResults.size() == 1 && matchedResults[0].substr(1) == this->getMasterNodeTag())
			{
				//ignore and do nothing
				continue;
			}
			//opening tag
			else if (matchedResults.size() == 1 && matchedResults[0][0] != '/')
			{
				//cout << matchedResults[0] << endl;
				XMLNode node;
				node.setNodeName(matchedResults[0]);
				nodeStack.push(node);
			}
			//closing tag
			else if (matchedResults.size() == 1 && matchedResults[0][0] == '/') 
			{
				string topTag = nodeStack.top().getNodeName();
				string endTag = matchedResults[0].substr(1);
				//cout << topTag << endl;
				//cout << endTag << endl;

				if (topTag == endTag)
				{
					XMLNode topNode = nodeStack.top();
					if (elementList.size() > 0)
					{
						topNode.setElements(elementList); //set the elements for this node
						elementList.clear(); //reset the elementList for the next node
					}
					//if there is parent node
					if (nodeStack.size() > 1) 
					{
						nodeStack.pop(); //pop the child node
						nodeStack.top().appendChildNode(topNode); //append the child node to parent node
					}
					//no parent node
					else
					{
						//nodeList.push_back(topNode); //put the complete node onto list
						this->xmlNodeList.push_back(topNode);
						nodeStack.pop();
					}
				}
			} //end of else if
			//element node
			else if (matchedResults.size() == 3 && matchedResults[2][0] == '/') 
			{
				string topTag = matchedResults[0];
				string endTag = matchedResults[2].substr(1);
				//cout << topTag << endl;
				//cout << endTag << endl;

				if (topTag == endTag)
				{
					XMLElement element;
					element.setTagName(matchedResults[0]);
					element.setData(matchedResults[1]);
					elementList.push_back(element);
				}
			}


		} //end of while loop
			
		inFile.close(); //close the file
				
	} //end of parse file method

	
}; //-----------end of XMLParser class


//--------------this is the Barcode class
class Barcode
{
private:
	char code;
	string symbology;
	bitset<24> binary;
	int decimalValue;
public:
	//default constructor
	Barcode(){}

	//other constructor
	Barcode(char code, string symbology, bitset<24> binary)
	{
		this->code = code;
		this->symbology = symbology;
		this->binary = binary;
		setDecimalValue(binary);
	}

	//getters and setters
	void setDecimalValue(bitset<24> binary)
	{
		this->decimalValue = (int)(binary.to_ulong());
	}
	void setCode(char code)
	{
		this->code = code;
	}
	void setBinary(bitset<24> binary)
	{
		this->binary = binary;
	}
	void setSymbology(string symbology)
	{
		this->symbology = symbology;
	}
	char getCode()
	{
		return this->code;
	}
	bitset<24> getBinary()
	{
		return this->binary;
	}
	string getSymbology()
	{
		return this->symbology;
	}
	int getDecimalValue()
	{
		return this->decimalValue;
	}

	//override the operator < based on the decimal values of the binary code of the barcode
	//will be used in sorting
	bool operator < (const Barcode &a) 
	{
		return (this->getDecimalValue() < a.decimalValue);
	}

}; //---------------end of Barcode class


//-------------------this is the BarcodeParser class
class BarcodeParser
{
private:
	vector<Barcode> barcodes;
	vector<int> sortedDecimals;
public:
	//default constructor
	BarcodeParser(){}
	//other constructor
	BarcodeParser(string barcodeXML)
	{
		setBarcodes(barcodeXML);
	}

	//getters and setters

	//this method parse the XMLNodes to spefic Barcodes
	//the XML for the barcode is:
	/*
		<Character>-</Character>
		<Symbology>n w n n n n w n w</Symbology>
		<Binary>101011010101010110101101</Binary>
	*/
	void setBarcodes(string barcodeXML)
	{
		XMLParser parser(barcodeXML);
		//vector<XMLNode> nodes = parser.getXMLNodeList();
		list<XMLNode> nodes = parser.getXMLNodeList();

		for (auto it = nodes.begin(); it != nodes.end(); ++it)
		{
			Barcode b1;
			list<XMLElement> elements = (*it).getElements();
			for (list<XMLElement>::iterator em = elements.begin();
				em != elements.end();
				++em)
			{
				//the character for the barcode
				if (em->getTagName() == "Character") 
				{
					char character = em->getData()[0];
					b1.setCode(character);
				}
				//the symboloty for the barcode
				else if (em->getTagName() == "Symbology")
				{
					b1.setSymbology(em->getData());
				}
				//the binary and decimal for the bar code
				else if (em->getTagName() == "Binary")
				{
					bitset<24> bset = bitset<24>(em->getData());
					b1.setBinary(bset);
					b1.setDecimalValue(bset);
				}
			}
			//push onto the barcode vector
			this->barcodes.push_back(b1);
		} //end of for
	}

	//get the barcodes
	vector<Barcode> getBarcodes()
	{
		return this->barcodes;
	}

	//get the barcodes based on the decimal values of the binary
	//also set the sortedDecimals vector
	vector<Barcode> getSortedBarcodes()
	{
		sort(this->barcodes.begin(), this->barcodes.end());
		for (auto & element : this->barcodes) {
			this->sortedDecimals.push_back(element.getDecimalValue());
		}
		return this->barcodes;
	}

	vector<int> getSortedDecimals()
	{
		return this->sortedDecimals;
	}


}; //------------end of BarcodeParser class


//-----------------------this is the Product class
class Product
{
private:
	string name;
	double price;
	int quantity;
	string nameNOSpace;

public:
	//default constructor
	Product()
	{
		name = "";
		price = 0;
		quantity = 0;
		nameNOSpace = "";
	}
	//other constructor
	Product(string name, double price, int quantity)
	{
		this->name = name;
		this->price = price;
		this->quantity = quantity;
		//replace space with 'S'
		this->nameNOSpace = name;
		for (int i = 0; i < name.length(); i++)
		{
			if (name[i] == 0x20) //if a space char
				nameNOSpace[i] = 'S'; //replace it with 'S'
		}
	}

	//getters and setters
	void setName(string name)
	{
		this->name = name;
		this->nameNOSpace = name;
		for (int i = 0; i < name.length(); i++)
		{
			if (name[i] == 0x20) //if a space char
				this->nameNOSpace[i] = 'S'; //repace it with 'S'
		}
	}
	void setPrice(double price)
	{
		this->price = price;
	}
	void setQuantity(int qty)
	{
		this->quantity = qty;
	}
	string getName()
	{
		return this->name;
	}
	string getNameNOSpace()
	{
		return this->nameNOSpace;
	}
	double getPrice()
	{
		return this->price;
	}
	int getQuantity()
	{
		return this->quantity;
	}
}; //--------the end of Product class


//----------------this is ProductParser class
class ProductParser
{
private:
	vector<Product> products;
	vector<string> productNames;
	vector<string> productNamesNOSpace;
public:
	//default constructor
	ProductParser(){}
	//other constructor
	ProductParser(string productsFile)
	{
		setProducts(productsFile);
	}

	//getters and setters
	void setProducts(string productsFile)
	{
		//open the file
		ifstream inFile;
		inFile.open(productsFile);
		//file opening exception
		if (!inFile)
			throw "Cannot open the products file!";

		//parser helper
		ParserHelper h1;
		//regex pattern
		string pattern = "[^,]*";

		string line = "";
		vector<string> matchedResults;

		while (getline(inFile, line))
		{
			//find match results
			matchedResults = h1.getMatchResults(line, pattern);
			if (matchedResults.size() == 3)
			{
				//construct a Product object
				Product p1;
				//set name
				string name = h1.toUpper(matchedResults[0]);
				p1.setName(name);
				this->productNames.push_back(name);
				this->productNamesNOSpace.push_back(p1.getNameNOSpace());
				//set price
				double price = stod(matchedResults[1]);
				p1.setPrice(price);
				//set quantity
				int quantity = stoi(matchedResults[2]);
				p1.setQuantity(quantity);

				//now add the Product object to the vector
				this->products.push_back(p1);

			}
		}//the end of while

		//close the file
		inFile.close();
	}//the end of setProducts

	//get the products vector
	vector<Product> getProducts()
	{
		return this->products;
	}

	//get the products name vector
	vector<string> getProductNames()
	{
		return this->productNames;
	}

	//get the products name no space vector
	vector<string> getProductNamesNOSpace()
	{
		return this->productNamesNOSpace;
	}

	
}; //----------------the end of ProductParser class


//-------------------this is Cart class
class Cart
{
private:
	int cartNum;
	vector<Product> productList;
public:
	//default constructor
	Cart(){}
	//other constructor
	Cart(int num)
	{
		this->cartNum = num;
	}

	//getters and setters
	void setCartNum(int num)
	{
		this->cartNum = num;
	}
	void setProductList(vector<Product> plist)
	{
		this->productList = plist;
	}
	int getCartNum()
	{
		return this->cartNum;
	}
	vector<Product> getProductList()
	{
		return this->productList;
	}

	//get total price of the products in this cart
	double getTotalPrice()
	{
		double total = 0.00;
		for (auto &product : this->productList)
		{
			total += product.getPrice();
		}

		return total;
	}

}; //------------the end of Cart class

//------------------this is the CartParser class
class CartParser
{
private:
	vector<Cart> carts;
public:
	//default constructor
	CartParser(){}
	//other constructor
	CartParser(string cartsFile)
	{
		setCarts(cartsFile);
	}

	//getters and setters
	//parse the Carts.csv
	void setCarts(string cartsFile)
	{
		//open the file
		ifstream inFile;
		inFile.open(cartsFile);
		//file opening exception
		if (!inFile)
			throw "Cannot open the carts file!";

		//parser helper
		ParserHelper h1;
		//regex pattern
		string productPattern = "[^,]*";
		string cartNumPattern = "Cart[0-9]*";

		//this program takes a while to run
		cout << "This program will take a while to run. Please be patient..." << endl;

		//Barcode parser
		BarcodeParser b1("Barcodes3of9.xml");
		vector<Barcode> barcodes = b1.getSortedBarcodes();
		vector<int> sortedDecimals = b1.getSortedDecimals();

		//Products parser
		ProductParser p1("ProductPrice.csv");
		vector<Product> products = p1.getProducts();
		vector<string> productNames = p1.getProductNames();
		vector<string> productNamesNOSpace = p1.getProductNamesNOSpace();
				
		//going through the carts file
		string line = "";
		vector<string> matchedResults;
		
		while (getline(inFile, line))
		{
			//declare a Cart object
			Cart c1;
			//declare the product vector for the cart
			vector<Product> productsInOneCart;
			//find match results for cart num
			matchedResults = h1.getMatchResults(line, cartNumPattern);
			if (matchedResults.size() == 1)
			{
				//strip the card number
				int cartNum = stoi(matchedResults[0].substr(4));
				c1.setCartNum(cartNum);
				
				getline(inFile, line); //get the line for the products

				//find match results for the products
				matchedResults = h1.getMatchResults(line, productPattern);
				//process each product
				for (int i = 0; i < matchedResults.size(); i++)
				{
					string strItem = ""; //this is what the string will be after converting
					string strHexItem = matchedResults[i];
					for (int j = 0; j < strHexItem.length(); j = j + 6)
					{
						//string strBinary = h1.getBinaryFromHex(strHexItem.substr(j, j + 6));
						bitset<24> strBits = bitset<24>(h1.getBinaryFromHex(strHexItem.substr(j, j + 6)));
						int decimal = (int)(strBits.to_ulong());
						int findPos = h1.binarySearch(sortedDecimals, decimal, barcodes.size());
						if (findPos != -1)

							strItem += barcodes[findPos].getCode();
					}

					//now get the product name matches the strItem
					if (!strItem.empty())
					{
						int productIndex = h1.binarySearch(productNamesNOSpace, strItem, productNames.size());
						if (productIndex != -1)
						{
							Product item;
							item.setName(productNames[productIndex]);
							item.setPrice(products[productIndex].getPrice());
							productsInOneCart.push_back(item); //add to the products on that one cart
						}
					}
				} //end of for loop

				//add the products to that Cart
				if (productsInOneCart.size() > 0)
					c1.setProductList(productsInOneCart);
				//add the cart to the cart vector
				//if cart num is not empty, then will add to the cart vector
				//in that case, the cart is just empty(no products)
				if (c1.getCartNum() > 0)
					this->carts.push_back(c1);
			} //end of matchresults == 1
			
				
		}//the end of while

		//close the file
		inFile.close();
	}//the end of setCarts


	//get the carts
	vector<Cart> getCarts()
	{
		return this->carts;
	}

}; //---------------the end of CartParser class


//----------------this is the converter class
class Converter
{
public:
	void output(vector<Cart> carts)
	{
		for (auto & cart : carts) 
		{
			double total = cart.getTotalPrice();
			
			cout << fixed << endl;
			cout << "Cart " << cart.getCartNum() << " total price: $" << setprecision(2) << total << endl;
			for (auto &product : cart.getProductList())
			{
				cout << product.getName() << ": $" << setprecision(2) << product.getPrice() << endl;
			}
			cout << "\n" << endl;
		}
	}

}; //-------the end of Converter class

//---------main function
int main()
{
	//read the carts file
	CartParser cp1("carts.csv");
	//get the carts
	vector<Cart> carts = cp1.getCarts();
	//output the decrpyted carts
	Converter c1;
	c1.output(carts);
	

	system("pause");
	return 0;
}