#ifndef HEADER_CHATH
#define HEADER_CHATH
#include <chrono>   
#include <thread>
#include <vector>
#include <string>
#include <cstdlib> 
#include "DES.h"
#include "tools.h"
#include "socket.h"
#include "RSA.h"


#define MAX_MESS_NUM 20
//using namespace std;


struct mess;
std::vector<mess> messBuffer;
std::string username="Fond";


bool Flag_View = true;
bool FLag_Input = true;
extern bool FLag_Recv = true;


void DrawInterface();

void ThreadView(int id);
void ThreadInput(int id);
void ThreadSend(std::string a, bool u);
void appendMess(std::string s);


struct mess {
    int type = 0;
    std::string username = "default";
    std::string message = "";
    time_t time;

    mess(std::vector<std::string> vec);

    mess(int t, const std::string u, const std::string m, time_t ti);


    std::string toString();

};


#endif // HEADER_H


