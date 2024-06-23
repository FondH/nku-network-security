#include "chat.h";
using namespace std;

std::string pad(25, ' ');

mess::mess(std::vector<std::string> vec) {
    type = stoi(vec[0]);
    username = vec[1];
    message = vec[2];
    time = stoll(vec[0]);
}

mess::mess(int t, const std::string u, const std::string m, time_t ti) {
    type = t;
    username = u;
    message = m;
    time = ti;
}

std::string mess::toString() {

    if (!type)
        return "\u001b[34;1m"+username + "\u001b[0m: " + message + "\n";

    else if (type==1) {
        std::string pad(58 - username.size() - message.size(), ' ');
        return  pad + message + " :\u001b[35;0m" + username + "\u001b[0m \n";
    }

    else {
        std::string pad(25, ' ');
        return pad + "exit" + pad;
    }

    //return username + ": " + message+ " " + time2s(time) + "\n";
}

void DrawInterface() {

    saveCursorPosition();
    cout << "\u001b[500A";    //将光标input上一行 清除聊天内容
    clearLines(25);
    cout << "\u001b[500A";

    // 绘制界面
    cout << "\u001b[36m+" << setw(60) << setfill('-') << "+\n";
    cout << "\u001b[36m\u001b[1m\u001b[4m"<<" >> ChatRoom" << "\u001b[0m Time : " << time2s(getCurrentTime()) << endl;
    cout << "\u001b[36m+" << setw(60) << setfill('-') << "+\n" << "\u001b[0m";

    //输出message
    int i = 0;
    for (auto it = messBuffer.begin(); it != messBuffer.end(); ++it, ++i) {
        cout << (*it).toString();
        if (i > MAX_MESS_NUM)
            break;
    }
    cout << "\u001b[36m+" << setw(60) << setfill('-') << "+\n" << "\u001b[0m";

    restoreCursorPosition();// 恢复光标位置
}
void ThreadSend(string a, bool user=1) {

    string tpstring = "";

    if (!user)
        tpstring += Encrypy(a);
    else 
        tpstring += Encrypy("1:" + username + ":" + a + ":" + "1");

    send(remote_sock, tpstring.c_str(), 255, 0);
}

void ThreadView(int id) {
    cout << "\u001b[50B" << "\u001b[50D";
    cout <<"\u001b[1m\u001b[36m"<< "INPUT:" << "\u001b[0m\n";

    while (Flag_View)
    {
        DrawInterface();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    cout << "View Exit" << endl;
}

void ThreadInput(int id) {
    string a;

    while (FLag_Input) {
        cin >> a;
        messBuffer.push_back(mess(0, username, a, 1));
        ThreadSend(a);
        cout << "\u001b[1A\033[2K";
        if (a == "exit") {
            ThreadSend("2:op:exit:0", 0);

            Sleep(1000);
            FLag_Input = false;
            Flag_View = false;
            FLag_Recv = false;
  
        }
    }

    cout << "input exit\n";
}

void appendMess(std::string s) {
    s = Decrypy(s);
    mess m(split(s, ':'));

    if (m.type == 2) {
        FLag_Input = false;
        Flag_View = false;
        FLag_Recv = false;
    }

    messBuffer.push_back(m);
}



uint64_t ThreadReciDesKey() {
    //buf格式 DES:64 bit
    //接受DES 密钥

    uint64_t tp = 0;
    auto st = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - st);

    cout << "Accepting DES key ..."<<endl;

    while (1) {

        char buf[255];
        int len = recv(remote_sock, buf, 255, 0);

        if(len && buf[0]=='D' && buf[1]=='E' && buf[2]=='S'){
            uint64_t* tmp = (uint64_t*)(buf + 4);
            
            tp = RsaDecrypt64(tmp);
            break;
        }
            
        end = chrono::steady_clock::now();
        duration = chrono::duration_cast<chrono::milliseconds>(end - st);
        if(duration.count() > 5000){
            cout << "Timeout" << endl;
            break;
        }

    }

    if (tp) {

        cout << "Execution time: " << duration.count() << " ms" << std::endl;
        cout<< "DES Key: " << hex << tp <<"\n";
        return tp;
    }
    else{

        cout<<"Accept DES key failed"<<endl;
        return 0;
    }
}

void ThreadSendDesKey() {
    uint64_t rs[4]{ 0 };
    uint64_t des_key = 0x2140316;
    RsaEncrypt64(des_key, rs);

    desTuple desKeyTuple = desTuple(rs);
    send(remote_sock, (char*)&desKeyTuple, 255, 0);
    set_key(des_key);
    cout << "Send DES key: " << hex << des_key << endl;
}

