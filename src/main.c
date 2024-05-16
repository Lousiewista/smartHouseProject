#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "voice_interface.h"
#include "msg_queue.h"
#include "control.h"
#include "global.h"
#include "uartTool.h"
#include "socket_interface.h"
#include "smoke_interface.h"


int main(int argc, char *argv[])
{
    // 初始化线程ID和控制链表
    pthread_t thread_id;
    struct control *control_phead = NULL; //控制链表头指针
    struct control *pointer = NULL; // 控制链表遍历指针

    ctrl_info_t *ctrl_info = NULL; //控制信息结构体指针

    ctrl_info = (ctrl_info_t *) malloc(sizeof(ctrl_info_t));
    ctrl_info->ctrl_phead = NULL; //控制链表头指针
    ctrl_info->mqd = -1; //消息队列描述符

    int node_num = 0; //节点数量

    ctrl_info->mqd = msg_queue_create();//创建消息队列

    if (ctrl_info->mqd == -1) {
        printf("%s|%s|%d, ctrl_info->mqd = %d\n", __FILE__, __func__, __LINE__, ctrl_info->mqd);
        return -1;
    }

    ctrl_info->ctrl_phead = add_voice_to_ctrl_list(ctrl_info->ctrl_phead); //添加语音控制节点
    ctrl_info->ctrl_phead = add_tcpsocket_to_ctrl_list(ctrl_info->ctrl_phead); //
    ctrl_info->ctrl_phead = add_smoke_to_ctrl_list(ctrl_info->ctrl_phead);
    //ctrl_info->ctrl_phead = add_file_to_ctrl_list(ctrl_info->ctrl_phead);

    pointer = ctrl_info->ctrl_phead;

    while (NULL != pointer) {
        if (NULL != pointer->init) {
            pointer->init();
        }
        pointer = pointer->next;
        node_num++;
    } //初始化节点

    pthread_t *tid = malloc(sizeof(int) * node_num); //线程ID数组

    int i = 0;

    pointer = ctrl_info->ctrl_phead;

    for (int i = 0; i < node_num; i++) {
        if (NULL != pointer->get) {
            pthread_create(&tid[i], NULL, (void *) pointer->get, (void *) ctrl_info);
        }
        pointer = pointer->next;
    } //创建线程

    for (int i = 0; i < node_num; i++) {
        pthread_join(tid[i], NULL);
    } //等待线程结束

    for (int i = 0; i < node_num; i++) {

        if (NULL != pointer->final) {
            pointer->final();
        }
        pointer = pointer->next;
    } //释放资源

    msg_queue_final(ctrl_info->mqd);

    return 0;

}