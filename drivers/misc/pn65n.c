/*
 * Copyright (C) 2012 Sony Mobile Communications AB.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>
#include <linux/spinlock.h>

#include <linux/nfc/pn65n.h>
#include <linux/regulator/consumer.h>

#define MAX_BUFFER_SIZE 512

struct pn65n_dev    {
    wait_queue_head_t   read_wq;
    struct mutex        read_mutex;
    struct i2c_client   *client;
    struct miscdevice   pn65n_device;
    unsigned int        ven_gpio;
    unsigned int        firm_gpio;
    unsigned int        irq_gpio;
    bool                irq_enabled;
    spinlock_t          irq_enabled_lock;
    bool                power_on; //Robert, 20120808
};

static int pn65n_power(int on);

static ssize_t pn65n_show_power_on(struct device *dev,
                   struct device_attribute *attr, char *buf)
{
    struct pn65n_i2c_platform_data *platform_data = dev->platform_data;

    printk("NFC -> pn65n_show_power_on\n");

    #if 0  //B: Robert, 20121003
    return sprintf(buf, "pn65n_show_power_on: VEN=%d, DL=%d\n",
        gpio_get_value_cansleep(platform_data->ven_gpio), gpio_get_value(platform_data->firm_gpio));
    #else
    return snprintf(buf, MAX_BUFFER_SIZE, "pn65n_show_power_on: VEN=%d, DL=%d\n",
        gpio_get_value_cansleep(platform_data->ven_gpio), gpio_get_value(platform_data->firm_gpio));
    #endif //E: Robert, 20121003
}

static ssize_t pn65n_store_power_on(struct device *dev,
                    struct device_attribute *attr,
                    const char *buf, size_t count)
{
    int ret = 0;
    struct pn65n_i2c_platform_data *platform_data = dev->platform_data;

    printk("NFC -> pn65n_store_power_on\n");

    /* power on */
    ret = pn65n_power(1);
    gpio_set_value(platform_data->firm_gpio, 0);
    gpio_set_value_cansleep(platform_data->ven_gpio, 1);
    printk("NFC -> VEN=%d, DL=%d\n",
        gpio_get_value_cansleep(platform_data->ven_gpio),
        gpio_get_value(platform_data->firm_gpio));

    msleep(10);

    return 0;
}

static DEVICE_ATTR(power_on, S_IWUSR | S_IRUGO,
           pn65n_show_power_on, pn65n_store_power_on);

static ssize_t pn65n_show_nfc_i2c(struct device *dev,
                   struct device_attribute *attr, char *buf)
{
    struct i2c_client *client = to_i2c_client(dev);
    struct pn65n_i2c_platform_data *platform_data = dev->platform_data;
    int ret = 0;
    int nfc_irq_value = 0;
    char tmp[MAX_BUFFER_SIZE] = {'0'};
    size_t count = MAX_BUFFER_SIZE;

    printk("NFC -> pn65n_show_nfc_i2c\n");

    nfc_irq_value = gpio_get_value(platform_data->irq_gpio);

    /* Read data */
    ret = i2c_master_recv(client, tmp, count);
    printk("NFC -> ret=%d, IRQ=%d\n", ret, nfc_irq_value);

    if (ret < 0) {
        printk("NFC -> %s: i2c_master_recv returned %d\n", __func__, ret);

        #if 0  //B: Robert, 20121003
        return sprintf(buf, "IRQ[%d] i2c_master_recv returned %d\n", nfc_irq_value, ret);
        #else
        return snprintf(buf, MAX_BUFFER_SIZE, "IRQ[%d] i2c_master_recv returned %d\n", nfc_irq_value, ret);
        #endif //E: Robert, 20121003
    }
    if (ret > count) {
        printk("NFC -> %s: received too many bytes from i2c (%d)\n",
            __func__, ret);
        #if 0  //B: Robert, 20121003
        return sprintf(buf, "[NFC]IRQ=%d received too many bytes from i2c (%d)\n", nfc_irq_value, ret);
        #else
        return snprintf(buf, MAX_BUFFER_SIZE, "[NFC]IRQ=%d received too many bytes from i2c (%d)\n", nfc_irq_value, ret);
        #endif //E: Robert, 20121003

    }

    printk("NFC -> data=%s\n", tmp);
    #if 0  //B: Robert, 20121003
    return sprintf(buf, "data=%s\n", tmp);
    #else
    return snprintf(buf, MAX_BUFFER_SIZE, "data=%s\n", tmp);
    #endif //E: Robert, 20121003

}

