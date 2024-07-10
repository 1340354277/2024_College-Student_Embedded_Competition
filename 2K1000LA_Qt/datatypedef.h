#ifndef DATATYPEDEF_H
#define DATATYPEDEF_H

#include <QMetaType>


//#define u1 unsigned char
//#define u2 unsigned short
//#define u4 unsigned int
//#define i1 char
//#define i2 short
//#define i4 int
//#define f4 float
typedef	long				i5;	//40位有符号整型数
typedef	unsigned long		u5;	//40位无符号整型数
#define f8 double

#ifndef _MFC_PLAT
#define LONG	int
#define BYTE	unsigned char
#define DWORD	unsigned int
#define WORD	unsigned short
#define BOOL	unsigned char
#endif

enum COMId
{
    COM_NULL =-1,
    COM0 = 0,
    COM1 = 1,
    COM2= 2

};

#define PI		3.1415926535897932384626433832795	//PI
#define FLOAT_ZERO				1.e-13

#define MAX_BUF_LEN  4096
#define	OBS_BUF_LEN	 5
#define	STX			0x02
#define	ETX			0x03


#define		MAXGPSWEEK				1024
#define		GPSWNLEADBDSWN			1356
#define     RB_TIME_OFF_LIM	3.0	//基站和移动站同步时差最大值[s]
#define     MU_GPS			3.986005e14    			 						//[meter^3/sec^2] WGS 84 value of the earth's gravitational constant for GPS user

#define		MS_IN_SEC				1000
#define		US_IN_SEC				1000000
#define		NS_IN_SEC				1000000000

#define		SEC_IN_WEEK				604800
#define		SEC_IN_HALF_WEEK        302400
#define		NS_IN_US				1000
#define		NS_IN_MS				1000000


#define		NS_TO_SEC				(1.0e-9)
#define		MS_TO_SEC				(1.0e-3)

#define		SPEED_OF_LIGHT			299792458.0				//	[m/s]
#define		CARR_FREQ_GPS_L1		1575.42e6
#define		CARR_FREQ_GPS_L2		1227.60e6
#define		CARR_FREQ_BD2_B1		1561.098e6
#define		CARR_FREQ_BD2_B3		1268.520e6

#define		CARR_WAVELEN_GPS_L1		(SPEED_OF_LIGHT/CARR_FREQ_GPS_L1)
#define		CARR_WAVELEN_GPS_L2		(SPEED_OF_LIGHT/CARR_FREQ_GPS_L2)
#define		CARR_WAVELEN_BD2_B1		(SPEED_OF_LIGHT/CARR_FREQ_BD2_B1)
#define		CARR_WAVELEN_BD2_B3		(SPEED_OF_LIGHT/CARR_FREQ_BD2_B3)


enum eRECSTATE
{
    SYNC,		//同步中
    RECEIVING	//接收中
};

typedef struct
{
    eRECSTATE       stat;	//状态
    unsigned int    len;	//数据长度
    unsigned int   	cnt;	//接收计数
    unsigned char   buf[MAX_BUF_LEN];
}RECCTRL;



#endif // DATATYPEDEF_H
