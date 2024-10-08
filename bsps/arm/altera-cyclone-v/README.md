Overview
========
Evaluation board for this BSP: 
- Cyclone V SoC FPGA Development Kit
- DK-DEV-5CSXC6N/ES-0L

RTC
---
The evaluation board contains a DS1339C RTC connected to I2C0. To use it you
have to set the following options:

```c
  #define CONFIGURE_APPLICATION_NEEDS_RTC_DRIVER
  #define CONFIGURE_BSP_PREREQUISITE_DRIVERS I2C_DRIVER_TABLE_ENTRY
```

Additional there has to be one free file descriptor to access the i2c. Set the
`CONFIGURE_MAXIMUM_FILE_DESCRIPTORS` accordingly.


Network
-------
The default PHY address can be overwritten by the application. To do this, the
drv_ctrl pointer of the rtems_bsdnet_ifconfig structure should point to a
dwmac_ifconfig_drv_ctrl object with the appropriate settings before the
rtems_bsdnet_initialize_network() is called. E.g.:

```c
  #include <libchip/dwmac.h>
  #include <bsp.h>

  static dwmac_ifconfig_drv_ctrl drv_ctrl = {
    .phy_addr = 1
  };
```

  ...

```c
  static struct rtems_bsdnet_ifconfig some_ifconfig = {
    .name = RTEMS_BSP_NETWORK_DRIVER_NAME,
    .attach = RTEMS_BSP_NETWORK_DRIVER_ATTACH,
    .drv_ctrl = &drv_ctrl
  };
```
  ...

```c
  rtems_bsdnet_initialize_network();
```

If drv_ctrl is the NULL pointer, default values will be used instead.
