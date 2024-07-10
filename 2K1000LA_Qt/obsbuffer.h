#ifndef OBSBUFFER_H
#define OBSBUFFER_H
#include "datatypedef.h"


class CObsBuffer
{
public:
    CObsBuffer();
    ~CObsBuffer();
private:
    int	m_Buf[OBS_BUF_LEN];
    int			m_BufIdx;	//缓存索引
    unsigned int			m_BufCnt;	//缓存计数
public:
    void Push(int& newObs);
    bool Pull(int& newObs);

};/* End of CObsBuffer */
#endif // OBSBUFFER_H
