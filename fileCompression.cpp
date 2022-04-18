#include <iostream>
#include <sstream>
#include <fstream>


#include "raylib.h"
#include "fileCompression.h"

std::string compress(std::string data)
{
	unsigned char* toCompress = (unsigned char*)data.c_str();

	int resultSize;
	int *resultSizePtr = &resultSize;
	unsigned char *compressed = CompressData(toCompress, data.length(), resultSizePtr);

	std::string result(reinterpret_cast<char const*>(compressed), resultSize);

	return result;
}

std::string decompress(std::string data)
{
	unsigned char* toDecompress = (unsigned char*)data.c_str();

	int resultSize;
	int *resultSizePtr = &resultSize;
	unsigned char *uncompressed = DecompressData(toDecompress, data.length(), resultSizePtr);

	std::string result(reinterpret_cast<char const*>(uncompressed), resultSize);

	return result;
}

bool compressAndSaveData(std::string filename, std::string data)
{
	std::string tempFilename = filename + ".temp";
	std::ofstream outputFile(tempFilename);
	if (outputFile.fail())
	{
		std::cout << "Unable to load file\n";
		return false;
	}

	std::string compressed = compress(data);

	outputFile.write(compressed.c_str(), compressed.size());
	outputFile.close();


	if (outputFile)
	{
		remove(filename.c_str());
		rename(tempFilename.c_str(), filename.c_str());
		return true;
	}

	std::cout << "Error: Write failed\n";
	remove(tempFilename.c_str());
	return false;
}

std::string loadCompressedData(std::string filename)
{
	std::ifstream inputFile;
	inputFile.open(filename, std::ios::binary);
	if (inputFile.fail())
	{
		std::cout << "Unable to load file\n";
		return "";
	}

	std::string data((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));

	inputFile.close();

	return decompress(data);
}