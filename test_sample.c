#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "lyx.h"

static uint8_t spa[64] = {
	0x09, 0xf9, 0xdf, 0x31, 0x1e, 0x54, 0x21, 0xa1, 0x50, 0xdd, 0x7d, 0x16, 0x1e, 0x4b, 0xc5, 0xc6,
	0x72, 0x17, 0x9f, 0xad, 0x18, 0x33, 0xfc, 0x07, 0x6b, 0xb0, 0x8f, 0xf3, 0x56, 0xf3, 0x50, 0x20,
	0xcc, 0xea, 0x49, 0x0c, 0xe2, 0x67, 0x75, 0xa5, 0x2d, 0xc6, 0xea, 0x71, 0x8c, 0xc1, 0xaa, 0x60,
	0x0a, 0xed, 0x05, 0xfb, 0xf3, 0x5e, 0x08, 0x4a, 0x66, 0x32, 0xf6, 0x07, 0x2d, 0xa9, 0xad, 0x13,
};

static uint8_t td[34] = {
	0xf5, 0xa0, 0x3b, 0x06, 0x48, 0xd2, 0xc4 ,0x63, 0x0e, 0xea, 0xc5, 0x13, 0xe1, 0xbb, 0x81, 0xa1,
	0x59, 0x44, 0xda, 0x38, 0x27, 0xd5, 0xb7, 0x41, 0x43, 0xac, 0x7e, 0xac, 0xee, 0xe7, 0x20, 0xb3,
	0x11, 0x22,
};

static int32_t show_hex(int8_t* title, void* buf, uint32_t total)
{
	uint32_t idx;
	uint8_t* data = buf;

	printf("%s(%d):\n\t", title, total);
	for (idx = 0; idx < total; idx++) {
		printf("%02hhx", data[idx]);
		printf("%s", 15 == (idx & 15) ? "\n\t" : ", ");
	}
	printf("\n");
	
	return 0;
}

static int32_t test_sm4(int32_t argc, int8_t** argv)
{
	uint8_t cipher[512];
	uint8_t buf[512];
	int32_t len;
	uint32_t type;
	sm4_ctx_t sm4;

	if (argc) {
		type = strtol(argv[0], NULL, 10);
	}

	memset(cipher, 0, sizeof(cipher));
	memset(buf, 0, sizeof(buf));
	memset(&sm4, 0, sizeof(sm4));

	switch (type) {
	case 1:
		printf("sm4 cbc test:\n");
		break;
	case 2:
		printf("sm4 cfb test:\n");
		break;
	case 3:
		printf("sm4 ofb test:\n");
		break;
	case 4:
		printf("sm4 ctr test:\n");
		break;
	default:
		printf("sm4 ecb test:\n");
		type = 0;
		break;
	}
	show_hex("data", td, sizeof(td));

	lyx_sm4_create(type);
	lyx_sm4_enckey(&sm4, spa);
	memcpy(sm4.iv, spa + 16, 16);

	if ((len = lyx_sm4_encrypt(&sm4, sizeof(td), td, cipher)) < 0) {
		return -1;
	}
	show_hex("encode", cipher, len);

	lyx_sm4_deckey(&sm4, spa);
	memcpy(sm4.iv, spa + 16, 16);

	if ((len = lyx_sm4_decrypt(&sm4, len, cipher, buf)) < 0) {
		return -1;
	}
	show_hex("decode", buf, len);

	return 0;
}

int32_t main(int32_t argc, int8_t** argv)
{
	argc -= 1;
	argv += 1;

	return test_sm4(argc, argv);
}

