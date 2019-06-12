/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shoot.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrache <akrache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/05 19:51:14 by akrache           #+#    #+#             */
/*   Updated: 2019/06/12 12:52:24 by akrache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

# define RADIUS 500

void		reload(t_weapon *weapon)
{
	int	r;

	r = weapon->clip_max - weapon->clip;
	(weapon->ammo - r < 0 ? r += weapon->ammo - r : 0);
	weapon->ammo -= r;
	weapon->clip = r;
}

void		shoot(t_player *player)
{
	if (player->weapons[player->hand].clip == 0)
		reload(&(player->weapons[player->hand]));
	else
	{
		if (player->hand == 4)//ROCKETLAUNCHER)
			;//send_bullet(player->stat.pos, /*RocketTexture*/);
		else
			;//bullet(player);
		player->weapons[player->hand].clip--;
	}
}

void		kick(t_doom *doom, t_player *player)
{
	t_fvct3	d;
	int		range;

	range = 1;
	d.x = range * sin(player->stat.rot.x * PI180) * cos(player->stat.rot.y * PI180);
	d.y = range * sin(player->stat.rot.x * PI180) * sin(player->stat.rot.y * PI180);
	d.z = -(range * cos(player->stat.rot.x * PI180)) + (player->stat.height / 2);
	Mix_PlayChannel(2, doom->sound.tab_effect[6], 0);
}

//===================================================================================================//

t_wall		*collision_bullet(t_doom *doom, t_fvct3 ori, t_fvct3 pos)// ne renvoie pas encore le mur le plus proche
{
	int		i;
	int		j;

	j = -1;
	while (++j < doom->sector->len_sub)
	{
		i = -1;
		while (++i < doom->sector->len)
		{
			if (vector_intersect(ori, pos, *(t_fvct3*)&doom->sector->ssector[j].wall[i].pillar.p, *(t_fvct3*)&doom->sector->ssector[j].wall[i].next->p))
				{printf("\nsubwall %d\n", i);return (&doom->sector->ssector[j].wall[i]);}
		}
	}
	i = 0;
	while (i < doom->sector->len)
	{
		if (vector_intersect(ori, pos, *(t_fvct3*)&doom->sector->wall[i].pillar.p, *(t_fvct3*)&doom->sector->wall[i].next->p))
			{printf("\nwall %d\n", i);return (&doom->sector->wall[i]);}
		++i;
	}
	return (NULL);
}

