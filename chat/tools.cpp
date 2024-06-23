#include "tools.h"
using std::cout;

std::vector<std::string> split(std::string& s, char delimiter) {
    //将string 按照;:, 划分
    std::vector<std::string> tokens;
    std::string token;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] != delimiter) {
            token += s[i];
        }
        else {
            tokens.push_back(token);
            token.clear();
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string time2s(time_t rawtime) {

    tm ptminfo;
    localtime_s(&ptminfo, &rawtime);
    char s[256];
    snprintf(s, sizeof(s), "%02d-%02d-%02d %02d:%02d:%02d",
        ptminfo.tm_year + 1900, ptminfo.tm_mon + 1, ptminfo.tm_mday,
        ptminfo.tm_hour, ptminfo.tm_min, ptminfo.tm_sec);

    return s;
}

time_t getCurrentTime() {

    std::time_t rawtime;
    time(&rawtime);

    return rawtime;
}
void saveCursorPosition() {
    cout << "\033[s"; // 保存光标位置
}

void restoreCursorPosition() {
   cout << "\033[u"; // 恢复光标位置
}

void clearThisLine() {
    cout << "\033[2K"; // 清除当前行内容
}


void clearLines(int numLines) {
    for (int i = 0; i <= numLines; ++i) {
        clearThisLine();
        if (i < numLines) {
            cout << "\033[1B";
        }
    }
   //cout << "\033[" << numLines << "A";
}



void printLines(const std::string& startMessage, int numLines) {
    for (int i = 0; i < numLines; ++i) {
        std::cout << startMessage << " " << i << std::endl;
    }
    // 将光标移动到行末
    std::cout << "\033[0E";
}



std::string to_hex(uint64_t n) {
    std::stringstream ss;
    ss << std::hex << n;
    return ss.str();
}

std::string to_binary(uint8_t i, int len = 8) {
    std::string ss;

    int id = len - 1;
    while (id >= 0) {
        ss += std::to_string(i >> (id--) & 0x1);
        if (!((id + 1) & 0b111))
            ss += " ";
    }
    return ss;

}

std::string to_binary(uint32_t i, int len = 32) {
    std::string ss;
    int id = len - 1;
    while (id >= 0) {
        ss += std::to_string(i >> (id--) & 0x1);
        if (!((id + 1) & 0b111))
            ss += " ";
    }
    return ss;

}

std::string to_binary(uint64_t i, int len = 64) {
    std::string ss;
    int id = len - 1;
    while (id >= 0) {
        ss += std::to_string(i >> (id--) & 0x1);
        if (!((id + 1) & 0b111))
            ss += " ";
    }
    return ss;

}
void clean_screen() {
    std::cout << "\033[2J";

}
