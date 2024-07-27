#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net   
db.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

// 函数：解析URL，获取主机名、端口号和路径
void parse_url(const char *url, char *host, int *port, char *path) {
    // ... 这里实现 URL 解析的逻辑，可以使用 strtok 或正则表达式等
    // 示例：假设 URL 格式为 http://host:port/path
    sscanf(url, "http://%[^:]:%d/%s", host, port, path);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <URL>\n", argv[0]);
        exit(1);
    }

    char *url = argv[1];
    char host[100], path[100];
    int port = 80; // 默认端口为80

    parse_url(url, host, &port, path);

    // 创建socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // 将域名解析为IP地址
    struct hostent *server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(1);
    }

    // 将IP地址和端口号封装到sockaddr_in结构体中
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr   
.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port   
);

    // 连接服务器
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    // 发送HTTP请求
    char request[BUFFER_SIZE];
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, host);
    if (send(sockfd, request, strlen(request), 0) < 0) {
        perror("send");
        exit(1);
    }

    // 接收数据并计时
    char buffer[BUFFER_SIZE];
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int bytes_received = 0;
    while (1) {
        int n = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (n <= 0)
            break;
        bytes_received += n;
    }
    gettimeofday(&end, NULL);

    // 计算下载速度
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    double speed = (double)bytes_received / elapsed_time;
    printf("Downloaded %d bytes in %.2f seconds.\n", bytes_received, elapsed_time);
    printf("Speed: %.2f MB/s\n", speed / (1024 * 1024));

    close(sockfd);
    return 0;
}
