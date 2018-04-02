/* 	C;	create MyFileName.htm from folder */
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>
#include <string.h>
#include <Windows.h>
void HeadMaker (FILE *pTextFile)
{
    fprintf(pTextFile, "<HTML>\n");
    fprintf(pTextFile, "<HEAD>\n");
    fprintf(pTextFile, "<TITLE>\n");
    fprintf(pTextFile, "</TITLE>\n");
    fprintf(pTextFile, "</HEAD>\n");
    fprintf(pTextFile,  
	"<body style=\"FONT-SIZE: 20px; LINE-HEIGHT: 25px\" class=\"menu\">\n");
}
void WhereIam (char *MainFolderName, char *ExeFileName)
{
    GetModuleFileName(NULL, MainFolderName, MAX_PATH);
    int sp = 0;
    while (sp!=MAX_PATH) {
        if(MainFolderName[sp]=='\0') break;
        sp++;
		}
	sp = sp-sizeof(ExeFileName);
    MainFolderName[sp+1]='\0';
}
void DivMaker (FILE *pTextFile,char *FolderName, char *MainFolderName)
{
	fprintf(pTextFile, 
		"<a href=\"%s\\index.htm\" target=\"mainFrame\">%s</a><br>\n", 
			FolderName, FolderName);
}
void BottomMaker (FILE *pTextFile, char *ExeFileName, char *IndexFile)
{
    fprintf(pTextFile, "	<br><a href=\"%s\">Обновить страницу</a>\n",
		IndexFile);
    // call that files, not recurse
	fprintf(pTextFile, "	<br><a href=\"%s\">Обновить каталоги</a>\n", 
		ExeFileName);
    fprintf(pTextFile, "</BODY></HTML>\n");
	fclose(pTextFile);
}
int main(int argc, char* argv[])
{
	//	create name of output file
	char IndexFile[MAX_PATH] = "";
	strncpy(IndexFile, argv[0], strlen(argv[0]) - 4);
	strcat(IndexFile, ".html");
	// try create output file
	FILE *pTextFile = fopen(IndexFile, "w");
    if(pTextFile == NULL) return EXIT_FAILURE;
	// Write head to output file
	HeadMaker(pTextFile);
	// Get path for my exe. Need for local web
	char MainFolderName[MAX_PATH];
	WhereIam(MainFolderName, argv[0]);
    //Search all objects in folder
	WIN32_FIND_DATA ffd;
    HANDLE hFind;
    TCHAR szFind[MAX_PATH + 1];
    lstrcat(szFind, "*.*");
    hFind = FindFirstFile(szFind, &ffd);
    do
    {
	// verify object is folder
		if(ffd.dwFileAttributes & 0x00000010)
            {
            if(lstrcmp(ffd.cFileName, ".") == 0 ||
                lstrcmp(ffd.cFileName, "..") == 0) continue;
				// if object is foldere, create link in output file
				DivMaker(pTextFile, ffd.cFileName, "");
            }
    }
    while(FindNextFile(hFind, &ffd));
    FindClose(hFind);
	// Write bottom to output file
	BottomMaker(pTextFile, argv[0], IndexFile);
	return EXIT_SUCCESS;
}
