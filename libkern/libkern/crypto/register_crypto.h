/*
 * Copyright (c) 2012 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */

#ifndef _CRYPTO_REGISTER_CRYPTO_H_
#define _CRYPTO_REGISTER_CRYPTO_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <corecrypto/ccdigest.h>
#include <corecrypto/cchmac.h>
#include <corecrypto/ccmode.h>
#include <corecrypto/ccrng.h>
#include <corecrypto/ccrsa.h>
#include <corecrypto/ccchacha20poly1305.h>

/* Function types */

/* digests */
typedef void (*ccdigest_init_fn_t)(const struct ccdigest_info *di, ccdigest_ctx_t ctx);
typedef void (*ccdigest_update_fn_t)(const struct ccdigest_info *di, ccdigest_ctx_t ctx,
    unsigned long len, const void *data);
typedef void (*ccdigest_final_fn_t)(const struct ccdigest_info *di, ccdigest_ctx_t ctx,
    void *digest);
typedef void (*ccdigest_fn_t)(const struct ccdigest_info *di, unsigned long len,
    const void *data, void *digest);

/* hmac */
typedef void (*cchmac_init_fn_t)(const struct ccdigest_info *di, cchmac_ctx_t ctx,
    unsigned long key_len, const void *key);
typedef void (*cchmac_update_fn_t)(const struct ccdigest_info *di, cchmac_ctx_t ctx,
    unsigned long data_len, const void *data);
typedef void (*cchmac_final_fn_t)(const struct ccdigest_info *di, cchmac_ctx_t ctx,
    unsigned char *mac);

typedef void (*cchmac_fn_t)(const struct ccdigest_info *di, unsigned long key_len,
    const void *key, unsigned long data_len, const void *data,
    unsigned char *mac);

/* gcm */
typedef int (*ccgcm_init_with_iv_fn_t)(const struct ccmode_gcm *mode, ccgcm_ctx *ctx,
    size_t key_nbytes, const void *key,
    const void *iv);
typedef int (*ccgcm_inc_iv_fn_t)(const struct ccmode_gcm *mode, ccgcm_ctx *ctx, void *iv);

typedef const struct ccchacha20poly1305_fns {
	const struct ccchacha20poly1305_info *(*info)(void);
	int (*init)(const struct ccchacha20poly1305_info *info, ccchacha20poly1305_ctx *ctx, const uint8_t *key);
	int (*reset)(const struct ccchacha20poly1305_info *info, ccchacha20poly1305_ctx *ctx);
	int (*setnonce)(const struct ccchacha20poly1305_info *info, ccchacha20poly1305_ctx *ctx, const uint8_t *nonce);
	int (*incnonce)(const struct ccchacha20poly1305_info *info, ccchacha20poly1305_ctx *ctx, uint8_t *nonce);
	int (*aad)(const struct ccchacha20poly1305_info *info, ccchacha20poly1305_ctx *ctx, size_t nbytes, const void *aad);
	int (*encrypt)(const struct ccchacha20poly1305_info *info, ccchacha20poly1305_ctx *ctx, size_t nbytes, const void *ptext, void *ctext);
	int (*finalize)(const struct ccchacha20poly1305_info *info, ccchacha20poly1305_ctx *ctx, uint8_t *tag);
	int (*decrypt)(const struct ccchacha20poly1305_info *info, ccchacha20poly1305_ctx *ctx, size_t nbytes, const void *ctext, void *ptext);
	int (*verify)(const struct ccchacha20poly1305_info *info, ccchacha20poly1305_ctx *ctx, const uint8_t *tag);
} *ccchacha20poly1305_fns_t;

/* pbkdf2 */
typedef void (*ccpbkdf2_hmac_fn_t)(const struct ccdigest_info *di,
    unsigned long passwordLen, const void *password,
    unsigned long saltLen, const void *salt,
    unsigned long iterations,
    unsigned long dkLen, void *dk);

