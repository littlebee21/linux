
上层的接口：
/home/vagrant/test/linux/drivers/char/ipmi/kcs_bmc_client.h
用于定义上层的驱动的实现方式



底层的实现
/home/vagrant/test/linux/drivers/char/ipmi/kcs_bmc_aspeed.c
```
static const struct kcs_bmc_device_ops aspeed_kcs_ops = {
	.irq_mask_update = aspeed_kcs_irq_mask_update,
	.io_inputb = aspeed_kcs_inb,
	.io_outputb = aspeed_kcs_outb,
	.io_updateb = aspeed_kcs_updateb,
};
```
