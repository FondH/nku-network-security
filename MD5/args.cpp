#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#include "Md5.h"

#define yellow "\u001b[33m"
#define red "\u001b[35m"
#define white "\u001b[0m"
#define blue "\u001b[34m"
#define lightGreen "\u001b[36;1m"
#define lightBlue "\u001b[1m\u001b[34;1m"
void printHelp() {
    std::cout << "Usage: md5.exe [option]\n"
        << "Options:\n"
        << "   -h        Show help information\n"
        << "   -s [text] Compute MD5 hash of the provided text\n"
        << "   -f [path] [md5path] Compute MD5 hash of the file at [path] and save it to [md5path]\n"
        << "   -t        Output test cases\n"
        << "   -c [md5value/file] [path] Compare MD5 value or file with MD5 of file at [path]\n";
}

std::string rstrip(const std::string& s) {
    size_t end = s.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}


std::string computeMD5(const std::string text) {
    std::string s = Update(text, false);
    std::cout << "MD5(" << "\'" << blue << text << white << "\')=" << lightGreen << s <<white<< std::endl;
    return s;
}

void handleFile(const char* filepath, const char* md5path) {


    std::string md5 = Update(filepath, md5path, true);
    std::cout << "MD5 of file: \'" << yellow << filepath << white << "\' is: "
        << lightGreen << md5 << white << std::endl;
}

void handleCompare(const char* targetPath) {

    std::string s;
    std::cout << "Please Input The old MD5 value of file(\'" << targetPath << "\'): \n";
    std::cin >> s;

    std::string md5 = Update(targetPath, nullptr, false);
    std::cout << "MD5 of file: \'" << yellow << targetPath << white << "\' is: "
              << lightGreen << md5 << white << std::endl;



    for(int i=0;i<md5.length();i++)
        if (md5[i] != s[i]) {
            std::cout << "MD5 does not match." << std::endl;
            return;
        }

        std::cout << "MD5 match successful." << std::endl;
    

}

void handleCompare(const char* md5OrFilePath, const char* targetPath) {
    std::string s;
    std::ifstream md5File(md5OrFilePath);
    if (md5File) {
        std::getline(md5File, s);
        md5File.close();
    }
    std::cout << "The old MD5 value of file(\'" << yellow << targetPath << white << "\') in (\'" 
                                            << yellow << md5OrFilePath << white << "\') is \n"
                                            << lightGreen << s << white <<std::endl;


    std::string md5 = Update(targetPath,nullptr,false);
    std::cout << "MD5 of file: \'" << yellow << targetPath << white << "\' is: "<< lightGreen << md5 << white << std::endl;

    for (int i = 0;i < md5.length();i++)
        if (md5[i] != s[i]) {
            std::cout << "MD5 does not match, File has been modified" << std::endl;
            return;
        }

    std::cout << "MD5 match successful." << std::endl;
    
}
void printWatermark() {
    std::cout << "========================================\n";
    std::cout << " MD5 Hash Generator \n";
    std::cout << " Created by: " << lightBlue << "Fond\n" << white;
    std::cout << "========================================\n\n\n";
}
int main(int argc, char* argv[]) {


    //test();
    if (argc < 2) {
        printWatermark();

        printHelp();
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0) {
        printWatermark();
        printHelp();
    }
    else if (strcmp(argv[1], "-t") == 0) {
        printWatermark();
        test();
    
    }

    else if (strcmp(argv[1], "-s") == 0) {
        if (argc < 3) {
            std::cerr << "No text provided for -s option." << std::endl;
            return 1;
        }
        printWatermark();

        computeMD5(argv[2]);
    }
    else if (strcmp(argv[1], "-f") == 0) {
        if (argc < 4) {
            std::cerr << "Insufficient arguments for -f option." << std::endl;
            return 1;
        }
        printWatermark();

        handleFile(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "-c") == 0) {
           
        if (argc < 3) {
            std::cerr << "Insufficient arguments for -c option." << std::endl;
            return 1;
        }

        printWatermark();

        if (argc == 3) {

            handleCompare(argv[2]);
            return 1;
        }

        handleCompare(argv[2], argv[3]);
    }
    else {
        std::cerr << "Unknown option: " << argv[1] << std::endl;
        printHelp();
   
    }

    return 0;
}
