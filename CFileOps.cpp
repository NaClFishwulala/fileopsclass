#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "CFileOps.h"

using namespace std;

#define WRITEFILENAME "../writefile.txt"
#define READFILENAME "../readfile.txt"
#define BUFFER_SIZE_READ_FILE 4096
#define BUFFER_SIZE_WRITE_FILE 4096


CFileOps::CFileOps()
{
    // 打开文件
    CFileStatus sRead = MyReadFileOpen();
    if(!sRead.IsSuccess()) {
        cout << "readfile open error" << endl;
    }
    cout << "readfile open success, m_Fd: " << m_ReadFd << endl;
    
    m_pReadBuffer = new char[BUFFER_SIZE_READ_FILE];    // 声明读文件缓存
    memset(m_pReadBuffer, 0, BUFFER_SIZE_READ_FILE);
    m_usedReadBufferSize = 0;
    readPos = 0;    // 读取文件偏移量

    CFileStatus sWrite = MyWriteFileOpen();
    if(!sWrite.IsSuccess()) {
        cout << "writefile open error" << endl;
    }
    cout << "writefile open success, m_Fd: " << m_WriteFd << endl;

    m_pWriteBuffer = new char[BUFFER_SIZE_WRITE_FILE];  // 声明写文件缓存
    memset(m_pWriteBuffer, 0, BUFFER_SIZE_WRITE_FILE);
    m_usedWriteBufferSize = 0;
}

CFileOps::~CFileOps()
{
    OnProcessExit();
    CFileStatus sRead = MyReadFileClose();
    if(!sRead.IsSuccess()) {
        cout << "readfile close error" << endl;
    }
    cout << "readfile close success" << endl;

    CFileStatus sWrite = MyWriteFileClose();
    if(!sWrite.IsSuccess()) {
        cout << "writefile close error" << endl;
    }
    cout << "writefile close success" << endl;
}

CFileOps& CFileOps::GetInstance()
{
    // 使用static确保只会声明一次CFileOps类
    static CFileOps m_pFile;
    return m_pFile;
}

CFileStatus CFileOps::MyReadFileOpen() 
{
    m_ReadFd = open(READFILENAME, O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR); //当open的第二个参数使用了O_CREAT, open还需要第三个参数指定新文件的访问权限位
    if(m_ReadFd == -1) {
        return CFileStatus(false, -1);
    }
    return CFileStatus(true, -1);
}

CFileStatus CFileOps::MyWriteFileOpen()
{
    m_WriteFd = open(WRITEFILENAME, O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR); //当open的第二个参数使用了O_CREAT, open还需要第三个参数指定新文件的访问权限位
    if(m_WriteFd == -1) {
        return CFileStatus(false, -1);
    }
    return CFileStatus(true, -1);    
}

CFileStatus CFileOps::MyReadFileClose()
{
    if(m_ReadFd != -1) {
        int returncode = close(m_ReadFd);
        if(returncode != -1) {
            return CFileStatus(true, -1);
        }
    }
    return CFileStatus(false, -1);
}

CFileStatus CFileOps::MyWriteFileClose() 
{
    if(m_WriteFd != -1) {
        int returncode = close(m_WriteFd);
        if(returncode != -1) {
            return CFileStatus(true, -1);
        }
    }
    return CFileStatus(false, -1);
}

CFileStatus CFileOps::MyFileRead(size_t bytes)
{
    MyFileLseek();
    size_t readBytes = 0;
    size_t totalReadBytes = 0;
    //读策略，攒够了数据量再读
    m_usedReadBufferSize += bytes;
    while(m_usedReadBufferSize >= BUFFER_SIZE_READ_FILE) {
        CFileStatus s = MyFileReadOps(BUFFER_SIZE_READ_FILE);
        totalReadBytes += s.m_clByteSize;
    }
    return CFileStatus(true, totalReadBytes);
}

CFileStatus CFileOps::MyFileReadOps(size_t bytes)
{
    cout << "MyFileReadOps: " << bytes << endl;
    size_t readBytes = read(m_ReadFd, m_pReadBuffer, bytes);
    if(readBytes == -1) {
        cout << "file read error" << endl;
        return CFileStatus(false, -1);
    }
    m_usedReadBufferSize -= readBytes;
    cout << "file read sccuess, readBytes: " << readBytes << endl;
    string s = m_pReadBuffer;
    if(readBytes != 0) {
        readPos += readBytes;
        fileContent += s.substr(0, readBytes);
    }
    return CFileStatus(true, readBytes);
}

CFileStatus CFileOps::MyFileWrite(char* buf)
{
    unsigned int bufSize = strlen(buf);
    cout << "bufSize: " << bufSize << endl;
    unsigned int totoal_size = m_usedWriteBufferSize + bufSize;
    size_t writeBytes = 0;

    // 将buf放到缓存m_pWriteBuffer中， 如果已写入缓存大小m_usedWriteBufferSize >= 规定值BUFFER_SIZE_WRITE_FILE，先填满缓存，如果剩下的还大就一次性写入，否则就留在缓存
    if(totoal_size >= BUFFER_SIZE_WRITE_FILE) {
        unsigned int lefBufferSize = BUFFER_SIZE_WRITE_FILE - m_usedWriteBufferSize;
        memcpy(m_pWriteBuffer + m_usedWriteBufferSize, buf, lefBufferSize);
        m_usedWriteBufferSize += lefBufferSize;
        CFileStatus s = MyFileWriteOps(m_pWriteBuffer, m_usedWriteBufferSize);
        writeBytes += s.m_clByteSize;
        m_usedWriteBufferSize = 0;
        totoal_size -= BUFFER_SIZE_WRITE_FILE;
        if(totoal_size >= BUFFER_SIZE_WRITE_FILE) {
            CFileStatus s2 = MyFileWriteOps(buf + lefBufferSize, totoal_size);
            writeBytes += s2.m_clByteSize;
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
    lseek(m_ReadFd, readPos, SEEK_SET);
    cout << "current position: " << readPos << endl;
    return CFileStatus(true, readPos);
}

size_t CFileOps::GetFileSize()
{
    // 这是一个存储文件(夹)信息的结构体，其中有文件大小和创建时间、访问时间、修改时间等
	struct stat statbuf;

	// 提供文件名字符串，获得文件属性结构体
	stat(READFILENAME, &statbuf);
	
	// 获取文件大小
	size_t filesize = statbuf.st_size;
    return filesize;
}

CFileStatus CFileOps::MyFileWriteOps(char* message, size_t message_len)
{
    size_t writeBytes = write(m_WriteFd, message, message_len);  
    if(writeBytes == -1) {
        cout << "from buffer write error" << endl;
        return CFileStatus(false, -1);
    }
    cout << "from buffer write sccuess, writeBytes: " << writeBytes << endl;
    return CFileStatus(true, writeBytes);
}

void CFileOps::FlushBuffer()
{
    MyFileReadOps(m_usedReadBufferSize);
    cout << "Content is: " << fileContent << endl;
    MyFileWrite((char*)fileContent.c_str());
    MyFileWriteOps(m_pWriteBuffer, m_usedWriteBufferSize);
    return;
}

void CFileOps::OnProcessExit()
{
    // Flush为static, 所以必须要获取实例对象才能使用
    CFileOps& fd = CFileOps::GetInstance();
    fd.FlushBuffer();
    return;
}