#include <stdio.h>

int str2int(char *data)
{
    if (data == NULL)
    {
        return 0;
    }

    // 去掉多余空格
    while ((*data) == '\n' || (*data) == '\t' || (*data) == '\r')
    {
        data++;
    }

    // 确定符号
    int sign = 1;
    if ((*data) == '-')
    {
        sign = -1;
        data++;
    }

    if ((*data) == '+')
    {
        sign = 1;
        data++;
    }

    // 确定数值
    int num = 0;
    while ((*data) >= '0' && (*data) <= '9')
    {
        num = num * 10 + (*data) - '0';
        data++;
    }

    return sign * num;
}

int main(int argc, char **argv)
{
    int i1 = str2int("0123456");
    printf("i1=%d\n", i1);
    return 0;
}