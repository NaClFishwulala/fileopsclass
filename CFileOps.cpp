#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "CFileOps.h"

using namespace std;

#define PATHNAME "../CFileOps.txt"
#define BUFFER_SIZE_READ_FILE 4096
#define BUFFER_SIZE_WRITE_FILE 4096
#define MAX_SIZE 256


CFileOps::CFileOps()
{
    // 打开文件
    CFileStatus s = MyFileOpen();
    if(!s.IsSuccess()) {
        cout << "file open error" << endl;
    }
    cout << "file open success, m_Fd: " << m_Fd << endl;
    
    m_pReadBuffer = new char[BUFFER_SIZE_READ_FILE];    // 声明读文件缓存
    readPos = 0;    // 读取文件偏移量
    m_pWriteBuffer = new char[BUFFER_SIZE_WRITE_FILE];  // 声明写文件缓存
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
    // 使用static确保只会声明一次CFileOps类
    static CFileOps m_pFile;
    return m_pFile;
}

CFileStatus CFileOps::MyFileOpen() 
{
    m_Fd = open(PATHNAME, O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR); //当open的第二个参数使用了O_CREAT, open还需要第三个参数指定新文件的访问权限位
    if(m_Fd == -1) {
        return CFileStatus(false, -1);
    }
    return CFileStatus(true, -1);
}

CFileStatus CFileOps::MyFileClose()
{

    if(m_Fd != -1) {
        int returncode = close(m_Fd);
        if(returncode != -1) {
            return CFileStatus(true, -1);
        }
    }
    return CFileStatus(false, -1);
}

CFileStatus CFileOps::MyFileRead(size_t bytes)
{
    MyFileLseek();
    size_t readBytes = read(m_Fd, m_pReadBuffer, bytes);
    if(readBytes == -1) {
        cout << "file read error" << endl;
        return CFileStatus(false, -1);
    }
    cout << "file read sccuess, readBytes: " << readBytes << endl;
    string s = m_pReadBuffer;
    if(readBytes != 0) {
        readPos += readBytes;
        cout << "Content is: " << s << endl;
    }
    return CFileStatus(true, readBytes);
}

CFileStatus CFileOps::MyFileWrite()
{
    char buf[MAX_SIZE];
    cout << "Please enter buf to write into file:";
    scanf("%s", buf);
    size_t writeBytes = write(m_Fd, buf, strlen(buf));
    if(writeBytes == -1) {
        cout << "file write error" << endl;
        return CFileStatus(false, -1);
    }
    cout << "file write sccuess, writeBytes: " << writeBytes << endl;
    return CFileStatus(true, writeBytes);
}

CFileStatus CFileOps::MyFileLseek()
{
    lseek(m_Fd, readPos, SEEK_SET);
    cout << "current position: " << readPos << endl;
    return CFileStatus(true, readPos);
}