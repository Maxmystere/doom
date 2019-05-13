/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbottini <tbottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/09 15:30:27 by tbottini          #+#    #+#             */
/*   Updated: 2019/05/08 21:23:52 by tbottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		fill_pixel(uint32_t *screen, t_vct2 size, t_vct2 pos, int color)
{
	if (0 <= pos.x && pos.x < size.x && 0 <= pos.y && pos.y < size.y)
	{
		screen[pos.x + pos.y * size.x] = color;
		return (1);
	}
	return (0);
}

void	editor_fill_line(t_editor *editor, t_vct2 pos0, t_vct2 pos1, int color)
{
	t_vct3	decal;
	t_vct2	orig;
	int		err;
	int		e2;

	orig.x = ft_abs(pos1.x - pos0.x);
	orig.y = ft_abs(pos1.y - pos0.y);
	decal.x = (pos0.x < pos1.x ? 1 : -1);
	decal.y = (pos0.y < pos1.y ? 1 : -1);
	err = (orig.x > orig.y ? orig.x : -orig.y) / 2;
	fill_pixel(editor->screen, editor->size, pos0, color);
	while ((pos0.x != pos1.x || pos0.y != pos1.y)
			&& fill_pixel(editor->screen, editor->size, pos0, color))
	{
		e2 = err;
		if (e2 > -orig.x && ((err -= orig.y) || 1))
			pos0.x += decal.x;
		if (e2 < orig.y && ((err += orig.x) || 1))
			pos0.y += decal.y;
	}
}

void	fill_line(t_sdl *sdl, t_vct2 pos0, t_vct2 pos1, int color)
{
	t_vct3	decal;
	t_vct2	orig;
	int		err;
	int		e2;

	orig.x = ft_abs(pos1.x - pos0.x);
	orig.y = ft_abs(pos1.y - pos0.y);
	decal.x = (pos0.x < pos1.x ? 1 : -1);
	decal.y = (pos0.y < pos1.y ? 1 : -1);
	err = (orig.x > orig.y ? orig.x : -orig.y) / 2;
	fill_pixel(sdl->screen, sdl->size, pos0, color);
	while ((pos0.x != pos1.x || pos0.y != pos1.y)
			&& fill_pixel(sdl->screen, sdl->size, pos0, color))
	{
		e2 = err;
		if (e2 > -orig.x && ((err -= orig.y) || 1))
			pos0.x += decal.x;
		if (e2 < orig.y && ((err += orig.x) || 1))
			pos0.y += decal.y;
	}
}