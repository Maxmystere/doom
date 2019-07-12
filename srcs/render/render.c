#include "doom_nukem.h"

/*
**	on recupere les information du secteur par rapport au frustum (champs de vision du joueur)
**	on definit un bunch contenant les mur visibles
**	on affiche ce bunch
*/
void				sector_render(t_game *game, t_sector *sector)
{
	t_wall			*bunch[50];

	sector_frustum(&game->arch, sector, &game->player);
	buncherisation(&game->arch, *sector, bunch);
	bunch_comsuption(game, bunch, sector);
}

void				clear_screen(t_sdl *sdl)
{
	int			i;

	i = 0;
	while (i < sdl->size.x * sdl->size.y)
	{
		sdl->screen[i] = 0;
		i++;
	}
}

/*
**	les etapes de rendu du jeu
*/
int					doom_render(t_doom *doom)
{
	int				i;

	i = 0;
	sector_render(&doom->game, doom->game.player.stat.sector);
	minimap(doom);
	sdl_MultiRenderCopy(&doom->sdl);
	architect_reset(&doom->game.arch);
	//clear_screen(&doom->sdl);
	return (1);
}
