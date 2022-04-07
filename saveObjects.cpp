// Helpful: https://www.geeksforgeeks.org/readwrite-class-objects-fromto-file-c/ 
#include <iostream>
#include <fstream>
#include <sstream>
#include "utils.h"

const std::string allSaveData = "all.sav";

class basicStructObj
{
public:
	std::string name;
	int someIntVal;
	bool someBoolVal;
	void setName(std::string n) { name = n; };
	void setSomeIntVal(int i) { someIntVal = i; };
	void setSomeBoolVal(bool b) { someIntVal = b; };
	std::string saveStruct()
	{
		std::string out = "basicStructObj{";
		out += '\0';
		out += "name=" + name + '\0';
		out += "someIntVal=" + std::to_string(someIntVal) + '\0';
		out += "someBoolVal=" + std::to_string(someBoolVal) + '\0';
		out += "}";
		out += '\0';

		return out;
	}

	void loadStruct(std::vector<std::string>& resultVector, int& count)
	{
		bool reading = true;
		while (reading)
		{
			std::string pair = resultVector[count];
			std::string param = biteString(pair, '=');
			std::string val = pair;
			if (param == "name")
				name = val;
			else if (param == "someIntVal")
				someIntVal = std::stoi(val);
			else if (param == "someBoolVal")
				someBoolVal = std::stoi(val) == 1;
			else if (param == "}")
				reading = false;

			if (reading)
				count++;
		}
	};
	std::string toString()
	{
		return name + " " + std::to_string(someIntVal) + " " + std::to_string(someBoolVal);
	};
};

class BasicOBJ
{
public:
	BasicOBJ(std::string n, int i, bool b) : name(n), someIntVal(i), someBoolVal(b){

		setStruct(n, i, b);
	};
	BasicOBJ(){
		name = "";
		someIntVal = 0;
		someBoolVal = NULL;
		setStruct("", 0, NULL);
	};
	std::string getName() { return name; };
	int getSomeIntVal() { return someIntVal; };
	bool getSomeBoolVal() { return someBoolVal; };
	basicStructObj* getStruct() { return objStruct; };
	void setName(std::string n) { name = n; };
	void setSomeIntVal(int i) { someIntVal = i; };
	void setSomeBoolVal(bool b) { someIntVal = b; };
	void setStruct(std::string n, int i, bool b)
	{
		for (int x = 0; x < 2; x++)
		{
			objStruct[x].name = n + "(struct " + std::to_string(x) + ")";
			objStruct[x].someIntVal = 10 + i + x;
			objStruct[x].someBoolVal = !b;
			if (x != 0)
				objStruct[x].someBoolVal = !objStruct[x - 1].someBoolVal;
		}
	};
	std::string toString() {
		std::string out = name + " " + std::to_string(someIntVal) + " " + std::to_string(someBoolVal);
		for (basicStructObj os : objStruct)
			out += "\n\t" + os.toString();
		return out;
	};
	std::string saveObj()
	{
		std::string out = "BasicOBJ{";
		out += '\0';
		out += "name=" + name + '\0';
		out += "someIntVal=" + std::to_string(someIntVal) + '\0';
		out += "someBoolVal=" + std::to_string(someBoolVal) + '\0';
		for (basicStructObj os : objStruct)
			out += os.saveStruct();

		out += "}";
		out += '\0';

		return out;
	};
	void loadObj(std::vector<std::string>& resultVector, int& count)
	{
		bool reading = true;
		int basicStructObjsRead = 0;
		while (reading)
		{
			std::string pair = resultVector[count];
			std::string param = biteString(pair, '=');
			std::string val = pair;
			if (param == "name")
				name = val;
			else if (param == "someIntVal")
				someIntVal = std::stoi(val);
			else if (param == "someBoolVal")
				someBoolVal = std::stoi(val) == 1;
			else if (param == "basicStructObj{")
			{
				objStruct[basicStructObjsRead].loadStruct(resultVector, count);
				basicStructObjsRead++;
			}
			else if (param == "}")
				reading = false;

			if (reading)
				count++;
		}
	};
private:
	std::string name;
	int someIntVal;
	bool someBoolVal;
	basicStructObj objStruct[2];
};


void saveAll(BasicOBJ& basicObj, basicStructObj& structObj)
{
	std::ofstream outputFile;
	std::string data = basicObj.saveObj() + structObj.saveStruct();;

	outputFile.open("all.temp", std::ios::out | std::ios::binary);
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

bool loadAll(BasicOBJ& basicObj, basicStructObj& structObj)
{
	std::ifstream inputFile(allSaveData);
	if (inputFile.fail())
		return false;
	else
	{
		// store all contents of the file in the string data
		std::string data((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));
		std::vector<std::string> resultVector = parseString(data, '\0');
		for (int x = 0; x < resultVector.size(); x++)
		{
			std::string pair = resultVector[x];
			std::string param = biteString(pair, '=');
			std::string val = pair;
			if (param == "BasicOBJ{")
				basicObj.loadObj(resultVector, x);
			else if (param == "basicStructObj{")
				structObj.loadStruct(resultVector, x);
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

			basicStructObj obj2;
			obj2.name = "some name";
			obj2.someIntVal = 15;
			obj2.someBoolVal = true;
			std::cout << "struct being written: " << obj2.toString() << std::endl;
			saveAll(obj, obj2);
		}
		else if (input == "loadall")
		{
			BasicOBJ obj;
			basicStructObj obj2;
			if (loadAll(obj, obj2))
			{
				std::cout << "obj that was read in: " << obj.toString() << std::endl;
				std::cout << "struct being read: " << obj2.toString() << std::endl;
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
