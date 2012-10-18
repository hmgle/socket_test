/*
 * client
 * usage: socket_test <count> <size> <serverip>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

enum {
    DEST_PORT = 7335,
    DEFSIZE = 67,
    DEFFREQ = 1,
};

static long n_send;

void show_usage(char *prog)
{
    fprintf(stderr, "Usage: %s [-c <count>] [-p port] [-s <size>] [-f <frequency>] [-q <cycle>] server_ip\n", prog);
    exit(1);
}

int main(int argc, char **argv)
{
    char *thisarg;

    long count;
    int size = DEFSIZE;
    int freq = DEFFREQ;
    int cycle = 0;
    unsigned int port = DEST_PORT;

    int socket_client = -1;
	struct sockaddr_in server;
    int len = sizeof(server);
    int ret;
    char **save_argv = argv;

    argc--;
    argv++;
    while (argc >= 1 && **argv == '-') {
        thisarg = *argv;
        thisarg++;
        switch (*thisarg) {
        case 'c':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            count = atoi(*argv);
            if (count < 11)
                count = 11;
            break;
        case 'p':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            port = atoi(*argv);
            break;
        case 's':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            size = atoi(*argv);
            break;
        case 'f':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            freq = atoi(*argv);
            break;
        case 'q':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            cycle = atoi(*argv);
            break;
        default:
            show_usage(save_argv[0]);
        }
        argc--;
        argv++;
    }
    if (argc < 1)
        show_usage(save_argv[0]);
    char *serverip = *argv;

    memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
    server.sin_port = htons(port);          
    server.sin_addr.s_addr = inet_addr(serverip); 
    socket_client = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_client == -1) {
        perror("socket");
        exit(-1);
    }

    if ((connect(socket_client, (const struct sockaddr *)&server, len)) == -1) {
        perror("connect");
        exit(-1);
    }

    /*
     * 填充缓存区
     */
    char *tmp_buf;
    tmp_buf = (char *)malloc(size);

    while (1) {
        tmp_buf[10] = n_send % 10 + '0';            /* 个位 */
        tmp_buf[9] = (n_send % 100) / 10 + '0';     /* 十位 */
        tmp_buf[8] = (n_send % 1000) / 100 + '0';   /* 百位 */
        tmp_buf[7] = (n_send % 10000) / 1000 + '0';   /* 千位 */
        tmp_buf[6] = (n_send % 100000) / 10000 + '0';   /* 万位 */
        tmp_buf[5] = (n_send % 1000000) / 100000 + '0';   /* 十万位 */
        tmp_buf[4] = (n_send % 10000000) / 1000000 + '0';   /* 百万位 */
        tmp_buf[3] = (n_send % 100000000) / 10000000 + '0';   /* 千万位 */
        tmp_buf[2] = (n_send % 1000000000) / 100000000 + '0';   /* 亿位 */
        // tmp_buf[1] = (n_send % 10000000000) / 1000000000 + '0';   /* 十亿位 */
        if (count > 0 && n_send == count - 1)
            tmp_buf[0] = 'q';

        ret = send(socket_client, tmp_buf, size, MSG_DONTWAIT);
        if (ret < 0) {
            perror("send");
        }
        n_send++;
        if (count > 0 && n_send >= count)
            break;

        int i;
        if (cycle)
            // usleep(1000 * 1000 / freq);
            usleep(cycle);

        if (freq)
            for (i = freq; i > 0; i--)
                ;
    }
#if 0
    for (i = 0; i < count; i++) {
        ret = send(socket_client, tmp_buf, size, MSG_DONTWAIT); 
        if (ret < 0) {
            perror("send");
        }
    }
#endif

    close(socket_client);
    return 0;
}
