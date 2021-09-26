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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "utils_file.h"
#include "hal_file.h"

int HalFileOpen(const char *path, int oflag, int mode)
{
    (void)mode;
    return open(path, oflag);
}

int HalFileClose(int fd)
{
    return close(fd);
}

int HalFileRead(int fd, char *buf, unsigned int len)
{
    return read(fd, buf, len);
}

int HalFileWrite(int fd, const char *buf, unsigned int len)
{
    return write(fd, buf, len);
}

int HalFileDelete(const char *path)
{
    return unlink(path);
}

int HalFileStat(const char *path, unsigned int *fileSize)
{
    struct stat st_buf = {0};
    if (stat(path, &st_buf) != 0) {
        return -1;
    }
    if (fileSize) {
        *fileSize = st_buf.st_size;
    }
    return 0;
}

int HalFileSeek(int fd, int offset, unsigned int whence)
{
    return (int)lseek(fd, (off_t)offset, whence);
}
