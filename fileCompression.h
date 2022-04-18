#pragma once

#include <string>

// Compresses data using Raylib's CompressData() function
std::string compress(std::string data);

// Decompresses data using Raylib's DecompressData function
std::string decompress(std::string data);

// Takes in data to be compressed as well as the file to save
// the compressed data to
// Returns if the save was successful
bool compressAndSaveData(std::string filename, std::string data);

// Takes in a filename which contains compressed data to read in
// and decompress
// Returns the data from file decompressed
std::string loadCompressedData(std::string filename);