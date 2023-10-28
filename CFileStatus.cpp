#include "CFileStatus.h"

CFileStatus::CFileStatus(bool bReturnCode, long lErrorCode) : m_cbReturnCode(m_bReturnCode), m_clErrorCode(m_lErrorCode)
{
    m_bReturnCode = bReturnCode;
    m_lErrorCode = lErrorCode;
}
CFileStatus::CFileStatus(const CFileStatus& s) : m_cbReturnCode(m_bReturnCode), m_clErrorCode(m_lErrorCode)
{
    m_bReturnCode = s.m_bReturnCode;
    m_lErrorCode = s.m_lErrorCode;
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
