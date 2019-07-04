/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mur_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbottini <tbottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 19:36:28 by tbottini          #+#    #+#             */
/*   Updated: 2019/04/27 13:13:03 by tbottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static t_lstmur ft_newwall(t_pilier *pil1, t_pilier *pil2, SDL_Texture *txtr)
{
	t_lstmur t;

	if (!(t = malloc(sizeof(t_mur))))
		return (NULL);
	t->pil1 = pil1;
	t->pil2 = pil2;
	t->txtr = txtr;
	t->prvs = NULL;
	t->next = NULL;
	t->portal_id = 0;
	t->portal_ptr = NULL;
	t->wproplist = NULL;
	return (t);
}

void ft_movewall(t_mur *wall, int addx, int addy, int zoom)
{
	t_lstent wprops;

	wall->pil1->pos.x += addx * (EDITORPRECISION) / zoom;
	wall->pil1->pos.y += addy * (EDITORPRECISION) / zoom;
	wall->pil2->pos.x += addx * (EDITORPRECISION) / zoom;
	wall->pil2->pos.y += addy * (EDITORPRECISION) / zoom;
	wprops = wall->wproplist;
	while (wprops)
	{
		wprops->stat.pos = line_percent(wall->pil1->pos, wall->pil2->pos, wprops->stat.roty / 100);
		wprops = wprops->next;
	}
}

void ft_removewall(t_lstmur *start, t_mur **mur)
{
	if (!mur || !(*mur))
		return ;
	if (*mur == *start)
		*start = (*start)->next;
	if ((*mur)->next)
		(*mur)->next->prvs = (*mur)->prvs;
	if ((*mur)->prvs)
		(*mur)->prvs->next = (*mur)->next;
	ft_clear_entity_list(&((*mur)->wproplist));
	free(*mur);
	*mur = NULL;
}

void ft_remove_walls_with_pillar(t_lstmur *start, t_pilier *pil)
{
	t_lstmur t;
	t_lstmur tmp;

	if (!start || !(*start))
		return;
	t = *start;
	tmp = NULL;
	while (t)
	{
		if (t->pil1 == pil || t->pil2 == pil)
		{
			tmp = t->next;
			ft_removewall(start, &t);
			t = tmp;
		}
		else
			t = t->next;
	}
}

int	ft_walllen(t_lstmur start)
{
	int x;

	x = 0;
	while (start)
	{
		++x;
		start = start->next;
	}
	return (x);
}

t_lstmur ft_wallpushend(t_lstmur *start, t_pilier *pil1, t_pilier *pil2, SDL_Texture *txtr)
{
	t_lstmur t;

	if (!start || pil1 == pil2)
		return (NULL);
	if (!(*start))
		return (*start = ft_newwall(pil1, pil2, txtr));
	t = *start;
	while (t->next)
	{
		if ((t->pil1 == pil1 && t->pil2 == pil2)
			|| (t->pil1 == pil2 && t->pil2 == pil1))
			return (NULL);
		t = t->next;
	}
	if ((t->pil1 == pil1 && t->pil2 == pil2) || (t->pil1 == pil2 && t->pil2 == pil1))
			return (NULL);
	if (!(t->next = ft_newwall(pil1, pil2, txtr)))
		return (NULL);
	t->next->prvs = t;
	return (t->next);
}

void ft_clear_wall_list(t_lstmur *start)
{
	t_lstmur tmp;

	if (!start || !(*start))
		return;
	tmp = *start;
	while (tmp->next && tmp->next != *start)
		tmp = tmp->next;
	while (tmp->prvs && tmp->prvs != *start)
	{
		tmp = tmp->prvs;
		ft_clear_entity_list(&(tmp->wproplist));
		free(tmp->next);
	}
	if (tmp != *start)
		free(tmp);
	free(*start);
	*start = NULL;
}
