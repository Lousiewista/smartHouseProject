#include "gdevice.h"

struct gdevice brled_gdev = {
    .dev_name = "BR led",
    .key = 0x42,
    .gpio_pin = 5,
    .gpio_mode = OUTPUT,
    .gpio_status = HIGH,
    .check_face_status = 0,
    .voice_set_status = 0,
};

struct gdevice *add_brled_to_gdevice_list(struct gdevice *pgdevhead)
{//头插法
    return add_device_to_gdevice_list(pgdevhead, &brled_gdev);
};