#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

typedef struct _FILE_HEX_HELPER
{
	unsigned char* pData;
	size_t dataSize;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS32 pNTHeader;
};
_FILE_HEX_HELPER* CreateFileHexHelper(const char* filePath);
void FreeFileHexHelper(_FILE_HEX_HELPER* fileHexHelper);
void PrintHexInfo(unsigned char* pData, size_t startAddress, size_t endAddress);
size_t RvaToOffset(_FILE_HEX_HELPER* fileHexHelper, size_t rva);