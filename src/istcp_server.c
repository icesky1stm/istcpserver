//
// Created by suitm on 2020/12/30.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "./istcp.h"
#include "./islog.h"

#define TEST_PORT 1234

int main(){
    char * ip;
    int ret = 0;
    int listen_sock = 0;
    int conn_sock = 0;
    char rcv_body[10240];
    char rcv_head[1000];
    int rcv_len;
    char snd_body[10240];
    char snd_head[1000];
    int snd_len;
    char msg[]=" {\n"
               " \"response\":{\n"
               " \"head\":{\n"
               " \"msgTp\":\"uops.011.000.01\",\n"
               " \"msgSndTm\":\"20180427130321\",\n"
               " \"msgId\":\"2018042713032100000000000000000000000000000000000000000000000001\",\n"
               " \"issrId\":\"C1010211000012\",\n"
               " \"reIssrId\":\"G4000311000018\",\n"
               " \"drctn\":\"20\",\n"
               " \"signSN\":\"4000406755\",\n"
               " \"reserve\":{\n"
               " \"key1\":\"value1\",\n"
               " \"key2\":\"value2\"\n"
               " }\n"
               " },\n"
               " \"body\":{\n"
               " \"rspCd\":\"0000\",\n"
               " \"rspMsg\":\"成功\" }\n"
               " },\n"
               " \"signature\":\"YyFR3TzdOGvBIHSnI76WgXOnoAtjoQy9aLfZP7WTJO6Jfww/5UL5E6aPoHRwmRyhNlH\n"
               " 0CzDPpzUP8OQzANEDHkR9V9O/8oIuuACJSY7w2YFQj0Tiiq0rpWjPHgDccgef7liDQEbQvz6oueRiEiO\n"
               " 3NkOS8BF7NNOrrXivGKKnx2w=\"\n"
               " }";

    printf("ISTCP_SERVER START, PORT is %d \n", TEST_PORT);
    islog_debug("ISTCP_SERVER START, PORT is %d ", TEST_PORT);

    listen_sock = istcp_listen(TEST_PORT);
    if( listen_sock < 0 ){
        islog_error("listen error [%d],[%s]", listen_sock, strerror(errno));
        printf("listen error [%d],[%s]\n", listen_sock, strerror(errno));
        return -5;
    }
    while(1){
        conn_sock = istcp_accept_gethost(listen_sock, &ip);
        if( conn_sock < 0) {
            islog_error("recv error [%d]", ret);
            return -5;
        }
        islog_info("---->ACCEPT A NEW CONNECTION FROM [%s]<----", ip);

        /***
         * 此处一般用来修改读取报文写信息
         * ***/

        /* 报文头 */
        memset( rcv_head, 0x00, sizeof( rcv_head));
        ret = istcp_recv( conn_sock, rcv_head, 8, 10);
        if( ret < 0){
            islog_error("head recv error [%d]", ret);
            continue;
            /*
            return -5;
            */
        }
        islog_debug("RECV HEAD[%s]", rcv_head);
        rcv_len = atol(rcv_head);
        ret = istcp_recv( conn_sock, rcv_body, rcv_len, 10);
        if( ret < 0){
            islog_error("recv error [%d]", ret);
            return -5;
        }
        islog_debug("RECV BODY[%s]", rcv_body);
        char uuid[33];
        memset( uuid, 0x00, sizeof( uuid));
        memcpy(uuid, rcv_body, 32);
        islog_debug("UUID [%s]", uuid);
        sprintf(snd_body, "%s%s", uuid, msg);
        snd_len = strlen(snd_body);
        sprintf(snd_head, "%08d", snd_len);
        /* 报文体 */
        islog_debug("SND HEAD[%s]", snd_head);
        ret = istcp_send( conn_sock, snd_head, 8, 10);
        if( ret < 0){
            islog_error("recv error [%d]", ret);
            return -5;
        }
        islog_debug("SND BODY[%s]", snd_body);
        ret = istcp_send( conn_sock, snd_body, snd_len, 10);
        if( ret < 0){
            islog_error("recv error [%d]", ret);
            return -5;
        }
        istcp_close(conn_sock);
    }

    return 0;
}
