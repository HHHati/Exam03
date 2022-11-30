#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_circle
{
	char	type;
	float	x;
	float	y;
	float	radius;
	char	style;
}		t_circle;

typedef struct s_info
{
	int		height;
	int		width;
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

float	get_distance(int x, int y, t_circle *circle)
{
	float	distance;
	float	diffx;
	float	diffy;

	diffx = powf(x - circle->x, 2);
	diffy = powf(y - circle->y, 2);
	distance = sqrtf(diffx + diffy);
	return (distance);
}

int	is_in(t_circle *circle, int x, int y)
{
	if (get_distance(x, y, circle) > circle->radius)
		return (0);
	else if (get_distance(x, y, circle) > circle->radius - 1)
		return (2);
	return (1);
}

void	exec_one(t_circle *circle, t_info *info)
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
			state = is_in(circle, x, y);
			if (!(state == 0 || (state == 1 && circle->type == 'c')))
				info->mat[y][x] = circle->style;
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

int	actions(t_info *data, FILE *file)
{
	int	good;
	t_circle	circle;

	good = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.style);
	while(good == 5)
	{
		exec_one(&circle, data);
		good = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.style);
	}
	if (good == -1)
		return (0);
	return (1);
}

int	minipaint(FILE *file)
{
	int	good;
	t_info	info;

	good = fscanf(file, "%d %d %c\n", &info.width, &info.height, &info.back);
	if (good != 3 || info.width <= 0 || info.width > 300 || info.height <= 0 || info.height > 300)
		return (1);
	init_draw(&info);
	if (actions(&info, file))
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (0);
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
	return (minipaint(file));
}