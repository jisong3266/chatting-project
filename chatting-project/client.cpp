#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h> //Winsock 헤더파일 include. WSADATA 들어있음.
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <Windows.h> // clear_screen()를 사용하기 위한 헤더
#include <conio.h> // _getch()를 사용하기 위한 헤더 
#include <mysql/jdbc.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <fstream>

#define MAX_SIZE 1024
#define ENTER 13
#define UDLR 224
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27
#define BACKSP 8 // 아스키 코드 정의

using std::cout;
using std::cin;
using std::endl;
using std::string; // namespace 정의

sql::mysql::MySQL_Driver* driver; // 추후 해제하지 않아도 Connector/C++가 자동으로 해제해 줌
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

SOCKET client_sock;
bool start = true;
//bool thread = true;
bool menu = true;
bool chat = true;
bool isSignin = false;
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

const string server = "tcp://127.0.0.1:3306"; // 데이터베이스 주소
const string username = "root"; // 데이터베이스 사용자
const string password = "goflvhxj"; // 데이터베이스 접속 비밀번호

// 화면 초기화
void clear_screen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };  // 화면 좌상단 좌표
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // 콘솔 화면 크기 가져오기
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // 화면을 공백 문자로 지우기
    if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten)) {
        return;
    }

    // 커서 위치 초기화
    SetConsoleCursorPosition(hConsole, coordScreen);
}
// 커서 깜빡임 제거
void cursor_view(bool show)
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
// 커서 이동 함수
void goto_xy(int x, int y) {
    COORD posXY = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posXY);
}
// 인터페이스
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
    cout << " "; cout << "*                    < 시작 화면 >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    1. 로그인                         *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    2. 회원가입                       *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    0. 종료                           *\n";
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
    cout << " "; cout << "*                    <   종 료   >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*           =▶  프로그램을 종료하시겠습니까?           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                [ENTER를 누르면 종료]                 *\n";
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
    cout << " "; cout << "*                    <  로 그 인  >                    *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    아이디 :                                          *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    비밀번호 :                                        *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*             [ESC를 누르면 시작화면으로]              *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
