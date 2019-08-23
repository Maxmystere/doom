#include "render.h"

int			trunc_int(int value, int min, int max)
{
	if (value < min)
		return (min);
	else if (value > max)
		return (max);
	return (value);
}

void		set_borne_vertical(t_arch *arch, t_vct2 surface, int i)
{

	arch->portal.b_up[i] = (uint32_t)trunc_int(surface.x, arch->portal.b_up[arch->px.x], arch->portal.b_down[arch->px.x] - 1);
	arch->portal.b_up[i] = (uint32_t)trunc_int(arch->portal.b_up[i], 0, arch->sdl->size.y - 1);
	arch->portal.b_down[i] = (uint32_t)trunc_int(surface.y, arch->portal.b_up[arch->px.x], arch->portal.b_down[arch->px.x] - 1);
	arch->portal.b_down[i] = (uint32_t)trunc_int(arch->portal.b_down[i], 0, arch->sdl->size.y - 1);
}

void		set_borne_horizontal(t_arch *arch)
{
	//ajouter la difference si jamais sdl->size est impair
	arch->portal.b_left = atan2((arch->sdl->size.x / 2) - arch->px.x, arch->cam->d_screen);
	arch->portal.b_right = atan2((arch->sdl->size.x / 2) - arch->px.y, arch->cam->d_screen);
	arch->portal.b_left *= TOANGLE;
	arch->portal.b_right *= TOANGLE;
}

extern inline void borne_secur(t_arch *arch)
{
	if (arch->portal.b_up[arch->px.x] > (uint32_t)arch->sdl->size.y)
		arch->portal.b_up[arch->px.x] = arch->sdl->size.y - 1;
	if (arch->portal.b_down[arch->px.x] > (uint32_t)arch->sdl->size.y)
		arch->portal.b_down[arch->px.x] = arch->sdl->size.y - 1;
}

/*
**	sauvegarde toute les configuration de borne dans une structure
**	dans une zone (la zone est les limite de px)
**	la sauvegarde de la profondeur avec le zline se fera au
**	fur et a mesure dans le pillar_to_pillar
*/
t_borne		*borne_svg(t_arch *arch, t_borne *borne, t_vct2 px)
{
	int		len;

	//len = arch->px.y - arch->px.x;
	len = px.y - px.x;
	//ne segv plus quand arch mais quand px
	borne->b_left = arch->portal.b_left;
	borne->b_right = arch->portal.b_right;
	borne->pillar = arch->portal.pillar;
	borne->next = arch->portal.next;
	borne->sector_svg = arch->sector;
	if (len > 0)
		borne_init(borne, len);
	return (borne);
}

/*
**	recharge une borne dans la borne arch
**	(une borne anciennement sauvegarde...)
*/
void		borne_load(t_arch *arch, t_borne *borne, t_vct2 px_draw)
{
	int		i;

	i = 0;
	arch->portal.b_left = borne->b_left;
	arch->portal.b_right = borne->b_right;
	while (px_draw.x < px_draw.y)
	{
		arch->portal.b_up[px_draw.x] = borne->b_up[i];
		arch->portal.b_down[px_draw.x] = borne->b_down[i];
		arch->portal.zline[px_draw.x] = borne->zline[i];
		px_draw.x++;
		i++;
	}
	arch->portal.pillar = borne->pillar;
	arch->portal.next = borne->next;
	arch->sector = borne->sector_svg;
	borne_free(borne);
}

void		render_recursivite(t_arch *arch, t_player *player, t_vct2 pixel_portal)
{
	arch->px = pixel_portal;
	set_borne_horizontal(arch);
	arch->portal.pillar = arch->pillar;
	arch->portal.next = arch->next;
	arch->depth_portal++;
	sector_render(arch, player, arch->wall->link);
	arch->depth_portal--;
}

void		save_pixels_portal(t_arch *arch, t_pil_render *render_stuff
	,t_vct2 *pixels)
{
	if (arch->wall->status == PORTAL)
		*pixels = arch->px;
	else if (arch->wall->status == OPEN_DOOR
		|| arch->wall->status == CLOSE_DOOR)
	{
		if (!render_stuff->open_invert)
		{
			pixels->x = render_stuff->px_inter;
			pixels->y = arch->px.y;
		}
		else
		{
			pixels->x = arch->px.x;
			pixels->y = render_stuff->px_inter;
		}
	}
}