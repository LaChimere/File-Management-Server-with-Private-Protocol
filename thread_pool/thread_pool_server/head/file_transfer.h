#ifndef FILE_TRANSFER_H
#define FILE_TRANSFER_H

#include "includings.h"

typedef struct {
    size_t length;
    char buf[1 << 10];
} train_t, * pTrain_t;

typedef struct {
    char f_type;
    char f_name[MAX_NAME_LEN];
    size_t f_size;
} file_t, * pFile_t;

int trans_file(int client_fd, const char * f_name);

#endif // FILE_TRANSFER_H