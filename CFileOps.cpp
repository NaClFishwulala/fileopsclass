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
    m_usedReadBufferSize = 0;
    readPos = 0;    // 读取文件偏移量

    m_pWriteBuffer = new char[BUFFER_SIZE_WRITE_FILE];  // 声明写文件缓存
    m_usedWriteBufferSize = 0;

    // //注册终止函数
    // if(atexit(OnProcessExit) != 0) {
    //     cout << "atexit error" << endl;
    // }
}

CFileOps::~CFileOps()
{
    Flush();
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
    unsigned int bufSize = strlen(buf);
    unsigned int totoal_size = m_usedWriteBufferSize + bufSize;
    size_t writeBytes = 0;

    // 将buf放到缓存m_pWriteBuffer中， 如果已写入缓存大小m_usedWriteBufferSize >= 规定值BUFFER_SIZE_WRITE_FILE，先填满缓存，如果剩下的还大就一次性写入，否则就留在缓存
    if(totoal_size >= BUFFER_SIZE_WRITE_FILE) {
        unsigned int lefBufferSize = BUFFER_SIZE_WRITE_FILE - m_usedWriteBufferSize;
        memcpy(m_pWriteBuffer + m_usedWriteBufferSize, buf, lefBufferSize);
        writeBytes = write(m_Fd, m_pWriteBuffer, BUFFER_SIZE_WRITE_FILE);
        m_usedWriteBufferSize = 0;
        totoal_size -= BUFFER_SIZE_WRITE_FILE;
        if(totoal_size >= BUFFER_SIZE_WRITE_FILE) {
            writeBytes += write(m_Fd, buf + lefBufferSize, totoal_size);  
            if(writeBytes == -1) {
                cout << "file write error" << endl;
                return CFileStatus(false, -1);
            }
            cout << "file write sccuess, writeBytes: " << writeBytes << endl;
            return CFileStatus(true, writeBytes);
        } else {
            memcpy(m_pWriteBuffer, buf + lefBufferSize, totoal_size);
            m_usedWriteBufferSize += totoal_size;
        }
    }
    // 小于的话直接写入缓存
    memcpy(m_pWriteBuffer, buf, bufSize);
    m_usedWriteBufferSize += bufSize;

    return CFileStatus(true, writeBytes);
}

CFileStatus CFileOps::MyFileLseek()
{
    lseek(m_Fd, readPos, SEEK_SET);
    cout << "current position: " << readPos << endl;
    return CFileStatus(true, readPos);
}

CFileStatus CFileOps::Flush()
{
    size_t writeBytes = write(m_Fd, m_pWriteBuffer, m_usedWriteBufferSize);  
    if(writeBytes == -1) {
        cout << "from buffer write error" << endl;
        return CFileStatus(false, -1);
    }
    cout << "from buffer write sccuess, writeBytes: " << writeBytes << endl;
    return CFileStatus(true, writeBytes);
}

// void CFileOps::OnProcessExit()
// {
//     // Flush为static, 所以必须要获取实例对象才能使用
//     CFileOps& fd = CFileOps::GetInstance();
//     fd.Flush();
//     return;
// }