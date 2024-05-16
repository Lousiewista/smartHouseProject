#ifndef __MSG_QUEUE_H
#define __MSG_QUEUE_H

#include <mqueue.h>
#include <pthread.h>
#include <string.h>
#include <error.h>
#include <errno.h>

mqd_t msg_queue_create(void);
mqd_t msg_queue_final(mqd_t mqd);
int send_message(mqd_t mqd, void *msg, int msg_len);

#endif