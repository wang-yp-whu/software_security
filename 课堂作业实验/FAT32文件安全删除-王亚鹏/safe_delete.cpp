#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

using namespace std;

struct DirEntry 
{
    char name[11];
    uint8_t attr;
    uint8_t ntRes;
    uint8_t crtTimeTenth;
    uint16_t crtTime;
    uint16_t crtDate;
    uint16_t lstAccDate;
    uint16_t fstClusHI;
    uint16_t wrtTime;
    uint16_t wrtDate;
    uint16_t fstClusLO;
    uint32_t fileSize;
};

// 打开磁盘驱动器
HANDLE openDisk(const string &drivePath) 
{
    HANDLE hDisk = CreateFile(drivePath.c_str(), GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hDisk == INVALID_HANDLE_VALUE) 
    {
        cerr << "无法打开磁盘: " << GetLastError() << endl;
        return NULL;
    }
    return hDisk;
}

// 获取文件起始簇号
uint32_t getStartingCluster(const DirEntry &entry) 
{
    return (entry.fstClusHI << 16) | entry.fstClusLO;
}

// 获取文件的所有簇链
vector<uint32_t> getFileClusters(HANDLE hDisk, uint32_t startCluster, uint32_t fatStartOffset) {
    vector<uint32_t> clusters;
    uint32_t currentCluster = startCluster;
    
    while (currentCluster < 0x0FFFFFF8) 
    {  // 0xFFFFFF8 表示结束
        clusters.push_back(currentCluster);
        // 读取 FAT 表获取下一个簇
        uint32_t fatOffset = fatStartOffset + currentCluster * 4;
        SetFilePointer(hDisk, fatOffset, NULL, FILE_BEGIN);
        DWORD bytesRead;
        ReadFile(hDisk, &currentCluster, sizeof(currentCluster), &bytesRead, NULL);
    }
    return clusters;
}

// 覆盖每个簇的内容
void overwriteCluster(HANDLE hDisk, uint32_t cluster, uint32_t clusterSize, int overwriteCount) 
{
    vector<char> buffer(clusterSize, 0);  // 填充 0 的缓冲区
    
    uint32_t clusterOffset = cluster * clusterSize;  // 根据簇号计算簇偏移
    for (int i = 0; i < overwriteCount; ++i) 
    {
        SetFilePointer(hDisk, clusterOffset, NULL, FILE_BEGIN);
        DWORD bytesWritten;
        WriteFile(hDisk, buffer.data(), buffer.size(), &bytesWritten, NULL);
    }
}

// 删除目录项
void deleteDirEntry(HANDLE hDisk, uint32_t dirEntryOffset) 
{
    SetFilePointer(hDisk, dirEntryOffset, NULL, FILE_BEGIN);
    BYTE deletedMarker = 0xE5;
    DWORD bytesWritten;
    WriteFile(hDisk, &deletedMarker, sizeof(deletedMarker), &bytesWritten, NULL);
}

// 主函数：执行文件安全删除
int main() 
{
    cout << "please input the file's route:";
    string drivePath;  // 目标驱动器路径（需要管理员权限）
    cin >> drivePath;
    HANDLE hDisk = openDisk(drivePath);
    if (hDisk == NULL) return 1;
    uint32_t fatStartOffset = 0x1000;  // FAT 表的起始偏移，我这里是这个值
    uint32_t clusterSize = 4096;       // 每个簇的大小，我这里是4kb
    // 假设我们已找到文件的起始簇和目录项位置
    uint32_t startCluster =     ;  // 需替换为实际起始簇号
    uint32_t dirEntryOffset =   ;   // 需替换为实际目录项偏移
    // 获取文件的所有簇
    auto clusters = getFileClusters(hDisk, startCluster, fatStartOffset);
    // 覆盖文件的所有簇内容
    for (auto cluster : clusters)
    {
        overwriteCluster(hDisk, cluster, clusterSize, 3);  // 默认覆盖3次
    }
    // 清除文件的目录项
    deleteDirEntry(hDisk, dirEntryOffset);
    CloseHandle(hDisk);
    cout << "文件已安全删除。" << endl;
    return 0;
}