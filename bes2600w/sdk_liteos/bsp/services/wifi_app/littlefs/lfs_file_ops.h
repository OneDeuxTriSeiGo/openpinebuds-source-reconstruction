/*
 * Copyright (c) 2021 bestechnic (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __LFS_FILE_OPS_H__
#define __LFS_FILE_OPS_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

void lfs_file_ops_first_call(void);
void *lfs_file_ops_create_file(const char *name, uint32_t size);
void *lfs_file_ops_open_file(const char *name, uint32_t size);
int lfs_file_ops_write_file(void *fh, const uint8_t *buf, uint32_t len);
int lfs_file_ops_read_file(void *fh, uint8_t *buf, uint32_t len);
int lfs_file_ops_close_file(void *fh);
int lfs_file_ops_get_file_size(const char *name, uint32_t *p_size);

#ifdef __cplusplus
}
#endif

#endif /* __LFS_FILE_OPS_H__ */