#include <iostream>
#include <Windows.h> // clear_screen()�� ����ϱ� ���� ���
#include <conio.h> // _getch()�� ����ϱ� ���� ��� 

using std::cout;
using std::cin;
using std::endl;
using std::string; // namespace ����

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