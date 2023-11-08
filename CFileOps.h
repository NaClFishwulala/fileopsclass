#ifndef CFILEOPS_H
#define CFILEOPS_H

#include "CFileStatus.h"
class CFileOps
{
private:
    int m_Fd;

    char *m_pReadBuffer;
    unsigned int m_usedReadBufferSize;

    char *m_pWriteBuffer;
    unsigned int m_usedWriteBufferSize;
    std::string fileContent;
    off_t readPos;

private:
    CFileOps();   // 当构造函数为私有成员时，要想在类外部声明访问就必须使用单例设计模式，借助public的static CFileOps* GetInstance()来访问
    ~CFileOps();

    CFileStatus MyFileLseek();
    CFileStatus MyFileReadOps(size_t bytes);           //封装read函数
    CFileStatus MyFileWriteOps(char* message);           //封装write函数, 指明写入字符串
    void FlushBuffer();                   // 将缓存中的数据写入文件或读取出来
    static void OnProcessExit();
public:

    static CFileOps& GetInstance();

    CFileStatus MyFileOpen();
    CFileStatus MyFileRead(size_t bytes);
    CFileStatus MyFileWrite();
    CFileStatus MyFileClose();
    size_t GetFileSize();
};

#endif