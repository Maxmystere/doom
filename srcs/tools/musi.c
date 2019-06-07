/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   musi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrache <akrache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 03:47:35 by akrache           #+#    #+#             */
/*   Updated: 2019/06/07 02:35:57 by akrache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "doom_nukem.h"

void		change_music(t_sound *sound, int n, int fade)
{
	if (n == -1 && Mix_PlayingMusic())
		Mix_FadeOutMusic(fade);
	else if (n >= 0 && n < 11)
	{
		//n != 0 ? sound->on = n : 0;
		sound->music = sound->tab_music[n];
		Mix_FadeInMusic(sound->music, -1, fade);
	}
}

void		change_volume(t_sound *sound)
{
	;
}