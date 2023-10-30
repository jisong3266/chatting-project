#include <iostream>
#include <Windows.h> // clear_screen()를 사용하기 위한 헤더
#include <conio.h> // _getch()를 사용하기 위한 헤더 

using std::cout;
using std::cin;
using std::endl;
using std::string; // namespace 정의

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