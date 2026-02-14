#include "FileHexHelper.h"

/// <summary>
/// 创建FileHexHelper
/// </summary>
/// <param name="filePath">PE文件路径</param>
/// <returns></returns>
_FILE_HEX_HELPER* CreateFileHexHelper(const char* filePath)
{
	//获取文件大小并读取文件内容到内存
	FILE* pFile = fopen(filePath, "rb");
	if (pFile == NULL)
	{
		return NULL;
	}
	fseek(pFile, 0, SEEK_END);
	size_t fileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	unsigned char* pData = NULL;
	if (fileSize > 0)
	{
		pData = (unsigned char*)malloc(fileSize);
		if (pData == NULL)
		{
			fclose(pFile);
			return NULL;
		}
		size_t readCount = fread(pData, 1, fileSize, pFile);
		if (readCount != fileSize)
		{
			free(pData);
			fclose(pFile);
			return NULL;
		}
	}
	fclose(pFile);
	//解析PE头
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pData;
	PIMAGE_NT_HEADERS32 pNTHeader;
	if (pDosHeader != NULL)
	{
		pNTHeader = (PIMAGE_NT_HEADERS32)(pData + pDosHeader->e_lfanew);
	}
	else
	{
		pNTHeader = NULL;
	}
	_FILE_HEX_HELPER* fileHexHelper = (_FILE_HEX_HELPER*)malloc(sizeof(_FILE_HEX_HELPER));
	if (fileHexHelper != NULL)
	{
		fileHexHelper->pData = pData;
		fileHexHelper->dataSize = fileSize;
		fileHexHelper->pDosHeader = pDosHeader;
		fileHexHelper->pNTHeader = pNTHeader;
		return fileHexHelper;
	}
	else
	{
		return NULL;
	}
}
/// <summary>
/// 释放创建的指针
/// </summary>
/// <param name="fileHexHelper"></param>
void FreeFileHexHelper(_FILE_HEX_HELPER* fileHexHelper)
{
	free(fileHexHelper);
}
/// <summary>
/// 以十六进制格式打印并显示指定地址范围内的数据及其相关信息。
/// </summary>
/// <param name="pData">指向包含要打印数据的缓冲区的指针（unsigned char*）。</param>
/// <param name="startAddress">要打印范围的起始偏移或地址（size_t），通常为包含边界。</param>
/// <param name="endAddress">要打印范围的结束偏移或地址（size_t），通常为包含边界。</param>
void PrintHexInfo(unsigned char* pData, size_t startAddress, size_t endAddress)
{
	for (size_t i = startAddress; i < endAddress; i++)
	{
		printf("%02x ", pData[i]);
		if ((i - startAddress + 1) % 16 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
}
/// <summary>
/// 将相对虚拟地址 (RVA) 转换为文件内的字节偏移。
/// </summary>
/// <param name="fileHexHelper">指向 _FILE_HEX_HELPER 结构的指针，提供执行 RVA 到文件偏移转换所需的映射或文件信息。</param>
/// <param name="rva">要转换的虚拟地址 (VA)。</param>
/// <returns>对应的文件偏移（以字节为单位）。</returns>
size_t RvaToOffset(_FILE_HEX_HELPER* fileHexHelper, size_t va)
{
	size_t offset = 0;
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(fileHexHelper->pNTHeader);
	for (int i = 0; i < fileHexHelper->pNTHeader->FileHeader.NumberOfSections - 1; i++)
	{
		PIMAGE_SECTION_HEADER pSectionHeaderNext = pSectionHeader + 1;
		if (va >= pSectionHeader->VirtualAddress + fileHexHelper->pNTHeader->OptionalHeader.ImageBase && va < pSectionHeaderNext->VirtualAddress + fileHexHelper->pNTHeader->OptionalHeader.ImageBase)
		{
			offset = va - (pSectionHeader->VirtualAddress + fileHexHelper->pNTHeader->OptionalHeader.ImageBase) + (pSectionHeader->PointerToRawData);
			break;
		}
		offset = va - (pSectionHeader->VirtualAddress + fileHexHelper->pNTHeader->OptionalHeader.ImageBase) + (pSectionHeaderNext->PointerToRawData);
		pSectionHeader++;
	}
	return offset;
}
