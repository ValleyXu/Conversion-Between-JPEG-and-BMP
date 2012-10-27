#ifndef Enode_H
#define Enode_H

#include "DCT.h"
#include "IO.h"
#include "BMP.h"

/*��ͼ��ֳ�8*8С��󣬽��б���*/
int EncodeBlock( OutputStream *strm, int *preCompDC, BYTE block[][DCT_SIZE],
      			BYTE compQuantTablePtr[][DCT_SIZE],
				DWORD *compDCHuffCodePtr, BYTE *compDCHuffSizePtr,
      			DWORD *compACHuffCodePtr, BYTE *compACHuffSizePtr );

/*��ʼ��Huffman��*/
void InitHuffmanTable();

/*��ȡBMPͼ������RGBֵ��ͨ������ȡ��ת����YCbCr��ֵ*/
void RGBToYCrCb(int *width, int *height);

/*��ͷ�ļ���Ϣ�Լ�������Huffman�����Ϣд��JPEG�ļ���*/
void writeHeaderTableInfo();

/*���ƽ�������BMP�ļ�*/
bool EncodeWholeFile(char *destName, char *srcName);

#endif
