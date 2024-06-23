#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <iomanip>
#include <sstream>


typedef unsigned int uint;
typedef unsigned char BYTE;

uint F(uint x, uint y, uint z);
uint G(uint x, uint y, uint z);
uint H(uint x, uint y, uint z);
uint I(uint x, uint y, uint z);

uint RotateLeft(uint x, int s);

uint FF(uint a, uint b, uint c, uint d, uint x, int s, uint ti);

uint GG(uint a, uint b, uint c, uint d, uint x, int s, uint ti);

uint HH(uint a, uint b, uint c, uint d, uint x, int s, uint ti);

uint II(uint a, uint b, uint c, uint d, uint x, int s, uint ti);



// For each 512 bits ...
void Transform(const BYTE blk[64], uint state[4]);


void reset();

void printState();

int getBufferIndex();

void _Update(const BYTE* stream, size_t len, bool isPrint = false);


void int2ByteArray(BYTE out[], uint input[], int len);

void finallize();


std::string getMD5AsHex(BYTE digest[16], bool isPrint = false);

void printMD5AsASCII(uint md5[4]);

/*
	input: test text
	cout: MD5('') = 32 digits HEX's val
*/
void test();

std::string Update(const std::string text, bool isPrint=false);

std::string Update(const char* filePath, const char* outPath, bool isPrint=false);

