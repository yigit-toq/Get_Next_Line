/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytop <ytop@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:20:15 by ytop              #+#    #+#             */
/*   Updated: 2024/02/15 17:02:26 by ytop             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>

static void	*free_backup(char **backup, int create)
{
	char	*line;

	if (!*backup)
		return (0);
	if (!create)
		return (free(*backup), *backup = 0);
	else
		return (line = ft_strdup(*backup), free(*backup), *backup = NULL, line);
	return (0);
}

static int	newline_checker(char *str)
{
	size_t	index;

	if (!str)
		return (0);
	index = -1;
	while (str[++index] != '\0')
		if (str[index] == '\n')
			return (1);
	return (0);
}

static char	*copy_to_backup(char *backup, char *buffer)
{
	char	*result;

	if (!backup && buffer)
		return (ft_strdup(buffer));
	result = ft_strjoin(backup, buffer);
	free_backup(&backup, 0);
	return (result);
}

static char	*extract_line(char *backup, int check)
{
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	while (backup[i] != '\n')
		i++;
	if (check)
	{
		str = malloc(sizeof(char) * (i + 2));
		if (!str)
			return (0);
		j = -1;
		while (++j < i + 1)
			str[j] = backup[j];
		str[j] = '\0';
	}
	else
	{
		if (backup[i + 1] == '\0')
			return (free_backup(&backup, 0));
		str = ft_substr(backup, i + 1, ft_strlen(backup));
		free_backup(&backup, 0);
	}
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*backup = 0;
	char		*line;
	char		buffer[BUFFER_SIZE + 1];
	int			size;

	line = 0;
	size = BUFFER_SIZE;
	while (size > 0)
	{
		size = read(fd, buffer, BUFFER_SIZE);
		if ((size <= 0 && !backup) || size == -1)
			return (free_backup(&backup, 0));
		buffer[size] = '\0';
		backup = copy_to_backup(backup, buffer);
		if (newline_checker(backup))
		{
			line = extract_line(backup, 1);
			if (!line)
				return (free_backup(&backup, 0));
			return (backup = extract_line(backup, 0), line);
		}
	}
	return (free_backup(&backup, 1));
}
