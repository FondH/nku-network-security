#pragma once
#include<iostream>
#include<string>
#include<sstream>
#include<iomanip>
#include<ctime>
#include<vector>


std::vector<std::string> split(std::string& s, char delimiter);

std::string time2s(time_t rawtime);

time_t getCurrentTime();

void saveCursorPosition();

void restoreCursorPosition() ;

void clearLines(int numLines);

void printLines(const std::string & startMessage, int numLines);

std::string to_binary(uint8_t i, int len);
std::string to_binary(uint32_t i, int len);
std::string to_binary(uint64_t i, int len);

void clean_screen();
