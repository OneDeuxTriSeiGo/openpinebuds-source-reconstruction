#ifdef WIFI_STORAGE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "wifi_device.h"
#include "hal_trace.h"
#define WIFI_STORAGE_FILE "/data/wifi_storage"

#ifdef WIFI_SECURITY_STORAGE
#include <mbedtls/cipher.h>
#include <mbedtls/platform.h>
#include "hal_trng.h"

#define WIFI_SECURITY_KEY "abcdefghijklmnop" // replace with pki
#define WIFI_SECURITY_ADD "zxczxxxxxxxsdfadascxz"

struct wifi_storage {
    unsigned char iv[16];
    unsigned char tag[16];
    unsigned char cipher[(sizeof(WifiDeviceConfig) + 0xF) & (~0xF)];
    unsigned int len;
};
#else
struct wifi_storage {
    WifiDeviceConfig cfg;
};
#endif
static struct wifi_storage storage;

#ifdef WIFI_SECURITY_STORAGE
static int WifiRandom(unsigned char *output, size_t len)
{
    int ret = hal_get_trngdata(output, len);
    if (ret) {
        for (size_t i = 0; i < len; i++) {
            output[i] = rand() & 0xFF;
        }
    }
    return 0;
}

static int WifiStorageEncrypt(const void *input, size_t input_size)
{
    // generate iv
    WifiRandom(storage.iv, sizeof(storage.iv));

    mbedtls_cipher_context_t ctx;
    const mbedtls_cipher_info_t *info;

    mbedtls_cipher_init(&ctx);
    info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_GCM);

    mbedtls_cipher_setup(&ctx, info);
    mbedtls_cipher_setkey(&ctx, (const unsigned char *)WIFI_SECURITY_KEY, strlen(WIFI_SECURITY_KEY) * 8,
                          MBEDTLS_ENCRYPT);

    int ret = mbedtls_cipher_auth_encrypt(&ctx, (const unsigned char *)storage.iv, sizeof(storage.iv),
                                          (const unsigned char *)WIFI_SECURITY_ADD, strlen(WIFI_SECURITY_ADD),
                                          (const unsigned char *)input, input_size,
                                          storage.cipher, &storage.len, storage.tag, sizeof(storage.tag));
    printf("%s: ret %d\n", __func__, ret);
    mbedtls_cipher_free(&ctx);
    return ret;
}

static int WifiStorageDecrypt(void *output, size_t *output_size)
{
    mbedtls_cipher_context_t ctx;
    const mbedtls_cipher_info_t *info;

    mbedtls_cipher_init(&ctx);
    info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_GCM);

    mbedtls_cipher_setup(&ctx, info);
    mbedtls_cipher_setkey(&ctx, (const unsigned char *)WIFI_SECURITY_KEY, strlen(WIFI_SECURITY_KEY) * 8,
                          MBEDTLS_DECRYPT);
    int ret = mbedtls_cipher_auth_decrypt(&ctx, (const unsigned char *)storage.iv, sizeof(storage.iv),
                                          (const unsigned char *)WIFI_SECURITY_ADD, strlen(WIFI_SECURITY_ADD),
                                          storage.cipher, storage.len, output, output_size,
                                          storage.tag, sizeof(storage.tag));
    mbedtls_cipher_free(&ctx);
    printf("%s: ret %d\n", __func__, ret);
    return ret;
}
#endif

int WriteWifiDeviceConfigToFile(const WifiDeviceConfig *cfg)
{
    int ret = -1;
    FILE *fp = fopen(WIFI_STORAGE_FILE, "w");
    if (fp == NULL) {
        printf("%s: fopen failed\n", __func__);
        return -1;
    }
    printf("%s: fopen succ!!!!!!!!!!!!!\n", __func__);
#ifndef WIFI_SECURITY_STORAGE
    if (fwrite(cfg, 1, sizeof(WifiDeviceConfig), fp) == sizeof(WifiDeviceConfig)) {
        ret = 0;
    }
#else
    if (WifiStorageEncrypt(cfg, sizeof(WifiDeviceConfig)) == 0) {
        if (fwrite(&storage, 1, sizeof(storage), fp) == sizeof(storage)) {
            ret = 0;
        }
    }
#endif
    fclose(fp);
    if (ret) {
        printf("%s: failed\n", __func__);
    }
    return ret;
}

int ReadWifiDeviceConfigFromFile(WifiDeviceConfig *cfg)
{
    int ret = -1;
    FILE *fp = fopen(WIFI_STORAGE_FILE, "r");
    if (fp == NULL) {
        printf("%s: fopen failed\n", __func__);
        return -1;
    }
#ifndef WIFI_SECURITY_STORAGE
    if (fread(cfg, 1, sizeof(WifiDeviceConfig), fp) == sizeof(WifiDeviceConfig)) {
        ret = 0;
    }
#else
    if (fread(&storage, 1, sizeof(storage), fp) == sizeof(storage)) {
        size_t len = 0;
        if (WifiStorageDecrypt(cfg, &len) == 0) {
            ret = (len == sizeof(WifiDeviceConfig)) ? 0 : -1;
        }
    }
#endif
    fclose(fp);
    if (ret) {
        printf("%s: failed\n", __func__);
    }
    return ret;
}

int DelWifiDeviceConfigFile()
{
    return unlink(WIFI_STORAGE_FILE);
}

#endif
