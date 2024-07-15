대충 코드 짜집기로 만든 좀비 게임


1.
Assimp 라이브러리가 제대로 로드가 안될경우, 소스코드 디버깅에 실패하게 됨.
이 때 라이브러리의 경로가 제대로 포함되었는지 확인하여야 함.
$(LocalDebuggerWorkingDirectory)매크로가 간혹 작동이 안되는 경우가 있으므로
프로젝트 속성 페이지에 들어가서 VC++디렉터리의 
"포함 디렉터리"와 "라이브러리 디렉터리"의 경로를 체크.

$(LocalDebuggerWorkingDirectory)\data\include;
$(LocalDebuggerWorkingDirectory)\data\lib;

예를 들어 이 부분을 실제 라이브러리 폴더의 경로로 변경하면 된다.

C:\Users\Username\Desktop\Test - 21\Keeper\data\include;
C:\Users\Username\Desktop\Test - 21\Keeper\data\lib;


2.
프로그램은 x64비트 기준으로 작성되었으며 x86비트로 디버깅 할 경우, 오류가 발생하니 주의

