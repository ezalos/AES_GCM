#include <stdio.h>
#include <stdlib.h>

int pal_check(int num)
{
    int len = 0;
    int save = num;
    int i;
    char buf[20];

    while (num || !len)
    {
        buf[len] = (num % 10);
        num = num / 10;
        len++;
    }
    num = save;
	buf[len] = 0;
    i = -1;
    while (++i < --len)
		if (save % 10 != buf[len])
            return (0);
		else
			save /= 10;
    printf("%d\n", num);
    return (1);
}

int main(int ac, char **av)
{
	int nb_num;
	int num;
	int i = 0;
	int done = 0;

    nb_num = ac - 1;
    if (nb_num < 1)
        return (0);
    while (done < nb_num)
    {
        if (i == 0)
			num = atoi(av[done + 1]);
        if (pal_check(num - i) || pal_check(num + i))
        {
            i = 0;
            done++;
        }
        else
            i++;
    }
	return 0;
}
