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
	int		back_height;
	int		back_width;
	char	back_style;
	char	**draw;

}		t_info;


void	init_draw(t_info *info)
{
	int	n;
	int	i;

	n = 0;
	info->draw = malloc((info->back_height + 1) * sizeof(char *));
	while (n < info->back_height)
	{
		i = 0;
		info->draw[n] = malloc((info->back_width + 1) * sizeof(char));
		while (i < info->back_width)
		{
			info->draw[n][i] = info->back_style;
			i++;
		}
		info->draw[n][i] = '\0';
		n++;
	}
	info->draw[n] = NULL;
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
	while (info->draw[y])
	{
		x = 0;
		while (info->draw[y][x])
		{
			state = is_in(circle, x, y);
			if (!(state == 0 || (state == 1 && circle->type == 'c')))
				info->draw[y][x] = circle->style;
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
	{
		draw(data->draw, data->back_width);
		return (0);
	}
	return (1);
}

int	minipaint(FILE *file)
{
	int	good;
	t_info	info;

	good = fscanf(file, "%d %d %c\n", &info.back_width, &info.back_height, &info.back_style);
	if (good != 3 || info.back_width <= 0 || info.back_width > 300 || info.back_height <= 0 || info.back_height > 300)
		return (1);
	init_draw(&info);
	if (!actions(&info, file))
		return (0);
	draw(info.draw, info.back_width);
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