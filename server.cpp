#pragma comment(lib, "ws2_32.lib") //������� ���̺귯���� ��ũ. ���� ���̺귯�� ����

#include <WinSock2.h>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <mysql/jdbc.h>

#define MAX_SIZE 50
#define MAX_CLIENT 3

using std::cout;
using std::cin;
using std::endl;
using std::string;

const string server = "tcp://127.0.0.1:3306"; // �����ͺ��̽� �ּ�
const string username = "root"; // �����ͺ��̽� �����
const string password = "1234"; // �����ͺ��̽� ���� ��й�ȣ

struct SOCKET_INFO { // ����� ���� ������ ���� Ʋ ����
    SOCKET sck;
    string user, name, id, pw, birth;
};

std::vector<SOCKET_INFO> sck_list; // ����� Ŭ���̾�Ʈ ���ϵ��� ������ �迭 ����.
SOCKET_INFO server_sock; // ���� ���Ͽ� ���� ������ ������ ���� ����.
int client_count = 0; // ���� ������ �ִ� Ŭ���̾�Ʈ�� count �� ���� ����.

void server_init(); // socket �ʱ�ȭ �Լ�. socket(), bind(), listen() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void add_client(); // ���Ͽ� ������ �õ��ϴ� client�� �߰�(accept)�ϴ� �Լ�. client accept() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void send_msg(const char* msg); // send() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void recv_msg(int idx); // recv() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void del_client(int idx); // ���Ͽ� ����Ǿ� �ִ� client�� �����ϴ� �Լ�. closesocket() �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.

// MySQL Connector/C++ �ʱ�ȭ
sql::mysql::MySQL_Driver* driver; // ���� �������� �ʾƵ� Connector/C++�� �ڵ����� ������ ��
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;
void start_sql(); //SQL���� - creatTable;

