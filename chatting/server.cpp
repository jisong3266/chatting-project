#pragma comment(lib, "ws2_32.lib") //������� ���̺귯���� ��ũ. ���� ���̺귯�� ����

#include <WinSock2.h> // Winsock ������� include. WSADATA �������.
#include <string>
#include <sstream>
#include <iostream>
#include <thread> // ������ ���
#include <vector>
#include <mysql/jdbc.h> // SQL ���
#include <ctime> // �ð� ������ ���� ���
#include <time.h> // ���� �ð� ǥ�ø� ���� ���

#define MAX_SIZE 1024
#define MAX_CLIENT 10

using std::cout;
using std::cin;
using std::endl;
using std::string; // namespace ����


sql::mysql::MySQL_Driver* driver; // ���� �������� �ʾƵ� Connector/C++�� �ڵ����� ������ ��
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result; // SQL ����

const string server = "tcp://127.0.0.1:3306"; // �����ͺ��̽� �ּ�
const string username = "root"; // �����ͺ��̽� �����
const string password = "1234"; // �����ͺ��̽� ���� ��й�ȣ

struct SOCKET_INFO { // ����� ���� ������ ���� Ʋ ����
    SOCKET sck;
    string user;
    int count = 0;
    bool hasSentEnterMsg; // ���� �޽����� ���´��� ����
    int ti;
};


std::vector<SOCKET_INFO> sck_list; // ����� Ŭ���̾�Ʈ ���ϵ��� ������ �迭 ����.
SOCKET_INFO server_sock; // ���� ���Ͽ� ���� ������ ������ ���� ����.
int client_count = 0; // ���� ������ �ִ� Ŭ���̾�Ʈ�� count �� ���� ����.
void server_init(); // socket �ʱ�ȭ �Լ�. socket(), bind(), listen() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void add_client(int a); // ���Ͽ� ������ �õ��ϴ� client�� �߰�(accept)�ϴ� �Լ�. client accept() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void send_msg(const char* msg, string sender); // send() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void recv_msg(string user_name); // recv() �Լ� �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void del_client(int idx); // ���Ͽ� ����Ǿ� �ִ� client�� �����ϴ� �Լ�. closesocket() �����. �ڼ��� ������ �Լ� �����ο��� Ȯ��.
void send_private_msg(const char* msg, string sender);
void send_adult_msg(const char* msg, string sender);
void send_msg_all(const char* msg, string sender);
void del_client(int idx);
int removeSocket(string user);
SOCKET_INFO get_socket_info(string username);

std::vector<string>chattingroom_member;
std::thread th1[MAX_CLIENT];
int recreate = -1;
int game_score;
string game_id;
string nickname = "";
string msg = "";
int t;
int num = 0; // �������� ����

SOCKET_INFO get_socket_info(string user) {
    for (int i = 0; i < sck_list.size(); i++) { // ������ �ִ� ��� client���� �޽��� ����
        if (sck_list[i].user == user) {
            return sck_list[i];
        }
    }
}

void recreate_thread() {
    while (1) {
        if (recreate > -1) {
            th1[recreate].join();
            th1[recreate] = std::thread(add_client, recreate);
            recreate = -1;
        }
        if (recreate == -2) {
            return;
        }
        Sleep(1000);
    }
}
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

