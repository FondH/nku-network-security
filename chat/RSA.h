#ifndef HEADER_RSAH
#define HEADER_RSAH

#include<iostream>
#include<random>
#include<iomanip>


#pragma pack(4)
struct rsaTuple {
    char sign[4];
    uint64_t d;
    uint64_t N;
    rsaTuple(uint64_t d, uint64_t N) : d(d), N(N) {
        sign[0] = 'R';sign[1] = 'S';
        sign[2] = 'A';sign[3] = '\0';
    };
};
#pragma pack()


rsaTuple getKeyStruct();

uint64_t mul(uint64_t a, uint64_t b, uint64_t mod);

uint64_t mul(uint64_t a, uint64_t mod);

uint64_t pow(uint64_t a, uint64_t d, uint64_t p);

bool miller(int m, uint64_t d, uint64_t n);

bool isPrime(uint64_t n, int epoch);

uint64_t getRandom(uint8_t bits);

uint64_t gcd(uint64_t p, uint64_t q);

uint64_t euclid(uint64_t e, uint64_t t_n);

void RsaInit();

void setRsaKey(uint64_t d, uint64_t N);

void outState();

uint64_t RsaEncrypt(uint16_t m);

uint64_t* RsaEncrypt64(uint64_t m, uint64_t * rs);

uint16_t RsaDecrypt(uint64_t c);

uint64_t RsaDecrypt64(uint64_t* c);


#endif // HEADER_H