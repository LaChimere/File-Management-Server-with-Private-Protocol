#include "../head/file_transfer.h"

int trans_file(int client_fd, const char * f_name) {

    train_t train1;
    memset(&train1, 0, sizeof(train1));
    int ret = 0;
    char path[MAX_PATH_LEN] = {0};
    sprintf(path, "%s%s", "./", f_name);
    // check file
    int fd = open(path, O_RDWR);
    //int has_file = 1;
    //printf("fd=%d",fd);
    if (fd == -1) {
        // file not exists
        // send error info
        //has_file = 0;
        //ret=send(client_fd, &has_file, sizeof(has_file), 0);
        printf("file not exists\n");
        return -1;

    }
    else {
        //ret=send(client_fd, &has_file, sizeof(has_file), 0);
        printf("check  succeed\n");
    }
    //path

    //printf("Path: %s\n", path);
    // send f_name to client
    train1.length = strlen(path);
    strcpy(train1.buf, path);
    ret = send(client_fd, &train1, sizeof(train1.length) + train1.length, 0);
    //printf("ret = %ld\n",sizeof(train1.length));
    ERROR_CHECK(ret, -1, "send");

    // send file length
    struct stat f_info;
    memset(&f_info, 0, sizeof(f_info));
    fstat(fd, &f_info);
    train1.length = sizeof(f_info.st_size);
    memcpy(train1.buf, &f_info.st_size, train1.length);
    ret = send(client_fd, &train1, sizeof(train1.length) + train1.length, 0);
    ERROR_CHECK(ret, -1, "send");

    // send file
    while (1) {
        ret = read(fd, train1.buf, sizeof(train1.buf));
        ERROR_CHECK(ret, -1, "read");

        train1.length = ret;
        ret = send(client_fd, &train1, sizeof(train1.length) + train1.length, 0);
        ERROR_CHECK(ret, -1, "send");
        //printf("ret = %d\n",ret);
        if (0 == train1.length) {
            break;
        }
    }

    close(fd);
    return 0;
}

int cycle_recv(int fd, void * buf, size_t data_length) {
    size_t len = 0;
    size_t total_len = 0;
    char * ptr = (char *) buf;
    while (total_len < data_length) {
        len = recv(fd, ptr + total_len, data_length - total_len, 0);
        total_len += len;
        if (0 == len) {
            return -1;
        }
    }

    return 0;
}

int file_puts(int client_fd ,char* path){
    int fd;
    int dataLen;
    char buf[1000] ={0};
    char temp[1024] = {0};
    cycle_recv(client_fd,&dataLen,sizeof(int));

    //收到-1说明对面输入错误，返回-1
    if(-1 == dataLen){
        return -1;
    }

    cycle_recv(client_fd,buf,dataLen);//接收文件名

    sprintf(temp,"%s/%s",path,buf);//拼接文件存放位置

    fd = open(temp,O_CREAT|O_RDWR,0666);//创建或打开文件
    ERROR_CHECK(fd,-1,"open");

    //接收文件大小
    off_t fileSize;
    off_t downLoadSize = 0;
    cycle_recv(client_fd,&dataLen,sizeof(int));
    cycle_recv(client_fd,&fileSize,dataLen);
    printf("%d %ld\n",dataLen,fileSize);

    //记录下载总时长
    struct timeval start;
    struct timeval end;
    gettimeofday(&start,NULL);

    //接收文件内容
    while(1){
        cycle_recv(client_fd,&dataLen,sizeof(int));//接收小火车数据长度信息

        //dataLen == 0 ，文件传输结束
        if(0 == dataLen){
            break;
        }
        cycle_recv(client_fd,buf,dataLen);//接收小火车中的数据信息

        write(fd,buf,dataLen);//写入到文件

        downLoadSize += dataLen;

    }

    gettimeofday(&end,NULL);
    printf("cost time=%ld\n", (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec);

    close(fd);
    return 0;
}
