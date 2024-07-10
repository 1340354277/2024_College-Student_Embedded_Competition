#include "dataparser.h"
#include "string.h"


CDataParser::CDataParser()
{
    memset(&m_RecCtrl,0,sizeof(RECCTRL));
    m_TimeFlag=false;
}

CDataParser::~CDataParser()
{
}


