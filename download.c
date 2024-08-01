#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netd   
b.h>

// hkspjbqrwaxbvqkz
    
#define SERVER "10.120.16.220"
#define PORT 80
#define FILE_PATH "/file/100kb.txt"

int main() {
    int sockfd = 0, bytesReceived = 0;
    char sendBuff[1024];
    char recvBuff[4096];
    struct sockaddr_in serv_addr;

    memset(sendBuff, '0', sizeof(sendBuff));
    memset(recvBuff, '0', sizeof(recvBuff));

    // 创建socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    // 设   
置服务器地址
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将域名转换为IP地址
    if(inet_pton(AF_INET, SERVER, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    // 连接服务器
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    // 构建HTTP GET请求
    sprintf(sendBuff, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", FILE_PATH, SERVER);

    // 发送请求
    if(send(sockfd, sendBuff, strlen(sendBuff), 0) < 0)
    {
        printf("\n Send failed \n");
        return 1;
    }

    // 接收响应
    while ((bytesReceived = read(sockfd, recvBuff, 4096)) > 0) {
        write(STDOUT_FILENO, recvBuff, bytesReceived);
    }

    // 关闭连接
    close(sockfd);

    return 0;
}
