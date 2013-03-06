//------------------------------------------------------------------------------
// 文件名称：FileMD5.cpp
// 文件版本：v1.0
// 创建日期：2006-05-05 16:36
// 作    者：Richard Ree
// 文件描述：计算文件的MD5摘要码
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include "stdio.h"
#include "md5.h"

using namespace std;

#define BUFFER_SIZE 1024000

BOOL CalculateMD5(const char *sFilename, unsigned char acMD5Digest[])
{
	if (sFilename == NULL || acMD5Digest == NULL)
	{
		return FALSE;
	}

	// Open file
	FILE *fp = fopen(sFilename, "rb");
	if (NULL == fp)
	{
		cerr << "Fail to open file " << sFilename << "." << endl;
		return FALSE;
	}

	// Get file size
	fseek(fp, 0, SEEK_END);
	int iFileSize = ftell(fp);

	// Allocate memory to store file content and initialize it
	unsigned char *pBuffer = new unsigned char[BUFFER_SIZE];
	memset(pBuffer, 0, BUFFER_SIZE);

	// Read file content and calculate MD5 digest
	int iProcessedLength = 0;
	int iLastBlockSize = iFileSize % BUFFER_SIZE;
	MD5_CTX md5;
	fseek(fp, 0, SEEK_SET);
	DWORD iCPUTick1 = 0;
	DWORD iCPUTick2 = 0;

	while (iProcessedLength + BUFFER_SIZE <= iFileSize)
	{
		// Reduce CPU usage
		iCPUTick1 = GetTickCount();
		Sleep(min((iCPUTick1 - iCPUTick2) / 2, 100));

		// Read file content to the buffer
		if (fread(pBuffer, BUFFER_SIZE, 1, fp) <= 0)
		{
			cerr << "Fail to read file." << endl;
			fclose(fp);
			delete []pBuffer;
			return FALSE;
		}

		// Reduce CPU usage
		iCPUTick2 = GetTickCount();
		Sleep(min((iCPUTick2 - iCPUTick1) / 2, 100));

		// Calculate MD5 digest
		md5.MD5Update(pBuffer, BUFFER_SIZE);

		iProcessedLength += BUFFER_SIZE;
	}
	// Process last block
	if (iLastBlockSize > 0)
	{
		// Read file content to the buffer
		if (fread(pBuffer, iLastBlockSize, 1, fp) <= 0)
		{
			cerr << "Fail to read file." << endl;
			fclose(fp);
			delete []pBuffer;
			return FALSE;
		}

		// Calculate MD5 digest
		md5.MD5Update(pBuffer, iLastBlockSize);
	}

	// For empty file
	if (iFileSize == 0)
	{
		md5.MD5Update(pBuffer, 0);
	}

	// Get result
	md5.MD5Final(acMD5Digest);

	// Close the file opened
	fclose(fp);

	// Free memory
	delete []pBuffer;

	return TRUE;
}

CString MD5toString(unsigned char acMD5Digest[])
{
	CString sMD5;

	sMD5.Format("%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X"
		, acMD5Digest[0], acMD5Digest[1], acMD5Digest[2], acMD5Digest[3]
		, acMD5Digest[4], acMD5Digest[5], acMD5Digest[6], acMD5Digest[7]
		, acMD5Digest[8], acMD5Digest[9], acMD5Digest[10], acMD5Digest[11]
		, acMD5Digest[12], acMD5Digest[13], acMD5Digest[14], acMD5Digest[15]);

	return sMD5;
}
