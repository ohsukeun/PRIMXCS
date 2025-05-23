
#include <BodyCompressor.h>
#include <string.h>

#include <base64.h>
#include <lz4.h>

#include <iostream>

BodyCompressor::BodyCompressor()
{
}


BodyCompressor::~BodyCompressor()
{
}

string BodyCompressor::CompressedBody(string plainBody)
{
	int originLen = plainBody.length();
	int nMaxCompressedSize = LZ4_compressBound(originLen);

	char *pszCompressed = new char[nMaxCompressedSize + 1];
	memset(pszCompressed, 0, nMaxCompressedSize + 1);

	int nCompressedSize = LZ4_compress(plainBody.c_str(), pszCompressed, originLen);

	string encoded = base64_encode(reinterpret_cast<const unsigned char*>(pszCompressed), nCompressedSize);
	delete [] pszCompressed;

	return encoded;
}

string BodyCompressor::CompressedBody(string plainBody, int& originLen)
{
	originLen = plainBody.length();
	int nMaxCompressedSize = LZ4_compressBound(originLen);

	char *pszCompressed = new char[nMaxCompressedSize + 1];
	memset(pszCompressed, 0, nMaxCompressedSize + 1);

	int nCompressedSize = LZ4_compress(plainBody.c_str(), pszCompressed, originLen);

//	string encoded = base64_encode(reinterpret_cast<const unsigned char*>(pszCompressed), nCompressedSize);
	string encoded = base64_encode((const unsigned char*)(pszCompressed), nCompressedSize);
	delete [] pszCompressed;

	return encoded;
}

string BodyCompressor::CompressedBody(string plainBody, int& originLen, int& compressedLen)
{
	originLen = plainBody.length();
	int nMaxCompressedSize = LZ4_compressBound(originLen);

	char *pszCompressed = new char[nMaxCompressedSize + 1];
	memset(pszCompressed, 0, nMaxCompressedSize + 1);

	int nCompressedSize = LZ4_compress(plainBody.c_str(), pszCompressed, originLen);

//	string encoded = base64_encode(reinterpret_cast<const unsigned char*>(pszCompressed), nCompressedSize);
	string encoded = base64_encode((const unsigned char*)(pszCompressed), nCompressedSize);
	delete [] pszCompressed;
	compressedLen = nCompressedSize;
	return encoded;
}

char* BodyCompressor::CompressedBodyNoBase64(string plainBody, int& originLen, int& compressedLen)
{
	originLen = plainBody.length();
	int nMaxCompressedSize = LZ4_compressBound(originLen);

	char *pszCompressed = new char[nMaxCompressedSize + 1];
	memset(pszCompressed, 0, nMaxCompressedSize + 1);

	compressedLen = LZ4_compress(plainBody.c_str(), pszCompressed, originLen);

	return pszCompressed;
}

char* BodyCompressor::CompressedBodyBase64First(string plainBody, int& originLen, int& compressedLen)
{
	originLen = plainBody.length();
	string encoded = base64_encode((const unsigned char*)(plainBody.data()), originLen);
	int encodedLen = encoded.length();
	int nMaxCompressedSize = LZ4_compressBound(encodedLen);

	char *pszCompressed = new char[nMaxCompressedSize + 1];
	memset(pszCompressed, 0, nMaxCompressedSize + 1);

	compressedLen = LZ4_compress(encoded.c_str(), pszCompressed, encodedLen);

	return pszCompressed;
}

string BodyCompressor::Uncompressedbody(string compressedBody, int originLen)
{
	string decoded = base64_decode(compressedBody);

	char *pszUncompressed = new char[originLen + 1];
	memset(pszUncompressed, 0, originLen + 1);
	LZ4_uncompress(decoded.c_str(), pszUncompressed, originLen);

	string uncompressed = pszUncompressed;
	delete [] pszUncompressed;

	return uncompressed;
}

