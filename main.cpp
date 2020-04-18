#include <cstdio>
#include <cstring>
#include <errno.h>
#include <cstdlib>
#include <iconv.h>

#ifdef _DEBUG
#pragma comment(lib, "lib/libiconvStaticD.lib")
#else
#pragma comment(lib, "lib/libiconvStatic.lib")
#endif

int main(int argc, char** argv)
{
	char *from;

	if(argc < 2) 
		from = "\xB3\xAA\xB4\xAE\xB0\xED\xB5\xF1\x00\x00";
	else
		from = argv[0];

	char to[1024] = { 0x00, };
	iconv_t iv = nullptr;
	size_t ret, in_size, out_size;

	memset(to, '\0', 1024);

	char* in_buf_ptr = from;
	char* out_buf_ptr = to;

	in_size = strlen(from);
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

	iconv_close(iv);

	return 0;

}