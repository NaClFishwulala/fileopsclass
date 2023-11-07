#include <iostream>
#include "CFileOps.h"
#include "CFileStatus.h"

using namespace std;

int main()
{
    CFileOps& fd = CFileOps::GetInstance();
    fd.MyFileWrite();
    // fd.MyFileRead(10);
    // fd.MyFileRead(10);
    // fd.MyFileWrite();
    // fd.MyFileRead(10);
    return 0;
}