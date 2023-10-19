#pragma comment(lib, "ws2_32.lib") //������� ���̺귯���� ��ũ. ���� ���̺귯�� ����

#include <WinSock2.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <vector>
#include <mysql/jdbc.h>
#include <ctime>
#include <time.h>
#define MAX_SIZE 1024
#define MAX_CLIENT 10

using std::cout;
using std::cin;
using std::endl;
using std::string;


sql::mysql::MySQL_Driver* driver; // ���� �������� �ʾƵ� Connector/C++�� �ڵ����� ������ ��
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

const string server = "tcp://127.0.0.1:3306"; // �����ͺ��̽� �ּ�
const string username = "root"; // �����ͺ��̽� �����
const string password = "goflvhxj"; // �����ͺ��̽� ���� ��й�ȣ
string nickname = "";
string msg = "";


struct SOCKET_INFO { // ����� ���� ������ ���� Ʋ ����
    SOCKET sck;
    string user;
    bool hasSentEnterMsg; // ���� �޽����� ���´��� ����

};

std::vector<SOCKET_INFO> sck_list; // ����� Ŭ���̾�Ʈ ���ϵ��� ������ �迭 ����.
SOCKET_INFO server_sock; // ���� ���Ͽ� ���� ������ ������ ���� ����.
int client_count = 0; // ���� ������ �ִ� Ŭ���̾�Ʈ�� count �� ���� ����.

void server_init(); // socket �ʱ�ȭ �Լ�. socket(), bind(), listen() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void add_client(); // ���Ͽ� ������ �õ��ϴ� client�� �߰�(accept)�ϴ� �Լ�. client accept() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void send_msg(const char* msg, string sender); // send() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void recv_msg(int idx); // recv() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void del_client(int idx); // ���Ͽ� ����Ǿ� �ִ� client�� �����ϴ� �Լ�. closesocket() �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void send_private_msg(const char* msg, string sender);
std::vector<string>chattingroom_member;

//start sql
void start_sql()
{
    // MySQL Connector/C++ �ʱ�ȭ
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(server, username, password);
    }
    catch (sql::SQLException& e) {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        exit(1);
    }

    // �����ͺ��̽� ����
    con->setSchema("chatting");

    // db �ѱ� ������ ���� ���� 
    stmt = con->createStatement();
    stmt->execute("set names euckr");
    if (stmt) { delete stmt; stmt = nullptr; }
}

int main() {
    WSADATA wsa;
    start_sql();
    // Winsock�� �ʱ�ȭ�ϴ� �Լ�. MAKEWORD(2, 2)�� Winsock�� 2.2 ������ ����ϰڴٴ� �ǹ�.
    // ���࿡ �����ϸ� 0��, �����ϸ� �� �̿��� ���� ��ȯ.
    // 0�� ��ȯ�ߴٴ� ���� Winsock�� ����� �غ� �Ǿ��ٴ� �ǹ�.
    int code = WSAStartup(MAKEWORD(2, 2), &wsa);

    if (!code) {
        server_init();

        std::thread th1[MAX_CLIENT];
        char buf[MAX_SIZE] = { };

        for (int i = 0; i < MAX_CLIENT; i++) {
            // �ο� �� ��ŭ thread �����ؼ� ������ Ŭ���̾�Ʈ�� ���ÿ� ������ �� �ֵ��� ��.
            th1[i] = std::thread(add_client);

            //std::thread th1(add_client); // �̷��� �ϸ� �ϳ��� client�� �޾���...
        }
        while (1) { // ���� �ݺ����� ����Ͽ� ������ ����ؼ� ä�� ���� �� �ִ� ���¸� ����� ��. �ݺ����� ������� ������ �� ���� ���� �� ����.
            string text, msg = "";

            std::getline(cin, text);
            const char* buf = text.c_str();
            msg = server_sock.user + " : " + buf;
            send_msg(msg.c_str(),server_sock.user);
        }


        for (int i = 0; i < MAX_CLIENT; i++) {
            th1[i].join();
            //join : �ش��ϴ� thread ���� ������ �����ϸ� �����ϴ� �Լ�.
            //join �Լ��� ������ main �Լ��� ���� ����Ǿ thread�� �Ҹ��ϰ� ��.
            //thread �۾��� ���� ������ main �Լ��� ������ �ʵ��� ����.
        }
        //th1.join();

        closesocket(server_sock.sck);
    }
    else {
        cout << "���α׷� ����. (Error code : " << code << ")";
    }

    WSACleanup();

    return 0;
}

