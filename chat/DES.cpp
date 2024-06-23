
#include "DES.h"

size_t block_size = 8;
const uint8_t DesInitial[64] = {
    58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
    57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7
};
const uint8_t DesFinal[64] = {
    40,8,48,16,56,24,64,32, 39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30, 37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28, 35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26, 33,1,41,9,49,17,57,25
};

const uint8_t DesExpansion[48] = {
    32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,
    12,13,14,15,16,17,16,17,18,19,20,21,
    20,21,22,23,24,25,24,25,26,27,28,29,
    28,29,30,31,32,1
};

//DES使用一个56位的初始密钥，但是这里提供的是一个64位的值，这是因为在硬件实现中每8位可以用于奇偶校验，在软件实现中多出的位只是简单的忽略掉。
const uint8_t DesTransform[56] = {
    57,49,41,33,25,17,9,1,58,50,42,34,26,18,
    10,2,59,51,43,35,27,19,11,3,60,52,44,36,
    63,55,47,39,31,23,15,7,62,54,46,38,30,22,
    14,6,61,53,45,37,29,21,13,5,28,20,12,4 };

const uint8_t Despermuted[48] ={
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

const uint8_t E[48] = {
	32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,
	12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,
	22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1
};

const uint8_t DesRotations[16] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

const uint8_t E_INV[48] = {};

const uint8_t S_Box[8][4][16] = {
    {
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
    },
    {
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},
    },
    {
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},
    },
    {
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},
    },
    {
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},
    },
    {
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},
    },
    {
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},
    },
    {
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11},
    }
};

const uint8_t P_Box[32] = {
    16,7,20,21, 29,12,28,17, 1,15,23,26, 5,18,31,10,
    2,8,24,14,  32,27,3,9,   19,13,30,6, 22,11,4,25
};

void Subtitute(uint64_t* src, uint64_t* dst) {
    // input 6*8  output 4*8
    int i=0, row, col;
    uint8_t t;
    uint64_t _t=0;
    while (i < 8)
    {

        t = *src >> (i * 6) & 0b111111;
        row = (t >> 5) << 1 | (t & 0b1);
        col = t >> 1 & 0b1111;

        _t |= (uint64_t(S_Box[7-i][row][col]) << (i * 4));
        //cout << to_binary(S_Box[7-i][row][col]) << " " << to_binary(*dst) << endl;
        i++;
    }
    memcpy(dst, &_t,8);

};

/*
从低位到高位以此将dst填充
src: dst:  table:
*/
void Permute(uint64_t* src, uint64_t* dst, const uint8_t* table,int src_len ,int dst_len) {
    
    //cout << to_binary(*src)<<endl;
    uint64_t _t = 0;
    


    for (int i = 0; i <= dst_len; i++) {
        _t |= ((*src >> (src_len - (table[dst_len - i])) & 0b1) << i);
        /* 
        cout << "W " << i << endl;;
        cout << "table val: " << int(table[55 - i]) << "||" << to_binary(*src >> (64 - (table[table_size - i])) & 0b1)<<endl;
        cout << to_binary(*dst)<<endl<<"--------------------------------------" << endl;
        */
    }
    memcpy(dst, &_t, 8);


}

void Permute_inv(uint8_t* src, uint8_t* dst, uint8_t* table) {};

void Rotate(uint32_t* src){
    /*
    cout << to_binary(*src) << endl;
    uint8_t h_ = (*src >> 27) & 1;
    cout << to_binary(h_)<<endl;
    uint32_t _ = (*src & 0x07ffffff) << 1;
    cout << to_binary(_)<<endl;

    cout << to_binary(*src)<<endl;
    */
    
    *src = ((*src >> 27) & 1) | ((*src & 0x07ffffff) << 1);
   

}

void Key_exp(uint64_t* K, uint64_t* Ki) {
    uint32_t l;
    uint32_t r;
    uint64_t _K = *K;
    Permute(K, &_K, DesTransform, 64, 55);

    for (int i = 0;i < 16;i++) {
        l = (_K >> 28) & 0xfffffff;
        r = _K & 0xfffffff;
        Rotate(&l);
        Rotate(&r);

        if (DesRotations[i] == 2)
        {
            Rotate(&l);
            Rotate(&r);
        }

        _K = (uint64_t(l) << 28 | r);

        //  cout << to_binary(_K)<<endl;

        Permute(&_K, Ki + i, Despermuted,56 , 47);
       
    }



   // cout << to_binary(*Ki);
}

uint64_t Get_key(uint64_t* Ks, int i, bool En) {
    if (En)
        return Ks[i];
    return Ks[15 - i];

}

void Des_Encrypy(uint64_t p, uint64_t* dst, uint64_t*  Ki,bool En) {

    //初始置换  dst结果为64位
    uint64_t *plain = new uint64_t(p);
    Permute(plain, plain, DesInitial,64, 63);
   

    //分两个32位的组
    uint64_t L = (*plain >> 32) & 0xffffffff;
    uint64_t R = *plain & 0xffffffff;
    uint64_t R_ ;
    int i;
    for (i = 0; i < 15;i++) {
        R_ = R;
        Permute(&R, &R, DesExpansion,32 ,47);   //32 -> 48

        R = Get_key(Ki, i, En) ^ R;
        Subtitute(&R, &R);                      //48 -> 32
        Permute(&R, &R, P_Box,32 ,31);

        R = L ^ R;
        L = R_;

    }


    R_ = R;
    Permute(&R, &R, DesExpansion,32 ,47);
    //Key_exp(K, &Ki, i);
    R = Get_key(Ki, i, En) ^ R;
    Subtitute(&R, &R);
    Permute(&R, &R, P_Box,32, 31);
    L = L ^ R;

    *dst = L << 32 | R_;
    Permute(dst, dst, DesFinal, 64,63);

   // cout << "En'rs : " << to_binary(*dst) << endl;
}

