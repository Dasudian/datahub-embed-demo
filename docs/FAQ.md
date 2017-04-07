Q: .\xxx\xxx.axf: Error: L6218E: Undefined symbol datahub_printf (referred from xxx.o).

A: 未实现datahub_printf().datahub_printf()用于输出调试信息,对于开发工作十分重要.请将下列代码加入源文件中,即可链接成功.如果您重定向了输出,可以使能vprintf()

```
#include <stdarg.h>
void datahub_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    /* 如果重定向了输出,可以取消注释 */
//  vprintf(format, ap);
    va_end(ap);
}
```

Q:

```
...
Error: L6367E: datahubclient.o attributes are not compatible with the provided attributes . Tag_CPU_arch = ARM v7E-M (=13)
...
```

A: 这是因为SDK与您的CPU架构不兼容.请在lib目录下找到合适的SDK库.如果没有提供,请联系我们