void sign_inter()
{
    cursor_view(true);
    cout << "\n";
    cout << " "; cout << "********************************************************\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                   < 회 원 가 입 >                    *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    이 름 :                                           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    아이디 :                                          *\n";
    cout << " "; cout << "*            (영문과 숫자 포함 8자리로 입력)           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    비밀번호 :                                        *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    생년월일 :                                        *\n";
    cout << " "; cout << "*             (YYYY-MM-DD로 입력)                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*             [ESC를 누르면 시작화면으로]              *\n";
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
    cout << " "; cout << "*                < 회 원 정 보 수 정 >                 *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*            ※ 비밀번호를 입력해주세요 !               *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    비밀번호 :                                        *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*             [ESC를 누르면 이전화면으로]              *\n";
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
    cout << " "; cout << "*                < 회 원 정 보 수 정 >                 *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    이 름 :                                           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    아이디 :                                          *\n";
    cout << " "; cout << "*            (영문과 숫자 포함 8자리로 입력)           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    비밀번호 :                                        *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    생년월일 :                                        *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*             [ESC를 누르면 이전화면으로]              *\n";
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
    cout << " "; cout << "*                 " << id << "님 환영합니다!               * \n";
    cout << " "; cout << "*                    <   메 뉴   >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    1. 채팅방 입장                    *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    2. 회원정보 수정                  *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    0. 로그아웃                       *\n";
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
    cout << " "; cout << "*                    < 로그아웃 >                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*           =▶  로그아웃 하시겠습니까?                 *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*               [ENTER를 누르면 로그아웃]              *\n";
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
    cout << " "; cout << "*                    < 채팅방 입장 >                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    1. 전체 채팅방                    *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    2. 성인 채팅방                    *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    3. 끝말잇기 게임방                *\n";
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
    cout << " "; cout << "*                   < 성인 확인 중 >                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*           =▶  잠시만 기다려주세요 . . .              *\n";
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
    cout << " "; cout << "             *          *      * * * *                 *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*             <  성인 채팅방 입장 중  >                *\n";
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
    cout << " "; cout << "*                 < 성인 확인 완료 >                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*              =▶  성인이 아닙니다 !                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}
// start sql
void start_sql()
{
    // MySQL Connector/C++ 초기화
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(server, username, password);
    }
    catch (sql::SQLException& e) {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        exit(1);
    }

    // 데이터베이스 선택
    con->setSchema("chatting");

    // db 한글 저장을 위한 셋팅 
    stmt = con->createStatement();
    stmt->execute("set names euckr");
    if (stmt) { delete stmt; stmt = nullptr; }
}
// 키보드 입력
int key_input(char move, string& str, char ch, int i) {
    while (move != ENTER) {
        move = _getch();
        if (move == UP || move == DOWN || move == LEFT || move == RIGHT) return -1; // 방향키 입력시
        if (move == ESC) return -1; // ESC 키를 누르면 이전 화면으로
        if (move == ENTER) break; // Enter 키를 누르면 입력 종료
        if (move == BACKSP) { // Backspace 키인 경우
            if (!str.empty()) { // 입력된 문자가 있으면
                str.pop_back(); // 마지막 문자를 삭제
                cout << "\b \b"; // 커서 위치를 왼쪽으로 이동시켜 공백을 출력한 뒤, 다시 커서 위치를 원래대로 이동시킴
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
// 키보드 입력 무시
void key_ignore() {
    while (_kbhit()) {
        char ch = _getch(); // 키 입력을 읽어서 버퍼에서 제거
    }
}
// id 체크
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
            return -1; // ESC일때 -1 반환
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
            cout << "※ 경고 : 잘못된 형식입니다 !";
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
                cout << "※ 경고 : 중복된 ID입니다 !";
                continue;
                // 아이디가 중복됨
            }
        }
        else {
            // 아이디 중복 없음
            goto_xy(2, 8);
            cout << "                                                      ";
            goto_xy(13, 8);
            id_output = id_input;
            cout << " =▶ ID 체크완료 ! " << id_output << endl;
            return 0; // ID 체크완료일때 0 반환
        }
    }
}
// 성인 인증
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
// 회원가입
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

            // 문자열 스트림을 사용하여 파싱
            std::istringstream ss(birth);
            ss >> year >> dash1 >> month >> dash2 >> day;

            if (ss && dash1 == '-' && dash2 == '-' && month < 13 && day < 32) {
                goto_xy(2, 13);
                cout << "                                                      ";
                break;
            }
            else {
                goto_xy(13, 13);
                cout << "※ 경고 : 잘못된 형식입니다!";
                continue;
            }
        }
        pstmt = con->prepareStatement("INSERT INTO userinfo(name, id, pw, birthday) VALUES(?,?,?,?)");
        pstmt->setString(1, name);
        pstmt->setString(2, id);
        pstmt->setString(3, pw);
        pstmt->setString(4, birth);
        try {
            pstmt->execute(); // SQL 쿼리 실행
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
// 로그인
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
                cout << " =▶ 로그인 완료 ! ";
                Sleep(1500);
                break;
            }
            else {
                clear_screen();
                goto_xy(10, 20);
                cout << "※ 경고 : 회원정보가 일치하지 않습니다!";
            }
        }
        else {
            clear_screen();
            goto_xy(10, 20);
            cout << "※ 경고 : 회원정보가 일치하지 않습니다!";
        }
    }
}
// 회원정보수정
void change_info() {
    string beforeID = id; // 이전 ID를 저장합니다.
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
            cout << "=▶";
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
                cout << " =▶ 비밀번호 확인 완료 !";
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

                        // 문자열 스트림을 사용하여 파싱
                        std::istringstream ss(birth);
                        ss >> year >> dash1 >> month >> dash2 >> day;

                        if (ss && dash1 == '-' && dash2 == '-' && month < 13 && day < 32) {
                            goto_xy(2, 13);
                            cout << "                                                      ";
                            break;
                        }
                        else {
                            goto_xy(13, 13);
                            cout << "※ 경고 : 잘못된 형식입니다!";
                            continue;
                        }
                    }
                    // newID 값을 id로 복사
                    id = newID;
                    pstmt = con->prepareStatement("UPDATE userinfo SET name = ?, id = ?, pw = ?, birthday = ? where id = ?");
                    pstmt->setString(1, name);
                    pstmt->setString(2, newID);
                    pstmt->setString(3, pw);
                    pstmt->setString(4, birth);
                    pstmt->setString(5, beforeID);

                    try {
                        pstmt->execute(); // SQL 쿼리 실행
                        goto_xy(5, 14);
                        cout << "※ 알림 : " << name << " 님의 회원정보가 수정되었습니다 !";
                        Sleep(1500);
                    }
                    catch (sql::SQLException& e) {
                        goto_xy(5, 14);
                        cout << "※ 오류 : " << name << " 님의 회원정보 수정이 실패했습니다 !" << endl;
                        goto_xy(5, 15);
                        cout << "자세한 사항은 회원 정보 수정 양식을 확인하세요 . . ." << endl;
                        Sleep(1500);
                    }
                }
            }
            else {
                goto_xy(5, 14);
                cout << "※ 경고 : 회원정보가 일치하지 않습니다!";
                Sleep(1000);
            }
        }
        else {
            goto_xy(5, 11);
            cout << "※ 경고 : 회원정보가 일치하지 않습니다!";
            Sleep(1000);
        }
    }
}