void server_init() {
    server_sock.sck = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Internet�� Stream ������� ���� ����
   // SOCKET_INFO�� ���� ��ü�� socket �Լ� ��ȯ��(��ũ���� ����)
   // ���ͳ� �ּ�ü��, ��������, TCP �������� �� ��. 

    SOCKADDR_IN server_addr = {}; // ���� �ּ� ���� ����
    // ���ͳ� ���� �ּ�ü�� server_addr

    server_addr.sin_family = AF_INET; // ������ Internet Ÿ�� 
    server_addr.sin_port = htons(7777); // ���� ��Ʈ ����
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // �����̱� ������ local �����Ѵ�. 
    //Any�� ���� ȣ��Ʈ�� 127.0.0.1�� ��Ƶ� �ǰ� localhost�� ��Ƶ� �ǰ� ���� �� ����ϰ� �� �� �ֵ�. �װ��� INADDR_ANY�̴�.
    //ip �ּҸ� ������ �� server_addr.sin_addr.s_addr -- ������ ���?

    bind(server_sock.sck, (sockaddr*)&server_addr, sizeof(server_addr)); // ������ ���� ������ ���Ͽ� ���ε��Ѵ�.
    listen(server_sock.sck, SOMAXCONN); // ������ ��� ���·� ��ٸ���.
    server_sock.user = "server";

    cout << "Server On" << endl;
}