static ssize_t pn65n_store_nfc_i2c(struct device *dev,
                    struct device_attribute *attr,
                    const char *buf, size_t count)
{
    struct i2c_client *client = to_i2c_client(dev);
    int ret = 0;

    printk("NFC -> pn65n_store_nfc_i2c\n");

    /* Write data */
    ret = i2c_master_send(client, buf, count);
    printk("NFC -> number of bytes written to I2C (%d)\n", ret);
    if (ret != count) {
        pr_err("%s : i2c_master_send returned %d\n", __func__, ret);
        ret = -EIO;
    }

    return ret;
}

static DEVICE_ATTR(nfc_i2c, S_IWUSR | S_IRUGO,
           pn65n_show_nfc_i2c, pn65n_store_nfc_i2c);

static struct attribute *pn65n_attributes[] = {
    &dev_attr_power_on.attr,
    &dev_attr_nfc_i2c.attr,
    NULL
};

static const struct attribute_group pn65n_attr_group = {
    .attrs = pn65n_attributes,
};

static void pn65n_gpio_init(struct pn65n_i2c_platform_data *platform_data)
{
    //B: Robert, 20120808
    gpio_tlmm_config(GPIO_CFG(platform_data->irq_gpio, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
    gpio_tlmm_config(GPIO_CFG(platform_data->firm_gpio, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
    gpio_tlmm_config(GPIO_CFG(platform_data->ven_gpio, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
    //E: Robert, 20120808
}

static int pn65n_power(int on)
{
    /* VREG_L11 for PN65N:PVDD */

    static struct regulator *vreg_l11;
    int ret = 0;

    vreg_l11 = regulator_get(NULL, "8038_l11");
    if (IS_ERR(vreg_l11))
    {
        printk("NFC -> regulator_get(8038_l11) vreg_l11 fail.\n");
        goto error;
    }

#if 1
    ret = regulator_set_voltage(vreg_l11, 1800000, 1800000);
    if (ret)
    {
        printk("NFC -> regulator_set_voltage(8038_l11) vreg_l11 fail.\n");
        regulator_put(vreg_l11);
        goto error;
    }
#endif
    
    if( on )
    {
        ret = regulator_enable(vreg_l11);
        if (ret)
        {
            printk("NFC -> regulator_enable(8038_l11) vreg_l11 fail.\n");
            regulator_put(vreg_l11);
            goto error;
        }
    }
    else
    {
        if ( regulator_is_enabled(vreg_l11) <= 0 )
        goto error;

        ret = regulator_disable(vreg_l11);
        if (ret)
        {
            printk("NFC -> regulator_disable(8038_l11) vreg_l11 fail\n");
            regulator_put(vreg_l11);
            goto error;
        }
    }

    return 0;

error:
    return -1;
}

static int pn65n_enable_ven(int enable, struct pn65n_dev *pn65n_dev_data)
{
    if( enable )
    {
        gpio_set_value_cansleep(pn65n_dev_data->ven_gpio, 1);
        //gpio_set_value(pn65n_dev_data->ven_gpio, 1);
        //printk("NFC -> NFC_EXT_EN high\n");
    }
    else
    {
        gpio_set_value_cansleep(pn65n_dev_data->ven_gpio, 0);
        //gpio_set_value(pn65n_dev_data->ven_gpio, 0);
        //printk("NFC -> NFC_EXT_EN low\n");
    }

    return 0;
}

static int pn65n_enable_firm(int enable, struct pn65n_dev *pn65n_dev_data)
{
    // PN65N:FIRM <== NFC_DL

    if( enable )
    {
        gpio_set_value(pn65n_dev_data->firm_gpio, 1);
        //printk("NFC -> NFC_DL high\n");
    }
    else
    {
        gpio_set_value(pn65n_dev_data->firm_gpio, 0);
        //printk("NFC -> NFC_DL low\n");
    }

    return 0;
}

static void pn65n_disable_irq(struct pn65n_dev *pn65n_dev_data)
{
    unsigned long flags;

    spin_lock_irqsave(&pn65n_dev_data->irq_enabled_lock, flags);
    if (pn65n_dev_data->irq_enabled) {
        disable_irq_nosync(pn65n_dev_data->client->irq);
        pn65n_dev_data->irq_enabled = false;
    }
    spin_unlock_irqrestore(&pn65n_dev_data->irq_enabled_lock, flags);
}

static irqreturn_t pn65n_dev_irq_handler(int irq, void *dev_id)
{
    struct pn65n_dev *pn65n_dev_data = dev_id;

    pn65n_disable_irq(pn65n_dev_data);

    /* Wake up waiting readers */
    wake_up(&pn65n_dev_data->read_wq);

    return IRQ_HANDLED;
}

static ssize_t pn65n_dev_read(struct file *filp, char __user *buf,
        size_t count, loff_t *offset)
{
    struct pn65n_dev *pn65n_dev_data = filp->private_data;
    char tmp[MAX_BUFFER_SIZE];
    int ret;

    //printk("NFC -> pn65n_dev_read()\n");
    if (count > MAX_BUFFER_SIZE)
        count = MAX_BUFFER_SIZE;

    pr_debug("%s : reading %zu bytes.\n", __func__, count);

    mutex_lock(&pn65n_dev_data->read_mutex);

    if (!gpio_get_value(pn65n_dev_data->irq_gpio)) {
        if (filp->f_flags & O_NONBLOCK) {
            ret = -EAGAIN;
            goto fail;
        }

        #if 0 //B: Robert, 20120813
        pn65n_dev_data->irq_enabled = true;
        enable_irq(pn65n_dev_data->client->irq);
        ret = wait_event_interruptible(pn65n_dev_data->read_wq,
                gpio_get_value(pn65n_dev_data->irq_gpio));

        pn65n_disable_irq(pn65n_dev_data);

        if (ret)
            goto fail;

        #else //

        while (1) {
            pn65n_dev_data->irq_enabled = true;
            enable_irq(pn65n_dev_data->client->irq);
            ret = wait_event_interruptible(
                    pn65n_dev_data->read_wq,
                    !pn65n_dev_data->irq_enabled);

            pn65n_disable_irq(pn65n_dev_data);

            if (ret)
                goto fail;

            if (gpio_get_value(pn65n_dev_data->irq_gpio))
                break;

            //pr_warning("%s: spurious interrupt detected\n", __func__);
        }
        #endif //E: Robert, 20120813
    }

    /* Read data */
    ret = i2c_master_recv(pn65n_dev_data->client, tmp, count);
    mutex_unlock(&pn65n_dev_data->read_mutex);

    /* pn544 seems to be slow in handling I2C read requests
     * so add 1ms delay after recv operation */
    udelay(1000); //Robert, 20120813

    if (ret < 0) {
        pr_err("%s: i2c_master_recv returned %d\n", __func__, ret);
        return ret;
    }
    if (ret > count) {
        pr_err("%s: received too many bytes from i2c (%d)\n",
            __func__, ret);
        return -EIO;
    }
    if (copy_to_user(buf, tmp, ret)) {
        pr_warning("%s : failed to copy to user space\n", __func__);
        return -EFAULT;
    }
    return ret;

fail:
    mutex_unlock(&pn65n_dev_data->read_mutex);
    return ret;
}

static ssize_t pn65n_dev_write(struct file *filp, const char __user *buf,
        size_t count, loff_t *offset)
{
    struct pn65n_dev  *pn65n_dev_data;
    char tmp[MAX_BUFFER_SIZE];
    int ret;

    //printk("NFC -> pn65n_dev_write()\n");
    pn65n_dev_data = filp->private_data;

    if (count > MAX_BUFFER_SIZE)
        count = MAX_BUFFER_SIZE;

    if (copy_from_user(tmp, buf, count)) {
        pr_err("%s : failed to copy from user space\n", __func__);
        return -EFAULT;
    }

    pr_debug("%s : writing %zu bytes.\n", __func__, count);
    /* Write data */
    ret = i2c_master_send(pn65n_dev_data->client, tmp, count);
    if (ret != count) {
        pr_err("%s : i2c_master_send returned %d\n", __func__, ret);
        ret = -EIO;
    }

    /* pn544 seems to be slow in handling I2C write requests
     * so add 1ms delay after I2C send oparation */
    udelay(1000); //Robert, 20120813

    return ret;
}

static int pn65n_dev_open(struct inode *inode, struct file *filp)
{
    struct pn65n_dev *pn65n_dev_data = container_of(filp->private_data,
                        struct pn65n_dev,
                        pn65n_device);

    filp->private_data = pn65n_dev_data;
    
    printk("NFC -> pn65n_dev_open pn65n_dev_data[%p]\n", pn65n_dev_data);
    pr_debug("%s : %d,%d\n", __func__, imajor(inode), iminor(inode));

    return 0;
}

static long pn65n_dev_ioctl(struct file *filp,
        unsigned int cmd, unsigned long arg)
{
        /* PN65N:VEN <== NFC_EXT_EN pull-up by PM8921 GPIO44 
                pn65n_enable_ven is "LOW" enable in DA85 EVT0
        */
    struct pn65n_dev *pn65n_dev_data = filp->private_data;

    switch (cmd) {
    case PN544_SET_PWR:
        if (arg == 2) {
            printk("NFC -> pn65n_dev_ioctl: power on with firmware download (requires hw reset)\n");
            /* power on with firmware download (requires hw reset) */
            pn65n_enable_ven(0, pn65n_dev_data);
            pn65n_enable_firm(1, pn65n_dev_data);
            msleep(10);
            pn65n_enable_ven(1, pn65n_dev_data);
            msleep(10);
            pn65n_enable_ven(0, pn65n_dev_data);
            msleep(10);
        } else if (arg == 1) {
            printk("NFC -> pn65n_ioctl: power on\n");
            /* power on */
            pn65n_power(1);
            msleep(20);
            pn65n_enable_firm(0, pn65n_dev_data);
            pn65n_enable_ven(0, pn65n_dev_data);
            pn65n_dev_data->power_on=1; //Robert, 20120808
            msleep(10);
        } else  if (arg == 0) {
            printk("NFC -> pn65n_ioctl: power off\n");
            /* power off */
            pn65n_enable_firm(0, pn65n_dev_data);
            pn65n_enable_ven(1, pn65n_dev_data);
            pn65n_power(0);
            pn65n_dev_data->power_on=0; //Robert, 20120808
            msleep(10);
        } else {
            pr_err("%s bad arg %u\n", __func__, (unsigned int)arg);
            return -EINVAL;
        }
        break;
    default:
        pr_err("%s bad ioctl %u\n", __func__, cmd);
        return -EINVAL;
    }

    return 0;
}

static const struct file_operations pn65n_dev_fops = {
    .owner  = THIS_MODULE,
    .llseek = no_llseek,
    .read   = pn65n_dev_read,
    .write  = pn65n_dev_write,
    .open   = pn65n_dev_open,
    .unlocked_ioctl = pn65n_dev_ioctl,
};

//-------------- Suspend ---------------//
static int pn65n_suspend(struct i2c_client *client, pm_message_t mesg)
{
    /* power off */
    struct pn65n_i2c_platform_data *platform_data;

    //B: Robert, 20120808
    struct pn65n_dev *pn65n_dev_data; 
    pn65n_dev_data = i2c_get_clientdata(client);
    //E: Robert, 20120808

    platform_data = client->dev.platform_data;
    gpio_set_value_cansleep(platform_data->ven_gpio,1);

    if( pn65n_dev_data->power_on == 0 ) //Robert, 20120808
        gpio_tlmm_config(GPIO_CFG(platform_data->irq_gpio, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);

    return 0;
}

//-------------- Resume ---------------//
static int pn65n_resume(struct i2c_client *client)
{

    /* power on */
    struct pn65n_i2c_platform_data *platform_data;  

    //B: Robert, 20120808
    struct pn65n_dev *pn65n_dev_data;
    pn65n_dev_data = i2c_get_clientdata(client);
    //E: Robert, 20120808

    platform_data = client->dev.platform_data;
    gpio_set_value_cansleep(platform_data->ven_gpio,0);

    if( pn65n_dev_data->power_on == 0 ) //Robert, 20120808
        gpio_tlmm_config(GPIO_CFG(platform_data->irq_gpio, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
    
    return 0;
}


static int pn65n_probe(struct i2c_client *client,
        const struct i2c_device_id *id)
{
    int ret;
    struct pn65n_i2c_platform_data *platform_data;
    struct pn65n_dev *pn65n_dev_data;

    platform_data = client->dev.platform_data;

    if (platform_data == NULL) {
        pr_err("%s : nfc probe fail\n", __func__);
        return  -ENODEV;
    }

    printk("NFC -> pn65n_probe++++\n");
    printk("    irq_gpio[%d], ven_gpio[%d], firm_gpio[%d]\n", platform_data->irq_gpio, platform_data->ven_gpio, platform_data->firm_gpio);

    if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
        pr_err("%s : need I2C_FUNC_I2C\n", __func__);
        return  -ENODEV;
    }

    ret = gpio_request(platform_data->irq_gpio, "nfc_int");
    if (ret)
        return  -ENODEV;
    ret = gpio_request(platform_data->ven_gpio, "nfc_ven");
    if (ret)
        goto err_ven;
    ret = gpio_request(platform_data->firm_gpio, "nfc_firm");
    if (ret)
        goto err_firm;

    pn65n_gpio_init(platform_data);

    pn65n_dev_data = kzalloc(sizeof(struct pn65n_dev), GFP_KERNEL);
    if (pn65n_dev_data == NULL) {
        dev_err(&client->dev,
                "failed to allocate memory for module data\n");
        ret = -ENOMEM;
        goto err_exit;
    }

    pn65n_dev_data->irq_gpio = platform_data->irq_gpio;
    pn65n_dev_data->ven_gpio  = platform_data->ven_gpio;
    pn65n_dev_data->firm_gpio  = platform_data->firm_gpio;
    pn65n_dev_data->client   = client;

    /* init mutex and queues */
    init_waitqueue_head(&pn65n_dev_data->read_wq);
    mutex_init(&pn65n_dev_data->read_mutex);
    spin_lock_init(&pn65n_dev_data->irq_enabled_lock);

    pn65n_dev_data->pn65n_device.minor = MISC_DYNAMIC_MINOR;
    pn65n_dev_data->pn65n_device.name = NFC_I2C_DEV_NAME;
    pn65n_dev_data->pn65n_device.fops = &pn65n_dev_fops;
    pn65n_dev_data->pn65n_device.parent = &client->dev;

    ret = misc_register(&pn65n_dev_data->pn65n_device);
    if (ret) {
        pr_err("%s : misc_register failed\n", __FILE__);
        goto err_misc_register;
    }

    /* request irq.  the irq is set whenever the chip has data available
     * for reading.  it is cleared when all data has been read.
     */
    pr_info("%s : requesting IRQ %d\n", __func__, client->irq);
    pn65n_dev_data->irq_enabled = true;
    ret = request_irq(client->irq, pn65n_dev_irq_handler,
              IRQF_TRIGGER_HIGH, client->name, pn65n_dev_data);
    if (ret) {
        dev_err(&client->dev, "request_irq failed\n");
        goto err_request_irq_failed;
    }
    pn65n_disable_irq(pn65n_dev_data);
    i2c_set_clientdata(client, pn65n_dev_data);

    /* register sysfs hooks */
    ret = sysfs_create_group(&client->dev.kobj, &pn65n_attr_group);
    if (ret)
        goto err_request_sysfs_failed;
    
    printk("NFC -> pn65n_probe----\n");

    return 0;

err_request_sysfs_failed:
err_request_irq_failed:
    misc_deregister(&pn65n_dev_data->pn65n_device);
err_misc_register:
    mutex_destroy(&pn65n_dev_data->read_mutex);
    kfree(pn65n_dev_data);
err_exit:
    gpio_free(platform_data->firm_gpio);
err_firm:
    gpio_free(platform_data->ven_gpio);
err_ven:
    gpio_free(platform_data->irq_gpio);
    return ret;
}

static int pn65n_remove(struct i2c_client *client)
{
    struct pn65n_dev *pn65n_dev_data;

    printk("NFC -> pn65n_remove\n");

    pn65n_dev_data = i2c_get_clientdata(client);
    free_irq(client->irq, pn65n_dev_data);
    misc_deregister(&pn65n_dev_data->pn65n_device);
    mutex_destroy(&pn65n_dev_data->read_mutex);
    gpio_free(pn65n_dev_data->irq_gpio);
    gpio_free(pn65n_dev_data->ven_gpio);
    gpio_free(pn65n_dev_data->firm_gpio);

    /* remove sysfs hooks */
    sysfs_remove_group(&client->dev.kobj, &pn65n_attr_group);
    kfree(pn65n_dev_data);

    return 0;
}

static const struct i2c_device_id pn65n_id[] = {
    { NFC_I2C_DEV_NAME, 0 },
    { }
};

static struct i2c_driver pn65n_driver = {
    .id_table   = pn65n_id,
    .probe      = pn65n_probe,
    .remove     = pn65n_remove,
    .suspend    = pn65n_suspend,
    .resume     = pn65n_resume,
    .driver     = {
        .owner  = THIS_MODULE,
        .name   = NFC_I2C_DEV_NAME,
    },
};

/*
 * module load/unload record keeping
 */

static int __init pn65n_dev_init(void)
{
    printk("NFC -> Loading pn65n driver\n");
    return i2c_add_driver(&pn65n_driver);
}
module_init(pn65n_dev_init);

static void __exit pn65n_dev_exit(void)
{
    printk("NFC -> Unloading pn65n driver\n");
    i2c_del_driver(&pn65n_driver);
}
module_exit(pn65n_dev_exit);

MODULE_AUTHOR("Sylvain Fonteneau");
MODULE_DESCRIPTION("NFC PN65N driver");
MODULE_LICENSE("GPL");
