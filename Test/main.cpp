#include <windows.h>

using namespace std;

int main()
{
    wchar_t wText[4096];
   
    char cText[] = u8R"(KARMA_DESC:0 "Karma increases from : \n - §YHaving a war declared on you§!\n - §YHonoring alliances§!\n - §YReleasing vassals§!\n - §YReleasing nations in peace deals§!\n - §YReturning cores§!\n\nKarma decreases from : \n - §YStarting wars§!\n - §YTaking provinces in peace deals§!")";
    
    MultiByteToWideChar(28591, 0, cText, strlen(cText), wText, 4096);

    MessageBoxW(NULL, wText, L"Fucked text", MB_OK);

    MultiByteToWideChar(CP_UTF8, 0, cText, strlen(cText), wText, 4096);
    
    MessageBoxW(NULL, wText, L"Fucked text", MB_OK);

    return 0;
}
