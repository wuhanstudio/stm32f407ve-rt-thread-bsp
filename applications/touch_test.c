#include <rtthread.h>
#include <rtdevice.h>

int touch_test(int argc, char* argv[])
{
    //Find the touch device
    rt_device_t touch = rt_device_find("xpt0");

    if (touch == RT_NULL)
    {
        rt_kprintf("can't find device:%s\n", "xpt0");
        while (1);
    }
    if (rt_device_open(touch, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        while (1);
    }
    while (1)
    {
        //Prepare variable to read out the touch data
        struct rt_touch_data read_data;
        rt_memset(&read_data, 0, sizeof(struct rt_touch_data));
        if (rt_device_read(touch, 0, &read_data, 1) == 1)
        {
            //Print the touch coordinate and the corresponding information
            rt_kprintf("%d %d %d %d %d\n",
                        read_data.event,
                        read_data.x_coordinate,
                        read_data.y_coordinate,
                        read_data.timestamp,
                        read_data.width);
        }
        rt_thread_mdelay(10);
    }
}
MSH_CMD_EXPORT(touch_test, touch screen test)
