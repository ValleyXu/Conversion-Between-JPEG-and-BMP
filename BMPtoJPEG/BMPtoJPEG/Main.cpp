#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "Encode.h"


int main()
{
	char bmp[16], jpeg[16];
	bool isBMP;

	strcpy(bmp,"lena_BMP.bmp");  
	strcpy(jpeg,"lena_JPEG.jpeg");   

	//verify whether input file is .bmp
	isBMP = EncodeWholeFile(jpeg, bmp);

	if( !isBMP )
	{
		printf("file open error\n");
	}	
    else
    {
       	printf("convert successfully\n");
    }
	
	return 0;
}
