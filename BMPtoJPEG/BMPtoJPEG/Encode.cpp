#include <stdio.h>

#include "BMP.h"
#include "JPEG.h"
#include "HuffEncode.h"
#include "Encode.h"


/*�洢���ص�RGB�Լ����������YCbCrֵ*/
BYTE R[MAX_HEIGHT][MAX_WIDTH];
BYTE G[MAX_HEIGHT][MAX_WIDTH];
BYTE B[MAX_HEIGHT][MAX_WIDTH];
BYTE Y[MAX_HEIGHT][MAX_WIDTH];
BYTE Cr[MAX_HEIGHT/2][MAX_WIDTH/2];
BYTE Cb[MAX_HEIGHT/2][MAX_WIDTH/2];

/*�洢BMPͷ�ļ��е���Ҫ��Ϣ*/
MainInfo mi;   
 
/*JPEG�ļ��������*/
OutputStream ops;  
     
/*���ƽ�������BMP�ļ���jpeg��bmp�ֱ���ת��ǰ����ļ���*/
bool EncodeWholeFile(char *jpeg, char *bmp)
{
	bool isBMP;         //�ж������ļ��Ƿ���BMP��ʽ
	bool openJPEG;      //��JPEG����ļ��Ƿ�ɹ�
	BMP_Header image;	//�洢BMPͷ�ļ�

	//��֤�򿪵��ļ��Ƿ���BMP��ʽ�����Ҷ���BMPͷ�ļ�����Ϣ
	isBMP = isBMPPic(&image, bmp);
	if ( !isBMP )
		return false;
	printf("�ɹ�����ͼƬͷ�ļ���Ϣ\n");

	//��ȡBMPͼ��ͷ�ļ��е�һЩ������Ϣ
	GetMainInfo(&mi, &image);

	//�������JPEG�ļ���
	openJPEG = InitJPEG(jpeg, &ops); 
	if ( !openJPEG )
		return false;	

	//��ʼ��Huffman��
	InitHuffmanTable();
	
	//��ȡBMPͼ������RGBֵ��ͨ������ȡ��ת����YCbCr��ֵ
	int width, height;
	RGBToYCrCb(&width, &height);
	printf("�ɹ�ʵ����ɫģ��ת��\n");
	
	//��ͷ�ļ���Ϣ�Լ�������Huffman�����Ϣд��JPEG�ļ���
	writeHeaderTableInfo();

	//һ�α���16x16��,ʹɫ�ȶ���ȡ�����Ϊ8x8��
	int preY = 0;
	int	preCb = 0;
	int preCr = 0;                    //��¼��һ8*8���DCֵ
	int i,j,k,l,m,n;
    BYTE block[DCT_SIZE][DCT_SIZE];   //��¼ÿ��8*8���е�Y��Cb��Crֵ

	for (i=0; i<height; i+=(DCT_SIZE<<1)) 
		for (j=0; j<width; j+=(DCT_SIZE<<1))
		{
			//����ÿ16*16�е�4��8*8�����ȿ�
			for (k=0; k<(DCT_SIZE<<1); k+=DCT_SIZE)
				for (l=0; l<(DCT_SIZE<<1); l+=DCT_SIZE)
				{
					for (m=0; m<DCT_SIZE; m++)
						for (n=0; n<DCT_SIZE; n++)
						{
							block[m][n] = Y[i+k+m][j+l+n];
						}
					EncodeBlock( &ops, &preY, block,lumQuantTable,HuffmanLumDCCode,
						            HuffmanLumDCSize,HuffmanLumACCode, HuffmanLumACSize );
				}

			//����ÿ16*16�е�1��8*8��ɫ��Cb��
			for (m=0; m<DCT_SIZE; m++)
				for (n=0; n<DCT_SIZE; n++)
					block[m][n] = Cb[(i>>1)+m][(j>>1)+n];
					
			EncodeBlock( &ops, &preCb, block,chrQuantTable,HuffmanChrDCCode, 
				            HuffmanChrDCSize,HuffmanChrACCode, HuffmanChrACSize );

			//����ÿ16*16�е�1��8*8��ɫ��Cr��
			for (m=0; m<DCT_SIZE; m++)
				for (n=0; n<DCT_SIZE; n++)
					block[m][n] = Cr[(i>>1)+m][(j>>1)+n];
			EncodeBlock( &ops, &preCr, block,chrQuantTable,HuffmanChrDCCode, 
				            HuffmanChrDCSize,HuffmanChrACCode, HuffmanChrACSize );
		}

	//���ļ������ļ���β��д��JPEG�ļ�
	FlushStream(&ops, 1);
	writeEOI(&ops);
	
	//�ͷ��ڴ�ռ䡢�ر��ļ���
	FreeStream(&ops, 0);
	DeleteMainInfo(&mi);
	CloseBMPHeader(&image);

	return true;
}

