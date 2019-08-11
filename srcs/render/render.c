#include "doom_nukem.h"
#include "render.h"
#include "debug.h"
/*
**	on recupere les information du secteur par rapport au frustum (champs de vision du joueur)
**	on definit un bunch contenant les mur visibles
**	on affiche ce bunch
*/
void				sector_render(t_arch *arch, t_player *player, t_sector *sector)
{
	t_wall			*wall;
	int				i;
	t_wall			*portal_tmp;
	t_sprite		*sprite;

	if (debug == 9)
	{
		printf(WBLUE"\nrender_sector(%d) up %d down %d\n"WEND, arch->depth_portal, arch->portal.b_up[arch->sdl->size.x/2], arch->portal.b_down[arch->sdl->size.x/2]);
	}

	if (debug_screen == 2 && arch->depth_portal > 0)
		draw_borne(arch, RED);
	i = 0;
	wall = sector->wall;
	sector_frustum(arch, sector, player);
	arch->sector = sector;
	portal_tmp = arch->wall;
	while (i < sector->len)
	{
		if (debug_screen == 3)
			debug_segment(arch, sector->wall[i].pillar->p, sector->wall[i].next->p, RED, WHITE);
		on_frustum(arch, player, wall[i].pillar);
		on_frustum(arch, player, wall[i].next);
		if (((wall[i].pillar->frust || wall[i].next->frust)
			||	borne_in_wall_angle(arch, &wall[i]))
				&& equal_pillar(&wall[i], arch->wall))
		{
			portal_tmp = arch->wall;
			arch->wall = &wall[i];
			render_wall(arch, player);
			arch->wall = portal_tmp;
		}
		i++;
	}
	if (debug_screen == 3)
	{
		uint32_t color[] = {BLUE_SOFT, YELLOW, RED, RED_SOFT};
		if (arch->depth_portal > 3)
			color[0] = color[3];
		else
			color[0] = color[arch->depth_portal];
		debug_sector_box(arch, &sector->box, color[0]);
	}
	if (debug == 9)
	{
		printf("render_enemy(%d) up %d down %d\n", arch->depth_portal, arch->portal.b_up[arch->sdl->size.x/2], arch->portal.b_down[arch->sdl->size.x/2]);
	}
	sprite = NULL;

	sprite_from_enemy(&sprite, sector->enemys, player, arch);
	sprite_from_props(&sprite, sector->props, player, sector->len_prop, arch);

	sprite_render_list(sprite, arch, player);


	if (debug == 7)
		sprite_iter(sprite, &sprite_print);

	sprite_iter(sprite, &sprite_free);

	//render_sector_enemy(arch, sector, player);
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
**	on initialise arch wall a null
**	(NULL est un signal pour indiquer que le rendu du secteur ne se fait
**		pas a travers un portail)
**	doom_render fait le premier rendu de secteur (celui dans lequel le joueur se trouve)
*/

int					doom_render(t_doom *doom)
{
	int				i;
	t_minimap		mini;

	i = 0;
	if (debug == 1)
		printf("\n-------start render-------\n");
	doom->game.arch.depth_portal = 0;
	doom->game.arch.wall = NULL;
	if (debug_screen == 2)
		draw_frustum(&doom->game.arch, SCREEN_ON | FOV_HORI);
	else if (debug_screen == 3)
	{
		b_point_debug(*(t_fvct2*)&doom->game.player.stat.pos, RED);
	}
	sector_render(&doom->game.arch, &doom->game.player, doom->game.player.stat.sector);
	mini = miniinit(&doom->sdl, &doom->ui);
	minimap(&mini, &doom->game.player);
	hud_aim(doom);
	debug_screen_copy(&doom->game.arch);
	sdl_MultiRenderCopy(&doom->sdl);
	miniinv(&mini, &doom->game.player);
	architect_reset(&doom->game.arch);
	//clear_screen(&doom->sdl);
	//ft_bzero(&doom->game.arch.sc_debug, sizeof(Uint32) * doom->sdl.size.x * doom->sdl.size.y);
	if (debug == 1)
		printf("\n--------------------------\n");
	return (1);
}
