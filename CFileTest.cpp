#include <iostream>
#include "CFileOps.h"
#include "CFileStatus.h"

using namespace std;

int main()
{
    CFileOps& fd = CFileOps::GetInstance();
    size_t fd_size = fd.GetFileSize();
    cout << "neet to read filesize: " << fd_size << endl;
    fd.MyFileRead(fd_size);
    return 0;
}