/* des weak key testing */
typedef int (*ccdes_key_is_weak_fn_t)(void *key, unsigned long  length);
typedef void (*ccdes_key_set_odd_parity_fn_t)(void *key, unsigned long length);

/* CBC padding (such as PKCS7 or CTSx per NIST standard) */
typedef size_t (*ccpad_cts3_crypt_fn_t)(const struct ccmode_cbc *cbc, cccbc_ctx *cbc_key,
    cccbc_iv *iv, size_t nbytes, const void *in, void *out);

/* rng */
typedef struct ccrng_state *(*ccrng_fn_t)(int *error);

/* rsa */
typedef int (*ccrsa_make_pub_fn_t)(ccrsa_pub_ctx_t pubk,
    size_t exp_nbytes, const uint8_t *exp,
    size_t mod_nbytes, const uint8_t *mod);

typedef int (*ccrsa_verify_pkcs1v15_fn_t)(ccrsa_pub_ctx_t key, const uint8_t *oid,
    size_t digest_len, const uint8_t *digest,
    size_t sig_len, const uint8_t *sig,
    bool *valid);

__enum_decl(crypto_digest_alg_t, unsigned int, {
	CRYPTO_DIGEST_ALG_NONE,
	CRYPTO_DIGEST_ALG_MD5,
	CRYPTO_DIGEST_ALG_SHA1,
	CRYPTO_DIGEST_ALG_SHA256,
	CRYPTO_DIGEST_ALG_SHA384,
	CRYPTO_DIGEST_ALG_SHA512
});

typedef size_t (*crypto_digest_ctx_size_fn_t)(
	crypto_digest_alg_t alg);

typedef void (*crypto_digest_init_fn_t)(
	crypto_digest_alg_t alg,
	void *ctx,
	size_t ctx_size);

typedef void (*crypto_digest_update_fn_t)(
	crypto_digest_alg_t alg,
	void *ctx,
	size_t ctx_size,
	const void *data,
	size_t data_size);

typedef void (*crypto_digest_final_fn_t)(
	crypto_digest_alg_t alg,
	void *ctx,
	size_t ctx_size,
	void *digest,
	size_t digest_size);

typedef void (*crypto_digest_fn_t)(
	crypto_digest_alg_t alg,
	const void *data,
	size_t data_size,
	void *digest,
	size_t digest_size);

typedef size_t (*crypto_hmac_ctx_size_fn_t)(
	crypto_digest_alg_t alg);

typedef void (*crypto_hmac_init_fn_t)(
	crypto_digest_alg_t alg,
	void *ctx,
	size_t ctx_size,
	const void *key,
	size_t key_size);

typedef void (*crypto_hmac_update_fn_t)(
	crypto_digest_alg_t alg,
	void *ctx,
	size_t ctx_size,
	const void *data,
	size_t data_size);

typedef void (*crypto_hmac_final_generate_fn_t)(
	crypto_digest_alg_t alg,
	void *ctx,
	size_t ctx_size,
	void *tag,
	size_t tag_size);

typedef bool (*crypto_hmac_final_verify_fn_t)(
	crypto_digest_alg_t alg,
	void *ctx,
	size_t ctx_size,
	const void *tag,
	size_t tag_size);

typedef void (*crypto_hmac_generate_fn_t)(
	crypto_digest_alg_t alg,
	const void *key,
	size_t key_size,
	const void *data,
	size_t data_size,
	void *tag,
	size_t tag_size);

typedef bool (*crypto_hmac_verify_fn_t)(
	crypto_digest_alg_t alg,
	const void *key,
	size_t key_size,
	const void *data,
	size_t data_size,
	const void *tag,
	size_t tag_size);