int main() {
    start_sql();
    WSADATA wsa;

    // Winsock�� �ʱ�ȭ�ϴ� �Լ�. MAKEWORD(2, 2)�� Winsock�� 2.2 ������ ����ϰڴٴ� �ǹ�.
    // ���࿡ �����ϸ� 0��, �����ϸ� �� �̿��� ���� ��ȯ.
    // 0�� ��ȯ�ߴٴ� ���� Winsock�� ����� �غ� �Ǿ��ٴ� �ǹ�.
    int code = WSAStartup(MAKEWORD(2, 2), &wsa);

    if (!code) {
        server_init();

        std::thread th1[MAX_CLIENT];
        for (int i = 0; i < MAX_CLIENT; i++) {
            // �ο� �� ��ŭ thread �����ؼ� ������ Ŭ���̾�Ʈ�� ���ÿ� ������ �� �ֵ��� ��.
            th1[i] = std::thread(add_client);
        }
        //std::thread th1(add_client); // �̷��� �ϸ� �ϳ��� client�� �޾���...

        while (1) { // ���� �ݺ����� ����Ͽ� ������ ����ؼ� ä�� ���� �� �ִ� ���¸� ����� ��. �ݺ����� ������� ������ �� ���� ���� �� ����.
            string text, msg = "";

            std::getline(cin, text);
            const char* buf = text.c_str();
            msg = server_sock.name + " : " + buf;
            send_msg(msg.c_str());
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
    con->setSchema("chating");

    // db �ѱ� ������ ���� ���� 
    stmt = con->createStatement();
    stmt->execute("set names euckr");
    if (stmt) { delete stmt; stmt = nullptr; }

    // �����ͺ��̽� ���� ����
    //stmt = con->createStatement();
    //stmt->execute("DROP TABLE IF EXISTS userInfo"); // DROP
    //cout << "Finished dropping table (if existed)" << endl;
    //stmt->execute("CREATE TABLE userInfo (id VARCHAR(8) PRIMARY KEY, name VARCHAR(10), pw VARCHAR(50), birthday date);");


    //cout << "Finished creating table" << endl;
    //delete stmt;

    //pstmt = con->prepareStatement("INSERT INTO userInfo(id_num, name, id, pw, birthday) VALUES(?,?)"); // INSERT

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
    server_sock.name = "server";

    cout << "Server On" << endl;
}

void add_client() {
    SOCKADDR_IN addr = {};
    int addrsize = sizeof(addr);
    char buf[MAX_SIZE] = { };

    ZeroMemory(&addr, addrsize); // addr�� �޸� ������ 0���� �ʱ�ȭ

    SOCKET_INFO new_client = {};
    new_client.sck = accept(server_sock.sck, (sockaddr*)&addr, &addrsize);

    if (new_client.sck != INVALID_SOCKET) {
        char buf[MAX_SIZE];
        ZeroMemory(&buf, MAX_SIZE);
        int bytesReceived;

        bytesReceived = recv(new_client.sck, buf, 10, 0);
        if (bytesReceived > 0) {
            cout << "buf name : " << buf << endl;
            new_client.name = std::string(buf, bytesReceived);
            ZeroMemory(&buf, 10);
        }

        bytesReceived = recv(new_client.sck, buf, 8, 0);
        if (bytesReceived > 0) {
            cout << "buf id : " << buf << endl;
            new_client.id = std::string(buf, bytesReceived);
            ZeroMemory(&buf, 8);
        }

        bytesReceived = recv(new_client.sck, buf, 50, 0);
        if (bytesReceived > 0) {
            cout << "buf pw : " << buf << endl;
            new_client.pw = std::string(buf, bytesReceived);
            ZeroMemory(&buf, 50);
        }

        bytesReceived = recv(new_client.sck, buf, MAX_SIZE, 0);
        if (bytesReceived > 0) {
            cout << "buf birth : " << buf << endl;
            new_client.birth = std::string(buf, bytesReceived);
            ZeroMemory(&buf, MAX_SIZE);
        }
    }

    string msg = "[����] " + new_client.name + " ���� �����߽��ϴ�.";
    cout << msg << endl;
    sck_list.push_back(new_client); // client ������ ��� sck_list �迭�� ���ο� client �߰�

    std::thread th(recv_msg, client_count);
    // �ٸ� �����κ��� ���� �޽����� ����ؼ� ���� �� �ִ� ���·� ����� �α�.

    client_count++; // client �� ����.
    cout << "[����] ���� ������ �� : " << client_count << "��" << endl;
    send_msg(msg.c_str()); // c_str : string Ÿ���� const chqr* Ÿ������ �ٲ���.
    pstmt = con->prepareStatement("INSERT INTO userinfo(name, id, pw, birthday) VALUES(?,?,?,?)");

    pstmt->setString(1, new_client.name);
    pstmt->setString(2, new_client.id);
    pstmt->setString(3, new_client.pw);
    pstmt->setString(4, new_client.birth);

    try {
        pstmt->execute(); // SQL ���� ����
        cout << "User " << new_client.name << " added to the database." << endl;
    }
    catch (sql::SQLException& e) {
        cout << "SQL Error: " << e.what() << endl;
    }

    // MySQL Connector/C++ ����
    delete pstmt;
    delete con;
    th.join();
}

void send_msg(const char* msg) {
    for (int i = 0; i < client_count; i++) { // ������ �ִ� ��� client���� �޽��� ����
        send(sck_list[i].sck, msg, MAX_SIZE, 0);
    }
}

void recv_msg(int idx) {
    char buf[MAX_SIZE] = { };
    string msg = "";

    //cout << sck_list[idx].user << endl;

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(sck_list[idx].sck, buf, MAX_SIZE, 0) > 0) { // ������ �߻����� ������ recv�� ���ŵ� ����Ʈ ���� ��ȯ. 0���� ũ�ٴ� ���� �޽����� �Դٴ� ��.
            msg = sck_list[idx].name + " : " + buf;
            cout << msg << endl;
            send_msg(msg.c_str());
        }
        else { //�׷��� ���� ��� ���忡 ���� ��ȣ�� �����Ͽ� ���� �޽��� ����
            msg = "[����] " + sck_list[idx].name + " ���� �����߽��ϴ�.";
            cout << msg << endl;
            send_msg(msg.c_str());
            del_client(idx); // Ŭ���̾�Ʈ ����
            return;
        }
    }
}

void del_client(int idx) {
    closesocket(sck_list[idx].sck);
    //sck_list.erase(sck_list.begin() + idx); // �迭���� Ŭ���̾�Ʈ�� �����ϰ� �� ��� index�� �޶����鼭 ��Ÿ�� ���� �߻�....��
    client_count--;
}