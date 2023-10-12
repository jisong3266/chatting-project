#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h> //Winsock 헤더파일 include. WSADATA 들어있음.
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
//#include <mysql/jdbc.h>
#include <Windows.h> // system("cls")를 사용하기 위한 헤더
#include <conio.h> // _getch()를 사용하기 위한 헤더 
#include <mysql/jdbc.h>

#define MAX_SIZE 1024
#define ENTER 13
#define UP 72
#define DOWN 80 // 아스키 코드 정의

using std::cout;
using std::cin;
using std::endl;
using std::string;

sql::mysql::MySQL_Driver* driver; // 추후 해제하지 않아도 Connector/C++가 자동으로 해제해 줌
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

SOCKET client_sock;
string my_nick;
string name, id, pw, birth;

const string server = "tcp://127.0.0.1:3306"; // 데이터베이스 주소
const string username = "root"; // 데이터베이스 사용자
const string password = "goflvhxj"; // 데이터베이스 접속 비밀번호

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
// 이름 recv 함수
int chat_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
            msg = buf;
            std::stringstream ss(msg);  // 문자열을 스트림화
            string user;
            ss >> user; // 스트림을 통해, 문자열을 공백 분리해 변수에 할당. 보낸 사람의 이름만 user에 저장됨.
            if (user != my_nick) cout << buf << endl; // 내가 보낸 게 아닐 경우에만 출력하도록.
        }
        else {
            cout << "Server Off" << endl;
            return -1;
        }
    }
}
// 커서 깜빡임 제거
void CursorView(bool show)
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
// 인터페이스
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
    cout << " "; cout << "*                    < 시작 화면 >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                 ▶  1. 로그인                         *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    2. 회원가입                       *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    0. 종료                           *\n";
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
    cout << " "; cout << "*                    < 시작 화면 >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    1. 로그인                         *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                 ▶  2. 회원가입                       *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    0. 종료                           *\n";
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
    cout << " "; cout << "*                    < 시작 화면 >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    1. 로그인                         *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                    2. 회원가입                       *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                 ▶  0. 종료                           *\n";
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
    cout << " "; cout << "*                    <   종 료   >                     *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*            ▶  프로그램을 종료하시겠습니까?           *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*               (Enter키를 누르면 종료)                *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "*                                                      *\n";
    cout << " "; cout << "********************************************************\n\n";
}

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
void check_id() {
    while (1) {
        cout << "아이디를 입력하세요(영문과 숫자 포함 8자리로 입력하세요) : ";
        cin >> id;
        pstmt = con->prepareStatement("SELECT id FROM userinfo WHERE id = ?");
        pstmt->setString(1, id);
        result = pstmt->executeQuery();
        if (result->next()) {
            string db_id = result->getString(1);
            if (db_id == id) {
                cout << "중복된 ID입니다 !" << endl;
                continue;
                // 아이디가 중복됨
            }
        }
        else {
             // 아이디 중복 없음
             cout << "ID 체크완료" << endl;
             send(client_sock, id.c_str(), 10, 0);
             break;
         }
    }
}
// start sql
void sign_in() {

    cout << "회원가입을 시작합니다" << endl;
        cout << "이름을 입력하세요 : ";
        cin >> name;
        check_id();
        send(client_sock, name.c_str(), 10, 0); // 연결에 성공하면 client 가 입력한 닉네임을 서버로 전송
    cout << "비밀번호를 입력하세요 : ";
    cin >> pw;
    cout << "생년월일을 입력하세요 : ";
    cin >> birth;
    user_info *u  = new user_info(name, id, pw, birth);

    pstmt = con->prepareStatement("INSERT INTO userinfo(name, id, pw, birthday) VALUES(?,?,?,?)");

    pstmt->setString(1, u->u_name);
    pstmt->setString(2, u->u_id);
    pstmt->setString(3, u->u_pw);
    pstmt->setString(4, u->u_birth);

    try {
        pstmt->execute(); // SQL 쿼리 실행
        cout << "User " << u->u_name << " added to the database." << endl;
    }
    catch (sql::SQLException& e) {
        cout << "SQL Error: " << e.what() << endl;
    }
   

}
void log_in() {
    cout << "로그인을 시작합니다!" << endl;
    cout << "아이디를 입력하세요(영문과 숫자 포함 8자리로 입력하세요) : ";
    cin >> id;
    cout << "비밀번호를 입력하세요 : ";
    cin >> pw;
}
int main() {

    WSADATA wsa;
    // Winsock를 초기화하는 함수. MAKEWORD(2, 2)는 Winsock의 2.2 버전을 사용하겠다는 의미.
    // 실행에 성공하면 0을, 실패하면 그 이외의 값을 반환.
    // 0을 반환했다는 것은 Winsock을 사용할 준비가 되었다는 의미.
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
            const char* buffer = text.c_str(); // string형을 char* 타입으로 변환
            send(client_sock, buffer, strlen(buffer), 0);
        }
        th2.join();
        closesocket(client_sock);
    }

    WSACleanup();
    return 0;
}
