#include <stdio.h>
#include <board.h>
#include "rtthread.h"
#include <flashdb.h>
#include "drivers/rtc.h"
#include "time.h"

#define FDB_LOG_TAG "FLASHDB"

/* key */
#define KEY_BOOT_COUNT  "boot_count"
#define KEY_DEVICE_ID   "device_id"

/* value */
static uint32_t value_boot_count = 0;
static char value_device_id[] = "54549921";

/* kvdb & tsdb */
struct fdb_kvdb _global_kvdb = {0};
struct fdb_tsdb _global_tsdb = {0};

/* kvdb table */
static struct fdb_default_kv_node default_kv_table[] = {
        {
                KEY_BOOT_COUNT,
                &value_boot_count,
                sizeof(value_boot_count)
        },
        {
                KEY_DEVICE_ID,
                value_device_id,
                sizeof(value_device_id)
        }
}; /* int type KV */

static fdb_time_t get_time(void)
{
    return (fdb_time_t)time(RT_NULL);
}

static void print_system_info(void)
{
    struct fdb_blob blob;
    { /* GET the KV value */
        /* get the "device_id" KV value */
        fdb_kv_get_blob(&_global_kvdb, KEY_DEVICE_ID, fdb_blob_make(&blob, value_device_id, sizeof(value_device_id)));
        /* the blob.saved.len is more than 0 when get the value successful */
        if (blob.saved.len > 0) {
            FDB_INFO("get the 'device_id' value is %s\n", value_device_id);
        } else {
            FDB_INFO("get the 'device_id' failed\n");
            { /* CHANGE the KV value */
                /* change the "boot_count" KV's value */
                fdb_kv_set_blob(&_global_kvdb, KEY_DEVICE_ID, fdb_blob_make(&blob, value_device_id, sizeof(value_device_id)));
                FDB_INFO("set the 'device_id' value to %s\n", value_device_id);
            }
        }
    }
    { /* GET the KV value */
        /* get the "boot_count" KV value */
        fdb_kv_get_blob(&_global_kvdb, KEY_BOOT_COUNT, fdb_blob_make(&blob, &value_boot_count, sizeof(value_boot_count)));
        /* the blob.saved.len is more than 0 when get the value successful */
        if (blob.saved.len > 0) {
            FDB_INFO("get the 'boot_count' value is %d\n", value_boot_count);
        } else {
            FDB_INFO("get the 'boot_count' failed\n");
        }
    }
    { /* CHANGE the KV value */
        /* increase the boot count */
        value_boot_count ++;
        /* change the "boot_count" KV's value */
        fdb_kv_set_blob(&_global_kvdb, KEY_BOOT_COUNT, fdb_blob_make(&blob, &value_boot_count, sizeof(value_boot_count)));
        FDB_INFO("set the 'boot_count' value to %d\n", value_boot_count);
    }
    FDB_INFO("===========================================================\n");
}
static int datbase_init(void)
{
    fdb_err_t result;
    struct fdb_default_kv default_kv;
    default_kv.kvs = default_kv_table;
    default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);

    result = fdb_kvdb_init(&_global_kvdb, "env", "easyflash", &default_kv, NULL);
    if (result != FDB_NO_ERR)
    {
        return -RT_ERROR;
    }
    else
    {
        print_system_info();
    }

    return RT_EOK;
}
INIT_ENV_EXPORT(datbase_init);
