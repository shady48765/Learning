#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#define WRITE_FILE_NAME             "change_ticks"
#define READ_FILE_NAME              "read_loops"
#define SYSFS_PATH                  "/sys/foo_sysfs"
#define SYSFS_WRITE_PATH            "/sys/foo_sysfs/change_ticks"
#define SYSFS_READ_PATH             "/sys/foo_sysfs/read_loops"


typedef struct {
    char * write_file_name;
    char * read_file_name;
    char * sysfs_folder_path;
}file_info;

file_info file_path = {
    .write_file_name = WRITE_FILE_NAME,
    .read_file_name = READ_FILE_NAME,
    .sysfs_folder_path = SYSFS_PATH,
};

typedef struct {
    unsigned int read_flag;
    unsigned int write_flag;
    unsigned int thread_exit_flag;
    
    pthread_t thread_id;
        
}usr_operations;




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

void parameter_input_format(void)
{
    printf("-------------------- input format -------------------\n");
    printf("\t when need change timer ticks:\n");
    printf("\t\t set: <milliseconds>\n");
    printf("\t when need read timer recall loop times:\n");
    printf("\t\t read\n");
    printf("-----------------------------------------------------\n");
}

int input_paramter_check(char *str)
{
    if(0 == strncmp("read", str, 4)) {
        return 0;
    } else if(0 == strncmp("set", str, 3)) {
        return 1;
    } else {
        return -1;
    }
    
}

void *thread_function(void * args)
{
    pthread_t new_id;
    
    usr_operations * thread_ops = (usr_operations *)args;
    new_id = pthread_self();
    printf("new thread read_flag = %u, write_flag = %u, thread_exit_flag = %u\n", 
            thread_ops->read_flag, thread_ops->write_flag, thread_ops->thread_exit_flag);
            
    if(thread_ops->thread_exit_flag) {
        pthread_exit(NULL);
        return NULL;
    }
}

int main(int argv, char **argc)
{
    int ret = 0;
    char buff[1024] = {'\0'};
    
    if(argv !=  2) {
        err_printf("input paramter number error.");
        input_format_notice();
    }
    printf("input paramter accepted, pram : %s\n", *(argc + 1));
    
    usr_operations * ops = (usr_operations *)malloc(sizeof(usr_operations));
    if(!ops) {
        err_printf("error : malloc usr_operations");
        return -1;
    }
    ops->read_flag = 0;
    ops->write_flag = 0;
    ops->thread_exit_flag = 0;
    
    // create thread to analysis input paramter format and information
    ops->thread_id = pthread_create(&ops->thread_id, NULL, thread_function, (void *)ops);
    input_paramter_check(buff);
    do {
        printf("which operations youn need: ");
        ret = scanf("%[^\n]", buff);
        if(ret < 2) {   // mo input parameter
            continue;
        }
        ret = input_paramter_check(buff);
        if(ret < 0) {
            err_printf("input format");
            memset(buff, '\0', sizeof(buff));
        }
    }while(0 != ret);
    
    
    
    
    return 0;
}

