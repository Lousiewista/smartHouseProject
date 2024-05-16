#include <wiringPi.h>

#include "gdevice.h"

/* //链表头插法
struct gdevice *add_device_to_gdevice_list(struct gdevice *pgdevhead, struct gdevice *gdev)
{

    if (NULL == pgdevhead) {
        pgdevhead = gdev;
    } else {
        pgdev->next = pgdevhead;
        pgdevhead = gdev;
    }
    return pgdevhead;
} */

//根据key值(buffer[2])查找设备节点
struct gdevice *find_gdevice_by_key(struct gdevice *pgdevhead, int key)
{
    struct gdevice *p = NULL;

    if (NULL == pgdevhead) {
        return NULL;
    }

    p = pgdevhead;

    while (NULL != p) {
        if (p->key == key) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

//设置GPIO引脚状态，输入输出和高低电平
int set_gpio_gdevice_status(struct gdevice *pgdev)
{
    if (NULL == pgdev) {
        return -1;
    }

    if (-1 != pgdev->gpio_pin) {
        if (-1 != pgdev->gpio_mode) {
            pinMode(pgdev->gpio_pin, pgdev->gpio_mode); //配置引脚输入输出的模式
        }

        if (-1 != pgdev->gpio_status) {
            digitalWrite(pgdev->gpio_pin, pgdev->gpio_status); //当配置为输出模式时候，引脚的高低状态
        }
    }

    return 0;

}


