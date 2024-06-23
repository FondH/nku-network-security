/* 
公钥生成
加解密函数

*/
#include "RSA.h"

uint64_t p = 0;
uint64_t q = 0;
uint64_t n = 0;
uint64_t f = 0;
uint64_t e = 0;
uint64_t d = 0;

std::mt19937_64 rng(time(NULL));
std::uniform_int_distribution<uint64_t> distribution(1, 65536);

rsaTuple getKeyStruct() {
	return rsaTuple(e, n);

}

uint64_t mul(uint64_t a, uint64_t b, uint64_t mod) {
	a %= mod;
	b %= mod;
	uint64_t res = 0;

	while (b > 0) {
		// 如果 b 的当前位是 1，则将 a 加到结果中
		if (b & 1) 
			res = (res + a) % mod;
		
		a = (a * 2) % mod;
		b >>= 1;
	}

	return res;
}

uint64_t mul(uint64_t a, uint64_t mod) {
	return mul(a, a, mod);

}

uint64_t pow(uint64_t a, uint64_t d, uint64_t p) {
	// return a^n mod p 
	uint64_t res = 1;
	while (d > 0) {
		if (d & 1)
			res = mul(res, a, p);

		d = d >> 1;
		a = mul(a, p);
	}
	return res;

}

bool miller(int m, uint64_t d, uint64_t n) {

	uint64_t  a = 2 + rand() % (n - 2);
	d = pow(a, d, n);

	if ((d % n == 1) || (d % n == n - 1)) {
		//cout << "Yes";
		return 1;
	}

	for (int i = 0;i < m;i++) {
		d = mul(d, n);
		if (d % (n) == 1) {
			//cout << "No";
			return 0;
		}
		if (d % (n) == n - 1) {
			//cout << "Yes";
			return 1;
		}
	}
	return 0;
}

bool isPrime(uint64_t n, int epoch) {
	srand(time(NULL));

	if(n==1)
		return 0;
	
	if (n == 3 || n == 2)
		return 1;
	
	uint64_t d = n - 1;
	int m = 0;
	while (!(d % 2)) {
		d /= 2;
		m++;
	}
	for (int k = 0;k < epoch;k++)
		if (!miller(m, d, n)) 
			return 0;

	return 1;
		
}

uint64_t getRandom(uint8_t bits){
	uint64_t base = 0;
	//std::mt19937_64 rng(4);  
 
	do {
		
		base = (uint64_t)1 << (bits - 1);
		base += distribution(rng);
	
	
	} while (!isPrime(base, 50));
	
	return base;
}

uint64_t gcd(uint64_t p, uint64_t q)
{
	uint64_t    a = p > q ? p : q;
	uint64_t    b = p < q ? p : q;
	uint64_t    t;
	if (p == q)
	{
		return p;   //两数相等,最大公约数就是本身 
	}
	else
	{
		while (b)    //辗转相除法,gcd(a,b)=gcd(b,a-qb) 
		{
			a = a % b;
			t = a;
			a = b;
			b = t;
		}
		return a;
	}
}

uint64_t euclid(uint64_t e, uint64_t t_n)
{
	uint64_t Max = 0xffffffffffffffff - t_n;
	uint64_t i = 1;
	while (1){
		if (((i * t_n) + 1) % e == 0)
		{
			return ((i * t_n) + 1) / e;
		}
		i++;
		uint64_t Tmp = (i + 1) * t_n;

		if (Tmp > Max)
			return 0;
		
	}
	return 0;

}

void RsaInit() {
	std::mt19937_64 rng(2);    // 种子，可以选择时间作为seed
	std::uniform_int_distribution<uint64_t> distribution(1, 65536);    // 设置范围

	p = getRandom(16);    
	std::cout << "GET RONDOM NUMBER: " << p << std::endl;
	q = getRandom(16);
	std::cout << "GET RONDOM NUMBER: " << q<< std::endl;
	n = p * q;
	f = (p - 1) * (q - 1);
	do
	{
		e = distribution(rng);
		e |= 1;
	} while (gcd(e, f) != 1);
	d = euclid(e, f);
}

void outState() {
	std::cout << "p: " << std::setw(16) << std::setfill('0') << std::hex << p << std::endl;
	std::cout << "q: " << std::setw(16) << std::setfill('0') << std::hex << q << std::endl;
	std::cout << "N: " << std::setw(16) << std::setfill('0') <<std::hex<<n<<std::endl;
	std::cout << "PUBLIC KEY: " << std::setw(16) << std::setfill('0') << std::hex << e << std::endl;
	std::cout << "PRIVATE KEY: "<< std::setw(16) << std::setfill('0') << std::hex << d << std::endl;


	std::cout << "N_oc: " <<std::dec<<  n << std::endl;
	std::cout << "PRIVATE KEY_oc: "<<std::dec << d << std::endl;

}
uint64_t RsaEncrypt(uint16_t m) {
	return pow(m, e, n);
}

uint16_t RsaDecrypt(uint64_t c) {
	return pow(c, d, n);
}

uint64_t* RsaEncrypt64(uint64_t m, uint64_t* rs) {

	//uint64_t rs[4]{0};
	uint16_t* tp = (uint16_t*) &m;
	for (int i = 0;i < 4;i++,tp++)
	{
		rs[i] = RsaEncrypt(*tp);
		
	}
	
	return rs;
}

uint64_t RsaDecrypt64(uint64_t* c) {

	uint64_t rs = 0;
	for (int i = 0;i < 4;i++)
	{
		rs |= uint64_t(RsaDecrypt(c[i])) << (i * 16);

	}

	return rs;
}

void setRsaKey(uint64_t dd, uint64_t Nn) {
	d = dd;
	n = Nn;
}
