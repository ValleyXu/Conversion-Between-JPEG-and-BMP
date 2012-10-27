#ifndef	BMP_H
#define	BMP_H

#include <windows.h> 
#include "DataType.h"

/*存储BMP图像的一些基本信息*/
typedef	struct
{
	WORD	bitCount;
	WORD	bpl;
	DWORD	height;
	DWORD	width;
	DWORD	palCount;
	DWORD	picSize;
	DWORD	offSet;
	HANDLE	hDIB;
	LPBITMAPINFO	lpbmi;
	LPVOID	lpvbits;
}MainInfo;

/*存储BMP文件头*/
typedef struct
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	FILE *fp;
}BMP_Header;

/*允许转化图片的最大宽度与最大高度*/
#define MAX_WIDTH	1024
#define MAX_HEIGHT	1024

/*读入BMP文件头，确定该文件是否是BMP文件*/
bool isBMPPic(BMP_Header *header, char *BMPfile);

/*根据BMP图像中infoHeader和fileHeader中的内容，获取BMP图像的一些基本信息*/
void GetMainInfo(MainInfo *mi, BMP_Header *image);

/*将各像素的RGB值转化为YCbCr值*/
int RGB2YCrCb(	BYTE Y[][MAX_WIDTH], BYTE Cb[][MAX_WIDTH/2], BYTE Cr[][MAX_WIDTH/2],BYTE R[][MAX_WIDTH], BYTE G[][MAX_WIDTH], BYTE B[][MAX_WIDTH],int width, int height );

/*通过边缘扩充的方法，将图片的width和height和均变成16的倍数*/
void AlignPic( BYTE R[][MAX_WIDTH], BYTE G[][MAX_WIDTH], BYTE B[][MAX_WIDTH],int *wid, int *hei, int width, int height);

/*读取BMP图片中每个像素的RGB值*/
void GetRGB( BYTE R[][MAX_WIDTH], BYTE G[][MAX_WIDTH], BYTE B[][MAX_WIDTH],BYTE BGR[], int width, int height, int bpl );

/*释放基本信息存储的空间*/
void DeleteMainInfo(MainInfo *mi);

/*关闭BMP文件*/
void CloseBMPHeader(BMP_Header *image);

#endif