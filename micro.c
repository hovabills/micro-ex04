




#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef enum e_type
{
    PIPE = 1,
    BREAK,
    END,

} t_type;

typedef struct s_cmd
{
    char **av;
    size_t size;
    struct s_cmd *prev;
    struct s_cmd *next;
    t_type type;

} t_cmd;


size_t get_arg_size(char **av)
{
    size_t size = 0;
    while (av[size])
    {
        if (!strcmp(av[size], "|") || !strcmp(av[size], ";"))
            break;
        size++;
    }
    return (size);
}   


t_type get_type(char *arg)
{
    if (!arg)
        return END;
    if (!strcmp(arg, "|"))
        return PIPE;
    if (!strcmp(arg, ";"))
        return BREAK;
    return (0);
}


void add_cmd(t_cmd **cmd_head, t_cmd *cmd)
{
    t_cmd *tmp = *cmd_head;
    if (!tmp)
        *cmd_head = cmd;
    else 
    {
        while(tmp->next)
            tmp = tmp->next;
        tmp->next = cmd;
        cmd->prev = tmp;
    }
}

size_t parse_cmd(t_cmd **cmd_head, char **av)
{
    size_t size = get_arg_size(av);
    size_t i = 0;
    t_cmd *cmd = (t_cmd*)malloc(sizeof(t_cmd));
    cmd->av = (char**)malloc(sizeof(char*) * size + 1);
    while(i < size) 
    {
        cmd->av[i] = av[i];
        i++;
    }
    cmd->av[i] = NULL;
    cmd->next = NULL;
    cmd->prev = NULL;
    cmd->size = size;
    cmd->type = get_type(av[i]);
    add_cmd(cmd_head, cmd);
    return (size);
}

int main(int ac, char **av, char **envp)
{
    t_cmd *cmd_head;

    cmd_head =  NULL;
    av++;
    while(*av)
    {
        size_t size =  parse_cmd(&cmd_head, av);
        // printf("%ld\n", size);
        av += size;
        if (*av)
            av++;
    }
    while (cmd_head)
    {   
        while (*cmd_head->av)
        {
            printf("%s ", *cmd_head->av);
            cmd_head->av++;
        }
        printf("size: %ld, type: %d\n", cmd_head->size, cmd_head->type);
        cmd_head = cmd_head->next;
    }
    return (0);
}