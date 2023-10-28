#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "CFileOps.h"

#define PATHNAME "CFileOps.txt"
using namespace std;

// CFileOps* CFileOps::m_pFile = 0;
CFileOps::CFileOps()
{
    CFileStatus s = MyFileOpen();
    if(!s.IsSuccess()) {
        cout << "file open error" << endl;
    }
    cout << "file open success, m_Fd: " << m_Fd << endl;
}

CFileOps::~CFileOps()
{
    CFileStatus s = MyFileClose();
    if(!s.IsSuccess()) {
        cout << "file close error" << endl;
    }
    cout << "file close success" << endl;
}

CFileOps& CFileOps::GetInstance()
{
    static CFileOps m_pFile;
    return m_pFile;
}

CFileStatus CFileOps::MyFileOpen() 
{
    m_Fd = open(PATHNAME, O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR); //当open的第二个参数使用了O_CREAT, open还需要第三个参数指定新文件的访问权限位
    if(m_Fd == -1) {
        return CFileStatus(false, 0);
    }
    return CFileStatus(true, 0);
}

CFileStatus CFileOps::MyFileClose()
{

    if(m_Fd != -1) {
        int returncode = close(m_Fd);
        if(returncode != -1) {
            return CFileStatus(true, 0);
        }
    }
    return CFileStatus(false, 0);
}