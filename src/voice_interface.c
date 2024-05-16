#if 0

struct control
{
    char control_name[128]; //监听模块名称 
    int (*init)(void); //初始化函数
    void (*final)(void);//结束释放函数
    void *(*get)(void *arg);//监听函数，如语音监听
    void *(*set)(void *arg); //设置函数，如语音播报

    struct control *next;
};

#endif

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "voice_interface.h"
#include "msg_queue.h"
#include "uartTool.h"
#include "control.h"
#include "global.h"


static int serial_fd = -1;

static int voice_init(void)
{
    int serial_fd = myserialOpen (SERIAL_DEV, BAUD);
    printf("%s | %s | %d: serial_fd = %d\n", __FILE__, __func__, __LINE__, serial_fd);

    return serial_fd;
}

static void voice_final(void)
{
    if (-1 != serial_fd) {
        close(serial_fd);
        serial_fd = -1;
    }

}

static void *voice_get(void *arg) //接收语音指令
{
    unsigned char buffer[6] = {0xAA, 0x55, 0x00, 0x00, 0x55, 0xAA};
    int len = 0;
    mqd_t mqd = -1;
    ctrl_info_t *ctrl_info = NULL;

    if (NULL != arg) {
        (ctrl_info_t *)arg;
    }
    
    if (-1 == serial_fd) {
        serial_fd = voice_init();
        
        if (-1 == serial_fd) {
            printf("%s | %s | %d: serial_fd = %d\n", __FILE__, __func__, __LINE__, serial_fd);
            pthread_exit(0);
        }
    }

    if (NULL != arg) {
        ctrl_info_t *ctrl_info_ptr = arg; // 将arg转换回ctrl_info_t *
        mqd = ctrl_info_ptr->mqd; // 现在可以安全地访问mqd成员了
    }

    if ((mqd_t)-1 == mqd) {
        pthread_exit(0);
    }

    pthread_detach(pthread_self());// 分离当前线程

    printf("%s thread star\n", __func__);

    while (1) {
        len = serialGetstring(serial_fd, buffer);
        printf("%s | %s | len = %d\n", __FILE__, __func__, __LINE__);
        if (len > 0) {
            if (buffer[0] == 0xAA && buffer[1] == 0x55 \
            && buffer[5] == 0xAA && buffer[4] == 0x55) {
                send_message(mqd, buffer, len); //注意不要用strlen去计算实际长度，以为信息包含00，会被中断
            }
            
            memset(buffer, 0, sizeof(buffer));
        }

    }

    pthread_exit(0);    
}

static void *voice_set(void *arg) //语音播报
{
    printf("voice_set\n");
    return NULL;
}

struct control voice_control = {
    .control_name = "voice_control",
    .init = voice_init,
    .final = voice_final,
    .get = voice_get,
    .set = voice_set,
    .next = NULL,
};


struct control *add_voice_to_ctrl_list(struct control *phead)
{
    //头插法

    return add_interface_to_ctrl_list(phead, &voice_control);

};