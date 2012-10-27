#ifndef	HUFFENCODE_H
#define	HUFFENCODE_H

#include "DataType.h"
#include "DCT.h"

extern BYTE zigZagTable[DCT_SIZE][DCT_SIZE];
extern BYTE huffmanLumDCBit[17];
extern BYTE huffmanLumDCVal[12];
extern BYTE huffmanLumACBit[17];
extern BYTE huffmanLumACVal[162];

extern BYTE huffmanChrDCBit[17];
extern BYTE huffmanChrDCVal[12];
extern BYTE huffmanChrACBit[17];
extern BYTE huffmanChrACVal[162];

extern DWORD HuffmanLumDCCode[256], HuffmanLumACCode[256];
extern BYTE HuffmanLumDCSize[256], HuffmanLumACSize[256];

extern DWORD HuffmanChrDCCode[256], HuffmanChrACCode[256];
extern BYTE HuffmanChrDCSize[256], HuffmanChrACSize[256];

/*��8*8�����Z��ɨ��*/
void ZigZagScan(int scan[], int quant[][DCT_SIZE]);

/*����RLE����*/
void RLE(int zlen[], int num[], int *cnt, int scan[]);

/*��ʼ�����ұ�*/
void InitSizeAmplitudeTable();

/*����ĳ����ֵ��sizeAmplitude���е�������ֵ*/
void VLE(int *code, int *group, int val);

/*����Huffman��*/
void MakeHuffTable(unsigned int code[], unsigned char size[], BYTE bit[], BYTE val[]);

/*��ʼ����Ҫ��Huffman���SizeAmplitude��*/
int InitHuffTable(BYTE *lumDCHuffBitPtr, BYTE *lumDCHuffValPtr,BYTE *lumACHuffBitPtr, BYTE *lumACHuffValPtr,BYTE *chrDCHuffBitPtr, BYTE *chrDCHuffValPtr,BYTE *chrACHuffBitPtr, BYTE *chrACHuffValPtr );

#endif