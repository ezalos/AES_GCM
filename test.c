#include <stdio.h>
#include <string.h>
#define     TESTEUR 1

int main() {
    char    *str1;
    char    *str2;
    int     nb_cases;
    int     actual_case = 0;
    int     len = -1;
    int     i;
    int     j;
    
    scanf("%d", &nb_cases);
    
	printf("Nb cases: %d\n", nb_cases);
    while (++actual_case <= nb_cases)
    {
	  printf("Case: %d/%d\n", actual_case, nb_cases);
        scanf("%s", str1);
        while (str1[++len] != ' ')
		  printf("[%d]=%c\n", len, str1[len]);
        
        str1[len] = '\0';
        str2 = str1 + len;
		printf("str1: %s\n", str1);
		printf("str2: %s\n", str2);
        if (len != strlen(str2))
        {
            printf("NO\n");
            continue ;
        }
        i = -1;
        while (str1[++i])
        {
            j = -1;
            while (str2[j] && str2[j] != str1[i])
                j++;
            if (str2[j] != str1[i])
            {
                printf("NO\n");
                break;
            }
            else
                str2[j] = TESTEUR;
        }
        printf("YES\n");
    }
	return 0;
}
