#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node *next;
};

struct node *merge_list(struct node *list1, struct node *list2)
{
    struct node *list = malloc(sizeof(struct node));
    list->data = 0;
    list->next = NULL;

    struct node *tail_node = list;
    while (list1 != NULL && list2 != NULL)
    {
        if (list1->data <= list2->data)
        {
            tail_node->next = list1;
            tail_node = list1;
            list1 = list1->next;
        }
        else
        {
            tail_node->next = list2;
            tail_node = list2;
            list2 = list2->next;
        }
    }

    if (list1 != NULL)
    {
        tail_node->next = list1;
    }

    if (list2 != NULL)
    {
        tail_node->next = list2;
    }

    return list;
}

void append_list(struct node *list, int value)
{
    if (!list)
    {
        return;
    }

    struct node *tmp_node = malloc(sizeof(struct node));
    tmp_node->data = value;
    tmp_node->next = NULL;

    while (list->next)
    {
        list = list->next;
    }

    list->next = tmp_node;
}

struct node *create_list()
{
    struct node *tmp_node = malloc(sizeof(struct node));
    tmp_node->data = 0;
    tmp_node->next = NULL;
    return tmp_node;
}

void printf_list(struct node *list)
{
    printf("list:");
    while (list)
    {
        printf("%d,", list->data);
        list = list->next;
    }
    printf("\n");
}

int main(int agc, char **argv)
{
    struct node *list1 = create_list();
    append_list(list1, 1);
    append_list(list1, 3);
    append_list(list1, 6);
    printf_list(list1);

    struct node *list2 = create_list();
    append_list(list2, 2);
    append_list(list2, 4);
    append_list(list2, 7);
    printf_list(list2);

    struct node *list = merge_list(list1->next, list2->next);
    printf_list(list);

    return 0;
}