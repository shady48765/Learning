#include <stdio.h>
#include <stdlib.h>


void err_printf(char *s)
{
    if(NULL != s) {
        printf("error: %s\n", s);
    }
}

void input_format_notice(void)
{
    printf("\t input format:\n");
    printf("\t\t ./<binary file> <work folder path>\n");
}

int main(int argv, char **argc)
{
    if(argv !=  2) {
        err_printf("input paramter number error.");
        input_format_notice();
    }
    printf("input paramter accepted, pram : %s\n", *(argc + 1));
    
    return 0;
}

