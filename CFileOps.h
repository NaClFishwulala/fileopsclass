#ifndef CFILEOPS_H
#define CFILEOPS_H

#include "CFileStatus.h"

class CFileOps
{
private:
    int m_Fd;

private:
    CFileOps(); // 当构造函数为私有成员时，要想在类外部声明访问就必须使用单例设计模式，借助public的static CFileOps* GetInstance()来访问
    ~CFileOps();
public:

    static CFileOps& GetInstance();

    CFileStatus MyFileOpen();
    // CFileStatus MyFileRead();
    // CFileStatus MyFileWrite();
    // CFileStatus MyFileLseek();
    CFileStatus MyFileClose();
};

#endif