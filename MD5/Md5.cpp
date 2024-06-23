
#include "Md5.h"
static int origin_a = 0x67452301;
static int origin_b = 0xefcdab89;
static int origin_c = 0x98badcfe;
static int origin_d = 0x10325476;
static BYTE PADDING[64] =
{
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0
};

BYTE blockBuffer[64]; // temporary storage less than 64 BYTE
uint count[2]; // current size(bits), lower in 0  upper in 1
uint state[4];
BYTE digest[16];


uint F(uint x, uint y, uint z) {
	return (x & y) | ((~x) & z);
}
uint G(uint x, uint y, uint z) {
	return (x & z) | (y & (~z));
}
uint H(uint x, uint y, uint z) {
	return  x^y^z;
}

uint I(uint x, uint y, uint z) {
	return y ^ (x | ~z);
}

uint RotateLeft(uint x, int s) {

	return  (x << s) | (x >> (32 - s));
}

uint FF(uint a, uint b, uint c, uint d, uint x, int s, uint ti) {

	return b + RotateLeft((a + F(b, c, d) + x + ti), s);

}

uint GG(uint a, uint b, uint c, uint d, uint x, int s, uint ti) {
	
	return b + RotateLeft((a + G(b, c, d) + x + ti), s);
}

uint HH(uint a, uint b, uint c, uint d, uint x, int s, uint ti) {

	return b + RotateLeft((a + H(b, c, d) + x + ti), s);
}


uint II(uint a, uint b, uint c, uint d, uint x, int s, uint ti) {

	return b + RotateLeft((a + I(b, c, d) + x + ti), s);
}



