#pragma comment(lib, "ws2_32.lib") //명시적인 라이브러리의 링크. 윈속 라이브러리 참조

#include <WinSock2.h> // Winsock 헤더파일 include. WSADATA 들어있음.
#include <string>
#include <sstream>
#include <iostream>
#include <thread> // 쓰레드 헤더
#include <vector>
#include <mysql/jdbc.h> // SQL 헤더
#include <ctime> // 시간 측정을 위한 헤더
#include <time.h> // 현재 시간 표시를 위한 헤더

#define MAX_SIZE 1024
#define MAX_CLIENT 10

using std::cout;
using std::cin;
using std::endl;
using std::string; // namespace 정의


sql::mysql::MySQL_Driver* driver; // 추후 해제하지 않아도 Connector/C++가 자동으로 해제해 줌
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result; // SQL 정의

const string server = "tcp://127.0.0.1:3306"; // 데이터베이스 주소
const string username = "root"; // 데이터베이스 사용자
const string password = "1234"; // 데이터베이스 접속 비밀번호

struct SOCKET_INFO { // 연결된 소켓 정보에 대한 틀 생성
    SOCKET sck;
    string user;
    int count = 0;
    bool hasSentEnterMsg; // 입장 메시지를 보냈는지 여부
    int ti;
};


std::vector<SOCKET_INFO> sck_list; // 연결된 클라이언트 소켓들을 저장할 배열 선언.
SOCKET_INFO server_sock; // 서버 소켓에 대한 정보를 저장할 변수 선언.
int client_count = 0; // 현재 접속해 있는 클라이언트를 count 할 변수 선언.
void server_init(); // socket 초기화 함수. socket(), bind(), listen() 함수 실행됨. 자세한 내용은 함수 구현부에서 확인.
void add_client(int a); // 소켓에 연결을 시도하는 client를 추가(accept)하는 함수. client accept() 함수 실행됨. 자세한 내용은 함수 구현부에서 확인.
void send_msg(const char* msg, string sender); // send() 함수 실행됨. 자세한 내용은 함수 구현부에서 확인.
void recv_msg(string user_name); // recv() 함수 실행됨. 자세한 내용은 함수 구현부에서 확인.
void del_client(int idx); // 소켓에 연결되어 있는 client를 제거하는 함수. closesocket() 실행됨. 자세한 내용은 함수 구현부에서 확인.
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
int num = 0; // 전역변수 정의

