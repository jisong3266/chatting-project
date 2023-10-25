#pragma comment(lib, "ws2_32.lib") // 명시적인 라이브러리의 링크. 윈속 라이브러리 참조

#include <WinSock2.h> // Winsock 헤더파일 include. WSADATA 들어있음.
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread> // 쓰레드 헤더
#include <Windows.h> // clear_screen()를 사용하기 위한 헤더
#include <conio.h> // _getch()를 사용하기 위한 헤더 
#include <mysql/jdbc.h> // SQL 헤더
#include <vector>
#include <cstdlib>
#include <ctime> // 시간 측정을 위한 헤더
#include <cstdio>
#include <fstream> // 텍스트 파일을 쓰기 위한 헤더

#define MAX_SIZE 1024
#define ENTER 13
#define UDLR -32
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
sql::ResultSet* result; // sql 정의

SOCKET client_sock; // 소켓 정의

bool start = true;
bool menu = true;
bool chat = true;
bool info = false;
bool isSignin = false;
bool chatout = false;
bool game = false;
char move;
int num;
string name, id, pw, birth; // 전역변수 정의

// 인터페이스 이동 enum
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
// 색상 enum
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

const string server = "tcp://127.0.0.1:3306"; // 데이터베이스 주소
const string username = "root"; // 데이터베이스 사용자
const string password = "1234"; // 데이터베이스 접속 비밀번호

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
void cursor_view(bool show) {
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
// 색깔 변경 함수
void textcolor(int foreground, int background) {
    int color = foreground + background * 16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
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
    cout << " "; cout << "*                 " << id << "님 환영합니다!               *\n";
    cout << " "; cout << "*                    <   메 뉴   >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    1. 채팅방 입장                    *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    2. 마이페이지                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    0. 로그아웃                       *\n";
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
    cout << " "; cout << "*                   <  마이페이지  >                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                   1. 회원정보 확인                   *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                   2. 회원정보 수정                   *\n";
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
    cout << " "; cout << "*                     < 회 원 정 보  >                 *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    이 름 :                                           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    아이디 :                                          *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    생년월일 :                                        *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*    랭킹 :                                            *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*             [ESC를 누르면 이전화면으로]              *\n";
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
    cout << " "; cout << "*                    3. 타자치기 게임방                *\n";
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
    cout << " "; cout << "*            *          *      * * * *                 *\n";
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
    cout << " "; cout << "*                         1. 게임 시작                          *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                         2. 게임 룰 소개                       *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                         3. 랭킹 보기                          *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                         4. 게임 종료                          *\n";
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
    cout << " "; cout << "*       =▶ 타자 치기 게임이란 . . .                             *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*      30초 동안 콘솔창 화면에 제시되는 단어를 똑같이 입력하면  *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*      성공 아니면 실패로 제한시간 내에 얼마나 많은 단어를      *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*      입력하는지를 겨루는 순발력 게임입니다 !                  *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                 [ ESC를 눌러 이전화면으로 ]                   *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*****************************************************************\n\n";
}
void game_rank_inter() {
    clear_screen();
    cursor_view(false);
    cout << "\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                         <  랭  킹 >                           *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*         순위     아 이 디                   점 수             *\n";
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
    cout << " "; cout << "*     [ 내 랭킹 ]                                               *\n";
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
    cout << " "; cout << "*                          <   종 료   >                        *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*               =▶  게임을 종료하시겠습니까 ?                   *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                     [ ENTER를 누르면 종료 ]                   *\n";
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
    cout << " "; cout << "*      제시어 :                                                 *\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*            →                                                  *\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*****************************************************************\n";
    cout << " "; cout << "*   현재 스코어 :           점                                  *\n";
    cout << " "; cout << "*                                                               *\n";
    cout << " "; cout << "*                                 경과시간 :          / 30 초   *\n";
    cout << " "; cout << "*****************************************************************\n\n";
}

// start SQL
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
// 키보드 입력 무시
void key_ignore() {
    while (_kbhit()) {
        char ch = _getch(); // 키 입력을 읽어서 버퍼에서 제거
    }
}
// 키보드 입력
int key_input(char move, string& str, char ch, int i) {
    while (move != ENTER) {
        char HPKM = '\0';
        move = _getch();

        if (move == UDLR) { // 방향키 입력시 -32값을 받아옴
            HPKM = _getch();
            if (HPKM == UP || HPKM == DOWN || HPKM == LEFT || HPKM == RIGHT) {
                key_ignore();
            }
        } 
        else if (move == ESC) return -1; // ESC 키를 누르면 이전 화면으로
        else if (move == ENTER) break; // Enter 키를 누르면 입력 종료
        else if (move == BACKSP) { // Backspace 키인 경우
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
                    pstmt = con->prepareStatement("UPDATE userinfo SET name = ?, id = ?, pw = ?, birthday = ? WHERE id = ?");
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
// 회원정보확인
void info_output() {
    // 쿼리로 name birthday 정보 가져오기
    std::vector<string>info;
    pstmt = con->prepareStatement("SELECT name, birthday FROM userinfo WHERE id = ?");
    pstmt->setString(1, id);
    result = pstmt->executeQuery();
    if (result->next()) {
        info.push_back(result->getString(1));
        info.push_back(result->getString(2));
    }

    // 쿼리를 통해 랭킹 정보 가져오기
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
        cout << info[2] << " 위";
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
// 메시지 수신
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
            if (msg.find("[공지]") != string::npos) {
                textcolor(YELLOW, BLUE);
                cout << buf;
                textcolor(LIGHTGRAY, BLACK);
                cout << endl;
            }
            else if (msg.find("[경고]") != string::npos) {
                textcolor(LIGHTRED, YELLOW);
                cout << buf;
                textcolor(LIGHTGRAY, BLACK);
                cout << endl;
            }
            else if (msg.find("[게임공지]") != string::npos) {
                textcolor(LIGHTGREEN, BLACK);
                cout << buf;
                textcolor(LIGHTGRAY, BLACK);
                cout << endl;
            }
            else if (msg.find("귓속말이 도착했습니다!") != string::npos) {
                textcolor(LIGHTMAGENTA, BLACK);
                cout << buf;
                textcolor(LIGHTGRAY, BLACK);
                cout << endl;
            }
            else
                cout << buf << endl;
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
// 채팅 입력
int get_chat(char move, string& str, char ch, int i) {
    while (move != ENTER) {
        char HPKM = '\0';
        move = _getch();

        if (move == UDLR) {
            HPKM = _getch();
            if (HPKM == UP || HPKM == DOWN || HPKM == LEFT || HPKM == RIGHT) {
                key_ignore();
            }
        } // 방향키 입력시
        else if (str == "/out") {
            cout << endl << "채팅을 종료합니다..." << endl;
            Sleep(1000);
            system("cls");
            chatout = true;
            return -1;
        }
        else if (move == ENTER) break; // Enter 키를 누르면 입력 종료
        else if (move == BACKSP) { // Backspace 키인 경우
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
// 욕설 및 비속어 감지
string check_badwords(string msg) {
    std::locale::global(std::locale("korean"));
    std::ifstream readFile;
    std::vector<string> badwords;
    string line;
    string replacement;
    int idx = 0;

    // 상대 경로로 텍스트 파일 열기
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
                msg.replace(idx, i.length(), replacement); // idx부터 i.length()까지 다 "-"로 대체
                idx = msg.find(i, idx + 1); // 다음 일치 항목을 찾기 위해 인덱스를 이동
            }
        }
    }
    return msg;
}
// 메세지 송신 ( 전체채팅 )
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
// 메세지 송신 ( 성인채팅 )
void send_msg_adult() {
    string text;
    while (1) {
        text = "";
        move = '\0';
        if (get_chat(move, text, '\0', 55) < 0) {
            text = "/out_adult";
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
// 전체 채팅방
void chatting_room1() {
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
    Sleep(1000);
    clear_screen();

    send(client_sock, id.c_str(), 8, 0);
    stmt = con->createStatement();
    result = stmt->executeQuery("SELECT nickname, message, time FROM chat order by chat_num desc limit 5");
    // 결과 출력
    cout << "=-=-=-=-=-=-=-=-=-= 최 근     메 세 지     입 니 다 =-=-=-=-=-=-=-=-=-=-=-\n";
    while (result->next()) {
        recent_msg.push_back(result->getString(1) + " : " + result->getString(2) + "   " + result->getString(3));
    }
    std::reverse(recent_msg.begin(), recent_msg.end());
    for (string i : recent_msg) cout << i << endl;

    cout << "=-=-=-=-=-=-=-=-=-= 귓 속 말     메 세 지     입 니 다 =-=-=-=-=-=-=-=-=-=\n";
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
// 타자치기 게임 랭킹 함수
void game_rank() {
    goto_xy(0, 0);
    game_rank_inter();
    std::vector<string>ranking;

    try {
        // 쿼리를 통해 상위 5명의 랭킹을 가져옵니다
        stmt = con->createStatement();
        result = stmt->executeQuery(
            "SELECT id, score, score_rank "
            "FROM ( "
            "    SELECT id, score, RANK() OVER (ORDER BY score DESC) AS score_rank "
            "    FROM game_rank "
            ") ranked "
            "WHERE score_rank <= 5");

        if (!result->next()) {
            //상위 5위 랭킹이 없을 때 처리할 내용을 추가
            goto_xy(15, 7);
            cout << "현재 랭킹이 존재하지않습니다 . . ." << endl;
            // 여기에 추가로 처리할 내용을 넣을 수 있습니다.
        }
        else {
            do {
                ranking.push_back(result->getString(3) + " 위     " + result->getString(1) + "          :          " + result->getString(2));
            } while (result->next());
            goto_xy(11, 7);
            cout << ranking[0];
            for (int i = 1; i < ranking.size(); i++) {
                goto_xy(11, 8 + (2 * i));
                cout << ranking[i];
            }
        }

        // 쿼리를 통해 사용자의 랭킹을 가져와 출력합니다
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

        // 쿼리에 사용자의 데이터가 존재하지 않을경우
        if (!result->next()) {
            goto_xy(25, 20);
            cout << "NO RANK";
        }
        // 존재할 경우
        else {
            string myrank = result->getString(1) + "          :          " + result->getString(2);
            goto_xy(20, 20);
            cout << myrank;
            string rank_num = result->getString(3);
            goto_xy(11, 20);
            cout << rank_num << " 위";
        }
    }
    catch (sql::SQLException& e) {
        goto_xy(15, 7);
        cout << "※ 오류 : " << e.what();
        // 다른 MySQL 예외에 대한 처리
    }
}
// 타자치기 게임방
void game_room() {
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
    cout << "지금부터 게임을 시작합니다 . . ." << endl;
    cout << "제한시간 30초 내에 나오는 단어를 정확히 입력해주세요 !" << endl;
    Sleep(1000);
    game_inter();
    clock_t startTime = clock(); // 시간 측정 시작
    while (!game_over) {
        for (int i = 0; i < voca.size(); i++) {
            string word = "";

            goto_xy(20, 10);
            cout << "                            "; // y10 행을 공백으로 덮어쓰기
            goto_xy(20, 12);
            cout << "                            "; // y12 행을 공백으로 덮어쓰기
            goto_xy(20, 10);
            cout << voca.at(i);
            goto_xy(20, 12);
            cin >> word;
            clock_t endTime = clock(); // 시간 측정 끝

            goto_xy(20, 14);
            cout << "                            "; // y14 행을 공백으로 덮어쓰기
            if ((endTime - startTime) / CLOCKS_PER_SEC > 30) {
                goto_xy(20, 12);
                cout << "타임 오버...";
                game_over = true;
                break;
            }

            if (word == voca.at(i)) {
                score++;
                goto_xy(20, 14);
                cout << "정답입니다 !";
                goto_xy(20, 16);
                cout << score;
                goto_xy(47, 18);
                cout << (endTime - startTime) / CLOCKS_PER_SEC << " 초";
            }
            else {
                goto_xy(20, 14);
                cout << "오답입니다 . . .";
                goto_xy(20, 16);
                cout << score;
                goto_xy(47, 18);
                cout << (endTime - startTime) / CLOCKS_PER_SEC << " 초";
            }
        }
    }
    pstmt = con->prepareStatement("INSERT INTO game_rank(id, score) VALUES(?,?)");
    pstmt->setString(1, id);
    pstmt->setInt(2, score);
    string msg = id + " " + std::to_string(score);
    send(client_sock, msg.c_str(), MAX_SIZE, 0);

    try {
        pstmt->execute(); // SQL 쿼리 실행
        goto_xy(5, 14);
        cout << id << "님의 SCORE가 등록되었습니다 !";
        Sleep(1500);
        start = false;
        menu = true;
        chat = false;
    }
    catch (sql::SQLException& e) {
        goto_xy(5, 14);
        cout << "※ 오류 : SQL Error ! " << e.what();
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
            case SIGN_IN:
                start_menu();
                goto_xy(17, 13 + (num - LOG_IN) * 2);
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
            case MYPAGE:
                login_menu(id);
                goto_xy(17, 15);
                cout << "=▶";
                if (move == ENTER) {
                    key_ignore();
                    num = OUTPUT_INFO;
                    menu = false;
                    info = true;
                    chat = false;
                    info_menu();
                    goto_xy(17, 13);
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
                cout << "=▶";
                if (move == ENTER) {
                    info_output();
                    move = _getch();
                    if (move == ESC) {
                        info_menu();
                        goto_xy(17, 13);
                        cout << "=▶";
                    }
                }
                break;
            case CHANGE_INFO:
                info_menu();
                goto_xy(17, 16);
                cout << "=▶";
                if (move == ENTER) {
                    change_info();
                    info_menu();
                    goto_xy(17, 16);
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
            case ADULT_CHAT:
            case GAME:
                chat_menu();
                goto_xy(17, 13 + (num - ALL_CHAT) * 2);
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
            num = GAME_START;
            key_ignore();
            game_menu();
            goto_xy(20, 11);
            cout << "=▶";
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
                cout << "=▶";
                if (move == ENTER) {
                    clear_screen();
                    cout << "타자치기 게임을 시작합니다 ! " << endl;
                    Sleep(1000);
                    game = false;
                    game_room();
                }
                break;
            case GAME_RULE:
                game_menu();
                goto_xy(20, 13);
                cout << "=▶";
                if (move == ENTER) {
                    game_rule();
                    move = _getch();
                    if (move == ESC) {
                        game_menu();
                        goto_xy(20, 13);
                        cout << "=▶";
                        break;
                    }
                }
                break;
            case GAME_RANK:
                game_menu();
                goto_xy(20, 15);
                cout << "=▶";
                if (move == ENTER) {
                    game_rank();
                    move = _getch();
                    if (move == ESC) {
                        game_menu();
                        goto_xy(20, 15);
                        cout << "=▶";
                        break;
                    }
                }
                break;
            case GAME_OUT:
                game_menu();
                goto_xy(20, 17);
                cout << "=▶";
                if (move == ENTER) {
                    game_out_inter();
                    move = _getch();
                    if (move == ESC) {
                        game_menu();
                        goto_xy(20, 17);
                        cout << "=▶";
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