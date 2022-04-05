// Helpful: https://www.geeksforgeeks.org/readwrite-class-objects-fromto-file-c/ 
#include <iostream>
#include <fstream>
#include <sstream>
#include "utils.h"

const std::string objSaveFile = "obj.sav";
const std::string structSaveFile = "struct.sav";
const std::string allSaveData = "all.sav";

struct basicStruct
{
	std::string name;
	int someIntVal;
	bool someBoolVal;
};

class BasicOBJ
{
public:
	BasicOBJ(std::string n, int i, bool b) : name(n), someIntVal(i), someBoolVal(b){
		objStruct.name = n + "(struct)";
		objStruct.someIntVal = i + 10;
		objStruct.someBoolVal = !b;
	};
	BasicOBJ(){
		name = "";
		someIntVal = 0;
		someBoolVal = NULL;
	};
	std::string getName() { return name; };
	int getSomeIntVal() { return someIntVal; };
	bool getSomeBoolVal() { return someBoolVal; };
	basicStruct getStruct() { return objStruct; };
	void setName(std::string n) { name = n; };
	void setSomeIntVal(int i) { someIntVal = i; };
	void setSomeBoolVal(bool b) { someIntVal = b; };
	void setStruct(basicStruct bs) { objStruct = bs; };
	std::string toString() {
		return name + " " + std::to_string(someIntVal) + " " + std::to_string(someBoolVal) + "\n\t" +
			objStruct.name + " " + std::to_string(objStruct.someIntVal) + " " + std::to_string(objStruct.someBoolVal);
	};
	std::string saveObj()
	{
		std::string out = "name=" + name + '\0';
		out += "someIntVal=" + std::to_string(someIntVal) + '\0';
		out += "someBoolVal=" + std::to_string(someBoolVal) + '\0';
		out += "struct::name=" + objStruct.name + '\0';
		out += "struct::someIntVal=" + std::to_string(objStruct.someIntVal) + '\0';
		out += "struct::someBoolVal=" + std::to_string(objStruct.someBoolVal) + '\0';
		out += '\n';

		return out;
	};
	void loadObj(std::string data)
	{
		std::vector<std::string> resultVector = parseString(data, '\0');
		for (int x = 0; x < resultVector.size(); x++)
		{
			std::string pair = resultVector[x];
			std::string param = biteString(pair, '=');
			std::string val = pair;
			if (param == "name")
				name = val;
			else if (param == "someIntVal")
				someIntVal = std::stoi(val);
			else if (param == "someBoolVal")
				someBoolVal = std::stoi(val) == 1;
			else if (param == "struct::name")
				objStruct.name = val;
			else if (param == "struct::someIntVal")
				objStruct.someIntVal = std::stoi(val);
			else if (param == "struct::someBoolVal")
				objStruct.someBoolVal = std::stoi(val) == 1;
		}
	};
private:
	std::string name;
	int someIntVal;
	bool someBoolVal;
	basicStruct objStruct;
};


std::string saveStruct(basicStruct bs)
{
	std::string out = "name=" + bs.name + '\0';
	out += "someIntVal=" + std::to_string(bs.someIntVal) + '\0';
	out += "someBoolVal=" + std::to_string(bs.someBoolVal) + '\0';
	out += "\n";

	return out;
}

void loadStruct(std::string data, basicStruct& bs)
{
	std::vector<std::string> resultVector = parseString(data, '\0');
	for (int x = 0; x < resultVector.size(); x++)
	{
		std::string pair = resultVector[x];
		std::string param = biteString(pair, '=');
		std::string val = pair;
		if (param == "name")
			bs.name = val;
		else if (param == "someIntVal")
			bs.someIntVal = std::stoi(val);
		else if (param == "someBoolVal")
			bs.someBoolVal = std::stoi(val) == 1;
	}
}


void saveAll(BasicOBJ& basicObj, basicStruct& structObj)
{
	std::ofstream outputFile;
	std::string data = basicObj.saveObj();

	outputFile.open("all.temp", std::ios::out | std::ios::app);
	outputFile.write(data.c_str(), data.size());
	data = saveStruct(structObj);
	outputFile.write(data.c_str(), data.size());
	outputFile.close();
	if (outputFile)
	{
		remove(allSaveData.c_str());
		rename("all.temp", allSaveData.c_str());
	}
	else
	{
		std::cout << "Error: Write failed\n";
		remove("all.temp");
	}
}

bool loadAll(BasicOBJ& basicObj, basicStruct& structObj)
{
	std::ifstream inputFile;
	inputFile.open(allSaveData, std::ios::in);
	if (inputFile.fail())
		return false;
	else
	{
		int typeToReadIn = 0;
		std::string data;
		while (!inputFile.eof())
		{
			if (typeToReadIn == 0)
			{
				std::string temp;
				temp = inputFile.get();
				if (temp == "\n")
				{
					typeToReadIn++;
					basicObj.loadObj(data);
					data = "";
				}
				else
					data += temp;
			}
			else
			{
				std::string temp;
				temp = inputFile.get();
				if (temp == "\n")
				{
					typeToReadIn = 0;
					loadStruct(data, structObj);
					data = "";
				}
				else
					data += temp;
			}
		}

	}
	inputFile.close();
	return true;
}

int main(int argc, char* argv[])
{

	if (argc != 2)
	{
		std::cout << "ERROR usage: " << argv[0] << " [desired action]\n";
		std::cout << "Possible desired actions: loadall, saveall\n";
	}
	else
	{
		std::string input = argv[1];
		if (input == "saveall")
		{
			BasicOBJ obj("some name", 15, true);
			std::cout << "obj being written: " << obj.toString() << std::endl;

			basicStruct obj2;
			obj2.name = "some name";
			obj2.someIntVal = 15;
			obj2.someBoolVal = true;
			std::cout << "struct being written: " << obj2.name << " " << obj2.someIntVal << " " << obj2.someBoolVal << std::endl;
			saveAll(obj, obj2);
		}
		else if (input == "loadall")
		{
			BasicOBJ obj;
			basicStruct obj2;
			if (loadAll(obj, obj2))
			{
				std::cout << "obj that was read in: " << obj.toString() << std::endl;
				std::cout << "struct being read: " << obj2.name << " " << obj2.someIntVal << " " << obj2.someBoolVal << std::endl;
			}
			else
				std::cout << "Error Failed to load struct from file \'" << allSaveData << "\'\n";
		}
		else
		{
			std::cout << "invalid option\n";
			std::cout << "Possible options include: loadall, saveall\n";
		}
	}

	return 0;
}