#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "goods_list.h"
#include "user_mgr.h"
#include "md5.h"


void init_gl(void);
void input_goods(void);
void delete_goods(void);
void update_goods(void);
void search_goods(void);


int main(int argc, char** argv)
{
	
	if(access(USER_INFO_FILE, R_OK) == 0)
	{
		login_auth();
	}

	int op;

	init_gl();

	printf("欢迎使用商品信息管理系统！\n");	
	printf("\n使用帮助：\n");
	printf("录入商品信息请输入1\n");
	printf("删除商品信息请输入2\n");
	printf("修改商品信息请输入3\n");
	printf("查询商品信息请输入4\n");
	printf("添加新用户请输入5\n");
	printf("退出系统请输入0\n");

	while(1)
	{
		printf("\n:> ");

		if(scanf("%d", &op) != 1)
		{
			while(getchar() != '\n');  // 清空标准输入缓冲区中的所有数据，否则会导致下次调用 scanf 立即失败返回。
			op = -1;
		}

		switch(op)
		{
			case 0:
			printf("\n谢谢使用，下次再见！\n");
			return 0;

			case 1:
			input_goods();
			break;
			
			case 2:
			delete_goods();
			break;

			case 3:
			update_goods();
			break;

			case 4:
			search_goods();
			break;

			case 5:
			create_user();
			break;

			default:
			printf("\n输入错误，请重新输入！\n");
		}
	}

	return 0;
}


void init_gl(void)
{
	gl = create();
	
	// 将商品信息数据文件中的数据都读取到链表中来
	struct goods g;
	FILE* fp = fopen(GOODS_INFO_FILE, "rb");
	if(NULL == fp) return;

	while(fread(&g, sizeof(g), 1, fp) == 1)
	{
		push_back(gl, &g);
	}

	fclose(fp);
}


void input_goods(void)
{
	struct goods g;

	printf("\n请按照提示依次录入商品信息。\n");
	printf("商品ID：");
	scanf("%d", &(g.gid));
	printf("商品名称：");
	scanf("%s", g.name);
	printf("商品单价：");
	scanf("%f", &(g.price));

	push_back(gl, &g);

	FILE* fp = fopen(GOODS_INFO_FILE, "ab");
	fwrite(&g, sizeof(g), 1, fp);
	fclose(fp);

	printf("\n商品信息录入成功！\n");
}


void delete_goods(void)
{
	int gid;

	printf("\n请输入要删除商品的ID：");
	scanf("%d", &gid);

	int pos;
	struct goods* pg = find(gl, gid, &pos);

	if(pg == NULL)
	{
		printf("\n商品不存在，删除失败！\n");
	}
	else
	{
		pg->is_delete = 1;  // 在链表中删除相应的商品信息

		// 在文件中删除相应的商品信息
		FILE* fp = fopen(GOODS_INFO_FILE, "r+b");
		fseek(fp, pos * sizeof(struct goods), SEEK_SET);
		fwrite(pg, sizeof(struct goods), 1, fp);
		fclose(fp);		

		printf("\n删除商品成功！\n");
	}

}


void update_goods(void)
{
        int gid;

        printf("\n请输入要修改商品的ID：");
        scanf("%d", &gid);

        int pos;
        struct goods* pg = find(gl, gid, &pos);

        if(pg == NULL)
        {
                printf("\n商品不存在，删除失败！\n");
        }
        else
        {
                printf("请输入商品名称：");
                scanf("%s", pg->name);

                printf("请输入商品价格：");
                scanf("%f", &pg->price);

                printf("请输入商品数量：");
                scanf("%d", &pg->num);

                printf("请输入商品厂家：");
                scanf("%s", pg->factor);

                printf("请输入商品余量：");
                scanf("%d", &pg->allow);

                // 在文件中删除相应的商品信息
                FILE* fp = fopen(GOODS_INFO_FILE, "r+b");
                fseek(fp, pos * sizeof(struct goods), SEEK_SET);
                fwrite(pg, sizeof(struct goods), 1, fp);
                fclose(fp);

                printf("\n修改商品信息成功！\n");
        }

}


int show_goods(struct goods* g)
{
	if(!g->is_delete)
	{
	
		printf("%d %s %g\n", g->gid, g->name, g->price);
		return 1;
	}

	return 0;
}


void search_goods(void)
{
	traverse(gl, show_goods);
}


