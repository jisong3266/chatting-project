#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h> //Winsock ������� include. WSADATA �������.
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
//#include <mysql/jdbc.h>
#include <Windows.h> // system("cls")�� ����ϱ� ���� ���
#include <conio.h> // _getch()�� ����ϱ� ���� ��� 
#include <mysql/jdbc.h>

#define MAX_SIZE 1024
#define ENTER 13
#define UP 72
#define DOWN 80 // �ƽ�Ű �ڵ� ����

using std::cout;
using std::cin;
using std::endl;
using std::string;

sql::mysql::MySQL_Driver* driver; // ���� �������� �ʾƵ� Connector/C++�� �ڵ����� ������ ��
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

SOCKET client_sock;
string my_nick;
string name, id, pw, birth;

const string server = "tcp://127.0.0.1:3306"; // �����ͺ��̽� �ּ�
const string username = "root"; // �����ͺ��̽� �����
const string password = "goflvhxj"; // �����ͺ��̽� ���� ��й�ȣ

class user_info {
public:
    string u_name, u_id, u_pw, u_birth;
    user_info(string u_name, string u_id, string u_pw, string u_birth) {
        this->u_name = u_name;
        this->u_id = u_id;
        this->u_pw = u_pw;
        this->u_birth = u_birth;
    }
};
// �̸� recv �Լ�
int chat_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
            msg = buf;
            std::stringstream ss(msg);  // ���ڿ��� ��Ʈ��ȭ
            string user;
            ss >> user; // ��Ʈ���� ����, ���ڿ��� ���� �и��� ������ �Ҵ�. ���� ����� �̸��� user�� �����.
            if (user != my_nick) cout << buf << endl; // ���� ���� �� �ƴ� ��쿡�� ����ϵ���.
        }
        else {
            cout << "Server Off" << endl;
            return -1;
        }
    }
}
// Ŀ�� ������ ����
void CursorView(bool show)
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
// �������̽�
void startMenu1()
{
    CursorView(false);
    system("cls");
    cout << "\n";
    cout << " "; cout << "********************************************************\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*       * * * *   *     *       *      * * * * *       *\n";
    cout << " "; cout << "*       *         *     *      * *         *           *\n";
    cout << " "; cout << "*       *         * * * *     * * *        *           *\n";
    cout << " "; cout << "*       *         *     *    *     *       *           *\n";
    cout << " "; cout << "*       * * * *   *     *   *       *      *           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    < ���� ȭ�� >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                 ��  1. �α���                         *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    2. ȸ������                       *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    0. ����                           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void startMenu2()
{
    CursorView(false);
    system("cls");
    cout << "\n";
    cout << " "; cout << "********************************************************\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*       * * * *   *     *       *      * * * * *       *\n";
    cout << " "; cout << "*       *         *     *      * *         *           *\n";
    cout << " "; cout << "*       *         * * * *     * * *        *           *\n";
    cout << " "; cout << "*       *         *     *    *     *       *           *\n";
    cout << " "; cout << "*       * * * *   *     *   *       *      *           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    < ���� ȭ�� >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    1. �α���                         *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                 ��  2. ȸ������                       *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    0. ����                           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void startMenu0()
{
    CursorView(false);
    system("cls");
    cout << "\n";
    cout << " "; cout << "********************************************************\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*       * * * *   *     *       *      * * * * *       *\n";
    cout << " "; cout << "*       *         *     *      * *         *           *\n";
    cout << " "; cout << "*       *         * * * *     * * *        *           *\n";
    cout << " "; cout << "*       *         *     *    *     *       *           *\n";
    cout << " "; cout << "*       * * * *   *     *   *       *      *           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    < ���� ȭ�� >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    1. �α���                         *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    2. ȸ������                       *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                 ��  0. ����                           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void endMenu()
{
    CursorView(false);
    system("cls");
    cout << "\n";
    cout << " "; cout << "********************************************************\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*       * * * *   *     *       *      * * * * *       *\n";
    cout << " "; cout << "*       *         *     *      * *         *           *\n";
    cout << " "; cout << "*       *         * * * *     * * *        *           *\n";
    cout << " "; cout << "*       *         *     *    *     *       *           *\n";
    cout << " "; cout << "*       * * * *   *     *   *       *      *           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    <   �� ��   >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*            ��  ���α׷��� �����Ͻðڽ��ϱ�?           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*               (EnterŰ�� ������ ����)                *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
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
    con->setSchema("chatting");

    // db �ѱ� ������ ���� ���� 
    stmt = con->createStatement();
    stmt->execute("set names euckr");
    if (stmt) { delete stmt; stmt = nullptr; }
}
void check_id() {
    while (1) {
        cout << "���̵� �Է��ϼ���(������ ���� ���� 8�ڸ��� �Է��ϼ���) : ";
        cin >> id;
        pstmt = con->prepareStatement("SELECT id FROM userinfo WHERE id = ?");
        pstmt->setString(1, id);
        result = pstmt->executeQuery();
        if (result->next()) {
            string db_id = result->getString(1);
            if (db_id == id) {
                cout << "�ߺ��� ID�Դϴ� !" << endl;
                continue;
                // ���̵� �ߺ���
            }
        }
        else {
             // ���̵� �ߺ� ����
             cout << "ID üũ�Ϸ�" << endl;
             send(client_sock, id.c_str(), 10, 0);
             break;
         }
    }
}
// start sql
void sign_in() {

    cout << "ȸ�������� �����մϴ�" << endl;
        cout << "�̸��� �Է��ϼ��� : ";
        cin >> name;
        check_id();
        send(client_sock, name.c_str(), 10, 0); // ���ῡ �����ϸ� client �� �Է��� �г����� ������ ����
    cout << "��й�ȣ�� �Է��ϼ��� : ";
    cin >> pw;
    cout << "��������� �Է��ϼ��� : ";
    cin >> birth;
    user_info *u  = new user_info(name, id, pw, birth);

    pstmt = con->prepareStatement("INSERT INTO userinfo(name, id, pw, birthday) VALUES(?,?,?,?)");

    pstmt->setString(1, u->u_name);
    pstmt->setString(2, u->u_id);
    pstmt->setString(3, u->u_pw);
    pstmt->setString(4, u->u_birth);

    try {
        pstmt->execute(); // SQL ���� ����
        cout << "User " << u->u_name << " added to the database." << endl;
    }
    catch (sql::SQLException& e) {
        cout << "SQL Error: " << e.what() << endl;
    }
   

}
void log_in() {
    cout << "�α����� �����մϴ�!" << endl;
    cout << "���̵� �Է��ϼ���(������ ���� ���� 8�ڸ��� �Է��ϼ���) : ";
    cin >> id;
    cout << "��й�ȣ�� �Է��ϼ��� : ";
    cin >> pw;
}
int main() {

    WSADATA wsa;
    // Winsock�� �ʱ�ȭ�ϴ� �Լ�. MAKEWORD(2, 2)�� Winsock�� 2.2 ������ ����ϰڴٴ� �ǹ�.
    // ���࿡ �����ϸ� 0��, �����ϸ� �� �̿��� ���� ��ȯ.
    // 0�� ��ȯ�ߴٴ� ���� Winsock�� ����� �غ� �Ǿ��ٴ� �ǹ�.
    start_sql();
    bool start = true;
    char move;
    int num = 1;
    startMenu1();
    while (start) {
        move = _getch();
        if (move == DOWN) {
            ++num;
            if (num > 2) {
                num = 0;
            }
        }
        else if (move == UP) {
            --num;
            if (num < 0) {
                num = 2;
            }
        }
        switch (num) {
        case 0:
            startMenu0();
            if (move == ENTER) {
                endMenu();
                move = _getch();
                if (move == ENTER) {
                    start = false;
                    system("cls");
                }
                else {
                    startMenu0();
                }
            }
            break;
        case 1:
            startMenu1();
            if (move == ENTER) {
                start = false;
            }
            break;
        case 2:
            startMenu2();
            if (move == ENTER) {
                start = false;
            }
            break;
        default:
            break;
        }
    }
    int code = WSAStartup(MAKEWORD(2, 2), &wsa);
    client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 
    // ������ ���� ���� ���� �κ�
    SOCKADDR_IN client_addr = {};
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(7777);
    InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);
    while (1) {
        if (!connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) { // ���� ������ ������ �ش��ϴ� server�� ����!
            cout << "Server Connect" << endl;
            break;
        }
        else
            cout << "Connecting..." << endl;
    }
    if (!code) {
        switch (num) {
        case 1:
            log_in();
            break;
        case 2:
            sign_in();
            break;
        }
        std::thread th2(chat_recv);

        while (1) {
            string text;
            std::getline(cin, text);
            const char* buffer = text.c_str(); // string���� char* Ÿ������ ��ȯ
            send(client_sock, buffer, strlen(buffer), 0);
        }
        th2.join();
        closesocket(client_sock);
    }

    WSACleanup();
    return 0;
}