void set_key(uint64_t K) {

    Key_exp(&K, SUBKEY);

    KEY = new uint64_t(K);


    cout << "SET KEY AS: 0x"<< to_hex(K);

    //for (int i = 0;i < 16;i++) 
      //  cout << "k_" << i << " " << to_hex(SUBKEY[i]) << endl;
    

}

void Encrypy(uint64_t plain, uint64_t* dst) {
    if (!KEY) {
        cout << "KEY NULL";
        return;
    }
    Des_Encrypy(plain, dst, SUBKEY, true);

}

void Decrypy(uint64_t cyber, uint64_t* dst) {
    if (!KEY) {
        cout << "KEY NULL";
        return;
    }
    Des_Encrypy(cyber, dst, SUBKEY, false);


}

string Decrypy(const string& plain) {
    // plain是8位对齐的
    
    string rs = "";
    uint64_t t = 0;
    vector<std::string> blocks = splitIntoBlocks(plain, block_size);
    for (const auto& block : blocks) {
        //依次解密
        Decrypy(block2ull(block), &t);
        int idx = 56;

        //64位转char* 
        for (int i = 0;i < 8;i++) {
            if ((t >> idx & 0xff) < 8)
                return rs;
            rs += char(t >> idx & 0xff);
            idx -= 8;
        }
    }
    return rs;
}

string Encrypy(const string& plain) {
    //set_key(0x02140316);

    string padded_string = padString(plain, block_size);
    string rs = "";
    uint64_t t = 0;
    // 将填充后的字符串切分为块
    vector<std::string> blocks = splitIntoBlocks(padded_string, block_size);
   
    for (const auto& block : blocks) {
        //每一次的加密结果在t里
        Encrypy(block2ull(block), &t);
        int idx = 56;

        //64 位 转为char*
        for (int i = 0;i < 8;i++) {
            rs += char(t >> idx & 0xff);
            idx -= 8;

        }
           
    }
    return rs;

}

// 使用 PKCS#7 填充算法填充字符串
std::string padString(const std::string& input, size_t block_size) {
    std::string result = input;
    size_t pad_len = (block_size - (input.length() & 0b111)) & 0b111;
    result.append(pad_len, static_cast<char>(pad_len));
    return result;
}

// 将字符串切分为块
std::vector<std::string> splitIntoBlocks(const std::string& input, size_t block_size) {
    std::vector<std::string> blocks;
    size_t num_blocks = input.length() / block_size;
    for (size_t i = 0; i < num_blocks; ++i) {
        blocks.push_back(input.substr(i * block_size, block_size));
    }
    return blocks;
}

void test_main() {

    uint64_t plain(0x4142434445464748);
    uint64_t* dst = new uint64_t(0);

    cout << "Plain: " << to_hex(plain)<< endl;

    uint64_t * K = new uint64_t(0x4142434445464748);
    cout << "K_-: " << to_binary(*K,64) << endl;



    bool En = 1;
    
   
    uint64_t  Ki[16];
    Key_exp(K, Ki);
    for (int i = 0;i < 16;i++) {
        
        cout <<"K_"<<i<<" " << to_hex(Ki[i]) << endl;
    }


    Des_Encrypy(plain, dst, Ki, En);
    cout << "结果: " << to_binary(*dst,64) << endl;


    uint64_t* ddst = new uint64_t(0);
    Des_Encrypy(*dst, ddst, Ki, !En);
    cout << "解密: " << to_binary(*ddst, 64) << endl;
   



}

uint64_t block2ull(const string& block) {
    uint64_t rs = 0;
    int idx = 56;
    for (char c : block){
        rs |= uint64_t(uint8_t(c)) << idx;   /////// 注意 先转uint8_t 在扩展64位   直接扩张导致例如 0xf1 ，最高位当作符号位，导致结果错误。
        idx -= 8;
    }
    return rs;
}

uint64_t generate_random_key() {
    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::uniform_int_distribution<uint64_t> distribution(0,0xffffffffffffffff);
    return distribution(rng);
}
/*
string to_hex(uint64_t n) {
    stringstream ss;
    ss << hex << n;
    return ss.str();
}

string to_binary(uint8_t i, int len = 8) {
    string ss;

    int id = len - 1;
    while (id >= 0) {
        ss += to_string(i >> (id--) & 0x1);
        if (!((id + 1) & 0b111))
            ss += " ";
    }
    return ss;

}

string to_binary(uint32_t i, int len = 32) {
    string ss;
    int id = len - 1;
    while (id >= 0) {
        ss += to_string(i >> (id--) & 0x1);
        if (!((id + 1) & 0b111))
            ss += " ";
    }
    return ss;

}

string to_binary(uint64_t i, int len = 64) {
    string ss;
    int id = len - 1;
    while (id >= 0) {
        ss += to_string(i >> (id--) & 0x1);
        if (!((id + 1) & 0b111))
            ss += " ";
    }
    return ss;

}

*/


