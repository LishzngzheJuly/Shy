#ifndef USER_MGR_H
#define USER_MGR_H


#define USER_INFO_FILE "./user_info.dat"


#include <time.h>



typedef struct
{
    char uname[51];
    char upass[33];
    char sex;
    char phone[12];
    char uid[19];
    char udress[100];
    time_t reg_time;         // 注册时间
    time_t last_login_time;  // 上一次登录时间
    int delete_flag;
    int role;  // 1 表示系统管理员，2 表示普通用户，......
    //...
    
} user_info;


int check_user_name(const char* uname);
void create_user(void);
void login_auth(void);



#endif