//void change_info() {
//    string checkID="";
//    string newID="";
//    char guard = '+';
//    isSignin = false;
//    while (1) {
//        pw = "";
//        move = '\0';
//        goto_xy(0, 0);
//        pw_inter();
//        goto_xy(17, 10);
//
//        if (key_input(move, pw, guard, 12) < 0) {
//            login_menu(id);
//            goto_xy(17, 15);
//            cout << "=▶";
//            return;
//        }
//
//        key_ignore();
//        pstmt = con->prepareStatement("SELECT pw FROM userinfo WHERE id = ?");
//        pstmt->setString(1, id);
//        result = pstmt->executeQuery();
//
//        if (result->next()) {
//            string db_pw = result->getString(1);
//
//            if (db_pw == pw) {
//                goto_xy(15, 11);
//                cout << " =▶ 비밀번호 확인 완료 !";
//                Sleep(1500);
//                goto_xy(0, 0);
//                change_inter();
//                name = "";
//                goto_xy(14, 5);
//
//                if (key_input(move, name, '\0', 10) < 0) {
//                    return;
//                }
//
//                if (check_id(checkID, newID) < 0) {
//                    return;
//                }
//                else {
//
//                    goto_xy(17, 10);
//                    pw = "";
//                    if (key_input(move, pw, guard, 12) < 0) {
//                        return;
//                    }
//                    while (1) {
//                        birth = "";
//                        goto_xy(17, 12);
//                        for (int i = 0; i < 35; i++) {
//                            cout << " ";
//                        }
//                        goto_xy(17, 12);
//                        if (key_input(move, birth, '\0', 10) < 0) {
//                            return;
//                        }
//
//                        int year, month, day;
//                        char dash1, dash2;
//
//                        // 문자열 스트림을 사용하여 파싱
//                        std::istringstream ss(birth);
//                        ss >> year >> dash1 >> month >> dash2 >> day;
//
//                        if (ss && dash1 == '-' && dash2 == '-' && month < 13 && day < 32) {
//                            goto_xy(2, 13);
//                            cout << "                                                      ";
//                            break;
//                        }
//                        else {
//                            goto_xy(13, 13);
//                            cout << "※ 경고 : 잘못된 형식입니다!";
//                            continue;
//                        }
//                    }
//                    pstmt = con->prepareStatement("UPDATE userinfo SET name = ?, id = ?, pw = ?, birthday = ? where id =?");
//                    pstmt->setString(1, name);
//                    pstmt->setString(2, newID);
//                    pstmt->setString(3, pw);
//                    pstmt->setString(4, birth);
//                    pstmt->setString(5, id);
//                    try {
//                        pstmt->execute(); // SQL 쿼리 실행
//                        goto_xy(5, 14);
//                        cout << "※ 알림 : " << name << " 님의 회원정보가 수정되었습니다 !";
//                        Sleep(1500);
//                    }
//                    catch (sql::SQLException& e) {
//                        goto_xy(5, 14);
//                        cout << "※ 오류 : " << name << " 님의 회원정보 수정이 실패했습니다 !" << endl;
//                        goto_xy(5, 15);
//                        cout << "자세한 사항은 회원 정보 수정 양식을 확인하세요 . . ." << endl;
//                        Sleep(1500);
//                    }
//                }
//            }
//            else {
//                goto_xy(5, 14);
//                cout << "※ 경고 : 회원정보가 일치하지 않습니다!";
//                Sleep(1000);
//            }
//        }
//        else {
//            goto_xy(5, 11);
//            cout << "※ 경고 : 회원정보가 일치하지 않습니다!";
//            Sleep(1000);
//        }
//    }
//}
// 메시지 받기
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
            // 서버가 연결을 끊었을 때
            cout << "Server disconnected" << endl;
            return -1;
        }
        else {
            // 오류 발생
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
            cout << endl << "채팅을 종료합니다..." << endl;
            chatout = true;
            return -1;
        }
        if (move == ENTER) break; // Enter 키를 누르면 입력 종료
        if (move == BACKSP) { // Backspace 키인 경우
            if (!str.empty()) { // 입력된 문자가 있으면
                str.pop_back(); // 마지막 문자를 삭제
                cout << "\b \b"; // 커서 위치를 왼쪽으로 이동시켜 공백을 출력한 뒤, 다시 커서 위치를 원래대로 이동시킴
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
    readFile.open("C:/Users/지니/source/repos/Project1/badwords.txt");
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
                msg.replace(idx, i.length(), replacement); // idx부터 i.length()까지 다 "-"로 대체
                idx = msg.find(i, idx + 1); // 다음 일치 항목을 찾기 위해 인덱스를 이동
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
            const char* buffer = text.c_str(); // string형을 char* 타입으로 변환
            send(client_sock, buffer, strlen(buffer), 0);
            cout << endl;
            return;//함수 종료
        }
        if (text == "/secret") {
            cout << endl << "비밀 모드에 진입합니다 !" << endl;
            while (1) {
                text = "";
                move = '\0';
                if (get_chat(move, text, '*', 55) < 0) {
                    const char* buffer = text.c_str(); // string형을 char* 타입으로 변환
                    send(client_sock, buffer, strlen(buffer), 0);
                    cout << endl;
                    return;
                }
                if (text == "/normal") {
                    text = "";
                    cout << endl << "노말 모드로 복귀합니다 !";
                    break;
                }
                text = check_badwords(text);
                const char* buffer = text.c_str(); // string형을 char* 타입으로 변환
                send(client_sock, buffer, strlen(buffer), 0);
                cout << endl;
            }
        }
        text = check_badwords(text);
        const char* buffer = text.c_str(); // string형을 char* 타입으로 변환
        send(client_sock, buffer, strlen(buffer), 0);
        cout << endl;
    }
}
void send_msg_adult() {
    string text;
    while (1) {
        text = "";
        move = '\0';
        if (get_chat(move, text, '\0', 55) < 0) {
            text += "_adult";
            const char* buffer = text.c_str(); // string형을 char* 타입으로 변환
            send(client_sock, buffer, strlen(buffer), 0);
            cout << endl;
            return;//함수 종료
        }
        if (text == "/secret") {
            cout << endl << "비밀 모드에 진입합니다 !" << endl;
            while (1) {
                text = "";
                move = '\0';
                if (get_chat(move, text, '*', 55) < 0) {
                    return;
                }
                if (text == "/normal") {
                    text = "";
                    cout << endl << "노말 모드로 복귀합니다 !" << endl;
                    break;
                }
                text = check_badwords(text);
                text += "*";
                send(client_sock, text.c_str(), MAX_SIZE, 0);
                cout << endl;
            }
        }
        text = check_badwords(text);
        text += "*";
        if (text != "*") {
            send(client_sock, text.c_str(), MAX_SIZE, 0);
            cout << endl;
        }
    }
}
// 전체 채팅방
void chatting_room1() {
    chatout = false;
    client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 연결할 서버 정보 설정 부분
    SOCKADDR_IN client_addr = {};
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(7777);
    InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);
    while (1) {
        if (!connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) { // 위에 설정한 정보에 해당하는 server로 연결!
            cout << "Server Connect" << endl;
            break;
        }
        else
            cout << "Connecting..." << endl;
    }
    cursor_view(true);
    std::vector<string>recent_msg;
    cout << "전체채팅방에 입장합니다!" << endl;
    send(client_sock, id.c_str(), 8, 0);
    stmt = con->createStatement();
    result = stmt->executeQuery("SELECT nickname, message, time FROM chat order by chat_num desc limit 5");
    // 결과 출력
    cout << "\n================최근 메세지입니다================\n";
    while (result->next()) {
        recent_msg.push_back(result->getString(1) + " : " + result->getString(2) + "   " + result->getString(3));
    }
    std::reverse(recent_msg.begin(), recent_msg.end());
    for (string i : recent_msg) cout << i << endl;
    cout << "\n================귓속말 메세지입니다================\n";
    pstmt = con->prepareStatement("SELECT sender, message, time FROM private_msg where receiver = ?");
    pstmt->setString(1, id);
    result = pstmt->executeQuery();
    while (result->next()) {
        cout << result->getString(1) + " : " + result->getString(2) + "   " + result->getString(3) << endl;
    }    std::thread th2(chat_recv);
    send_msg();
    if (chatout) {
        closesocket(client_sock);
        //shutdown(client_sock, SD_RECEIVE);
        //Sleep(1000);
        start = false;
        menu = true;
        chat = false;
        //  thread = false;
        th2.join();
        return;
    }
    //  thread = false;
    th2.join();

}
//성인 채팅방
void chatting_room2() {
    client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 연결할 서버 정보 설정 부분
    SOCKADDR_IN client_addr = {};
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(7777);
    InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);
    while (1) {
        if (!connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) { // 위에 설정한 정보에 해당하는 server로 연결!
            cout << "Server Connect" << endl;
            break;
        }
        else
            cout << "Connecting..." << endl;
    }
    cursor_view(true);
    cout << "성인 채팅방에 입장합니다 ! " << endl;
    string game1_id;
    game1_id = id + "*";
    send(client_sock, game1_id.c_str(), 9, 0);
    std::thread th3(chat_recv);
    send_msg_adult();
    if (chatout) {
        closesocket(client_sock);
        //shutdown(client_sock, SD_RECEIVE);
        start = false;
        menu = true;
        chat = false;
        //  thread = false;
        th3.join();
        return;
    }
    //  thread = false;
}
//끝말잇기 게임

