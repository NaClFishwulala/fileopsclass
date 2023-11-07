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

    off_t readPos;

private:
    CFileOps();   // 当构造函数为私有成员时，要想在类外部声明访问就必须使用单例设计模式，借助public的static CFileOps* GetInstance()来访问
    ~CFileOps();

    CFileStatus MyFileLseek();
    CFileStatus Flush();                   // 将缓存中的数据写入文件
    static void OnProcessExit();

public:

    static CFileOps& GetInstance();

    CFileStatus MyFileOpen();
    CFileStatus MyFileRead(size_t bytes);
    CFileStatus MyFileWrite();
    CFileStatus MyFileClose();
};

#endif