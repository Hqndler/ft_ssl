#include "utils.h"
#include "get_next_line.h"

static int	list_new_elem_str(t_list **new, char *elem)
{
	(*new) = malloc(sizeof(t_list));
	if (*new == NULL)
		return (0);
	(*new)->content = elem;
	(*new)->next = NULL;
	(*new)->prev = NULL;
	return (1);
}

int	add_to_list(t_list **list, char *buf)
{
	t_list	*new;

	if (!list_new_elem_str(&new, buf))
		return (0);
	if (!(*list))
	{
		(*list) = new;
		(*list)->prev = *list;
		(*list)->next = *list;
	}
	else
	{
		new->prev = (*list)->prev;
		new->next = (*list);
		(*list)->prev->next = new;
		(*list)->prev = new;
	}
	return (1);
}

int	free_list(t_list **list)
{
	t_list	*tmp;
	t_list	*current;

	current = *list;
	if (!*list)
		return (0);
	while (current->next != *list)
	{
		tmp = current;
		current = current->next;
		free(tmp->content);
		free(tmp);
	}
	free(current->content);
	free(current);
	*list = NULL;
	return (0);
}

int	write_in_list(int fd, t_list **list)
{
	char	*buf;
	int		r_res;

	r_res = 1;
	while (!new_line(*list) && r_res != 0)
	{
		buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buf)
			return (free_list(list));
		r_res = read(fd, buf, BUFFER_SIZE);
		if (r_res <= 0 && !(*list))
			return (free(buf), free_list(list));
		buf[r_res] = '\0';
		if (!add_to_list(list, buf))
			return (free(buf), free_list(list));
	}
	return (1);
}

int	clear_list(t_list **list)
{
	t_list	*last;
	char	*content;
	int		i;
	int		j;

	if (!list)
		return (0);
	last = (*list)->prev;
	i = 0;
	while (last->content[i] != '\n' && last->content[i])
		i++;
	if (last->content[i] == '\n' && last->content[i])
		i++;
	j = ft_strlen(last->content);
	content = malloc(sizeof(char) * ((j - i) + 1));
	if (!content)
		return (free_list(list));
	j = 0;
	while (last->content[i])
		content[j++] = last->content[i++];
	content[j] = '\0';
	free_list(list);
	if (!add_to_list(list, content))
		return (free(content), 0);
	return (1);
}

int	how_many(t_list *list, char **line)
{
	t_list	*tmp;
	int		len;
	int		i;

	tmp = list;
	len = 0;
	while (tmp->next != list)
	{
		len += ft_strlen(tmp->content);
		tmp = tmp->next;
	}
	i = -1;
	while (tmp->content[++i])
	{
		if (tmp->content[i] == '\n')
		{
			++len;
			break ;
		}
		++len;
	}
	*line = malloc(sizeof(char *) * (len + 1));
	return (len);
}

int	make_line(t_list *list, char **line)
{
	int	i;
	int	j;
	int	len;

	len = how_many(list, line);
	if (!line)
		return (free_list(&list));
	j = 0;
	while (j < len)
	{
		i = -1;
		while (list->content[++i])
		{
			if (list->content[i] == '\n')
			{
				(*line)[j++] = list->content[i];
				break ;
			}
			(*line)[j++] = list->content[i];
		}
		list = list->next;
	}
	(*line)[j] = '\0';
	return (1);
}

int	new_line(t_list *list)
{
	int		i;
	t_list	*current;

	if (!list)
		return (0);
	current = list->prev;
	i = -1;
	while (current->content[++i])
		if (current->content[i] == '\n')
			return (1);
	return (0);
}

char	*get_next_line(int fd)
{
	static t_list	*list = NULL;
	char			*line;

	line = NULL;
	if (fd < 0)
		free_list(&list);
	if (fd < 0 || BUFFER_SIZE < 0)
		return (NULL);
	if (!write_in_list(fd, &list) || !list)
		return (NULL);
	if (!make_line(list, &line) || !clear_list(&list))
		return (NULL);
	if (line[0] == '\0')
		return (free(line), free_list(&list), NULL);
	return (line);
}