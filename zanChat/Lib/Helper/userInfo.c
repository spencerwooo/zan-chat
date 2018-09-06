#include "../../Include/userInfo.h"
#include "stdio.h"

/**************************************************/
/*名称：userinfo.c
/*描述：定义与用户、联系人相关的函数
/*作成日期： 2018-09-02
/*作者： 韩昌云、邵子涵、程孟祺、刘嘉伟
/***************************************************/

extern struct userinfo *head;

struct userinfo *addUser(struct userinfo *head, struct userinfo *user)
{
    printf("enter add user\n");
    if (NULL == user)
    {
        return head;
    }

    if (NULL != seekUser(head, user->ip))
    {
        head = deleteUser(head, user->ip);
        displayAll(head);
        user->next = head;
        head = user;
        displayAll(head);
        return head;
    }

    user->next = head;
    head = user;
    return head;
}

struct userinfo *deleteUser(struct userinfo *head, char *ip)
{

    struct userinfo *p, *p1;
    p1 = NULL;
    p = head;
    while ((NULL != p) && (0 != strcmp(ip, p->ip)))
    {
        p1 = p;
        p = p->next;
    }
    if (NULL == p)
    {
        printf("%s   不存在!\n", ip);
        return head;
    }
    else
    {
        p1->next = p->next;
    }
    free(p);
    return head;
}

struct userinfo *seekUser(struct userinfo *head, char *ip)
{
    struct userinfo *p, *p1;
    p = head;
    while ((NULL != p) && (0 != strcmp(ip, p->ip)))
    {
        p1 = p;
        p = p->next;
    }
    if (NULL == p)
    {
        return NULL;
    }
    else
        return p;
}
struct userinfo *refreshUser(struct userinfo *head, struct userinfo *user)
{
    struct userinfo *p;

    //找到IP为ip的用户节点，如果当前节点指针！＝NULL，且ip不同
    p = seekUser(head, user->ip);
    if (NULL != p)
    {
        //更新数据信息
        strcpy(p->name, user->name);
        strcpy(p->group, user->group);
        strcpy(p->signature, user->signature);
        p->picture = user->picture;
    }
    return head;
}

void displayAll(struct userinfo *head)
{
    g_print("enter display\n");
    while (NULL != head)
    {
        display(head);
        head = head->next;
    }
}
void display(struct userinfo *user)
{
    printf("-------------------------------\n");
    printf("%s \n%s \n%s \n%s \n%d \n", user->name, user->group, user->ip, user->signature, user->picture);
    if (user->textViewAll == NULL)
    {
        printf("textViewAll = null\n");
    }
    else
    {
        printf("textViewAll != null\n");
    }
    printf("-------------------------------\n");
}
struct userinfo *createOneUser(char *name, char *group, char *ip, char *signature, int image_code, int avatar_code)
{
    struct userinfo *p;
    p = (struct userinfo *)malloc(LEN);
    strcpy(p->name, name);
    strcpy(p->group, group);
    strcpy(p->ip, ip);
    strcpy(p->signature, signature);
    //p->picture=picture;
    p->image_code = image_code;
    p->avatar_code = avatar_code;
    p->textViewAll = NULL; //add by wanglong 2010-07-08
    p->record = NULL;      //add record 2010-07-12
    p->next = NULL;
    return p;
}

char *createMyUser()
{
}

//utf16
void dealCharacter(char *group)
{
    int i;
    for (i = 0; group[i] != '\0'; i += 2)
    {
        printf("%c%c", group[i], group[i + 1]);
    }
}

int setOnTextView(char *ip, textView *tv)
{
    struct userinfo *temp;
    temp = head;
    //找到IP为ip的用户节点，如果当前节点指针！＝NULL，且ip不同
    while ((NULL != temp) && (0 != strcmp(ip, temp->ip)))
    {
        temp = temp->next;
    }

    if (NULL == temp)
    {
        printf("IP＝ %s的用户不存在!\n", ip);
        return -1;
    }
    else
    {
        temp->textViewAll = tv;
    }
    return 0;
}
void setOffTextView(char *ip)
{
    struct userinfo *temp;
    temp = head;
    while ((NULL != temp) && (0 != strcmp(ip, temp->ip)))
    {
        temp = temp->next;
    }

    if (NULL == temp)
    {
        printf("IP＝ %s的用户不存在!\n", ip);
    }
    else
    {
        temp->textViewAll = NULL;
    }
    display(temp);
}
