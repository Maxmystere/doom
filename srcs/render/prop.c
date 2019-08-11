/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbottini <tbottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/11 21:02:06 by tbottini          #+#    #+#             */
/*   Updated: 2019/08/11 21:48:09 by tbottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "debug.h"

void				prop_init(t_prop *prop, t_wall *wall)
{
	double			hyp;
	double			hyp_pos;
	t_fvct2			diff;

	diff.x = wall->next->p.x - wall->pillar->p.x;
	diff.y = wall->next->p.y - wall->pillar->p.y;
	hyp = sqrt(diff.x * diff.x + diff.y * diff.y);
	diff.x = prop->pos.x - wall->pillar->p.x;
	diff.y = prop->pos.y - wall->pillar->p.y;
	hyp_pos = sqrt(diff.x * diff.x + diff.y * diff.y);
	prop->percent.x = (hyp_pos - 0.5) / (hyp);
	prop->percent.y = (hyp_pos + 0.5) / (hyp);


}

void				prop_iter(t_prop *prop, int len, void(*prop_iter)(t_prop*))
{
	int				i;

	i = 0;
	while (i < len)
	{
		prop_iter(&prop[i]);
		i++;
	}
}

void				prop_iter_v(t_prop *prop, int len, void(*prop_iter)(t_prop*, void*), void *sup)
{
	int				i;

	i = 0;
	while (i < len)
	{
		prop_iter(&prop[i], sup);
		i++;
	}
}

/*
**	a partir du pourcentage du prop sur le mur et les information a propos du mur et de l'ecran
**	on recupere le pixel sur l'ecran du props
**	- recuperation du pourcentage dans le mur actuelle
**	- recuperation de la position dans la ref de la camera
**	- recuperation projection
*/

t_vct2				prop_get_screen_pixel(t_prop *prop, t_arch *arch)
{
	t_fvct2			percent_wall;
	t_fvct2			delta_wall;
	t_fvct2			prop_left_pos;
	t_fvct2			prop_rigth_pos;
	t_vct2			px_props;

	//printf("percent %f %f\n", prop->percent.x, prop->percent.y);
	percent_wall.x = (prop->percent.x - (arch->shift_txtr.x)) / (arch->shift_txtr.y - arch->shift_txtr.x);
	percent_wall.y = (prop->percent.y - (arch->shift_txtr.x)) / (arch->shift_txtr.y - arch->shift_txtr.x);
	//printf("percent %f %f\n", percent_wall.x, percent_wall.y);
	delta_wall.x = arch->next.x - arch->pillar.x;
	delta_wall.y = arch->next.y - arch->pillar.y;
	//printf("delta_wall .x %f %f\n", delta_wall.x, delta_wall.y);
	prop_left_pos.x = delta_wall.x * percent_wall.x + arch->pillar.x;
	prop_left_pos.y = delta_wall.y * percent_wall.x + arch->pillar.y;
	prop_rigth_pos.x = delta_wall.x * percent_wall.y + arch->pillar.x;
	prop_rigth_pos.y = delta_wall.y * percent_wall.y + arch->pillar.y;

	//printf("left prop %f %f\n", prop_left_pos.x, prop_left_pos.y);

	px_props.y = arch->sdl->size.x / 2 - ((prop_rigth_pos.y / prop_rigth_pos.x) * (arch->sdl->size.x / 2));
	px_props.x = arch->sdl->size.x / 2 - ((prop_left_pos.y / prop_left_pos.x) * (arch->sdl->size.x / 2));

	prop->px.x = px_props.y;
	prop->px.y = px_props.x;

	printf("prop px %d %d\n", prop->px.x, prop->px.y);

	if (debug_screen == 8)
	{
		fill_line_debug(arch, arch->sdl, (t_vct2){px_props.x, arch->sdl->size.y / 2}, (t_vct2){px_props.y, arch->sdl->size.y / 2}, 0xffffffff);
	}

	return (px_props);
}

void				prop_wall_render(t_prop *prop, void *arch)
{
	t_arch			*arc;
	t_vct2			px;

	arc = (t_arch*)arch;
	px = prop_get_screen_pixel(prop, arc);
}

void				props_draw_column(t_prop *props, t_arch *arch, t_fvct2 surface)
{
	int				i;
	int				cursor;
	t_vct2			surface_tmp;
	t_fvct2			heigth_percent;
	bool			col_print;
	int				padding_render;

	i = 0;
	col_print = false;
	//heigth_percent.x = ((arch->sector->h_ceil - 1) / 2) / arch->sector->h_ceil;
	heigth_percent.x = (arch->sector->h_ceil - 2) / arch->sector->h_ceil;
	heigth_percent.y = 1 / arch->sector->h_ceil;
	while (i < arch->wall->nb_props && !col_print)
	{
		//printf("props[%d].px %d %d %d\n", i, arch->px.x, props[i].px.x, props[i].px.y);
		if (arch->px.x > props[i].px.x && arch->px.x < props[i].px.y)
		{
			//printf("print arch->px.x %d\n", arch->px.x);

			padding_render = (surface.y - surface.x) * heigth_percent.x;
			printf("heigth ceil %f\n", arch->sector->h_ceil);
			surface_tmp.x = surface.x + padding_render;
			surface_tmp.y = surface.y - padding_render;

			surface_tmp.x = surface.x + (surface.y - surface.x) * heigth_percent.x;
			surface_tmp.y = surface.y - (surface.y - surface.x) * heigth_percent.y;
			cursor = arch->px.x + surface_tmp.x * arch->sdl->size.x;
			props[i].tex = &arch->sector->txtrtop;
			draw_part_texture(arch, cursor, surface_tmp, props[i].tex);

			//arch->sdl->screen[arch->px.x + (arch->sdl->size.y / 2) * arch->sdl->size.x] = 0xffffffff;
			col_print = true;
		}
		i++;
	}
}