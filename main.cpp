#include <cstdio>
#include <cstring>
#include <errno.h>
#include <cstdlib>
#include "include\iconv.h"
#include <Windows.h>

#ifdef _DEBUG
#pragma comment(lib, "lib/libiconvStaticD.lib")
#else
#pragma comment(lib, "lib/libiconvStatic.lib")
#endif

char* hex_to_char(char **from)
{
	char* pBuffer = *from;
	int len = strlen(pBuffer);
	char* hex = new char[len];
	memset(hex, '\0', len);
	int target = 0;
	int i = 0;
	int level = 0;
	unsigned char data = '\0';
	int val = 0;

	while (*pBuffer)
	{
		unsigned char byte = *pBuffer++;
		if (byte == '\\')
		{
			target = 1;
			continue;
		}
		else if (byte == 'x' && target == 1)
		{
			target = 0;
			continue;
		}
		else if (byte >= '0' && byte <= '9')
		{
			byte = byte - '0';
		}
		else if (byte >= 'a' && byte <= 'f')
		{
			byte = byte - 'a' + 10;
		}
		else if (byte >= 'A' && byte <= 'F')
		{
			byte = byte - 'A' + 10;
		}
		
		level++;

		if (level >= 2) {
			data |= byte;
			hex[i] = data;
			level = 0;
			i++;
		}
		else if (level == 1) {
			data = byte << 4; // 1바이트(0xFF)가 8비트이므로 글자를 4바이트씩 밀어야 한다.
			continue;
		}
	}

	return hex;
}

int main(int argc, char** argv)
{
	char *from;

	if(argc < 2) 
		from = "\xB3\xAA\xB4\xAE\xB0\xED\xB5\xF1\x00\x00";
	else
		from = argv[1];

	char* hex = hex_to_char(&from);

	char to[1024] = { 0x00, };
	iconv_t iv = nullptr;
	size_t ret, in_size, out_size;

	memset(to, '\0', 1024);

	char* in_buf_ptr = hex;
	char* out_buf_ptr = to;

	in_size = strlen(hex);
	out_size = sizeof(to); // in_size * 2

	// EUC-KR에서 UTF-8로 변경
	if ((iv = iconv_open("UTF-8", "EUC-KR")) < 0)
	{
		fprintf_s(stderr, "지원하지 않는 형식입니다.\n");
		exit(-1);
	}

	if ((ret = iconv(iv, &in_buf_ptr, &in_size, &out_buf_ptr, &out_size)) < 0) {
		fprintf_s(stderr, "변환에 실패하였습니다.%d:%d\n", ret, errno);
		iconv_close(iv);
		exit(-1);
	}

	system("chcp 65001 >nul");

	printf_s("%s\n", to);

	delete[] hex;

	iconv_close(iv);

	return 0;

}