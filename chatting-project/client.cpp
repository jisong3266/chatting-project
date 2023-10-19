#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h> //Winsock ������� include. WSADATA �������.
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <Windows.h> // system("cls")�� ����ϱ� ���� ���
#include <conio.h> // _getch()�� ����ϱ� ���� ��� 
#include <mysql/jdbc.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <fstream>

#define MAX_SIZE 1024
#define ENTER 13
#define UP 72
#define DOWN 80
#define ESC 27 
#define BACKSP 8 // �ƽ�Ű �ڵ� ����

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
bool start = true;
bool thread = true;
bool flag = true;
bool chatout = false;
char move;
int num;
string name, id, pw, birth;

enum START_MOVE {
    OFF,
    LOG_IN,
    SIGN_IN,
    LOG_OUT,
    CHAT_IN,
    CHANGE,
    ALL_CHAT,
    ADULT_CHAT,
    GAME
};

const string server = "tcp://127.0.0.1:3306"; // �����ͺ��̽� �ּ�
const string username = "root"; // �����ͺ��̽� �����
const string password = "goflvhxj"; // �����ͺ��̽� ���� ��й�ȣ

// ȭ�� �ʱ�ȭ
void clear_screen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };  // ȭ�� �»�� ��ǥ
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // �ܼ� ȭ�� ũ�� ��������
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // ȭ���� ���� ���ڷ� �����
    if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten)) {
        return;
    }

    // Ŀ�� ��ġ �ʱ�ȭ
    SetConsoleCursorPosition(hConsole, coordScreen);
}
// Ŀ�� ������ ����
void cursor_view(bool show)
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
// Ŀ�� �̵� �Լ�
void goto_xy(int x, int y) {
    COORD posXY = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posXY);
}
// �������̽�
void start_menu()
{
    clear_screen();
    cursor_view(false);
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
    cout << " "; cout << "*                    0. ����                           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void end_menu()
{
    clear_screen();
    cursor_view(false);
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
    cout << " "; cout << "*           =��  ���α׷��� �����Ͻðڽ��ϱ�?           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                [ENTER�� ������ ����]                 *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void login_inter()
{
    cursor_view(true);
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
    cout << " "; cout << "*                    <  �� �� ��  >                    *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    ���̵� :                                          *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    ��й�ȣ :                                        *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*             [ESC�� ������ ����ȭ������]              *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void sign_inter()
{
    cursor_view(true);
    cout << "\n";
    cout << " "; cout << "********************************************************\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                   < ȸ �� �� �� >                    *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    �� �� :                                           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    ���̵� :                                          *\n";
    cout << " "; cout << "*            (������ ���� ���� 8�ڸ��� �Է�)           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    ��й�ȣ :                                        *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    ������� :                                        *\n";
    cout << " "; cout << "*             (YYYY-MM-DD�� �Է�)                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*             [ESC�� ������ ����ȭ������]              *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void pw_inter()
{
    clear_screen();
    cursor_view(true);
    cout << "\n";
    cout << " "; cout << "********************************************************\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                < ȸ �� �� �� �� �� >                 *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*            �� ��й�ȣ�� �Է����ּ��� !               *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    ��й�ȣ :                                        *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*             [ESC�� ������ ����ȭ������]              *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void change_inter()
{
    clear_screen();
    cursor_view(true);
    cout << "\n";
    cout << " "; cout << "********************************************************\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                < ȸ �� �� �� �� �� >                 *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    �� �� :                                           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    ���̵� :                                          *\n";
    cout << " "; cout << "*            (������ ���� ���� 8�ڸ��� �Է�)           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    ��й�ȣ :                                        *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    ������� :                                        *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*             [ESC�� ������ ����ȭ������]              *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void login_menu()
{
    clear_screen();
    cursor_view(false);
    cout << "\n";
    cout << " "; cout << "********************************************************\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*       * * * *   *     *       *      * * * * *       *\n";
    cout << " "; cout << "*       *         *     *      * *         *           *\n";
    cout << " "; cout << "*       *         * * * *     * * *        *           *\n";
    cout << " "; cout << "*       *         *     *    *     *       *           *\n";
    cout << " "; cout << "*       * * * *   *     *   *       *      *           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                 " << id << "�� ȯ���մϴ�!               * \n";
    cout << " "; cout << "*                    <   �� ��   >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    1. ä�ù� ����                    *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    2. ȸ������ ����                  *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    0. �α׾ƿ�                       *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void logout_menu()
{
    clear_screen();
    cursor_view(false);
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
    cout << " "; cout << "*                    < �α׾ƿ� >                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*           =��  �α׾ƿ� �Ͻðڽ��ϱ�?                 *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*               [ENTER�� ������ �α׾ƿ�]              *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void chat_menu()
{
    clear_screen();
    cursor_view(false);
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
    cout << " "; cout << "*                    < ä�ù� ���� >                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    1. ��ü ä�ù�                    *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    2. ���� ä�ù�                    *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    3. �����ձ� ���ӹ�                *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void adult_menu()
{
    clear_screen();
    cursor_view(false);
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
    cout << " "; cout << "*                   < ���� Ȯ�� �� >                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*           =��  ��ø� ��ٷ��ּ��� . . .              *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*               [ESC�� ������ �ڷ� ����]               *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void adultY_inter()
{
    clear_screen();
    cursor_view(false);
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
    cout << " "; cout << "*            *          *      * * * *                 *\n";
    cout << " "; cout << "*            *          *      *     *                 *\n";
    cout << " "; cout << "*      *  *  *  *  *    *      * * * *                 *\n";
    cout << " "; cout << "*            *          *            *                 *\n";
    cout << " "; cout << "             *          *      * * * *                 *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*             <  ���� ä�ù� ���� ��  >                *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void adultN_inter()
{
    clear_screen();
    cursor_view(false);
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
    cout << " "; cout << "*                 < ���� Ȯ�� �Ϸ� >                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*              =��  ������ �ƴմϴ�!                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*               [ESC�� ������ �ڷ� ����]               *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
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
// �Է� �Լ�
int getCharacter(char move, string& str, char ch, int i) {
    while (move != ENTER) {
        move = _getch();
        if (move == ESC) return -1;
        if (move == ENTER) break; // Enter Ű�� ������ �Է� ����
        if (move == BACKSP) { // Backspace Ű�� ���
            if (!str.empty()) { // �Էµ� ���ڰ� ������
                str.pop_back(); // ������ ���ڸ� ����
                cout << "\b \b"; // Ŀ�� ��ġ�� �������� �̵����� ������ ����� ��, �ٽ� Ŀ�� ��ġ�� ������� �̵���Ŵ
            }
        }
        else {
            if (str.size() < i) {
                str.push_back(move);
                if (ch != '\0') {
                    cout << ch;
                }
                else {
                    cout << move;
                }
            }
        }
    }
    return 0;
}
// id üũ
void check_id(string c_id) {
    while (1) {
        c_id = "";
        bool hangul = false;
        goto_xy(15, 7);
        for (int i = 0; i < 40; i++) {
            cout << " ";
        }
        goto_xy(15, 7);
        if (getCharacter(move, c_id, '\0', 8) < 0) {
            start = true;
            flag = false;
            return;
        }
        for (int i = 0; i < c_id.size(); i++) {
            if ((c_id[i] & 0x80) == 0x80) {
                hangul = true;
            }
        }
        if (hangul || c_id.size() != 8) {
            goto_xy(2, 8);
            cout << "                                                      ";
            goto_xy(13, 8);
            cout << "�� ��� : �߸��� �����Դϴ� !";
            continue;
        }
        pstmt = con->prepareStatement("SELECT id FROM userinfo WHERE id = ?");
        pstmt->setString(1, c_id);
        result = pstmt->executeQuery();
        if (result->next()) {
            string db_id = result->getString(1);
            if (db_id == c_id) {
                goto_xy(2, 8);
                cout << "                                                      ";
                goto_xy(13, 8);
                cout << "�� ��� : �ߺ��� ID�Դϴ� !";
                continue;
                // ���̵� �ߺ���
            }
        }
        else {
            // ���̵� �ߺ� ����
            goto_xy(2, 8);
            cout << "                                                      ";
            goto_xy(13, 8);
            id = c_id;
            cout << " =�� ID üũ�Ϸ� ! " << id << endl;
            break;
        }
    }
}
// ���� ����
int check_birth(string c_id) {
    adult_menu();
    Sleep(2000);
    pstmt = con->prepareStatement("SELECT birthday FROM userinfo WHERE id = ? and birthday < '2004.12.31'");
    pstmt->setString(1, c_id);
    result = pstmt->executeQuery();
    if (result->next()) {
        string db_birth = result->getString(1);
        if (db_birth == "") {
            adultN_inter();
            return 0;
            // ������ �ƴ� ���
        }
        else {
            adultY_inter();
            return -1;
            // ������ ���
        }
    }
    return 0;
}
// ȸ������
void sign_in() {
    name = "";
    pw = "";
    char guard = '+';
    goto_xy(0, 0);
    sign_inter();
    move = '\0';
    goto_xy(14, 5);
    if (getCharacter(move, name, '\0', 10) < 0) {
        start = true;
        flag = false;
        return;
    }
    check_id(id);
    goto_xy(17, 10);
    if (getCharacter(move, pw, guard, 12) < 0) {
        start = true;
        flag = false;
        return;
    }
    while (1) {
        birth = "";
        goto_xy(17, 12);
        for (int i = 0; i < 35; i++) {
            cout << " ";
        }
        goto_xy(17, 12);
        if (getCharacter(move, birth, '\0', 10) < 0) {
            start = true;
            flag = false;
            return;
        }

        int year, month, day;
        char dash1, dash2;

        // ���ڿ� ��Ʈ���� ����Ͽ� �Ľ�
        std::istringstream ss(birth);
        ss >> year >> dash1 >> month >> dash2 >> day;

        if (ss && dash1 == '-' && dash2 == '-' && month < 13 && day < 32) {
            goto_xy(2, 13);
            cout << "                                                      ";
            break;
        }
        else {
            goto_xy(13, 13);
            cout << "�� ��� : �߸��� �����Դϴ�!";
            continue;
        }
    }
    pstmt = con->prepareStatement("INSERT INTO userinfo(name, id, pw, birthday) VALUES(?,?,?,?)");
    pstmt->setString(1, name);
    pstmt->setString(2, id);
    pstmt->setString(3, pw);
    pstmt->setString(4, birth);
    try {
        pstmt->execute(); // SQL ���� ����
        goto_xy(5, 14);
        cout << "User " << name << " added to the database.";
        Sleep(3000);
        start = true;
        flag = false;
    }
    catch (sql::SQLException& e) {
        goto_xy(5, 14);
        cout << "SQL Error: " << e.what();
        Sleep(2000);
        start = true;
        flag = false;
    }
}
// �α���
void log_in() {
    while (1) {
        id = "";
        pw = "";
        char guard = '+';
        goto_xy(0, 0);
        login_inter();
        move = '\0';
        goto_xy(17, 13);
        if (getCharacter(move, id, '\0', 8) < 0) {
            start = true;
            flag = false;
            return;
        }
        goto_xy(17, 15);
        if (getCharacter(move, pw, guard, 12) < 0) {
            start = true;
            flag = false;
            return;
        }
        pstmt = con->prepareStatement("SELECT id, pw FROM userinfo WHERE id = ? AND pw = ?");
        pstmt->setString(1, id);
        pstmt->setString(2, pw);
        result = pstmt->executeQuery();

        if (result->next()) {
            string db_id = result->getString(1);
            string db_pw = result->getString(2);

            if (db_id == id && db_pw == pw) {
                goto_xy(2, 17);
                cout << "                                                      ";
                goto_xy(16, 17);
                cout << " =�� �α��� �Ϸ� ! ";
                Sleep(2000);
                break;
            }
            else {
                clear_screen();
                goto_xy(10, 20);
                cout << "�� ��� : ȸ�������� ��ġ���� �ʽ��ϴ�!";
            }
        }
        else {
            clear_screen();
            goto_xy(10, 20);
            cout << "�� ��� : ȸ�������� ��ġ���� �ʽ��ϴ�!";
        }
    }
}
// ȸ����������
void change_info() {
    string beforeID = id;
    char guard = '+';
    while (1) {
        pw = "";
        move = '\0';
        goto_xy(0, 0);
        pw_inter();
        goto_xy(17, 10);
        if (getCharacter(move, pw, guard, 12) < 0) {
            return;
        }
        pstmt = con->prepareStatement("SELECT pw FROM userinfo WHERE id = ?");
        pstmt->setString(1, beforeID);
        result = pstmt->executeQuery();

        if (result->next()) {
            string db_pw = result->getString(1);

            if (db_pw == pw) {
                goto_xy(15, 11);
                cout << " =�� ��й�ȣ Ȯ�� �Ϸ� !";
                Sleep(2000);
                goto_xy(0, 0);
                change_inter();
                name = "";
                goto_xy(14, 5);
                if (getCharacter(move, name, '\0', 10) < 0) {
                    return;
                }
                check_id(id);
                goto_xy(17, 10);
                pw = "";
                if (getCharacter(move, pw, guard, 12) < 0) {
                    return;
                }
                while (1) {
                    birth = "";
                    goto_xy(17, 12);
                    for (int i = 0; i < 35; i++) {
                        cout << " ";
                    }
                    goto_xy(17, 12);
                    if (getCharacter(move, birth, '\0', 10) < 0) {
                        return;
                    }

                    int year, month, day;
                    char dash1, dash2;

                    // ���ڿ� ��Ʈ���� ����Ͽ� �Ľ�
                    std::istringstream ss(birth);
                    ss >> year >> dash1 >> month >> dash2 >> day;

                    if (ss && dash1 == '-' && dash2 == '-' && month < 13 && day < 32) {
                        goto_xy(2, 13);
                        cout << "                                                      ";
                        break;
                    }
                    else {
                        goto_xy(13, 13);
                        cout << "�� ��� : �߸��� �����Դϴ�!";
                        continue;
                    }
                }
                pstmt = con->prepareStatement("UPDATE userinfo SET name = ?, id = ?, pw = ?, birthday = ? where id =?");
                pstmt->setString(1, name);
                pstmt->setString(2, id);
                pstmt->setString(3, pw);
                pstmt->setString(4, birth);
                pstmt->setString(5, beforeID);
                try {
                    pstmt->execute(); // SQL ���� ����
                    goto_xy(5, 14);
                    cout << "User " << name << " added to the database.";
                    Sleep(3000);
                }
                catch (sql::SQLException& e) {
                    goto_xy(5, 14);
                    cout << "SQL Error: " << e.what();
                    Sleep(3000);
                }
                start = true;
                flag = false;
                break;
            }
            else {
                goto_xy(5, 14);
                cout << "�� ��� : ȸ�������� ��ġ���� �ʽ��ϴ�!";
                Sleep(1500);
            }
        }
        else {
            goto_xy(5, 11);
            cout << "�� ��� : ȸ�������� ��ġ���� �ʽ��ϴ�!";
            Sleep(1500);
        }
    }
}
// �޽��� �ޱ�
int chat_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        int bytesReceived = recv(client_sock, buf, MAX_SIZE, 0);

        if (bytesReceived > 0) {
            msg = buf;
            std::stringstream ss(msg);
            string user;
            ss >> user;
            if (user != id) cout << buf << endl;
        }
        else if (bytesReceived == 0) {
            // ������ ������ ������ ��
            cout << "Server disconnected" << endl;
            return -1;
        }
        else {
            // ���� �߻�
            cout << "Error in recv()" << endl;
            cout << WSAGetLastError() << endl;
            return -1;
        }
    }
}
int get_chat(char move, string& str, char ch, int i) {
    while (move != ENTER) {
        move = _getch();
        if (str == "/out") {
            cout << endl << "ä���� �����մϴ�..." << endl;
            chatout = true;
            return -1;
        }
        if (move == ENTER) break; // Enter Ű�� ������ �Է� ����
        if (move == BACKSP) { // Backspace Ű�� ���
            if (!str.empty()) { // �Էµ� ���ڰ� ������
                str.pop_back(); // ������ ���ڸ� ����
                cout << "\b \b"; // Ŀ�� ��ġ�� �������� �̵����� ������ ����� ��, �ٽ� Ŀ�� ��ġ�� ������� �̵���Ŵ
            }
        }
        else {
            if (str.size() < i) {
                str.push_back(move);
                if (ch != '\0') {
                    cout << ch;
                }
                else {
                    cout << move;
                }
            }
        }
    }
    return 0;
}
string check_badwords(string msg) {
    std::locale::global(std::locale("korean"));
    std::ifstream readFile;
    std::vector<string> badwords;
    string line;
    string replacement;
    int idx = 0;
    readFile.open("C:/Users/����/source/repos/Project1/badwords.txt");
    if (!readFile.is_open()) {
        cout << "Could not open the file - '" << "badwords" << "'" << endl;
    }
    while (getline(readFile, line)) {
        badwords.push_back(line);
    }
    for (string i : badwords) {
        replacement = "";
        size_t idx = msg.find(i);
        for (char a : msg) {
            if (idx != string::npos) {
                for (int j = 0; j < i.length() / 2; j++) {
                    replacement += "-";
                }
                msg.replace(idx, i.length(), replacement); // idx���� i.length()���� �� "-"�� ��ü
                idx = msg.find(i, idx + 1); // ���� ��ġ �׸��� ã�� ���� �ε����� �̵�
            }
        }
    }
    return msg;
}
void send_msg() {
    string text;
    while (1) {
        text = "";
        move = '\0';
        if (get_chat(move, text, '\0', 55) < 0) {
            return;//�Լ� ����
        }
        if (text == "/secret") {
            cout << endl << "��� ��忡 �����մϴ� !" << endl;
            while (1) {
                text = "";
                move = '\0';
                if (get_chat(move, text, '*', 55) < 0) {
                    return;
                }
                if (text == "/normal") {
                    text = "";
                    cout << endl << "�븻 ���� �����մϴ� !";
                    break;
                }
                text = check_badwords(text);
                const char* buffer = text.c_str(); // string���� char* Ÿ������ ��ȯ
                send(client_sock, buffer, strlen(buffer), 0);
                cout << endl;
            }
        }
        text = check_badwords(text);
        const char* buffer = text.c_str(); // string���� char* Ÿ������ ��ȯ
        send(client_sock, buffer, strlen(buffer), 0);
        cout << endl;
    }
}
void send_game1_msg() {
    string text;
    while (1) {
        std::getline(cin, text);
        text += "*";
        send(client_sock, text.c_str(), MAX_SIZE, 0);
    }
    
}
//��ü ä�ù�
void chatting_room1() {
    client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
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
    cursor_view(true);
    std::vector<string>recent_msg;
    cout << "��üä�ù濡 �����մϴ�!" << endl;
    send(client_sock, id.c_str(), 8, 0);
    stmt = con->createStatement();
    result = stmt->executeQuery("SELECT nickname, message, time FROM chat order by chat_num desc limit 5");
    // ��� ���
    while (result->next()) {
        recent_msg.push_back(result->getString(1) + " : " + result->getString(2) + "   " + result->getString(3));
    }
    std::reverse(recent_msg.begin(), recent_msg.end());
    for (string i : recent_msg) cout << i << endl;
    cout << "\n================�ֱ� �޼����Դϴ�================\n";
    pstmt = con->prepareStatement("SELECT sender, message, time FROM private_msg where receiver = ?");
    pstmt->setString(1, id);
    result = pstmt->executeQuery();
    while (result->next()) {
        cout << result->getString(1) + " : " + result->getString(2) + "   " + result->getString(3) << endl;
    }
    cout << "\n================�ӼӸ� �޼����Դϴ�================\n";
    std::thread th2(chat_recv);
    send_msg();
    if (chatout) {
        closesocket(client_sock);
        shutdown(client_sock, SD_RECEIVE);
        start = false;
        flag = true;
        thread = false;
        th2.join();
        return;
    }
    thread = false;
    th2.join();

}
//���� ä�ù�
void chatting_room2() {
    client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
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
    cursor_view(true);
    cout << "���� ä�ù濡 �����մϴ� ! " << endl;
    string game1_id;
    game1_id = id + "*";
    send(client_sock, game1_id.c_str(), 9, 0);
    std::thread th3(chat_recv);
    send_game1_msg();
    th3.join();
    thread = false;
}
//�����ձ� ����

int main() {
    bool interFace = true;

    WSADATA wsa;
    // Winsock�� �ʱ�ȭ�ϴ� �Լ�. MAKEWORD(2, 2)�� Winsock�� 2.2 ������ ����ϰڴٴ� �ǹ�.
    // ���࿡ �����ϸ� 0��, �����ϸ� �� �̿��� ���� ��ȯ.
    // 0�� ��ȯ�ߴٴ� ���� Winsock�� ����� �غ� �Ǿ��ٴ� �ǹ�.
    start_sql();
    int code = WSAStartup(MAKEWORD(2, 2), &wsa);

    while (interFace) {

        while (_kbhit()) {
            char ch = '\0';
            ch = _getch(); // Ű �Է��� �о ���ۿ��� ����
        }
        num = LOG_IN;
        bool log = true;
        bool chat = false;
        start_menu();
        goto_xy(17, 13);
        cout << "=��";
        while (start) {
            move = _getch();
            if (move == DOWN) {
                ++num;
                if (num > SIGN_IN) {
                    num = OFF;
                }
            }
            else if (move == UP) {
                --num;
                if (num < OFF) {
                    num = SIGN_IN;
                }
            }
            switch (num) {
            case OFF:
                start_menu();
                goto_xy(17, 17);
                cout << "=��";
                if (move == ENTER) {
                    end_menu();
                    move = _getch();
                    if (move == ENTER) {
                        clear_screen();
                        start = false;
                        flag = false;
                        log = false;
                        num = 100;
                        interFace = false;
                        break;
                    }
                    else {
                        start_menu();
                        goto_xy(17, 17);
                        cout << "=��";
                    }
                }
                break;
            case LOG_IN:
                start_menu();
                goto_xy(17, 13);
                cout << "=��";
                if (move == ENTER) {
                    start = false;
                    flag = true;
                }
                break;
            case SIGN_IN:
                start_menu();
                goto_xy(17, 15);
                cout << "=��";
                if (move == ENTER) {
                    start = false;
                    flag = true;
                }
                break;
            default:
                break;
            }
        }
        if (!code && flag) {
            switch (num) {
            case LOG_IN:
                log_in();
                break;
            case SIGN_IN:
                clear_screen();
                sign_in();
                break;
            }
            if (flag && num == LOG_IN) {
                login_menu();
                goto_xy(17, 13);
                cout << "=��";
            }

            while (_kbhit()) {
                char ch = '\0';
                ch = _getch(); // Ű �Է��� �о ���ۿ��� ����
            }

            num = CHAT_IN;
            //bool inChatRoom = false;  // ä�ù濡 �����ߴ��� ���θ� ��Ÿ���� �÷���
            //bool inWordChain = false;  // �����ձ� ���� ������ ���θ� ��Ÿ���� �÷���
            while (flag) {
                while (log) {
                    move = _getch();
                    if (move == DOWN) {
                        ++num;
                        if (num > CHANGE) {
                            num = LOG_OUT;
                        }
                    }
                    else if (move == UP) {
                        --num;
                        if (num < LOG_OUT) {
                            num = CHANGE;
                        }
                    }

                    switch (num) {
                    case LOG_OUT:
                        login_menu();
                        goto_xy(17, 17);
                        cout << "=��";
                        if (move == ENTER) {
                            logout_menu();
                            move = _getch();
                            if (move == ENTER) {
                                start = true;
                                flag = false;
                                log = false;
                            }
                            else {
                                login_menu();
                                goto_xy(17, 17);
                                cout << "=��";
                            }
                        }
                        break;
                    case CHAT_IN:
                        login_menu();
                        goto_xy(17, 13);
                        cout << "=��";
                        if (move == ENTER) {
                            log = false;
                            chat = true;
                            //inChatRoom = true;  // ä�ù濡 ������ ���·� �÷��� ����
                        }
                        break;
                    case CHANGE:
                        login_menu();
                        goto_xy(17, 15);
                        cout << "=��";
                        if (move == ENTER) {
                            change_info();
                        }
                        break;
                    default:
                        break;
                    }
                }

                if (chat && num == CHAT_IN) {
                    chat_menu();
                    goto_xy(17, 13);
                    cout << "=��";
                }

                while (_kbhit()) {
                    char ch = '\0';
                    ch = _getch(); // Ű �Է��� �о ���ۿ��� ����
                }

                while (chat) {
                    move = _getch();
                    if (move == DOWN) {
                        ++num;
                        if (num > GAME) {
                            num = ALL_CHAT;
                        }
                    }
                    else if (move == UP) {
                        --num;
                        if (num < ALL_CHAT) {
                            num = GAME;
                        }
                    }
                    else if (move == ESC) {
                        chat = false;
                        log = true;
                        num = CHAT_IN;
                        //inChatRoom = false;  // ä�ù濡�� ������ �� �÷��� ����
                        break;
                    }
                    switch (num) {
                    case ALL_CHAT:
                        chat_menu();
                        goto_xy(17, 13);
                        cout << "=��";
                        if (move == ENTER) {
                            chat = false;
                            flag = false;
                        }
                        else if (move == ESC) {
                            chat = false;
                            log = true;
                            num = CHAT_IN;
                            break;
                        }
                        break;
                    case ADULT_CHAT:
                        chat_menu();
                        goto_xy(17, 15);
                        cout << "=��";
                        if (move == ENTER) {
                            if (check_birth(id) < 0) {
                                chat = false;
                                flag = false;
                            }
                            else {
                                chat = false;
                                log = true;
                                num = CHAT_IN;
                                break;
                            }
                        }
                        else if (move == ESC) {
                            chat = false;
                            log = true;
                            num = CHAT_IN;
                            break;
                        }
                        break;
                    case GAME:
                        chat_menu();
                        goto_xy(17, 17);
                        cout << "=��";
                        if (move == ENTER) {
                            chat = false;
                            flag = false;
                        }
                        else if (move == ESC) {
                            chat = false;
                            log = true;
                            num = CHAT_IN;
                            break;
                        }
                        break;
                    default:
                        break;
                    }
                }
            }

            if (num == ALL_CHAT) {
                clear_screen();
                chatting_room1();
            }
            else if (num == ADULT_CHAT) {
                Sleep(1000);
                clear_screen();
                chatting_room2();
                //  inWordChain = true;
            }
            else if (num == GAME) {
                clear_screen();
                cout << "���� ���� ����Դϴ�...";
                // inWordChain = true;
            }
        }
    }
    //th2.join();
    closesocket(client_sock);
    WSACleanup();
}