int main() {
    bool interFace = true;

    WSADATA wsa;
    // Winsock를 초기화하는 함수. MAKEWORD(2, 2)는 Winsock의 2.2 버전을 사용하겠다는 의미.
    // 실행에 성공하면 0을, 실패하면 그 이외의 값을 반환.
    // 0을 반환했다는 것은 Winsock을 사용할 준비가 되었다는 의미.
    start_sql();
    int code = WSAStartup(MAKEWORD(2, 2), &wsa);

    while (interFace) {

        if (start) {
            key_ignore();
            num = LOG_IN;
            start_menu();
            goto_xy(17, 13);
            cout << "=▶";
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
                cout << "=▶";
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
                        cout << "=▶";
                    }
                }
                break;
            case LOG_IN:
                start_menu();
                goto_xy(17, 13);
                cout << "=▶";
                if (move == ENTER) {
                    start = false;
                    menu = true;
                }
                break;
            case SIGN_IN:
                start_menu();
                goto_xy(17, 15);
                cout << "=▶";
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
                cout << "=▶";
                break;
            case SIGN_IN:
                clear_screen();
                sign_in();
                break;
            default:
                num = CHAT_IN;
                login_menu(id);
                goto_xy(17, 13);
                cout << "=▶";
                break;
            }
        }
        while (menu) {
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
                login_menu(id);
                goto_xy(17, 17);
                cout << "=▶";
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
                        cout << "=▶";
                    }
                }
                break;
            case CHAT_IN:
                login_menu(id);
                goto_xy(17, 13);
                cout << "=▶";
                if (move == ENTER) {
                    key_ignore();
                    menu = false;
                    chat = true;
                }
                else if (move == ESC) {
                    login_menu(id);
                    goto_xy(17, 13);
                    cout << "=▶";
                }
                break;
            case CHANGE:
                login_menu(id);
                goto_xy(17, 15);
                cout << "=▶";
                if (move == ENTER) {
                    change_info();
                    login_menu(id);
                    goto_xy(17, 15);
                    cout << "=▶";
                }
                else if (move == ESC) {
                    login_menu(id);
                    goto_xy(17, 15);
                    cout << "=▶";
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
            cout << "=▶";
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
                chat_menu();
                goto_xy(17, 13);
                cout << "=▶";
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
            case ADULT_CHAT:
                chat_menu();
                goto_xy(17, 15);
                cout << "=▶";
                if (move == ENTER) {
                    if (check_birth(id) < 0) {
                        chat = false;
                        menu = false;
                    }
                    else {
                        chat = false;
                        menu = true;
                        num = CHAT_IN;
                        break;
                    }
                }
                else if (move == ESC) {
                    chat = false;
                    menu = true;
                    num = CHAT_IN;
                    break;
                }
                break;
            case GAME:
                chat_menu();
                goto_xy(17, 17);
                cout << "=▶";
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
            clear_screen();
            cout << "아직 없는 기능입니다...";
        }
    }
    closesocket(client_sock);
    WSACleanup();
}