void		bullet(t_doom *doom, t_player *player)
{
	t_fvct3	d;
	t_wall	*hit;

	d.x = RADIUS * sin(player->stat.rot.x * PI180) * cos(player->stat.rot.y * PI180);
	d.y = RADIUS * sin(player->stat.rot.x * PI180) * sin(player->stat.rot.y * PI180);
	d.z = -(RADIUS * cos(player->stat.rot.x * PI180)) + (player->stat.height / 2);
	hit = collision_bullet(doom, d, player->stat.pos);
	if (hit)
	{
		printf("HIT\n");
	}
	else
		printf("MISSED\n");
	printf("\rRot : %f\t%f\n", player->stat.rot.x, player->stat.rot.y);
	printf("\rFov : %d\n", player->fov);
	printf("\rbullet landed : x = %f | y = %f | z = %f\n", d.x, d.y, d.z);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

double			bullet_clipping(t_wall wall, t_fvct3 pos, double angle)
{
	t_fvct2		inter;
	t_fvct2		diff;
	t_fvct2		diff2;
	double		coef_ang;
	double		coef_wall;
	double		b;

	diff.x = wall.pillar.p.x - pos.x;
	diff.y = wall.pillar.p.y - pos.y;
	diff2.x = wall.next->p.x - pos.x;
	diff2.y = wall.next->p.y - pos.y;
	coef_ang = tan(angle * PI180);
	if (diff2.x - diff.x < 0.00001 && diff2.x - diff.x > -0.000001)
	{
		inter.x = diff.x;
		inter.y = diff.x * coef_ang;
	}
	else
	{
		coef_wall = (diff2.y - diff.y) / (diff2.x - diff.x);
		b = diff.y - diff.x * coef_wall;
		inter.x = b / (coef_ang - coef_wall);
		inter.y = coef_wall * inter.x + b;
	}
	return (distance((t_fvct2){0.0, 0.0}, inter));
}
/*
t_wall		**possible_walls(t_doom *doom, t_fvct3 ori, t_fvct3 pos)
{
	t_wall	*walls[50];
	int		i;
	int		j;
	int		index;

	j = -1;
	index = 0;
	while (++j < doom->sector->len_sub)
	{
		i = -1;
		while (++i < doom->sector->len)
		{
			if (vector_intersect(ori, pos, *(t_fvct3*)&doom->sector->ssector[j].wall[i].pillar.p, *(t_fvct3*)&doom->sector->ssector[j].wall[i].next->p))
			{
				walls[index] = &doom->sector->ssector[j].wall[i];
				index++;
				printf("sub wall %d\n", i);
			}
		}
	}
	i = -1;
	while (++i < doom->sector->len)
	{
		if (vector_intersect(ori, pos, *(t_fvct3*)&doom->sector->wall[i].pillar.p, *(t_fvct3*)&doom->sector->wall[i].next->p))
		{
			walls[index] = &doom->sector->wall[i];
			index++;
			printf("sec wall %d\n", i);
		}
	}
	walls[index] = NULL;
	return (walls);
}

t_wall		*real_hit(t_wall **walls, t_fvct3 pos, double angle)
{
	t_wall *hit;
	int		i;
	double	res;
	double	tmp;

	i = 0;
	res = 987654312.0;
	hit = NULL;
	while (walls[i])
	{
		if ((tmp = bullet_clipping(*walls[i], pos, angle)) < res)
		{
			res = tmp;
			hit = walls[i];
		}
		++i;
	}
	return (hit);
}*/

t_wall		*real_hit(t_wall **walls, t_fvct3 pos, double angle)
{
	t_wall *hit;
	int		i;
	double	res;
	double	tmp;

	i = 0;
	res = 987654312.0;
	hit = NULL;
	while (walls[i])
	{
		if ((tmp = bullet_clipping(*walls[i], pos, angle)) < res)
		{
			res = tmp;
			hit = walls[i];
		}
		++i;
	}
	printf("MURMUR AU MUR PAS MUR || %f ||\n", res);
	return (hit);
}

t_wall		*possible_walls(t_doom *doom, t_fvct3 ori, t_fvct3 pos)
{
	t_wall	*walls[50];
	int		i;
	int		j;
	int		index;

	j = -1;
	index = 0;
	while (++j < doom->sector->len_sub)
	{
		i = -1;
		while (++i < doom->sector->len)
		{
			if (vector_intersect(ori, pos, *(t_fvct3*)&doom->sector->ssector[j].wall[i].pillar.p, *(t_fvct3*)&doom->sector->ssector[j].wall[i].next->p))
			{
				walls[index] = &doom->sector->ssector[j].wall[i];
				index++;
				printf("sub wall %d\n", i);
			}
		}
	}
	i = -1;
	while (++i < doom->sector->len)
	{
		if (vector_intersect(ori, pos, *(t_fvct3*)&doom->sector->wall[i].pillar.p, *(t_fvct3*)&doom->sector->wall[i].next->p))
		{
			walls[index] = &doom->sector->wall[i];
			index++;
			printf("sec wall %d\n", i);
		}
	}
	walls[index] = NULL;
	return (real_hit(walls, pos, doom->player.stat.rot.y));
}

void		bulletV42(t_doom *doom, t_player *player)
{
	t_fvct3	d;
	t_wall	**walls;
	t_wall	*hit;

	d.x = player->stat.pos.x + (RADIUS * sin(player->stat.rot.x * PI180) * cos(player->stat.rot.y * PI180));
	d.y = player->stat.pos.y + (RADIUS * sin(player->stat.rot.x * PI180) * sin(player->stat.rot.y * PI180));
	d.z = player->stat.pos.z + (-(RADIUS * cos(player->stat.rot.x * PI180)) + (player->stat.height / 2));
	//walls = possible_walls(doom, d, player->stat.pos);
	//hit = real_hit(walls, player->stat.pos, player->stat.rot.y);
	hit = possible_walls(doom, d, player->stat.pos);
	if (hit)
	{
		printf("HIT\n");
	}
	else
		printf("MISSED\n");
	printf("\rRot : %f\t%f\n", player->stat.rot.x, player->stat.rot.y);
	printf("\rFov : %d\n", player->fov);
	printf("\rbullet landed : x = %f | y = %f | z = %f\n", d.x, d.y, d.z);
}
