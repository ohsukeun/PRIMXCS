#pragma once

#include <string>

using namespace std;

class BodyCompressor
{
public:
	BodyCompressor(void);
	~BodyCompressor(void);

	static string CompressedBody(string plainBody);
	static string CompressedBody(string plainBody, int& originLen);
	static string CompressedBody(string plainBody, int& originLen, int& compressedLen);
	static char*  CompressedBodyNoBase64(string plainBody, int& originLen, int& compressedLen);
	static char*  CompressedBodyBase64First(string plainBody, int& originLen, int& compressedLen);
	static string Uncompressedbody(string compressedBody, int originLen);
};

