#ifndef CFLIESTATUS_H //防止头文件的重复包含和编译。
#define CFLIESTATUS_H

class CFileStatus
{
private:
    bool m_bReturnCode; //true表示文件操作成功，false表示文件操作失败
	long m_lErrorCode;  //保留

public:
	const bool& m_cbReturnCode; //引用声明
	const long& m_clErrorCode;  //保留

public:
    CFileStatus(bool bReturnCode, long lErrorCode);
    CFileStatus(const CFileStatus& s);
    virtual ~CFileStatus();

    bool IsSuccess();
};

#endif