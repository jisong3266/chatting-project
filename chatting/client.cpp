#pragma comment(lib, "ws2_32.lib") // ������� ���̺귯���� ��ũ. ���� ���̺귯�� ����

#include <WinSock2.h> // Winsock ������� include. WSADATA �������.
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread> // ������ ���
#include <Windows.h> // clear_screen()�� ����ϱ� ���� ���
#include <conio.h> // _getch()�� ����ϱ� ���� ��� 
#include <mysql/jdbc.h> // SQL ���
#include <vector>
#include <cstdlib>
#include <ctime> // �ð� ������ ���� ���
#include <cstdio>
#include <fstream> // �ؽ�Ʈ ������ ���� ���� ���

#define MAX_SIZE 1024
#define ENTER 13
#define UDLR -32
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27 
#define BACKSP 8 // �ƽ�Ű �ڵ� ����

using std::cout;
using std::cin;
using std::endl;
using std::string; // namespace ����

sql::mysql::MySQL_Driver* driver; // ���� �������� �ʾƵ� Connector/C++�� �ڵ����� ������ ��
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result; // sql ����

SOCKET client_sock; // ���� ����

bool start = true;
bool menu = true;
bool chat = true;
bool info = false;
bool isSignin = false;
bool chatout = false;
bool game = false;
char move;
int num;
string name, id, pw, birth; // �������� ����

// �������̽� �̵� enum
enum MOVE_NUM {
    OFF,
    LOG_IN,
    SIGN_IN,
    LOG_OUT,
    CHAT_IN,
    MYPAGE,
    OUTPUT_INFO,
    CHANGE_INFO,
    ALL_CHAT,
    ADULT_CHAT,
    GAME,
    GAME_START,
    GAME_RULE,
    GAME_RANK,
    GAME_OUT
};
// ���� enum
enum COLOR {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};

