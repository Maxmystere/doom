/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sector_menu.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magrab <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 02:57:09 by magrab            #+#    #+#             */
/*   Updated: 2019/08/13 02:57:10 by magrab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "render.h"

void draw_sector_menu(t_editor *editor, t_font font)
{
	SDL_Rect box;
	t_lstsec currsec;
	int x;

	x = 0;
	box = editor->sectbox;
	if (editor->currmur || editor->currstat)
		SDL_SetRenderDrawColor(editor->rend, 99, 99, 99, 255);
	else
		SDL_SetRenderDrawColor(editor->rend, 66, 66, 66, 255);
	SDL_RenderFillRect(editor->rend, &box);
	SDL_SetRenderDrawColor(editor->rend, 255, 255, 255, 255);
	box.h = SECTORBOXHEIGHT;
	box.y += editor->sectscroll;
	currsec = editor->sectors;
	while (currsec)
	{
		SDL_RenderDrawRect(editor->rend, &box);
		if (editor->currmur && editor->currmur->portal_ptr == currsec)
			sdl_int_put(editor->rend, font.s32, (t_vct2){box.x + 5, box.y + 5}, "Walls ", ft_walllen(currsec->murs), (SDL_Color){200, 200, 150, 0xFF});
		else if (editor->currstat && editor->currstat->sector == currsec)
		{
			if (editor->currstat == &editor->player.stat)
				sdl_int_put(editor->rend, font.s32, (t_vct2){box.x + 5, box.y + 5}, "Walls ", ft_walllen(currsec->murs), (SDL_Color){100, 205, 100, 0xFF});
			else if (ISPROP(editor->currstat->type) || ISWALLPROP(editor->currstat->type))
				sdl_int_put(editor->rend, font.s32, (t_vct2){box.x + 5, box.y + 5}, "Walls ", ft_walllen(currsec->murs), (SDL_Color){100, 125, 240, 0xFF});
			else
				sdl_int_put(editor->rend, font.s32, (t_vct2){box.x + 5, box.y + 5}, "Walls ", ft_walllen(currsec->murs), (SDL_Color){170, 100, 205, 0xFF});
		}
		else if (currsec == editor->map)
			sdl_int_put(editor->rend, font.s32, (t_vct2){box.x + 5, box.y + 5}, "Walls ", ft_walllen(currsec->murs), (SDL_Color){0xDD, 0xDD, 0xDD, 0xFF});
		else
			sdl_int_put(editor->rend, font.s32, (t_vct2){box.x + 5, box.y + 5}, "Walls ", ft_walllen(currsec->murs), (SDL_Color){0x88, 0xAA, 0xBB, 0xFF});
		if (!editor->currmur && !editor->currstat)
			sdl_string_put(editor->rend, font.s32, (t_vct2){box.x + box.w - 40, box.y + 5}, "[X]", (SDL_Color){0xFF, 0x55, 0x55, 0xFF});
		box.y += box.h;
		currsec = currsec->next;
		++x;
	}
	if (!editor->currmur && !editor->currstat)
		sdl_string_put(editor->rend, font.s32, (t_vct2){box.x + box.w / 2 - 20, box.y + 5}, "(+)", (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF});
	SDL_SetRenderDrawColor(editor->rend, 0, 0, 0, 255);
}