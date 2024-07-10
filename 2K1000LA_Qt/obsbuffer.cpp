#include "obsbuffer.h"
#include <math.h>
#include "string.h"


CObsBuffer::CObsBuffer()
{
    memset(&m_Buf, 0, sizeof(m_Buf));
    m_BufIdx = 0;
    m_BufCnt = 0;
}

CObsBuffer::~CObsBuffer()
{
}

void CObsBuffer::Push(int& newObs)
{
    m_Buf[ m_BufIdx ] = newObs;

    if(m_BufCnt < OBS_BUF_LEN)
        m_BufCnt++;

    m_BufIdx++;

    if(m_BufIdx == OBS_BUF_LEN)
        m_BufIdx = 0;
}
//取出最新缓存的观测量
bool CObsBuffer::Pull(int& obs)
{
    int lastIdx = 0;
    if(m_BufCnt)
    {
        lastIdx = m_BufIdx - 1;
        if(lastIdx < 0)
            lastIdx += OBS_BUF_LEN;
        obs = m_Buf[lastIdx];
        return true;
    }
    return false;
}


