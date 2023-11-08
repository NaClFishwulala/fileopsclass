#include "CFileStatus.h"

CFileStatus::CFileStatus(bool bReturnCode, long lByteSize) : m_cbReturnCode(m_bReturnCode), m_clByteSize(m_lByteSize)
{
    m_bReturnCode = bReturnCode;
    m_lByteSize = lByteSize;
}

CFileStatus::CFileStatus(const CFileStatus& s) : m_cbReturnCode(m_bReturnCode), m_clByteSize(m_lByteSize)
{
    m_bReturnCode = s.m_bReturnCode;
    m_lByteSize = s.m_lByteSize;
}

CFileStatus::~CFileStatus()
{
}

bool CFileStatus::IsSuccess()
{
    if (m_bReturnCode == false)
    {
        return false;
    }
    return true;
}
