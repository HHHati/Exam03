#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_rect
{
	char	type;
	float	x;
	float	y;
	float	height;
    float	width;
	char	style;
}		t_rect;

typedef struct s_info
{
	int	    height;
	int 	width;
	char	back;
	char	**mat;

}		t_info;


void	init_draw(t_info *info)
{
	int	n;
	int	i;

	n = 0;
	info->mat = malloc((info->height + 1) * sizeof(char *));
	while (n < info->height)
	{
		i = 0;
		info->mat[n] = malloc((info->width + 1) * sizeof(char));
		while (i < info->width)
		{
			info->mat[n][i] = info->back;
			i++;
		}
		info->mat[n][i] = '\0';
		n++;
	}
	info->mat[n] = NULL;
}

int	is_in(t_rect *rect, int x, int y)
{
	if (x < rect->x || rect->x + rect->width < x || y < rect->y || rect->y + rect->height < y)
		return (0);
	else if (x - rect->x < 1.00000000 || rect->x + rect->width - x < 1.00000000 || y - rect->y < 1.00000000 || rect->y + rect->height - y < 1.00000000)
		return (2);
	return (1);
}

void	exec_one(t_rect *rect, t_info *info)
{
	int	x;
	int	y;
	int	state;

	y = 0;
	while (info->mat[y])
	{
		x = 0;
		while (info->mat[y][x])
		{
			state = is_in(rect, x, y);
			if (!(state == 0 || (state == 1 && rect->type == 'r')))
				info->mat[y][x] = rect->style;
			x++;
		}
		y++;
	}
}

void	draw(char **mat, int len)
{
	int	n;

	n = 0;
	while (mat[n])
	{
		write(1, mat[n], len);
		write(1, "\n", 1);
		n++;
	}
}

int	actions(t_info *info, FILE *file)
{
	int	good;
	t_rect	rect;

	good = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.height, &rect.width, &rect.style);
	while(good == 6)
	{
		exec_one(&rect, info);
		good = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.height, &rect.width, &rect.style);
	}
	if (good == -1)
		return (0);
	return (1);
}

int	micropaint(FILE *file)
{
	int	good;
	t_info	info;

	good = fscanf(file, "%i %i %c\n", &info.width, &info.height, &info.back);
	if (good != 3 || info.width <= 0 || info.width > 300 || info.height <= 0 || info.height > 300)
		return (1);
	init_draw(&info);
	if (actions(&info, file))
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	draw(info.mat, info.width);
	return (0);
}

int	main(int argc, char **argv)
{
	FILE *file;

	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	file = fopen(argv[1], "r");
	if (!file)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	return (micropaint(file));
}