const string server = "tcp://127.0.0.1:3306"; // �����ͺ��̽� �ּ�
const string username = "root"; // �����ͺ��̽� �����
const string password = "1234"; // �����ͺ��̽� ���� ��й�ȣ

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
void cursor_view(bool show) {
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
// ���� ���� �Լ�
void textcolor(int foreground, int background) {
    int color = foreground + background * 16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
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
void login_menu(string& id)
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
    cout << " "; cout << "*                 " << id << "�� ȯ���մϴ�!               *\n";
    cout << " "; cout << "*                    <   �� ��   >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    1. ä�ù� ����                    *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    2. ����������                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    0. �α׾ƿ�                       *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void info_menu()
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
    cout << " "; cout << "*                   <  ����������  >                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                   1. ȸ������ Ȯ��                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                   2. ȸ������ ����                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void info_inter()
{
    clear_screen();
    cursor_view(false);
    cout << "\n";
    cout << " "; cout << "********************************************************\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                     < ȸ �� �� ��  >                 *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    �� �� :                                           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    ���̵� :                                          *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    ������� :                                        *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    ��ŷ :                                            *\n";
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
    cout << " "; cout << "*                    3. Ÿ��ġ�� ���ӹ�                *\n";
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
    cout << " "; cout << "*                                                      *\n";
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
    cout << " "; cout << "*            *          *      * * * *                 *\n";
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
    cout << " "; cout << "*              =��  ������ �ƴմϴ� !                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void game_menu()
{
    clear_screen();
    cursor_view(false);
    cout << "\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*       * * * *         *         *         *     * * * *       *\n";
    cout << " "; cout << "*       *              * *        * *     * *     *             *\n";
    cout << " "; cout << "*       *   * *       * * *       *  *   *  *     * * * *       *\n";
    cout << " "; cout << "*       *     *      *     *      *   * *   *     *             *\n";
    cout << " "; cout << "*       * * * *     *       *     *    *    *     * * * *       *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                         1. ���� ����                          *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                         2. ���� �� �Ұ�                       *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                         3. ��ŷ ����                          *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                         4. ���� ����                          *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*****************************************************************\n\n";
}
void game_rule()
{
    clear_screen();
    cursor_view(false);
    cout << "\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*       * * * *         *         *         *     * * * *       *\n";
    cout << " "; cout << "*       *              * *        * *     * *     *             *\n";
    cout << " "; cout << "*       *   * *       * * *       *  *   *  *     * * * *       *\n";
    cout << " "; cout << "*       *     *      *     *      *   * *   *     *             *\n";
    cout << " "; cout << "*       * * * *     *       *     *    *    *     * * * *       *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*       =�� Ÿ�� ġ�� �����̶� . . .                             *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*      30�� ���� �ܼ�â ȭ�鿡 ���õǴ� �ܾ �Ȱ��� �Է��ϸ�  *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*      ���� �ƴϸ� ���з� ���ѽð� ���� �󸶳� ���� �ܾ      *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*      �Է��ϴ����� �ܷ�� ���߷� �����Դϴ� !                  *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                 [ ESC�� ���� ����ȭ������ ]                   *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*****************************************************************\n\n";
}
void game_rank_inter() {
    clear_screen();
    cursor_view(false);
    cout << "\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                         <  ��  ŷ >                           *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*         ����     �� �� ��                   �� ��             *\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*     [ �� ��ŷ ]                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*****************************************************************\n\n";
}
void game_out_inter()
{
    clear_screen();
    cursor_view(false);
    cout << "\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*       * * * *         *         *         *     * * * *       *\n";
    cout << " "; cout << "*       *              * *        * *     * *     *             *\n";
    cout << " "; cout << "*       *   * *       * * *       *  *   *  *     * * * *       *\n";
    cout << " "; cout << "*       *     *      *     *      *   * *   *     *             *\n";
    cout << " "; cout << "*       * * * *     *       *     *    *    *     * * * *       *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                          <   �� ��   >                        *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*               =��  ������ �����Ͻðڽ��ϱ� ?                   *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                     [ ENTER�� ������ ���� ]                   *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*****************************************************************\n\n";
}
void game_inter()
{
    clear_screen();
    cursor_view(true);
    cout << "\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*       * * * *         *         *         *     * * * *       *\n";
    cout << " "; cout << "*       *              * *        * *     * *     *             *\n";
    cout << " "; cout << "*       *   * *       * * *       *  *   *  *     * * * *       *\n";
    cout << " "; cout << "*       *     *      *     *      *   * *   *     *             *\n";
    cout << " "; cout << "*       * * * *     *       *     *    *    *     * * * *       *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*      ���þ� :                                                 *\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*            ��                                                  *\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*   ���� ���ھ� :           ��                                  *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                 ����ð� :          / 30 ��   *\n";
    cout << " "; cout << "*****************************************************************\n\n";
}

// start SQL
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
// Ű���� �Է� ����
void key_ignore() {
    while (_kbhit()) {
        char ch = _getch(); // Ű �Է��� �о ���ۿ��� ����
    }
}
// Ű���� �Է�
int key_input(char move, string& str, char ch, int i) {
    while (move != ENTER) {
        char HPKM = '\0';
        move = _getch();

        if (move == UDLR) { // ����Ű �Է½� -32���� �޾ƿ�
            HPKM = _getch();
            if (HPKM == UP || HPKM == DOWN || HPKM == LEFT || HPKM == RIGHT) {
                key_ignore();
            }
        } 
        else if (move == ESC) return -1; // ESC Ű�� ������ ���� ȭ������
        else if (move == ENTER) break; // Enter Ű�� ������ �Է� ����
        else if (move == BACKSP) { // Backspace Ű�� ���
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
int check_id(string id_input, string& id_output) {
    while (1) {
        bool hangul = false;
        goto_xy(15, 7);
        for (int i = 0; i < 40; i++) {
            cout << " ";
        }
        goto_xy(15, 7);
        if (key_input(move, id_input, '\0', 8) < 0) {
            if (isSignin) {
                start = true;
                menu = false;
                chat = false;
            }
            else {
                start = false;
                menu = true;
                chat = false;
            }
            return -1; // ESC�϶� -1 ��ȯ
        }
        for (int i = 0; i < id_input.size(); i++) {
            if ((id_input[i] & 0x80) == 0x80) {
                hangul = true;
            }
        }
        if (hangul || id_input.size() != 8) {
            id_input = "";
            goto_xy(2, 8);
            cout << "                                                      ";
            goto_xy(13, 8);
            cout << "�� ��� : �߸��� �����Դϴ� !";
            continue;
        }
        pstmt = con->prepareStatement("SELECT id FROM userinfo WHERE id = ?");
        pstmt->setString(1, id_input);
        result = pstmt->executeQuery();
        if (result->next()) {
            string db_id = result->getString(1);
            if (db_id == id_input) {
                id_input = "";
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
            id_output = id_input;
            cout << " =�� ID üũ�Ϸ� ! " << id_output << endl;
            return 0; // ID üũ�Ϸ��϶� 0 ��ȯ
        }
    }
}
// ���� ����
int check_birth(string c_id) {
    adult_menu();
    Sleep(1500);
    pstmt = con->prepareStatement("SELECT birthday FROM userinfo WHERE id = ? and birthday < '2004.12.31'");
    pstmt->setString(1, c_id);
    result = pstmt->executeQuery();
    if (result->next()) {
        adultY_inter();
        return -1;
    }
    else {
        adultN_inter();
        Sleep(1000);
        return 0;
    }
}
// ȸ������
void sign_in() {
    name = "";
    pw = "";
    string sign_id = "";
    char guard = '+';
    isSignin = true;
    goto_xy(0, 0);
    sign_inter();
    move = '\0';
    goto_xy(14, 5);
    if (key_input(move, name, '\0', 10) < 0) {
        start = true;
        menu = false;
        chat = false;
        return;
    }
    if (check_id(sign_id, id) < 0) {
        return;
    }
    else {
        goto_xy(17, 10);
        if (key_input(move, pw, guard, 12) < 0) {
            start = true;
            menu = false;
            chat = false;
            return;
        }
        while (1) {
            birth = "";
            goto_xy(17, 12);
            for (int i = 0; i < 35; i++) {
                cout << " ";
            }
            goto_xy(17, 12);
            if (key_input(move, birth, '\0', 10) < 0) {
                start = true;
                menu = false;
                chat = false;
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
            Sleep(1500);
            start = true;
            menu = false;
            chat = false;
        }
        catch (sql::SQLException& e) {
            goto_xy(5, 14);
            cout << "SQL Error: " << e.what();
            Sleep(1500);
            start = true;
            menu = false;
            chat = false;
        }
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
        if (key_input(move, id, '\0', 8) < 0) {
            start = true;
            menu = false;
            chat = false;
            return;
        }
        goto_xy(17, 15);
        if (key_input(move, pw, guard, 12) < 0) {
            start = true;
            menu = false;
            chat = false;
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
                Sleep(1500);
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
    string beforeID = id; // ���� ID�� �����մϴ�.
    string checkID = "";
    string newID = "";
    char guard = '+';
    isSignin = false;

    while (1) {
        pw = "";
        move = '\0';
        goto_xy(0, 0);
        pw_inter();
        goto_xy(17, 10);

        if (key_input(move, pw, guard, 12) < 0) {
            login_menu(id);
            goto_xy(17, 15);
            cout << "=��";
            return;
        }

        key_ignore();
        pstmt = con->prepareStatement("SELECT pw FROM userinfo WHERE id = ?");
        pstmt->setString(1, id);
        result = pstmt->executeQuery();

        if (result->next()) {
            string db_pw = result->getString(1);

            if (db_pw == pw) {
                goto_xy(15, 11);
                cout << " =�� ��й�ȣ Ȯ�� �Ϸ� !";
                Sleep(1500);
                goto_xy(0, 0);
                change_inter();
                name = "";
                goto_xy(14, 5);

                if (key_input(move, name, '\0', 10) < 0) {
                    return;
                }

                if (check_id(checkID, newID) < 0) {
                    id = beforeID;
                    return;
                }
                else {
                    goto_xy(17, 10);
                    pw = "";
                    if (key_input(move, pw, guard, 12) < 0) {
                        return;
                    }

                    while (1) {
                        birth = "";
                        goto_xy(17, 12);

                        for (int i = 0; i < 35; i++) {
                            cout << " ";
                        }

                        goto_xy(17, 12);
                        if (key_input(move, birth, '\0', 10) < 0) {
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
                    // newID ���� id�� ����
                    id = newID;
                    pstmt = con->prepareStatement("UPDATE userinfo SET name = ?, id = ?, pw = ?, birthday = ? WHERE id = ?");
                    pstmt->setString(1, name);
                    pstmt->setString(2, newID);
                    pstmt->setString(3, pw);
                    pstmt->setString(4, birth);
                    pstmt->setString(5, beforeID);

                    try {
                        pstmt->execute(); // SQL ���� ����
                        goto_xy(5, 14);
                        cout << "�� �˸� : " << name << " ���� ȸ�������� �����Ǿ����ϴ� !";
                        Sleep(1500);
                    }
                    catch (sql::SQLException& e) {
                        goto_xy(5, 14);
                        cout << "�� ���� : " << name << " ���� ȸ������ ������ �����߽��ϴ� !" << endl;
                        goto_xy(5, 15);
                        cout << "�ڼ��� ������ ȸ�� ���� ���� ����� Ȯ���ϼ��� . . ." << endl;
                        Sleep(1500);
                    }
                }
            }
            else {
                goto_xy(5, 14);
                cout << "�� ��� : ȸ�������� ��ġ���� �ʽ��ϴ�!";
                Sleep(1000);
            }
        }
        else {
            goto_xy(5, 11);
            cout << "�� ��� : ȸ�������� ��ġ���� �ʽ��ϴ�!";
            Sleep(1000);
        }
    }
}
// ȸ������Ȯ��
void info_output() {
    // ������ name birthday ���� ��������
    std::vector<string>info;
    pstmt = con->prepareStatement("SELECT name, birthday FROM userinfo WHERE id = ?");
    pstmt->setString(1, id);
    result = pstmt->executeQuery();
    if (result->next()) {
        info.push_back(result->getString(1));
        info.push_back(result->getString(2));
    }

    // ������ ���� ��ŷ ���� ��������
    pstmt = con->prepareStatement(
        "SELECT t1.id, t1.score, "
        "CASE "
        "WHEN t1.score = max_score THEN 1 "
        "ELSE t1.score_rank "
        "END AS corrected_score_rank "
        "FROM ( "
        "SELECT id, score, RANK() OVER (ORDER BY score DESC) AS score_rank "
        "FROM game_rank "
        ") t1 "
        "JOIN ( "
        "SELECT MAX(score) AS max_score "
        "FROM game_rank "
        ") t2 "
        "WHERE t1.id = ? limit 1"
    );
    pstmt->setString(1, id);
    result = pstmt->executeQuery();
    if (result->next()) {
        info.push_back(result->getString(3));
        goto_xy(0, 0);
        info_inter();
        goto_xy(20, 5);
        cout << info[0];
        goto_xy(20, 8);
        cout << id;
        goto_xy(20, 11);
        cout << info[1];
        goto_xy(20, 14);
        cout << info[2] << " ��";
    }
    else {
        goto_xy(0, 0);
        info_inter();
        goto_xy(20, 5);
        cout << info[0];
        goto_xy(20, 8);
        cout << id;
        goto_xy(20, 11);
        cout << info[1];
        goto_xy(20, 14);
        cout << "NO RANK";
    }
}
// �޽��� ����
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
            if (msg.find("[����]") != string::npos) {
                textcolor(YELLOW, BLUE);
                cout << buf;
                textcolor(LIGHTGRAY, BLACK);
                cout << endl;
            }
            else if (msg.find("[���]") != string::npos) {
                textcolor(LIGHTRED, YELLOW);
                cout << buf;
                textcolor(LIGHTGRAY, BLACK);
                cout << endl;
            }
            else if (msg.find("[���Ӱ���]") != string::npos) {
                textcolor(LIGHTGREEN, BLACK);
                cout << buf;
                textcolor(LIGHTGRAY, BLACK);
                cout << endl;
            }
            else if (msg.find("�ӼӸ��� �����߽��ϴ�!") != string::npos) {
                textcolor(LIGHTMAGENTA, BLACK);
                cout << buf;
                textcolor(LIGHTGRAY, BLACK);
                cout << endl;
            }
            else
                cout << buf << endl;
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
// ä�� �Է�
int get_chat(char move, string& str, char ch, int i) {
    while (move != ENTER) {
        char HPKM = '\0';
        move = _getch();

        if (move == UDLR) {
            HPKM = _getch();
            if (HPKM == UP || HPKM == DOWN || HPKM == LEFT || HPKM == RIGHT) {
                key_ignore();
            }
        } // ����Ű �Է½�
        else if (str == "/out") {
            cout << endl << "ä���� �����մϴ�..." << endl;
            Sleep(1000);
            system("cls");
            chatout = true;
            return -1;
        }
        else if (move == ENTER) break; // Enter Ű�� ������ �Է� ����
        else if (move == BACKSP) { // Backspace Ű�� ���
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
// �弳 �� ��Ӿ� ����
string check_badwords(string msg) {
    std::locale::global(std::locale("korean"));
    std::ifstream readFile;
    std::vector<string> badwords;
    string line;
    string replacement;
    int idx = 0;

    // ��� ��η� �ؽ�Ʈ ���� ����
    readFile.open("../chatting/badwords.txt");

    if (!readFile.is_open()) {
        cout << "Could not open the file - '" << "badwords.txt" << "'" << endl;
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
// �޼��� �۽� ( ��üä�� )
void send_msg() {
    string text;
    while (1) {
        text = "";
        move = '\0';
        if (get_chat(move, text, '\0', 55) < 0) {
            const char* buffer = text.c_str(); // string���� char* Ÿ������ ��ȯ
            send(client_sock, buffer, strlen(buffer), 0);
            cout << endl;
            return;//�Լ� ����
        }
        if (text == "/secret") {
            cout << endl << "��� ��忡 �����մϴ� !" << endl;
            while (1) {
                text = "";
                move = '\0';
                if (get_chat(move, text, '*', 55) < 0) {
                    const char* buffer = text.c_str(); // string���� char* Ÿ������ ��ȯ
                    send(client_sock, buffer, strlen(buffer), 0);
                    cout << endl;
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
// �޼��� �۽� ( ����ä�� )
void send_msg_adult() {
    string text;
    while (1) {
        text = "";
        move = '\0';
        if (get_chat(move, text, '\0', 55) < 0) {
            text = "/out_adult";
            const char* buffer = text.c_str(); // string���� char* Ÿ������ ��ȯ
            send(client_sock, buffer, strlen(buffer), 0);
            cout << endl;
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
                    cout << endl << "�븻 ���� �����մϴ� !" << endl;
                    break;
                }
                text += "*";
                send(client_sock, text.c_str(), MAX_SIZE, 0);
                cout << endl;
            }
        }
        text += "*";
        if (text != "*") {
            send(client_sock, text.c_str(), MAX_SIZE, 0);
            cout << endl;
        }
    }
}
// ��ü ä�ù�
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
    Sleep(1000);
    clear_screen();

    send(client_sock, id.c_str(), 8, 0);
    stmt = con->createStatement();
    result = stmt->executeQuery("SELECT nickname, message, time FROM chat order by chat_num desc limit 5");
    // ��� ���
    cout << "=-=-=-=-=-=-=-=-=-= �� ��     �� �� ��     �� �� �� =-=-=-=-=-=-=-=-=-=-=-\n";
    while (result->next()) {
        recent_msg.push_back(result->getString(1) + " : " + result->getString(2) + "   " + result->getString(3));
    }
    std::reverse(recent_msg.begin(), recent_msg.end());
    for (string i : recent_msg) cout << i << endl;

    cout << "=-=-=-=-=-=-=-=-=-= �� �� ��     �� �� ��     �� �� �� =-=-=-=-=-=-=-=-=-=\n";
    pstmt = con->prepareStatement("SELECT sender, message, time FROM private_msg where receiver = ? order by chat_num desc limit 5");
    pstmt->setString(1, id);
    result = pstmt->executeQuery();
    int message_count = 0;
    while (result->next()) {
        message_count++;
        cout << result->getString(1) + " : " + result->getString(2) + "   " + result->getString(3) << endl;
    }
    cout << "=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=\n";

    std::thread th2(chat_recv);
    send_msg();
    if (chatout) {
        closesocket(client_sock);
        shutdown(client_sock, SD_RECEIVE);
        start = false;
        menu = true;
        chat = false;
        th2.join();
        return;
    }
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
    string chatting_room2_id;
    chatting_room2_id = id + "*";
    send(client_sock, chatting_room2_id.c_str(), 9, 0);
    std::thread th3(chat_recv);
    send_msg_adult();
    if (chatout) {
        closesocket(client_sock);
        shutdown(client_sock, SD_RECEIVE);
        start = false;
        menu = true;
        chat = false;
        //  thread = false;
        th3.join();
        return;
    }
    //  thread = false;
}
// Ÿ��ġ�� ���� ��ŷ �Լ�
void game_rank() {
    goto_xy(0, 0);
    game_rank_inter();
    std::vector<string>ranking;

    try {
        // ������ ���� ���� 5���� ��ŷ�� �����ɴϴ�
        stmt = con->createStatement();
        result = stmt->executeQuery(
            "SELECT id, score, score_rank "
            "FROM ( "
            "    SELECT id, score, RANK() OVER (ORDER BY score DESC) AS score_rank "
            "    FROM game_rank "
            ") ranked "
            "WHERE score_rank <= 5");

        if (!result->next()) {
            //���� 5�� ��ŷ�� ���� �� ó���� ������ �߰�
            goto_xy(15, 7);
            cout << "���� ��ŷ�� ���������ʽ��ϴ� . . ." << endl;
            // ���⿡ �߰��� ó���� ������ ���� �� �ֽ��ϴ�.
        }
        else {
            do {
                ranking.push_back(result->getString(3) + " ��     " + result->getString(1) + "          :          " + result->getString(2));
            } while (result->next());
            goto_xy(11, 7);
            cout << ranking[0];
            for (int i = 1; i < ranking.size(); i++) {
                goto_xy(11, 8 + (2 * i));
                cout << ranking[i];
            }
        }

        // ������ ���� ������� ��ŷ�� ������ ����մϴ�
        pstmt = con->prepareStatement(
            "SELECT t1.id, t1.score, "
            "CASE "
            "WHEN t1.score = max_score THEN 1 "
            "ELSE t1.score_rank "
            "END AS corrected_score_rank "
            "FROM ( "
            "SELECT id, score, RANK() OVER (ORDER BY score DESC) AS score_rank "
            "FROM game_rank "
            ") t1 "
            "JOIN ( "
            "SELECT MAX(score) AS max_score "
            "FROM game_rank "
            ") t2 "
            "WHERE t1.id = ? limit 1"
        );
        pstmt->setString(1, id);
        result = pstmt->executeQuery();

        // ������ ������� �����Ͱ� �������� �������
        if (!result->next()) {
            goto_xy(25, 20);
            cout << "NO RANK";
        }
        // ������ ���
        else {
            string myrank = result->getString(1) + "          :          " + result->getString(2);
            goto_xy(20, 20);
            cout << myrank;
            string rank_num = result->getString(3);
            goto_xy(11, 20);
            cout << rank_num << " ��";
        }
    }
    catch (sql::SQLException& e) {
        goto_xy(15, 7);
        cout << "�� ���� : " << e.what();
        // �ٸ� MySQL ���ܿ� ���� ó��
    }
}
// Ÿ��ġ�� ���ӹ�
void game_room() {
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
    string game_id = id + "#";
    send(client_sock, game_id.c_str(), 9, 0);

    int score = 0;
    bool game_over = false;
    std::vector<string>voca;
    stmt = con->createStatement();
    result = stmt->executeQuery("SELECT voca FROM chatting.game_voca order by rand()");
    while (result->next()) {
        voca.push_back(result->getString(1));
    }
    cout << "���ݺ��� ������ �����մϴ� . . ." << endl;
    cout << "���ѽð� 30�� ���� ������ �ܾ ��Ȯ�� �Է����ּ��� !" << endl;
    Sleep(1000);
    game_inter();
    clock_t startTime = clock(); // �ð� ���� ����
    while (!game_over) {
        for (int i = 0; i < voca.size(); i++) {
            string word = "";

            goto_xy(20, 10);
            cout << "                            "; // y10 ���� �������� �����
            goto_xy(20, 12);
            cout << "                            "; // y12 ���� �������� �����
            goto_xy(20, 10);
            cout << voca.at(i);
            goto_xy(20, 12);
            cin >> word;
            clock_t endTime = clock(); // �ð� ���� ��

            goto_xy(20, 14);
            cout << "                            "; // y14 ���� �������� �����
            if ((endTime - startTime) / CLOCKS_PER_SEC > 30) {
                goto_xy(20, 12);
                cout << "Ÿ�� ����...";
                game_over = true;
                break;
            }

            if (word == voca.at(i)) {
                score++;
                goto_xy(20, 14);
                cout << "�����Դϴ� !";
                goto_xy(20, 16);
                cout << score;
                goto_xy(47, 18);
                cout << (endTime - startTime) / CLOCKS_PER_SEC << " ��";
            }
            else {
                goto_xy(20, 14);
                cout << "�����Դϴ� . . .";
                goto_xy(20, 16);
                cout << score;
                goto_xy(47, 18);
                cout << (endTime - startTime) / CLOCKS_PER_SEC << " ��";
            }
        }
    }
    pstmt = con->prepareStatement("INSERT INTO game_rank(id, score) VALUES(?,?)");
    pstmt->setString(1, id);
    pstmt->setInt(2, score);
    string msg = id + " " + std::to_string(score);
    send(client_sock, msg.c_str(), MAX_SIZE, 0);

    try {
        pstmt->execute(); // SQL ���� ����
        goto_xy(5, 14);
        cout << id << "���� SCORE�� ��ϵǾ����ϴ� !";
        Sleep(1500);
        start = false;
        menu = true;
        chat = false;
    }
    catch (sql::SQLException& e) {
        goto_xy(5, 14);
        cout << "�� ���� : SQL Error ! " << e.what();
        Sleep(1500);
        start = false;
        menu = true;
        chat = false;
    }

}

int main() {
    bool interFace = true;
    WSADATA wsa;
    start_sql();
    int code = WSAStartup(MAKEWORD(2, 2), &wsa);

    while (interFace) {
        if (start) {
            key_ignore();
            num = LOG_IN;
            start_menu();
            goto_xy(17, 13);
            cout << "=��";
        }

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
                        menu = false;
                        chat = false;
                        interFace = false;
                        break;
                    }
                    else if (move == ESC) {
                        start_menu();
                        goto_xy(17, 17);
                        cout << "=��";
                    }
                }
                break;
            case LOG_IN:
            case SIGN_IN:
                start_menu();
                goto_xy(17, 13 + (num - LOG_IN) * 2);
                cout << "=��";
                if (move == ENTER) {
                    start = false;
                    menu = true;
                }
                break;
            default:
                break;
            }
        }

        if (menu) {
            switch (num) {
            case LOG_IN:
                log_in();
                key_ignore();
                num = CHAT_IN;
                login_menu(id);
                goto_xy(17, 13);
                cout << "=��";
                break;
            case SIGN_IN:
                clear_screen();
                sign_in();
                break;
            default:
                num = CHAT_IN;
                login_menu(id);
                goto_xy(17, 13);
                cout << "=��";
                break;
            }
        }

        while (menu) {
            move = _getch();

            if (move == DOWN) {
                ++num;
                if (num > MYPAGE) {
                    num = LOG_OUT;
                }
            }
            else if (move == UP) {
                --num;
                if (num < LOG_OUT) {
                    num = MYPAGE;
                }
            }

            switch (num) {
            case LOG_OUT:
                login_menu(id);
                goto_xy(17, 17);
                cout << "=��";
                if (move == ENTER) {
                    logout_menu();
                    move = _getch();
                    if (move == ENTER) {
                        start = true;
                        menu = false;
                        chat = false;
                    }
                    else if (move == ESC) {
                        login_menu(id);
                        goto_xy(17, 17);
                        cout << "=��";
                    }
                }
                break;
            case CHAT_IN:
                login_menu(id);
                goto_xy(17, 13);
                cout << "=��";
                if (move == ENTER) {
                    key_ignore();
                    menu = false;
                    chat = true;
                }
                else if (move == ESC) {
                    login_menu(id);
                    goto_xy(17, 13);
                    cout << "=��";
                }
                break;
            case MYPAGE:
                login_menu(id);
                goto_xy(17, 15);
                cout << "=��";
                if (move == ENTER) {
                    key_ignore();
                    num = OUTPUT_INFO;
                    menu = false;
                    info = true;
                    chat = false;
                    info_menu();
                    goto_xy(17, 13);
                    cout << "=��";
                }
                else if (move == ESC) {
                    login_menu(id);
                    goto_xy(17, 15);
                    cout << "=��";
                }
                break;
            default:
                break;
            }
        }

        while (info) {
            move = _getch();
            if (move == DOWN) {
                ++num;
                if (num > CHANGE_INFO) {
                    num = OUTPUT_INFO;
                }
            }
            else if (move == UP) {
                --num;
                if (num < OUTPUT_INFO) {
                    num = CHANGE_INFO;
                }
            }
            else if (move == ESC) {
                info = false;
                menu = true;
                num = CHAT_IN;
                break;
            }

            switch (num) {
            case OUTPUT_INFO:
                info_menu();
                goto_xy(17, 13);
                cout << "=��";
                if (move == ENTER) {
                    info_output();
                    move = _getch();
                    if (move == ESC) {
                        info_menu();
                        goto_xy(17, 13);
                        cout << "=��";
                    }
                }
                break;
            case CHANGE_INFO:
                info_menu();
                goto_xy(17, 16);
                cout << "=��";
                if (move == ENTER) {
                    change_info();
                    info_menu();
                    goto_xy(17, 16);
                    cout << "=��";
                }
                break;
            default:
                break;
            }
        }

        if (!code && chat) {
            num = ALL_CHAT;
            chat_menu();
            goto_xy(17, 13);
            cout << "=��";
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
                menu = true;
                num = CHAT_IN;
                break;
            }

            switch (num) {
            case ALL_CHAT:
            case ADULT_CHAT:
            case GAME:
                chat_menu();
                goto_xy(17, 13 + (num - ALL_CHAT) * 2);
                cout << "=��";
                if (move == ENTER) {
                    chat = false;
                    menu = false;
                }
                else if (move == ESC) {
                    chat = false;
                    menu = true;
                    num = CHAT_IN;
                    break;
                }
                break;
            default:
                break;
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
        }
        else if (num == GAME) {
            num = GAME_START;
            key_ignore();
            game_menu();
            goto_xy(20, 11);
            cout << "=��";
            game = true;
        }

        while (game) {
            move = _getch();

            if (move == DOWN) {
                ++num;
                if (num > GAME_OUT) {
                    num = GAME_START;
                }
            }
            else if (move == UP) {
                --num;
                if (num < GAME_START) {
                    num = GAME_OUT;
                }
            }

            switch (num) {
            case GAME_START:
                game_menu();
                goto_xy(20, 11);
                cout << "=��";
                if (move == ENTER) {
                    clear_screen();
                    cout << "Ÿ��ġ�� ������ �����մϴ� ! " << endl;
                    Sleep(1000);
                    game = false;
                    game_room();
                }
                break;
            case GAME_RULE:
                game_menu();
                goto_xy(20, 13);
                cout << "=��";
                if (move == ENTER) {
                    game_rule();
                    move = _getch();
                    if (move == ESC) {
                        game_menu();
                        goto_xy(20, 13);
                        cout << "=��";
                        break;
                    }
                }
                break;
            case GAME_RANK:
                game_menu();
                goto_xy(20, 15);
                cout << "=��";
                if (move == ENTER) {
                    game_rank();
                    move = _getch();
                    if (move == ESC) {
                        game_menu();
                        goto_xy(20, 15);
                        cout << "=��";
                        break;
                    }
                }
                break;
            case GAME_OUT:
                game_menu();
                goto_xy(20, 17);
                cout << "=��";
                if (move == ENTER) {
                    game_out_inter();
                    move = _getch();
                    if (move == ESC) {
                        game_menu();
                        goto_xy(20, 17);
                        cout << "=��";
                        break;
                    }
                    else if (move == ENTER) {
                        chat = true;
                        game = false;
                        num = ALL_CHAT;
                    }
                }
                break;
            }
        }
    }

    closesocket(client_sock);
    WSACleanup();
}