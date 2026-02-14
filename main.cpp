#include "main.h"

int main(int argc, char* argv[])
{
	const char* filePath = "E:\\我的课程\\bugkuCTF平台逆向题目\\入门逆向\\baby.exe";
	_FILE_HEX_HELPER* fileHexHelper = CreateFileHexHelper(filePath);
	if (fileHexHelper != NULL)
	{
		printf("File size: %zu bytes\n", fileHexHelper->dataSize);
		printf("test:%08x\n", RvaToOffset(fileHexHelper, 0x40147a));
		PrintHexInfo(fileHexHelper->pData, RvaToOffset(fileHexHelper, 0x40147a), RvaToOffset(fileHexHelper, 0x4014d9));
		//根据题目需求输出flag
		for (size_t i = RvaToOffset(fileHexHelper, 0x40147a) + 4; i < RvaToOffset(fileHexHelper, 0x4014d9); i = i + 5)
		{
			printf("%c", fileHexHelper->pData[i]);
		}
		FreeFileHexHelper(fileHexHelper);
	}
	else
	{
		printf("Failed to read file: %s\n", argv[1]);
	}
	return 0;
}