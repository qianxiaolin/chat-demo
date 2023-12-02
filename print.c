#include <stdio.h>
#include <stdarg.h>
void myPrintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    while (*format != '\0') {
        if (*format == '%') {
            format++;  // 检查下一个字符
            switch (*format) {
                case 'd': {
                    int num = va_arg(args, int);
                    printf("%d", num);
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char*);
                    printf("%s", str);
                    break;
                }
                // 这里可以添加更多的格式控制符
                default:
                    printf("未知格式控制符： %c", *format);
                    break;
            }
        } else {
            printf("%c", *format);
        }
        format++;
    }
    va_end(args);
}
int main() {
    int a = 10;
    char b[] = "hello";
    myPrintf("The number is: %d, and the string is: %s\n", a, b);
    return 0;
}