/*��ʼ��Huffman��*/
void InitHuffmanTable()
{
	InitHuffTable(  huffmanLumDCBit, huffmanLumDCVal,
					huffmanLumACBit, huffmanLumACVal,
     				huffmanChrDCBit, huffmanChrDCVal,
					huffmanChrACBit, huffmanChrACVal );
}

/*��ȡBMPͼ������RGBֵ��ͨ������ȡ��ת����YCbCr��ֵ*/
void RGBToYCrCb(int *width, int *height)
{
	//��ȡBMPͼ��ÿ������RGBֵ��������R��G��B��
	GetRGB(R, G, B, (BYTE *)mi.lpvbits, mi.width, mi.height, mi.bpl);

	//��ͼƬ��width��height�;����16�ı������������16*16��Ļ���
	AlignPic(R, G, B, width, height, mi.width, mi.height);

	//�������ص�RGBֵת��ΪYCbCrֵ
	RGB2YCrCb(Y, Cr, Cb, R, G, B, *width, *height);
}

/*��ͷ�ļ���Ϣ�Լ�������Huffman�����Ϣд��JPEG�ļ���*/
void writeHeaderTableInfo()
{
	//��SOI��APP0д��JPEG�ļ�
	writeSOI(&ops);
	writeAPP0(&ops);
	
	//�����Ⱥ�ɫ�ȵ�������д��JPEG�ļ�
	writeQuantTable(&ops);

  	//��Huffman��д��JPEG�ļ�
	writeHuffmanTable(&ops, &mi);
}

/*�Ա��ֳ�8*8С����ͼ����б���*/
int EncodeBlock(	OutputStream *ops, int *pre, BYTE block[][DCT_SIZE],
      				BYTE quantTable[][DCT_SIZE],
					DWORD *compDCHuffCodePtr, BYTE *compDCHuffSizePtr,
      				DWORD *compACHuffCodePtr, BYTE *compACHuffSizePtr )
{
	int i, j, k;

	double doubleBlock[DCT_SIZE][DCT_SIZE];      //��8*8���е�����ת���ɸ�����
	for (i=0; i<DCT_SIZE; i++)
		for (j=0; j<DCT_SIZE; j++)
			doubleBlock[i][j] = (double)block[i][j];

    //����DCT�仯��dct������¼�仯���DCTϵ��
	double dct[DCT_SIZE][DCT_SIZE];
	getDCT(dct, doubleBlock);

	//����DCTϵ����quant��������������Ľ��
	int quantDCT[DCT_SIZE][DCT_SIZE];
	QuantDCTValue(quantDCT, dct, quantTable); 

	/*��ÿ���DCϵ������Huffman����*/
	int difDC = quantDCT[0][0]-(*pre);              //pre������¼����ʱǰһ���DCֵ
	(*pre) = quantDCT[0][0];

	//ͨ����sizeAmplitude������Huffman����
	int amplitude, size;                             //size��code�ֱ���������ֵ��sizeAmplitude���е�����ֵ
 	VLE(&amplitude, &size, difDC);
	WriteStream(ops, compDCHuffCodePtr[size], compDCHuffSizePtr[size], 1);
 	if (difDC != 0)
  		WriteStream(ops, amplitude, size, 1);

  	
	//��ÿ���ACϵ������z�����Huffman����
	int order[DCT_SIZE*DCT_SIZE];
	ZigZagScan(order, quantDCT);                     //�����������ֵ����Z����󱣴���order��

	//����RL����
	int zlen[DCT_SIZE*DCT_SIZE];
	int num[DCT_SIZE*DCT_SIZE];
	int cnt;
	int val;
	RLE(zlen, num, &cnt, order);

	for (k=0; k<cnt; k++)
	{
		VLE(&amplitude, &size, num[k]);
		val = (zlen[k]<<4)|size;
		WriteStream(ops, compACHuffCodePtr[val], compACHuffSizePtr[val], 1);
		if (zlen[k]!=0 || num[k]!=0)
			WriteStream(ops, amplitude, size, 1);
	}
	return 0;
}