// For each 512 bits ...
void Transform(const BYTE blk[64], uint state[4]) {
	uint a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	for (int i = 0;i < 64;i += 4) {
		x[i / 4] = 0;
		for (int j = 0;j < 4;j++)
			x[i / 4] |= ((uint)blk[i + j]) << (j) * 8;
	}
	
	// 第一次运算FF
	a = FF(a, b, c, d, x[0], 7, 0xd76aa478L);
	d = FF(d, a, b, c, x[1], 12, 0xe8c7b756L);
	c = FF(c, d, a, b, x[2], 17, 0x242070dbL);
	b = FF(b, c, d, a, x[3], 22, 0xc1bdceeeL);
	a = FF(a, b, c, d, x[4], 7, 0xf57c0fafL);
	d = FF(d, a, b, c, x[5], 12, 0x4787c62aL);
	c = FF(c, d, a, b, x[6], 17, 0xa8304613L);
	b = FF(b, c, d, a, x[7], 22, 0xfd469501L);
	a = FF(a, b, c, d, x[8], 7, 0x698098d8L);
	d = FF(d, a, b, c, x[9], 12, 0x8b44f7afL);
	c = FF(c, d, a, b, x[10], 17, 0xffff5bb1L);
	b = FF(b, c, d, a, x[11], 22, 0x895cd7beL);
	a = FF(a, b, c, d, x[12], 7, 0x6b901122L);
	d = FF(d, a, b, c, x[13], 12, 0xfd987193L);
	c = FF(c, d, a, b, x[14], 17, 0xa679438eL);
	b = FF(b, c, d, a, x[15], 22, 0x49b40821L);

	// 第二轮运算GG
	a = GG(a, b, c, d, x[1], 5, 0xf61e2562L);
	d = GG(d, a, b, c, x[6], 9, 0xc040b340L);
	c = GG(c, d, a, b, x[11], 14, 0x265e5a51L);
	b = GG(b, c, d, a, x[0], 20, 0xe9b6c7aaL);
	a = GG(a, b, c, d, x[5], 5, 0xd62f105dL);
	d = GG(d, a, b, c, x[10], 9, 0x2441453L);
	c = GG(c, d, a, b, x[15], 14, 0xd8a1e681L);
	b = GG(b, c, d, a, x[4], 20, 0xe7d3fbc8L);
	a = GG(a, b, c, d, x[9], 5, 0x21e1cde6L);
	d = GG(d, a, b, c, x[14], 9, 0xc33707d6L);
	c = GG(c, d, a, b, x[3], 14, 0xf4d50d87L);
	b = GG(b, c, d, a, x[8], 20, 0x455a14edL);
	a = GG(a, b, c, d, x[13], 5, 0xa9e3e905L);
	d = GG(d, a, b, c, x[2], 9, 0xfcefa3f8L);
	c = GG(c, d, a, b, x[7], 14, 0x676f02d9L);
	b = GG(b, c, d, a, x[12], 20, 0x8d2a4c8aL);

	// 第三轮运算HH
	a = HH(a, b, c, d, x[5], 4, 0xfffa3942L);
	d = HH(d, a, b, c, x[8], 11, 0x8771f681L);
	c = HH(c, d, a, b, x[11], 16, 0x6d9d6122L);
	b = HH(b, c, d, a, x[14], 23, 0xfde5380cL);
	a = HH(a, b, c, d, x[1], 4, 0xa4beea44L);
	d = HH(d, a, b, c, x[4], 11, 0x4bdecfa9L);
	c = HH(c, d, a, b, x[7], 16, 0xf6bb4b60L);
	b = HH(b, c, d, a, x[10], 23, 0xbebfbc70L);
	a = HH(a, b, c, d, x[13], 4, 0x289b7ec6L);
	d = HH(d, a, b, c, x[0], 11, 0xeaa127faL);
	c = HH(c, d, a, b, x[3], 16, 0xd4ef3085L);
	b = HH(b, c, d, a, x[6], 23, 0x4881d05L);
	a = HH(a, b, c, d, x[9], 4, 0xd9d4d039L);
	d = HH(d, a, b, c, x[12], 11, 0xe6db99e5L);
	c = HH(c, d, a, b, x[15], 16, 0x1fa27cf8L);
	b = HH(b, c, d, a, x[2], 23, 0xc4ac5665L);

	// 第四轮运算II
	a = II(a, b, c, d, x[0], 6, 0xf4292244L);
	d = II(d, a, b, c, x[7], 10, 0x432aff97L);
	c = II(c, d, a, b, x[14], 15, 0xab9423a7L);
	b = II(b, c, d, a, x[5], 21, 0xfc93a039L);
	a = II(a, b, c, d, x[12], 6, 0x655b59c3L);
	d = II(d, a, b, c, x[3], 10, 0x8f0ccc92L);
	c = II(c, d, a, b, x[10], 15, 0xffeff47dL);
	b = II(b, c, d, a, x[1], 21, 0x85845dd1L);
	a = II(a, b, c, d, x[8], 6, 0x6fa87e4fL);
	d = II(d, a, b, c, x[15], 10, 0xfe2ce6e0L);
	c = II(c, d, a, b, x[6], 15, 0xa3014314L);
	b = II(b, c, d, a, x[13], 21, 0x4e0811a1L);
	a = II(a, b, c, d, x[4], 6, 0xf7537e82L);
	d = II(d, a, b, c, x[11], 10, 0xbd3af235L);
	c = II(c, d, a, b, x[2], 15, 0x2ad7d2bbL);
	b = II(b, c, d, a, x[9], 21, 0xeb86d391L);
	
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}




void reset() {
	memset(blockBuffer, 0, sizeof(blockBuffer));
	memset(count, 0, sizeof(count));
	memset(digest, 0, sizeof(digest));
	state[0] = origin_a;	state[1] = origin_b;
	state[2] = origin_c;	state[3] = origin_d;

	//std::cout << "MD5 INIT"<<std::endl;
}

void printState() {

	std::string pad(4,'-');
	std::cout << "State: \n"
		<< pad << "Count: " << count[0] << " bits\n"
		<< pad << "state: " << std::endl
		<< pad << pad << "A " << std::hex << state[0] << std::endl
		<< pad << pad << "B " << std::hex << state[1] << std::endl
		<< pad << pad << "C " << state[2] << std::endl
		<< pad << pad << "D " << state[3] << std::endl;
				


}
int getBufferIndex() {
	
	return (count[0] >> 3) & 0x3f;
}

