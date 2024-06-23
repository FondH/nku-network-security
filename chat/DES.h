#pragma once
#include<iostream>
#include<string>
#include <sstream>
#include<iomanip>
#include <vector>
#include <random>
#include"tools.h"
using namespace std;
extern size_t block_size;

static uint64_t* KEY;

static uint64_t SUBKEY[16] = {0};



#pragma pack(4)
struct desTuple {
    char name[3];
    uint64_t k[4];
    desTuple(uint64_t* N){
        name[0] = 'D';
        name[1] = 'E';
        name[2] = 'S';
        name[3] = '\0';
        memcpy(k, N, 32);
    }
};
#pragma pack()


void set_key(uint64_t K);

void Encrypy(uint64_t plain, uint64_t* dst);

string Encrypy(const string& plain);

void Decrypy(uint64_t cyber, uint64_t* dst);

string Decrypy(const string& cyber);

uint64_t Get_key(uint64_t* Ks, int i, bool En);

void Des_Encrypy(uint64_t plain, uint64_t* dst, uint64_t* Ki, bool En);

void Key_exp(uint64_t* K, uint64_t* Ki);

void Rotate(uint32_t* src);

void Permute(uint64_t* src, uint64_t* dst, const uint8_t* table, int src_len, int dst_len);

void Subtitute(uint64_t* src, uint64_t* dst);

string padString(const string& input, size_t block_size);

vector<string> splitIntoBlocks(const string& input, size_t block_size);

uint64_t block2ull(const string& block);

string to_hex(uint64_t n);

void test_main();

uint64_t generate_random_key();
/*
string to_binary(uint8_t i, int len);
string to_binary(uint32_t i, int len);
string to_binary(uint64_t i, int len);
*/