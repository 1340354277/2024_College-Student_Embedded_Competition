#ifndef COMMON_H
#define COMMON_H



typedef struct Region {
    int iLeftUpX;
    int iLeftUpY;
    int iWidth;
    int iHeigh;
}Region, *PRegion;

typedef struct RegionCartesian {
    int iLeftUpX;
    int iLeftUpY;
    int iWidth;
    int iHeigh;
}RegionCartesian, *PRegionCartesian;


typedef bool			uint1;
typedef unsigned char   uint8;
typedef char			int8;
typedef unsigned short  uint16;
typedef short			int16;
typedef unsigned int    uint32;
typedef int				int32;
//typedef unsigned int	size_t;
















#endif // COMMON_H