void add_client(int ti) {
    bool flag = true;
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
    new_client.ti = ti;

    if (msg.find("*") != string::npos) {
        msg.resize(msg.size() - 1);
        chattingroom_member.push_back(msg);
        new_client.user = msg;
        string msg = "[����] " + new_client.user + " ���� ����ä�ù濡 �����߽��ϴ�.";
        cout << msg << endl;
        send_adult_msg(msg.c_str(), new_client.user);
        flag = false;
    }
    if (msg.find("#") != string::npos) {
        msg.resize(msg.size() - 1);
        new_client.user = msg;
        string msg = "[����] " + new_client.user + " ���� ���ӹ濡 �����߽��ϴ�.";
        cout << msg << endl;
        flag = false;
    }

    new_client.user = msg;
    if (!new_client.hasSentEnterMsg && flag) {
        string msg = "[����] " + new_client.user + " ���� ��üä�ù濡 �����߽��ϴ�.";
        cout << msg << endl;
        send_msg(msg.c_str(), new_client.user);
        new_client.hasSentEnterMsg = true;
    }

    sck_list.push_back(new_client);


    std::thread th(recv_msg, new_client.user);
    // �ٸ� �����κ��� ���� �޽����� ����ؼ� ���� �� �ִ� ���·� ����� �α�.

    client_count++; // client �� ����.

    cout << "[����] ���� ������ �� : " << sck_list.size() << "��" << endl;
    //send_msg(msg.c_str(), new_client.user); // c_str : string Ÿ���� const chqr* Ÿ������ �ٲ���.

    th.join();
}
void send_msg_all(const char* msg, string sender) {
    string text = msg;
    int count = 0;
    for (int i = 0; i < sck_list.size(); i++) {
        send(sck_list[i].sck, text.c_str(), MAX_SIZE, 0);
    }
}
void send_msg(const char* msg, string sender) {
    string text = msg;
    int count = 0;
    for (int i = 0; i < sck_list.size(); i++) { // sender ������ ��� client���� �޽��� ����
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
    for (int i = 0; i < sck_list.size(); i++) { // sender���Ը� client���� �޽��� ����
        if (sck_list[i].user == sender) {
            send(sck_list[i].sck, text.c_str(), MAX_SIZE, 0);
        }
    }
}
void send_adult_msg(const char* msg, string sender) {
    string text = msg;
    for (int i = 0; i < sck_list.size(); i++) { // sender���Ը� client���� �޽��� ����
        for (int j = 0; j < chattingroom_member.size(); j++) {
            if (sck_list[i].user == chattingroom_member[j] && sck_list[i].user != sender) {
                send(sck_list[i].sck, text.c_str(), MAX_SIZE, 0);
            }
        }
    }
}
void countdown() {
    while (t > 0) {
        t--;
        Sleep(1000);
    }
}
void if_badwords(int ti, const char* msg) {
    t = 180;
    char box[MAX_SIZE] = { };
    string text = msg;
    sck_list[ti].count++;
    cout << "�弳 ����!" << endl;
    text = "[���] " + nickname + "���� �弳�� ����ϼ̽��ϴ�! ���� ��� Ƚ�� : " + std::to_string(sck_list[ti].count);
    send_msg(text.c_str(), "");
    if (sck_list[ti].count == 3) {
        sck_list[ti].count = 0;
        text = "[����] " + nickname + "���� ��������� 3�а� ��ȭ�Ͻ� �� �����ϴ�!";
        send_msg(text.c_str(), "");
        std::thread th3(countdown);
        while (t > 0) {
            int min = t / 60;
            int sec = t % 60;
            int bytes_received = recv(sck_list[ti].sck, box, MAX_SIZE, 0);
            if (bytes_received > 0) { // ������ �߻����� ������ recv�� ���ŵ� ����Ʈ ���� ��ȯ. 0���� ũ�ٴ� ���� �޽����� �Դٴ� ��.
                text = "[����] ��ȭ�� �����Ͻ� �� �����ϴ�(���� �ð� : " + std::to_string(min) + "��" + std::to_string(sec) + "��)";
                send_private_msg(text.c_str(), sck_list[ti].user);
            }
        }
        th3.join();
        num = 1;
    }
    else
        return;
}
void recv_msg(string user) {
    char buf[MAX_SIZE] = { };
    SOCKET_INFO sck_info = get_socket_info(user);
    while (1) {
        num = 0;
        //Sleep(100);
        int i = 0;
        msg = "";
        struct tm timeinfo;
        __time64_t temp;
        string time;
        _time64(&temp); // �ý��� �ð��� ����ϴ�.
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(sck_info.sck, buf, MAX_SIZE, 0) > 0) { // ������ �߻����� ������ recv�� ���ŵ� ����Ʈ ���� ��ȯ. 0���� ũ�ٴ� ���� �޽����� �Դٴ� ��.
            nickname = user;
            msg = buf;
            std::stringstream buf2(msg.c_str());
            string first, id, p_msg;
            buf2 >> first >> id;
            stmt = con->createStatement();
            result = stmt->executeQuery("SELECT id, score FROM game_rank order by score desc limit 1");
            if (result->next()) {
                game_id = result->getString(1);
                game_score = result->getInt(2);
            }
            else {
                game_id = "";
                game_score = 0;
            }
            if (first == user) {
                if (first == game_id || game_id == "") {
                    if (game_score < stoi(id)) {
                        game_score = stoi(id);
                    }
                    msg = "[���Ӱ���] : " + first + "�� 1���Դϴ�! ���� : " + std::to_string(game_score);
                }
                else {
                    if (game_score > stoi(id))
                        msg = "[���Ӱ���] : " + game_id + "�� ������ 1���Դϴ�! ���� : " + std::to_string(game_score);
                    else
                        msg = "[���Ӱ���] :  " + first + "�� " + game_id + "�� �̰���ϴ�! ���� : " + id;
                }
                cout << msg << endl;
                cout << "[����] " + user + " ���� ����ä�ù濡�� �����߽��ϴ�." << endl;
                send_msg_all(msg.c_str(), "");
                int remove = removeSocket(user);
                if (remove > -1) {
                    recreate = remove;
                }
                return;
            }
            if (msg == "/out") {
                num = 1;
                msg = "[����] " + user + " ���� �����߽��ϴ�.";
                cout << "[����] " + user + " ���� ��üä�ù濡�� �����߽��ϴ�." << endl;
                send_msg(msg.c_str(), user);
            }
            if (msg == "/out_adult") {
                num = 1;
                int adult_idx = find(chattingroom_member.begin(), chattingroom_member.end(), nickname) - chattingroom_member.begin(); //����ä�ù��� ������ ����� idxã��
                chattingroom_member[adult_idx] = "";
                msg = "[����] " + user + " ���� �����߽��ϴ�.";
                cout << "[����] " + user + " ���� ����ä�ù濡�� �����߽��ϴ�." << endl;
                send_adult_msg(msg.c_str(), user);
            }
            if (msg.find("-") != string::npos) {
                if_badwords(sck_info.ti, msg.c_str());
            }
            for (int i = 0; i < sck_list.size(); i++) {
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
                    if (p_msg.find("*") != string::npos) {
                        p_msg.resize(msg.size() - 1);
                    }
                    if (p_msg.find("-") != string::npos) {
                        if_badwords(sck_info.ti, msg.c_str());
                    }
                    pstmt = con->prepareStatement("INSERT INTO private_msg(sender, receiver ,message, time) VALUES(?,?,?,?)");
                    pstmt->setString(1, nickname);
                    pstmt->setString(2, id);
                    pstmt->setString(3, p_msg);
                    pstmt->setString(4, time);

                    pstmt->execute();
                    msg = "�ӼӸ��� �����߽��ϴ�! [" + nickname + "]" + " : " + p_msg;
                    send_private_msg(msg.c_str(), id);
                }
            }
            if (msg.find("*") != string::npos && num == 0) {
                msg.resize(msg.size() - 1);
                msg = nickname + " : " + msg;
                cout << msg << endl;
                send_adult_msg(msg.c_str(), nickname);
            }
            else if (num == 0 && msg.find("[����]") == string::npos) {
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
                send_msg(msg.c_str(), user);
            }
        }
        else { //�׷��� ���� ��� ���忡 ���� ��ȣ�� �����Ͽ� ���� �޽��� ����
            //del_client(idx); // Ŭ���̾�Ʈ ����
            int remove = removeSocket(user);
            if (remove > -1) {
                recreate = remove;
            }
            return;
        }
    }
}
void del_client(int idx) {
    closesocket(sck_list[idx].sck);
    sck_list.erase(sck_list.begin() + idx); // �迭���� Ŭ���̾�Ʈ�� �����ϰ� �� ��� index�� �޶����鼭 ��Ÿ�� ���� �߻�....��
    client_count--;
}
int removeSocket(string user) {
    int ti;
    for (int i = 0; i < sck_list.size(); i++) { // ������ �ִ� ��� client���� �޽��� ����
        if (sck_list[i].user == user) {
            ti = sck_list[i].ti;
            del_client(i);
            return ti;
        }
    }
    return -1;
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


        char buf[MAX_SIZE] = { };

        for (int i = 0; i < MAX_CLIENT; i++) {
            // �ο� �� ��ŭ thread �����ؼ� ������ Ŭ���̾�Ʈ�� ���ÿ� ������ �� �ֵ��� ��.
            th1[i] = std::thread(add_client, i);

        }
        std::thread th2(recreate_thread);

        while (1) { // ���� �ݺ����� ����Ͽ� ������ ����ؼ� ä�� ���� �� �ִ� ���¸� ����� ��. �ݺ����� ������� ������ �� ���� ���� �� ����.
            string text, msg = "";

            std::getline(cin, text);
            const char* buf = text.c_str();
            msg = server_sock.user + " : " + buf;
            send_msg(msg.c_str(), server_sock.user);
        }


        for (int i = 0; i < MAX_CLIENT; i++) {
            th1[i].join();
            //join : �ش��ϴ� thread ���� ������ �����ϸ� �����ϴ� �Լ�.
            //join �Լ��� ������ main �Լ��� ���� ����Ǿ thread�� �Ҹ��ϰ� ��.
            //thread �۾��� ���� ������ main �Լ��� ������ �ʵ��� ����.
        }
        th2.join();
        closesocket(server_sock.sck);
    }
    else {
        cout << "���α׷� ����. (Error code : " << code << ")";
    }

    WSACleanup();

    return 0;
}