typedef struct crypto_functions {
	/* digests common functions */
	ccdigest_init_fn_t ccdigest_init_fn;
	ccdigest_update_fn_t ccdigest_update_fn;
	ccdigest_final_fn_t ccdigest_final_fn;
	ccdigest_fn_t ccdigest_fn;
	/* digest implementations */
	const struct ccdigest_info * ccmd5_di;
	const struct ccdigest_info * ccsha1_di;
	const struct ccdigest_info * ccsha256_di;
	const struct ccdigest_info * ccsha384_di;
	const struct ccdigest_info * ccsha512_di;

	/* hmac common function */
	cchmac_init_fn_t cchmac_init_fn;
	cchmac_update_fn_t cchmac_update_fn;
	cchmac_final_fn_t cchmac_final_fn;
	cchmac_fn_t cchmac_fn;

	/* ciphers modes implementations */
	/* AES, ecb, cbc and xts */
	const struct ccmode_ecb *ccaes_ecb_encrypt;
	const struct ccmode_ecb *ccaes_ecb_decrypt;
	const struct ccmode_cbc *ccaes_cbc_encrypt;
	const struct ccmode_cbc *ccaes_cbc_decrypt;
	const struct ccmode_ctr *ccaes_ctr_crypt;
	const struct ccmode_xts *ccaes_xts_encrypt;
	const struct ccmode_xts *ccaes_xts_decrypt;
	const struct ccmode_gcm *ccaes_gcm_encrypt;
	const struct ccmode_gcm *ccaes_gcm_decrypt;

	ccgcm_init_with_iv_fn_t ccgcm_init_with_iv_fn;
	ccgcm_inc_iv_fn_t ccgcm_inc_iv_fn;

	ccchacha20poly1305_fns_t ccchacha20poly1305_fns;

	/* DES, ecb and cbc */
	const struct ccmode_ecb *ccdes_ecb_encrypt;
	const struct ccmode_ecb *ccdes_ecb_decrypt;
	const struct ccmode_cbc *ccdes_cbc_encrypt;
	const struct ccmode_cbc *ccdes_cbc_decrypt;
	/* Triple DES, ecb and cbc */
	const struct ccmode_ecb *cctdes_ecb_encrypt;
	const struct ccmode_ecb *cctdes_ecb_decrypt;
	const struct ccmode_cbc *cctdes_cbc_encrypt;
	const struct ccmode_cbc *cctdes_cbc_decrypt;
	/* DES key helper functions */
	ccdes_key_is_weak_fn_t ccdes_key_is_weak_fn;
	ccdes_key_set_odd_parity_fn_t ccdes_key_set_odd_parity_fn;
	/* CTS3 padding+encrypt functions */
	ccpad_cts3_crypt_fn_t ccpad_cts3_encrypt_fn;
	ccpad_cts3_crypt_fn_t ccpad_cts3_decrypt_fn;

	/* rng */
	ccrng_fn_t ccrng_fn;

	/* rsa */
	ccrsa_make_pub_fn_t        ccrsa_make_pub_fn;
	ccrsa_verify_pkcs1v15_fn_t ccrsa_verify_pkcs1v15_fn;

	// Digest functions
	crypto_digest_ctx_size_fn_t digest_ctx_size_fn;
	crypto_digest_init_fn_t digest_init_fn;
	crypto_digest_update_fn_t digest_update_fn;
	crypto_digest_final_fn_t digest_final_fn;
	crypto_digest_fn_t digest_fn;

	// HMAC functions
	crypto_hmac_ctx_size_fn_t hmac_ctx_size_fn;
	crypto_hmac_init_fn_t hmac_init_fn;
	crypto_hmac_update_fn_t hmac_update_fn;
	crypto_hmac_final_generate_fn_t hmac_final_generate_fn;
	crypto_hmac_final_verify_fn_t hmac_final_verify_fn;
	crypto_hmac_generate_fn_t hmac_generate_fn;
	crypto_hmac_verify_fn_t hmac_verify_fn;
} *crypto_functions_t;

int register_crypto_functions(const crypto_functions_t funcs);

#ifdef  __cplusplus
}
#endif

#endif /*_CRYPTO_REGISTER_CRYPTO_H_*/
