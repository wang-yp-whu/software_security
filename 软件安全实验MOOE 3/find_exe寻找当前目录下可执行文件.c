#include <windows.h>
#include <stdio.h>

void SearchAndDisplayExeFiles(const char* directoryPath) 
{
    WIN32_FIND_DATAA findFileData;
    char searchPath[MAX_PATH];
    snprintf(searchPath, MAX_PATH, "%s\\*.exe", directoryPath);  // 构建搜索路径
    HANDLE hFind = FindFirstFileA(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) 
    {
        MessageBoxA(NULL, "No exe files found!", "Search Result", MB_OK);
        return;
    } 
    else 
    {
        // 初始化一个字符串用于存储所有exe文件的名称
        char allExeFiles[4096] = "";  // 假设4096字节足够存储所有文件名
        do 
        {
            // 将找到的exe文件名添加到allExeFiles中，并换行
            strcat(allExeFiles, findFileData.cFileName);
            strcat(allExeFiles, "\n");
        } while (FindNextFileA(hFind, &findFileData) != 0);
        FindClose(hFind);
        // 显示所有文件名
        MessageBoxA(NULL, allExeFiles, "Found exe files", MB_OK);
    }
}

int main() 
{
    // 设置要搜索的目录路径为 E:\.vscode\vscode\code
    const char* directory = "E:\\.vscode\\vscode\\code";
    SearchAndDisplayExeFiles(directory);
    system("pause");
    return 0;
}
