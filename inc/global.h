#ifndef __GLOBAL_H__
#define __GLOBAL_H__

typedef struct {   //结构体
    mqd_t mqd;    //消息队列描述符
    struct control *ctrl_phead; //控制链表头指针
} ctrl_info_t; //控制信息结构体

#endif