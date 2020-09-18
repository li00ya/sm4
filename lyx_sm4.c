#include <stdio.h>

#include "lyx.h"

static lyx_func_t *ops;
void lyx_sm4_register(lyx_func_t *op)
{
	ops = op;
}

void lyx_sm4_unregister(void)
{
	ops = NULL;
}

int32_t lyx_sm4_create(uint32_t type)
{
	lyx_func_t *func = NULL;

	if (NULL == ops || type >= SM4_MODE_MAX) {
		return -1;
	}
	func = ops;

	ops = &func[type];

	return 0;
}

int32_t lyx_sm4_destroy(void)
{
	return 0;
}

int32_t lyx_sm4_enckey(sm4_ctx_t *ctx, uint8_t* key)
{
	ops->enckey(ctx, key);

	return 0;
}

int32_t lyx_sm4_deckey(sm4_ctx_t *ctx, uint8_t* key)
{
	ops->deckey(ctx, key);
	return 0;
}

int32_t lyx_sm4_encrypt(sm4_ctx_t *ctx, uint32_t len, uint8_t *input, uint8_t *output)
{
	return ops->encdata(ctx, len, input, output);
}

int32_t lyx_sm4_decrypt(sm4_ctx_t *ctx, uint32_t len, uint8_t *input, uint8_t *output)
{
	return ops->decdata(ctx, len, input, output);
}
