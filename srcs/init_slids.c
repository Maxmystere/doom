/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_slids.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magrab <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 00:18:28 by magrab            #+#    #+#             */
/*   Updated: 2019/04/16 00:18:29 by magrab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_slid		add_fov_slider(t_doom *doom)
{
	t_slid			tmp;

	tmp.txture = SDL_CreateTexture(doom->sdl.rend,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 200, 200);
	tmp.loc.area.w = 500;
	tmp.loc.area.h = 50;
	tmp.loc.area.x = 0;
	tmp.loc.area.y = 0;
	tmp.loc.snapx = 1;
	tmp.loc.snapy = 3;
	tmp.loc.pos.x = 50;
	tmp.loc.pos.y = 20;
	tmp.bgcolor.r = 191;
	tmp.bgcolor.g = 35;
	tmp.bgcolor.b = 44;
	tmp.fgcolor.r = 255;
	tmp.fgcolor.g = 255;
	tmp.fgcolor.b = 255;
	tmp.grip.w = tmp.loc.area.h;
	tmp.grip.h = tmp.loc.area.h;
	tmp.min = 30;
	tmp.val = &doom->fov;
	tmp.max = 110;
	update_slider_txt(doom, &tmp);
	return (tmp);
}
