/*
 * server 
 * usage: 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#ifndef MAX_RECV_BUF
#define MAX_RECV_BUF    1500
#endif

enum {
    DEST_PORT = 7335,
};

static long count;

void show_usage(char *prog)
{
    fprintf(stderr, "Usage: %s [-p port]\n", prog);
    exit(1);
}

int main(int argc, char **argv)
{
    uint32_t port = DEST_PORT;

    char *thisarg;
    argc--;
    argv++;
    while (argc >= 1 && **argv == '-') {
        thisarg = *argv;
        thisarg++;
        switch (*thisarg) {
        case 'p':
            if (--argc <= 0)
                show_usage(argv[0]);
            argv++;
            port = atoi(*argv);
            break;
        default:
            show_usage(argv[0]);
        }
        argc--;
        argv++;
    }

    struct sockaddr_in si_me;
    struct sockaddr_in si_from;
    int socket_s = -1;
    int client_len;
    int ret;
    int i;
    char recv_buf[MAX_RECV_BUF];

    socket_s = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_s < 0) {
        perror("socket");
        exit(errno);
    }

    // fprintf(stderr, "---%s: %d----\n", __FUNCTION__, __LINE__);

    memset((char *)&si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    fprintf(stderr, "port is %d\n", port);
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(socket_s, (const struct sockaddr *)&si_me, (socklen_t)sizeof(si_me));
    if (ret == -1) {
        perror("bind");
        exit(errno);
    }

    // fprintf(stderr, "---%s: %d----\n", __FUNCTION__, __LINE__);

    while (1) {
        ret = recvfrom(socket_s, recv_buf, MAX_RECV_BUF, 0, (struct sockaddr *)&si_from, (socklen_t *)&client_len);
        if (ret < 0) {
            perror("recvfrom");
        } else if (ret == MAX_RECV_BUF) {           /* 长度正确 */
            fprintf(stderr, "--count is %ld--\n", count);
            count++;
        } else {
            fprintf(stderr, "rev is %d\n", ret);    /* 长度截断 */
            fprintf(stderr, "--count is %ld--\n", count);
            count++;
        }

        if (recv_buf[0] == 'q') {
            fprintf(stderr, "quit\n");
            break;
        }
        /*
         * 顺序打印
         */
        fprintf(stderr, "----origin order: ");
        for (i = 1; i <= 10; i++)
            fputc(recv_buf[1], stderr);
    }

    return 0;
}
