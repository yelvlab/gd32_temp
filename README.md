# gd32e23x_template
from mo10


## 关于链接脚本

注意芯片选型, 不同型号的芯片 FLASH 和 RAM 大小不同。需要修改链接脚本`ld/gd32e23x_gcc.ld`

| 芯片型号       | FLASH | RAM |
|------------|-------|-----|
| GD32E230F4 | 16K   | 4K  |
| GD32E230F8 | 64K   | 8K  |

```linkerscript
/* memory map */
MEMORY
{
FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 16K
RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 4K
}
```


## Ref

1. 参考LD/Startup

[https://github.com/Noveren/gd32e23x-template/blob/main/gd32e23x/template/linker.ld](https://github.com/Noveren/gd32e23x-template/blob/main/gd32e23x/template/linker.ld)

[https://github.com/Noveren/gd32e23x-template/blob/main/gd32e23x/template/startup.s](https://github.com/Noveren/gd32e23x-template/blob/main/gd32e23x/template/startup.s)

2. 官方LD/Startup

使用Embedded Builder工具生成的C标准库生成的模板
