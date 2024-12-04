#pragma once

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

#include "ft_ssl.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

typedef struct s_list
{
	char			*content;
	struct s_list	*next;
	struct s_list	*prev;
}					t_list;

int		ft_strlen(const char *str);
int		free_list(t_list **list);
int		how_many(t_list *list, char **line);
int		make_line(t_list *list, char **line);
int		new_line(t_list *list);
int		add_to_list(t_list **list, char *buf);
int		write_in_list(int fd, t_list **list);
int		clear_list(t_list **list);