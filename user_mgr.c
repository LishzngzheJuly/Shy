#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include "user_mgr.h"
#include "md5.h"



// 判断用户名是否存在
int check_user_name(const char* uname)
{
    FILE* fp = fopen(USER_INFO_FILE, "rb");

    if(NULL == fp) return 1;

    user_info ui;
    int exist_flag = 0;

    while(fread(&ui, sizeof(ui), 1, fp) == 1)
    {
        if(strcmp(ui.uname, uname) == 0)
        {
            exist_flag = 1;
            break;
        }
    }

    fclose(fp);

    return !exist_flag;
}



// 创建新用户
void create_user(void)
{
    user_info ui;
   // char password[51];
   char* password;
    int i = 0;

    while(1)
    {
        printf("\n用户名：");
        scanf("%s", ui.uname);

        if(check_user_name(ui.uname))
            break;

        printf("\n用户名已存在，请重新输入！\n");
    }

   	//printf("\n密码：");
   
	password = getpass("请输入一个6位数的密码:");
 	                        			// 密码输入应该无回显或回显圆点等其他符号，防止旁边的人偷窥

    if (password != NULL) 
    {
        							// 在这里处理密码，比如进行验证
        							// 注意：不应该直接打印密码，因为这会导致安全风险
        printf("\n你输入的密码长度是: %zu\n", strlen(password));
    }
    else 
    {
        perror("getpass");
        return;
    }
    getchar();

    printf("\n性别：");
    scanf("%c", &(ui.sex));

    printf("\n手机号：");
    scanf("%s", ui.phone);

    printf("\n身份证：");
    scanf("%s" , ui.uid);

    printf("\n居住地址(详细地址)：");
    scanf("%s" , ui.udress);

    md5_string(password, ui.upass);

    ui.delete_flag = 0;
    ui.role = 1;
    ui.reg_time = time(NULL);
    ui.last_login_time = 0;

    FILE* fp = fopen(USER_INFO_FILE, "ab");

    if(NULL == fp)
    {
        perror("fopen");
        return;
    }

    fwrite(&ui, sizeof(ui), 1, fp);

    fclose(fp);

    printf("\n创建用户成功！\n");
}



// 登录验证
void login_auth(void)
{
    char uname[51], upass[51], upass_md5[33];

    printf("\n请登录。\n");
    printf("用户名：");
    scanf("%s", uname);
    printf("密码：");
    scanf("%s", upass);

    md5_string(upass, upass_md5);

    FILE* fp = fopen(USER_INFO_FILE, "rb");

    user_info ui;
    int auth_flag = 0;

    while(fread(&ui, sizeof(ui), 1, fp) == 1)
    {
        if(strcmp(ui.uname, uname) == 0 && strcmp(ui.upass, upass_md5) == 0)
        {
            auth_flag = 1;
            break;
        }
    }

    fclose(fp);

    if(auth_flag)
    {
        printf("\n用户(%s)，你好，欢迎使用本系统！\n", uname);

        ui.last_login_time = time(NULL);
    } 
    else
    {
        printf("\n用户名或密码错误，登录失败！\n");
        exit(1);
    }
}