SOCKET_INFO get_socket_info(string user) {
    for (int i = 0; i < sck_list.size(); i++) { // 접속해 있는 모든 client에게 메시지 전송
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

void server_init() {
    server_sock.sck = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Internet의 Stream 방식으로 소켓 생성
   // SOCKET_INFO의 소켓 객체에 socket 함수 반환값(디스크립터 저장)
   // 인터넷 주소체계, 연결지향, TCP 프로토콜 쓸 것. 

    SOCKADDR_IN server_addr = {}; // 소켓 주소 설정 변수
    // 인터넷 소켓 주소체계 server_addr

    server_addr.sin_family = AF_INET; // 소켓은 Internet 타입 
    server_addr.sin_port = htons(7777); // 서버 포트 설정
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 서버이기 때문에 local 설정한다. 
    //Any인 경우는 호스트를 127.0.0.1로 잡아도 되고 localhost로 잡아도 되고 양쪽 다 허용하게 할 수 있따. 그것이 INADDR_ANY이다.
    //ip 주소를 저장할 땐 server_addr.sin_addr.s_addr -- 정해진 모양?

    bind(server_sock.sck, (sockaddr*)&server_addr, sizeof(server_addr)); // 설정된 소켓 정보를 소켓에 바인딩한다.
    listen(server_sock.sck, SOMAXCONN); // 소켓을 대기 상태로 기다린다.
    server_sock.user = "server";

    cout << "Server On" << endl;
}

void add_client(int ti) {
    bool flag = true;
    msg = " ";
    SOCKADDR_IN addr = {};
    int addrsize = sizeof(addr);
    char buf[MAX_SIZE] = { };

    ZeroMemory(&addr, addrsize); // addr의 메모리 영역을 0으로 초기화

    SOCKET_INFO new_client = {};

    new_client.sck = accept(server_sock.sck, (sockaddr*)&addr, &addrsize);
    // Winsock2의 recv 함수. client가 보낸 닉네임을 받음.

    recv(new_client.sck, buf, MAX_SIZE, 0);
    msg = buf;
    new_client.ti = ti;

    if (msg.find("*") != string::npos) {
        msg.resize(msg.size() - 1);
        chattingroom_member.push_back(msg);
        new_client.user = msg;
        string msg = "[공지] " + new_client.user + " 님이 성인채팅방에 입장했습니다.";
        cout << msg << endl;
        send_adult_msg(msg.c_str(), new_client.user);
        flag = false;
    }
    if (msg.find("#") != string::npos) {
        msg.resize(msg.size() - 1);
        new_client.user = msg;
        string msg = "[공지] " + new_client.user + " 님이 게임방에 입장했습니다.";
        cout << msg << endl;
        flag = false;
    }

    new_client.user = msg;
    if (!new_client.hasSentEnterMsg && flag) {
        string msg = "[공지] " + new_client.user + " 님이 전체채팅방에 입장했습니다.";
        cout << msg << endl;
        send_msg(msg.c_str(), new_client.user);
        new_client.hasSentEnterMsg = true;
    }

    sck_list.push_back(new_client);


    std::thread th(recv_msg, new_client.user);
    // 다른 사람들로부터 오는 메시지를 계속해서 받을 수 있는 상태로 만들어 두기.

    client_count++; // client 수 증가.

    cout << "[공지] 현재 접속자 수 : " << sck_list.size() << "명" << endl;
    //send_msg(msg.c_str(), new_client.user); // c_str : string 타입을 const chqr* 타입으로 바꿔줌.

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
    for (int i = 0; i < sck_list.size(); i++) { // sender 제외한 모든 client에게 메시지 전송
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
    for (int i = 0; i < sck_list.size(); i++) { // sender에게만 client에게 메시지 전송
        if (sck_list[i].user == sender) {
            send(sck_list[i].sck, text.c_str(), MAX_SIZE, 0);
        }
    }
}
void send_adult_msg(const char* msg, string sender) {
    string text = msg;
    for (int i = 0; i < sck_list.size(); i++) { // sender에게만 client에게 메시지 전송
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
    cout << "욕설 감지!" << endl;
    text = "[경고] " + nickname + "님이 욕설을 사용하셨습니다! 누적 경고 횟수 : " + std::to_string(sck_list[ti].count);
    send_msg(text.c_str(), "");
    if (sck_list[ti].count == 3) {
        sck_list[ti].count = 0;
        text = "[공지] " + nickname + "님이 경고누적으로 3분간 대화하실 수 없습니다!";
        send_msg(text.c_str(), "");
        std::thread th3(countdown);
        while (t > 0) {
            int min = t / 60;
            int sec = t % 60;
            int bytes_received = recv(sck_list[ti].sck, box, MAX_SIZE, 0);
            if (bytes_received > 0) { // 오류가 발생하지 않으면 recv는 수신된 바이트 수를 반환. 0보다 크다는 것은 메시지가 왔다는 것.
                text = "[공지] 대화에 참여하실 수 없습니다(남은 시간 : " + std::to_string(min) + "분" + std::to_string(sec) + "초)";
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
        _time64(&temp); // 시스템 시간을 얻습니다.
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(sck_info.sck, buf, MAX_SIZE, 0) > 0) { // 오류가 발생하지 않으면 recv는 수신된 바이트 수를 반환. 0보다 크다는 것은 메시지가 왔다는 것.
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
                    msg = "[게임공지] : " + first + "가 1등입니다! 점수 : " + std::to_string(game_score);
                }
                else {
                    if (game_score > stoi(id))
                        msg = "[게임공지] : " + game_id + "가 여전히 1등입니다! 점수 : " + std::to_string(game_score);
                    else
                        msg = "[게임공지] :  " + first + "가 " + game_id + "을 이겼습니다! 점수 : " + id;
                }
                cout << msg << endl;
                cout << "[공지] " + user + " 님이 게임채팅방에서 퇴장했습니다." << endl;
                send_msg_all(msg.c_str(), "");
                int remove = removeSocket(user);
                if (remove > -1) {
                    recreate = remove;
                }
                return;
            }
            if (msg == "/out") {
                num = 1;
                msg = "[공지] " + user + " 님이 퇴장했습니다.";
                cout << "[공지] " + user + " 님이 전체채팅방에서 퇴장했습니다." << endl;
                send_msg(msg.c_str(), user);
            }
            if (msg == "/out_adult") {
                num = 1;
                int adult_idx = find(chattingroom_member.begin(), chattingroom_member.end(), nickname) - chattingroom_member.begin(); //성인채팅방을 나가는 사람의 idx찾기
                chattingroom_member[adult_idx] = "";
                msg = "[공지] " + user + " 님이 퇴장했습니다.";
                cout << "[공지] " + user + " 님이 성인채팅방에서 퇴장했습니다." << endl;
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
                        // 현지 시간 정보를 성공적으로 얻었습니다.
                        char buffer[26]; // 문자열 버퍼를 생성합니다.
                        if (asctime_s(buffer, sizeof(buffer), &timeinfo) == 0) {
                            std::string timeString(buffer); // asctime로부터 얻은 문자열
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
                    msg = "귓속말이 도착했습니다! [" + nickname + "]" + " : " + p_msg;
                    send_private_msg(msg.c_str(), id);
                }
            }
            if (msg.find("*") != string::npos && num == 0) {
                msg.resize(msg.size() - 1);
                msg = nickname + " : " + msg;
                cout << msg << endl;
                send_adult_msg(msg.c_str(), nickname);
            }
            else if (num == 0 && msg.find("[공지]") == string::npos) {
                if (_localtime64_s(&timeinfo, &temp) == 0) {
                    // 현지 시간 정보를 성공적으로 얻었습니다.
                    char buffer[26]; // 문자열 버퍼를 생성합니다.
                    if (asctime_s(buffer, sizeof(buffer), &timeinfo) == 0) {
                        std::string timeString(buffer); // asctime로부터 얻은 문자열
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
        else { //그렇지 않을 경우 퇴장에 대한 신호로 생각하여 퇴장 메시지 전송
            //del_client(idx); // 클라이언트 삭제
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
    sck_list.erase(sck_list.begin() + idx); // 배열에서 클라이언트를 삭제하게 될 경우 index가 달라지면서 런타임 오류 발생....ㅎ
    client_count--;
}
int removeSocket(string user) {
    int ti;
    for (int i = 0; i < sck_list.size(); i++) { // 접속해 있는 모든 client에게 메시지 전송
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
    // Winsock를 초기화하는 함수. MAKEWORD(2, 2)는 Winsock의 2.2 버전을 사용하겠다는 의미.
    // 실행에 성공하면 0을, 실패하면 그 이외의 값을 반환.
    // 0을 반환했다는 것은 Winsock을 사용할 준비가 되었다는 의미.
    int code = WSAStartup(MAKEWORD(2, 2), &wsa);

    if (!code) {
        server_init();


        char buf[MAX_SIZE] = { };

        for (int i = 0; i < MAX_CLIENT; i++) {
            // 인원 수 만큼 thread 생성해서 각각의 클라이언트가 동시에 소통할 수 있도록 함.
            th1[i] = std::thread(add_client, i);

        }
        std::thread th2(recreate_thread);

        while (1) { // 무한 반복문을 사용하여 서버가 계속해서 채팅 보낼 수 있는 상태를 만들어 줌. 반복문을 사용하지 않으면 한 번만 보낼 수 있음.
            string text, msg = "";

            std::getline(cin, text);
            const char* buf = text.c_str();
            msg = server_sock.user + " : " + buf;
            send_msg(msg.c_str(), server_sock.user);
        }


        for (int i = 0; i < MAX_CLIENT; i++) {
            th1[i].join();
            //join : 해당하는 thread 들이 실행을 종료하면 리턴하는 함수.
            //join 함수가 없으면 main 함수가 먼저 종료되어서 thread가 소멸하게 됨.
            //thread 작업이 끝날 때까지 main 함수가 끝나지 않도록 해줌.
        }
        th2.join();
        closesocket(server_sock.sck);
    }
    else {
        cout << "프로그램 종료. (Error code : " << code << ")";
    }

    WSACleanup();

    return 0;
}