void _Update(const BYTE* stream, size_t len, bool isPrint) {
	

	int ind = getBufferIndex();

	count[0] += ((uint)len << 3);
	count[1] += ((uint)len >> 29);
	if (count[0] < ((uint)len << 3)) //判断是否进位
		count[1]++;
	

	int partLen = 64 - ind; //blk 里剩下的

	if (len < partLen) {
		//stream加上buffer也不够512
		memcpy(&blockBuffer[ind], stream, len);
		//printState();

		return ;
	} 

	/* 64 一组进行transform*/
	memcpy(&blockBuffer[ind], stream, partLen);
	Transform(blockBuffer, state);
	
	int i = partLen;  
	for (i; i < len;i += 64) 

		Transform(&stream[i], state);

	memcpy(blockBuffer, &stream[i - 64], len + 64 - i);
	if(isPrint)
		printState();
}



void int2ByteArray(BYTE out[], uint input[], int len) {

	for (int i = 0;i < len;i += 4) {
		out[i] = input[i/4] & 0xff;
		out[i + 1] = (input[i/4] >> 8) & 0xff;
		out[i + 2] = (input[i/4] >> 16) & 0xff;
		out[i + 3] = (input[i/4] >> 24) & 0xff;

	}

}
void finallize() {

	/* 以64位二进制表示的填充前的信息长度 */
	BYTE bit_count[8];
	int2ByteArray(bit_count,count,8);
	/* length of padding: count % 512 = 448  */
	int indx = getBufferIndex();
	int padLen = (indx < 56) ? (56 - indx) : (120 - indx);

	_Update(PADDING, padLen);
	_Update(bit_count, 8,true);

	int2ByteArray(digest, state, 16);
}


std::string getMD5AsHex(BYTE digest[16], bool isPrint) {
	std::stringstream ss;
	for (int i = 0; i < 16; ++i) {
		ss << std::hex << std::setfill('0') << std::setw(2) << (uint)digest[i];
	}

	if(isPrint)
		std::cout << "MD5 as Hex: " << ss.str() << std::endl;

	return ss.str();
}

void printMD5AsASCII(uint md5[4]) {
	std::stringstream ss;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			char c = (md5[i] >> (8 * j)) & 0xFF;  // Extract each byte
			ss << c;
		}
	}
	std::cout << "MD5 as ASCII: " << ss.str() << std::endl;
}

/*
	input: test text
	cout: MD5('') = 32 digits HEX's val
*/
std::string Update(const std::string text, bool isPrint) {

	//std::string stream = "a";
	const int l = text.length();
	BYTE* tp = new BYTE[l];

	for (int i = 0;i < l;i++)
		tp[i] = text[i];


	reset();
	_Update(tp, l);
	finallize();
	std::string s = getMD5AsHex(digest);

	if(isPrint)
		std::cout << "MD5(" << "\'"<< text << "\')=" << s << std::endl;
	return s;
}

std::string Update(const char* filePath, const char* outPath, bool isPrint) {
	
	std::ifstream file(filePath, std::ifstream::binary);
	if (!file) {
		std::cerr << "File could not be opened: " << filePath << std::endl;
		return "";
	}

	reset();

	char buffer[1024]; // Define a buffer to hold file parts
	BYTE ubuffer[1024];

	while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
		

		// 将 char 数组转换为 unsigned char 数组
		for (size_t i = 0; i < 1024; ++i) 
			ubuffer[i] = static_cast<BYTE>(ubuffer[i]);
		

		_Update(ubuffer, file.gcount());
	}

	finallize();
	std::string s = getMD5AsHex(digest, false);


	if (isPrint)
		std::cout << "MD5 of file: \'" << filePath << "\' is " << s << std::endl;

	if (!outPath)
		return s;


	std::ofstream md5file(outPath);
	if (md5file) {
		md5file << s;
		md5file.close();

		std::cout << ".md5 outpath: \'" << outPath << "\'"<<std::endl;
	}
	else {
		std::cerr << "MD5 file could not be opened: " << outPath << std::endl;
	}



	return s;
}

void test() {
	Update("", true);
	Update("a", true);
	Update("abc", true);
	Update("abc", true);
	Update("Md5 Test example", true);
	Update("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", true);
	Update("HHHHHH, This is NKU Cyber security", true);

}
/* 测试样例
int main() {
	std::string text = "a";
	const int l = text.length();
	BYTE* tp = new BYTE[l];

	for (int i = 0;i < l;i++)
		tp[i] = text[i];

	reset();
	_Update(tp, l);
	finallize();
	std::string s = getMD5AsHex(digest);

	std::cout << "MD5(" << "\'" << text << "\')=" << s << std::endl;

}*/