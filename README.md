# KeyLogger

pure GetMessage hooker

has only use of jmp XXXXXXXX instruction, instead of SetWindowsHookEx

# Usage

## Setup

키로깅 대상 컴퓨터에 KeyHook32.dll과 KeyHook64.dll을 적당한 경로에 설치해줍니다

키로그를 전송받을 컴퓨터에선 KeyLoggerServer.exe만 실행합니다

키로깅 대상 컴퓨터에서 KeyLoggerSetup을 실행해 활성화합시다

![셋업](https://user-images.githubusercontent.com/67177785/132224743-9af79027-3cca-4125-b2df-07b8c90f7bc5.PNG)

IP는 키 입력이 전송될 서버의 IP를 입력하면 됩니다

![IP확인](https://user-images.githubusercontent.com/67177785/132224755-32f7cb91-5c1a-472a-b00d-424291a6ec5f.PNG)

두 호스트가 같은 내부망 안에 있는 경우(같은 공유기를 사용하는 경우) KeyLoggerServer.exe에 뜨는 IP를 사용하면 되고

외부망에서 접근해야 하는경우는 따로.. 공유기의 방화벽 설정을 해주셔야 합니다

## 본격적인 활용

제대로 설정되었다면 

KeyLoggerServer.exe 메인 화면에 클라이언트 접속 정보가 뜰것입니다

IP가 실제로 접속된 IP이고, Internal_IP는 대상 컴퓨터의 내부망 IP입니다.

Internal_IP는 같은 IP를 사용하는 둘 이상의 호스트가 접속했을때를 대비해 각각의 호스트를 구별하기 위해 수집합니다(MAC주소 대신입니다)

![키로거 연결](https://user-images.githubusercontent.com/67177785/132224758-3733bbe6-6c31-4e19-b7ac-fa4f01dba228.PNG)

확인하고 싶은 클라이언트의 넘버를 더블클릭해 정보를 확인합니다

각 탭을 클릭해 키 입력 정보를 프로세스별로 확인할 수 있습니다

![키로깅확인 codeblocks](https://user-images.githubusercontent.com/67177785/132224762-cf230989-2caa-443a-9edf-6832f67c1af6.PNG)

![키로깅확인 notepad](https://user-images.githubusercontent.com/67177785/132224764-4f3efbb5-9933-4562-9618-e49c2b293a1e.PNG)

## 주의사항

중간에 보이는 저 검정색에 구멍뚫린것 같은 문자는 백스페이스입니다

프로세스별로 사용하는 api가 다르기 때문에 한글 입력값은 깨지기도 합니다

기술적인 한계로 후킹이 불안정합니다. 중간에 튕길수 있어요
