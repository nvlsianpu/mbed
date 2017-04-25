/*
 *  aes_alt.h
 *
 *  Copyright (C) 2017, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef __AES_ALT__
#define __AES_ALT__

#if defined(MBEDTLS_AES_ALT)
#include "ssi_aes.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{

    int keysize_nr;             /*number of rounds for SW use case, and key size for CC use case*/
    uint32_t *rk;               /*!<  AES round keys for SW use case, and AES key for CC use case*/
    uint32_t* buf;           /*!<  unaligned data  for sw use case  */
    int is_sw;
}
mbedtls_aes_context;

void mbedtls_aes_init( mbedtls_aes_context *ctx );

void mbedtls_aes_free( mbedtls_aes_context *ctx );

int mbedtls_aes_setkey_enc( mbedtls_aes_context *ctx, const unsigned char *key,
                    unsigned int keybits );

int mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx, const unsigned char *key,
                    unsigned int keybits );

int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                    int mode,
                    const unsigned char input[16],
                    unsigned char output[16] );

#if defined(MBEDTLS_CIPHER_MODE_CBC)
int mbedtls_aes_crypt_cbc( mbedtls_aes_context *ctx,
                    int mode,
                    size_t length,
                    unsigned char iv[16],
                    const unsigned char *input,
                    unsigned char *output );
#endif /*MBEDTLS_CIPHER_MODE_CBC*/

#if defined(MBEDTLS_CIPHER_MODE_CTR)
int mbedtls_aes_crypt_ctr( mbedtls_aes_context *ctx,
                       size_t length,
                       size_t *nc_off,
                       unsigned char nonce_counter[16],
                       unsigned char stream_block[16],
                       const unsigned char *input,
                       unsigned char *output );
#endif /*MBEDTLS_CIPHER_MODE_CTR*/

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/*
 * AES CFB is not Supported by Cryptocell.
 * AES-CFB will use SW implementation
 */

int mbedtls_aes_crypt_cfb128( mbedtls_aes_context *ctx,
                       int mode,
                       size_t length,
                       size_t *iv_off,
                       unsigned char iv[16],
                       const unsigned char *input,
                       unsigned char *output );


int mbedtls_aes_crypt_cfb8( mbedtls_aes_context *ctx,
                    int mode,
                    size_t length,
                    unsigned char iv[16],
                    const unsigned char *input,
                    unsigned char *output );
#endif /*MBEDTLS_CIPHER_MODE_CFB */


#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_AES_ALT */
#endif /* __AES_ALT__ */