void add_client() {
    msg = " ";
    SOCKADDR_IN addr = {};
    int addrsize = sizeof(addr);
    char buf[MAX_SIZE] = { };

    ZeroMemory(&addr, addrsize); // addr�� �޸� ������ 0���� �ʱ�ȭ

    SOCKET_INFO new_client = {};

    new_client.sck = accept(server_sock.sck, (sockaddr*)&addr, &addrsize);
    // Winsock2�� recv �Լ�. client�� ���� �г����� ����.
   
    recv(new_client.sck, buf, MAX_SIZE, 0);
    msg = buf;

    if (msg.find("*") != string::npos) {
        msg.resize(msg.size() - 1);
        chattingroom_member.push_back(msg);
    }

    new_client.user = msg;
    if (!new_client.hasSentEnterMsg) {
        string msg = "[����] " + new_client.user + " ���� �����߽��ϴ�.";
        cout << msg << endl;
        send_msg(msg.c_str(), NULL);
        new_client.hasSentEnterMsg = true;
    }
    sck_list.push_back(new_client);
    std::thread th(recv_msg, client_count);
    // �ٸ� �����κ��� ���� �޽����� ����ؼ� ���� �� �ִ� ���·� ����� �α�.

    client_count++; // client �� ����.
    cout << "[����] ���� ������ �� : " << client_count << "��" << endl;
    send_msg(msg.c_str(),new_client.user); // c_str : string Ÿ���� const chqr* Ÿ������ �ٲ���.

    th.join();
}
void send_msg(const char* msg, string sender) {
    string text = msg;
    int count = 0;
    for (int i = 0; i < client_count; i++) { // sender ������ ��� client���� �޽��� ����
        if (sck_list[i].user != sender) {
            for (int j = 0; j < chattingroom_member.size(); j++) {
                if (sck_list[i].user == chattingroom_member[j])
                    count++;
            }
            if (count == 0) {
                send(sck_list[i].sck, text.c_str(), MAX_SIZE, 0);
            }
        }
    }
}
void send_private_msg(const char* msg, string sender) {
    string text = msg;
    for (int i = 0; i < client_count; i++) { // sender���Ը� client���� �޽��� ����
        if (sck_list[i].user == sender) {
          
            send(sck_list[i].sck, text.c_str(), MAX_SIZE, 0);
        }
    }
}
void send_game1_msg(const char* msg, string sender,std::vector<string>chattingroom_member) {
    string text = msg;
    for (int i = 0; i < client_count; i++) { // sender���Ը� client���� �޽��� ����
        for (int j = 0; j < chattingroom_member.size(); j++) {
            if (sck_list[i].user == chattingroom_member[j] && sck_list[i].user != sender) {
                send(sck_list[i].sck, text.c_str(), MAX_SIZE, 0);
            }
        }
    }
}
void recv_msg(int idx) {
    char buf[MAX_SIZE] = { };
    while (1) {
        //Sleep(100);
        int num = 0;
        int i = 0;
        msg = "";
        struct tm timeinfo;
        __time64_t temp;
        string time;
        _time64(&temp); // �ý��� �ð��� ����ϴ�.

        ZeroMemory(&buf, MAX_SIZE);
        if (recv(sck_list[idx].sck, buf, MAX_SIZE, 0) > 0) { // ������ �߻����� ������ recv�� ���ŵ� ����Ʈ ���� ��ȯ. 0���� ũ�ٴ� ���� �޽����� �Դٴ� ��.
            nickname = sck_list[idx].user;
            msg = buf;
            std::stringstream buf2(msg.c_str());
            string first, id, p_msg;
            buf2 >> first >> id;
            for (int i = 0; i < client_count; i++) {
                if (id == sck_list[i].user && first == "/w") {
                    num = 1;
                    p_msg = msg.erase(0, 11);
                    if (_localtime64_s(&timeinfo, &temp) == 0) {
                        // ���� �ð� ������ ���������� ������ϴ�.
                        char buffer[26]; // ���ڿ� ���۸� �����մϴ�.
                        if (asctime_s(buffer, sizeof(buffer), &timeinfo) == 0) {
                            std::string timeString(buffer); // asctime�κ��� ���� ���ڿ�
                            time = timeString;
                        }
                    }
                    pstmt = con->prepareStatement("INSERT INTO private_msg(sender, receiver ,message, time) VALUES(?,?,?,?)");
                    pstmt->setString(1, nickname);
                    pstmt->setString(2, id);
                    pstmt->setString(3, p_msg);
                    pstmt->setString(4, time);

                    pstmt->execute();
                    msg = "�ӼӸ��� �����߽��ϴ�! [" + nickname + "]" + " : " + p_msg;
                    send_private_msg(msg.c_str(), sck_list[i].user);
                }
            }
            if (msg.find("*") != string::npos && num == 0) {
                msg.resize(msg.size() - 1);
                msg = nickname + " : " + msg;
                cout << msg << endl;
                send_game1_msg(msg.c_str(), nickname, chattingroom_member);
            }
            else if (num == 0) {
                if (_localtime64_s(&timeinfo, &temp) == 0) {
                    // ���� �ð� ������ ���������� ������ϴ�.
                    char buffer[26]; // ���ڿ� ���۸� �����մϴ�.
                    if (asctime_s(buffer, sizeof(buffer), &timeinfo) == 0) {
                        std::string timeString(buffer); // asctime�κ��� ���� ���ڿ�
                        time = timeString;
                    }
                }
                cout << nickname + " : " + msg << endl;

                pstmt = con->prepareStatement("INSERT INTO chat(nickname,message, time) VALUES(?,?,?)");

                pstmt->setString(1, nickname);
                pstmt->setString(2, msg);
                pstmt->setString(3, time);
                pstmt->execute();
                msg = nickname + " : " + buf;
                send_msg(msg.c_str(), sck_list[idx].user);
            }
        }
            else { //�׷��� ���� ��� ���忡 ���� ��ȣ�� �����Ͽ� ���� �޽��� ����
                msg = "[����] " + sck_list[idx].user + " ���� �����߽��ϴ�.";
                cout << msg << endl;
                send_msg(msg.c_str(), "");
                del_client(idx); // Ŭ���̾�Ʈ ����
                return;
            }
        }
}
void del_client(int idx) {
    closesocket(sck_list[idx].sck);
    sck_list.erase(sck_list.begin() + idx); // �迭���� Ŭ���̾�Ʈ�� �����ϰ� �� ��� index�� �޶����鼭 ��Ÿ�� ���� �߻�....��
    client_count--;
}