void RSA_Test() {
    //RSA 单元测试
    RsaInit();
    outState();
    cout << "---------------- RSA TEST -----------------" << endl;

    int input = 0x00001314;
    cout << "\u001b[31m\u001b[1minput:" << hex << input << endl << "\u001b[0m";

    cout << "\u001b[32m\u001b[1mRSA Encrypt Result:" << hex << RsaEncrypt(input) << endl << "\u001b[0m";

    cout << "\u001b[31m\u001b[1mRSA Decrypt Result:" << hex << RsaDecrypt(RsaEncrypt(input)) << "\u001b[0m";
}
void RSA_slice_test(){
    RsaInit();
    outState();
    cout << "---------------- RSA Slice TEST -----------------" << endl;

    uint64_t DES_key = generate_random_key();
    cout << "\u001b[31m\u001b[1minput: " << hex << DES_key << endl << "\u001b[0m";
    cout << endl;


    uint64_t * rs = new uint64_t[4];
    RsaEncrypt64(DES_key, rs);
    desTuple d(rs);
    
    //cout << "desTuple.name[2] addr: " << &(d.name) +2 << endl;
    //cout << "desTuple.k[0] addr: " << &d.k << endl;

    cout << "\u001b[32m\u001b[1mRSA Encrypt64 Result:" << "\u001b[0m"<<endl;
    for(int i=0;i<4;++i)
        cout << "   Slice[" << i + 1 << "]" << hex << rs[i] << endl;
    cout << endl;

    uint64_t* tmp = (uint64_t*)(rs);
    cout << "\u001b[31m\u001b[1mRSA Decrypt64 Result: " << hex << RsaDecrypt64(tmp) << "\u001b[0m";

}
using std::this_thread::sleep_for;
int main() {

    RsaInit();
    
    outState();
    
    StartServ(0);

    // 发送私钥 (d, N) 
    rsaTuple rsaPrivateKey = getKeyStruct();
    send(remote_sock, (char *) &rsaPrivateKey, 255, 0);

    // 接受 Des key
    uint64_t desKey = ThreadReciDesKey();

    set_key(desKey);
    std::cout << "N: " << std::setw(16) << std::setfill('0') << std::hex << desKey << std::endl;

    cout << "prepare chat room....";
    sleep_for(std::chrono::milliseconds(1000));

    system("cls");
    thread  t0 = std::thread(RecvFromRemote, 0);
    thread  t1 = std::thread(ThreadView, 1);
    thread  t2 = std::thread(ThreadInput, 2);

    t0.join();
    t1.join();
    t2.join();

    WSACleanup();
    cout << "Exit\n";
        
    

    /*
    uint64_t plain(0x223ffeffaabbcc21);
    uint64_t* dst = new uint64_t(0);

    cout << "\u001b[31m\u001b[1m" << "Plain: " << to_binary(plain, 64) << "\u001b[0m" << endl;

    uint64_t* K = new uint64_t(0x4142434445464748);
    cout << "K_-: " << to_binary(*K, 64) << endl;

    bool En = 1;

    uint64_t  Ki[16];
    Key_exp(K, Ki);
    for (int i = 0;i < 16;i++) 
        cout << "K_" << i << " " << to_hex(Ki[i]) << endl;

    Des_Encrypy(plain, dst, Ki, En);
    cout << "\u001b[32m\u001b[1m"<<"加密: " << to_binary(*dst, 64) << "\u001b[0m" << endl;

    uint64_t* ddst = new uint64_t(0);
    Des_Encrypy(*dst, ddst, Ki, !En);
    cout << "\u001b[31m\u001b[1m"<<"解密: " << to_binary(*ddst, 64)<<"\u001b[0m" << endl;*/

    /*


    thread  t1 = std::thread(ThreadView, 1);
    
    string a;
    while (true) {

        cin >> a;
        messBuffer.push_back(mess(0, username, a, 1));
        cout << "\u001b[1A\033[2K";
        if (a == "exit") {
            Flag_View = false;
            break;
        }

    }
    cout << "input exit\n";
    t1.join();
    

    */
    /*
  set_key(0x02140316);
  extern size_t block_size;
  std::string plain_string = "这是一个测试pad结果和加解密是否正确的样例";

  cout << "\u001b[32mPlain_string: " << plain_string<<"\u001b[0m\n" << endl;

  string padded_string = padString(plain_string, block_size);

  cout << "\u001b[35mPadding_string: " << plain_string << "\u001b[0m\n" << endl;

  string r = Encrypy(padded_string);
 
  cout << "\u001b[36mEncrypy_string: " << r << "\u001b[0m\n" << endl;

  string r_ = Decrypy(r);
  cout << "\u001b[32mDecrypt_string: " << r_ << "\u001b[0m\n" << endl;

 
  */
  


    return 0;
}

