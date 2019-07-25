/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readfileeditor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbottini <tbottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 21:39:35 by magrab            #+#    #+#             */
/*   Updated: 2019/07/21 13:35:00 by tbottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#define EDITORSTEPX 100.0
#define EDITORSTEPY -100.0

SDL_Texture		*find_texture(SDL_Texture **txtrs, char **edpath, char *surfpath)
{
	int			x;

	x = 0;
	while (edpath[x] && ft_strcmp(edpath[x], surfpath))
		x++;
	return (txtrs[x]);
}

t_secteur		*find_secteur(t_lstsec secteurs, t_sector *stock_sector, t_sector *sector, int nb_sect)
{
	int			index;

	index = 0;
	while (sector != &stock_sector[index] && index < nb_sect)
		index++;
	while (index--)
		secteurs = secteurs->next;
	return (secteurs);

}

t_pilier *find_pillar_from_game(t_pillar *pillars, t_pillar *to_find, t_lstpil pillst)
{
	int x;

	x = 0;
	while (&(pillars[x]) != to_find)
	{
		pillst = pillst->next;
		x++;
	}
	return (pillst);
}

void add_prop(t_game *game, t_editor *edit, t_sector *gamesec)
{
	int y;
	t_prop *prop;
	
	y = 0;
	while (y < gamesec->len_prop)
	{
		prop = &gamesec->props[y];
		ft_enemypushend(&edit->ennlist, (t_vct2){prop->pos.x * EDITORSTEPX, prop->pos.y * EDITORSTEPY}, prop->type, find_secteur(edit->sectors, game->sectors, prop->sector, game->len.nb_sects));
		y++;
	}
}

void add_wall_prop(t_game *game, t_editor *edit, t_wall *gamewall, t_mur *mur)
{
	int y;
	t_prop *prop;
	t_entity *ent;
	
	y = 0;
	while (y < gamewall->nb_props)
	{
		prop = &gamewall->props[y];
		ent = ft_enemypushend(&mur->wproplist, (t_vct2){prop->pos.x * EDITORSTEPX, prop->pos.y * EDITORSTEPY}, prop->type, find_secteur(edit->sectors, game->sectors, prop->sector, game->len.nb_sects));
		y++;
	}
}

void add_walls(t_game *game, t_editor *edit, t_sector *gamesec, t_secteur *sec)
{
	int		y;
	t_mur	*mur;

	y = 0;
	while (y < gamesec->len)
	{
		mur = ft_wallpushend(&sec->murs, find_pillar_from_game(game->pillars, gamesec->wall[y].pillar, edit->pillist), find_pillar_from_game(game->pillars, gamesec->wall[y].next, edit->pillist), find_texture(edit->txtrgame, edit->txtrname, game->surfpath[gamesec->wall[y].txtr.id]));
		add_wall_prop(game, edit, &gamesec->wall[y], mur);
		mur->portal_id = gamesec->wall[y].status;
		y++;
	}
}

void add_enemies(t_game *game, t_editor *edit, t_sector *gamesec, t_secteur *sec)
{
	t_enemy		*enn;

	enn = gamesec->enemys;
	while (enn)
	{
		ft_enemypushend(&edit->ennlist, (t_vct2){enn->stat.pos.x * EDITORSTEPX, enn->stat.pos.y * EDITORSTEPY}, enn->type, sec);
		enn = enn->next;
	}
}

int game_to_editor(t_game *game, t_editor *edit)
{
	int x;
	t_vct2 pos;
	t_secteur *sec;

	x = 0;
	while (x < game->len.nb_pills)
	{
		pos.x = game->pillars[x].p.x * EDITORSTEPX;
		pos.y = game->pillars[x].p.y * EDITORSTEPY;
		ft_pillarpushend(&edit->pillist, pos);
		x++;
	}
	x = 0;
	while (x < game->len.nb_sects)
	{
		;
		sec = push_secteur(&edit->sectors, find_texture(edit->txtrgame, edit->txtrname, game->surfpath[game->sectors[x].txtrtop.id]), find_texture(edit->txtrgame, edit->txtrname, game->surfpath[game->sectors[x].txtrsol.id]));
		sec->htop = game->sectors[x].h_ceil * EDITORSTEPX;
		sec->hsol = game->sectors[x].h_floor * EDITORSTEPX;
		sec->gravity = ((game->sectors[x].gravity.z == G_MOON));
		x++;
	}
	
	edit->player.stat.pos.x = game->player.stat.pos.x * EDITORSTEPX;
	edit->player.stat.pos.y = game->player.stat.pos.y * EDITORSTEPY;
	edit->player.stat.roty = game->player.stat.rot.y + 90.0;
	edit->player.stat.sector = find_secteur(edit->sectors, game->sectors, game->player.stat.sector, game->len.nb_sects);
	sec = edit->sectors;
	x = 0;
	while (x < game->len.nb_sects)
	{
		add_walls(game, edit, &game->sectors[x], sec);
		add_prop(game, edit, &game->sectors[x]);
		add_enemies(game, edit, &game->sectors[x], sec);
		sec = sec->next;
		x++;
	}
	return (0);
}

int	relink_sector(t_game *game, t_editor *edit)
{
	t_secteur			*secteur;
	t_mur				*mur;
	int					idsec;
	int					idmur;

	idsec = 0;
	secteur = edit->sectors;
	while (secteur)
	{
		idmur = 0;
		mur = secteur->murs;
		while (mur)
		{
			// Parcours
			if (mur->portal_id != WALL)
				mur->portal_ptr = find_secteur(edit->sectors, game->sectors, game->sectors[idsec].wall[idmur].link, game->len.nb_sects);

			mur = mur->next;
			idmur++;
		}
		secteur = secteur->next;
		idsec++;
	}
	return (1);
}

int	read_file_to_editor(t_editor *edit, const char *file)
{
	t_game	game;
	int		returncode;
	char path[512];

	ft_strcpy(path, "ressources/map/");
	ft_strcpy(&(path[15]), file);
	printf("ouverture de :%s\n", path);
	ft_bzero(&game, sizeof(t_game));
	if ((returncode = read_file(&game, path, true)))
	{
		printf("Error : %d\n", returncode);
		return (-1);
	}
	if (game_to_editor(&game, edit))
	{
		free_game(&game);
		return (-2);
	}
	relink_sector(&game, edit);
	free_game(&game);
	ft_putendl("Successfully read ressources/map/editor.map\n");
	edit->map = edit->sectors;
	if (!(edit->player.stat.sector))
		edit->player.stat.sector = edit->map;
	edit->status = ED_LOADED;
	return (0);
}
