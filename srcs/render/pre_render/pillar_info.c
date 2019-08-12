#include "render.h"
#include "debug.h"
/*
**	si au moins l'un des pilier est hors frustum il passe pas l'extremite de
**	l'ecran, on determine laquelle avec une polarite (-1 == 0px et 1 == max)
**	si l'angle entre le joueur est les deux pillier est superieur a 180
**	la polarite de depart (position du premier pillier) s'inverse
*/
void		px_polarite(t_arch *arch)
{
	t_fvct2	angle;
	double	diff;
	int		polarite;

	angle.x = local_angle(arch->portal.b_left, arch->wall->pillar->angle);
	angle.y = local_angle(arch->portal.b_left, arch->wall->next->angle);

	diff = fabs(angle.x - angle.y);
	polarite = (arch->wall->next->angle > arch->portal.b_left ? -1 : 1) * (diff < 180 ? 1 : -1);
	arch->px.x = (polarite == -1) ? 0 : arch->sdl->size.x -1;
	arch->px.y = arch->sdl->size.x - 1 - arch->px.x;
}

int			pillar_polarite(t_arch *arch, t_pillar *pillar, t_pillar *next)
{
	double	angle;
	double	angle_next;
	double	borne;

	angle = local_angle(arch->portal.b_left, pillar->angle);
	angle_next = local_angle(arch->portal.b_left, next->angle);
	borne = local_angle(arch->portal.b_left, arch->portal.b_right);


	if (angle < borne - 180)
		return (0);
	else if (angle > 180)
		return (arch->sdl->size.x - 1);
	else if (fabs(angle - angle_next) > 180)
		return (0);
	else
		return (arch->sdl->size.x - 1);
}

void			pillar_screen_info(t_arch *arch, t_player *p)
{
	float		angle;
	int			size;
	t_fvct2		tmp;

	size = arch->sdl->size.x;
	if (arch->wall->pillar->frust)
	{
		arch->px.x = fish_bowl_px(arch, *arch->wall->pillar);
		arch->pillar.x = distance(*(t_fvct2*)&p->stat.pos, arch->wall->pillar->p);
		arch->pillar.y = sin(arch->wall->pillar->angle * PI180) * arch->pillar.x;
		arch->pillar.x = cos(arch->wall->pillar->angle * PI180) * arch->pillar.x;
		arch->shift_txtr.x = 1;
	}
	else
	{
		arch->px.x = pillar_polarite(arch, arch->wall->pillar, arch->wall->next);
		if (arch->px.x == 0)
		{
			arch->px.x = arch->sdl->size.x / 2.0 - (tan(arch->portal.b_left * PI180) * arch->cam->d_screen);
			angle = p->stat.rot.y + arch->portal.b_left;
		}
		else
		{
			arch->px.x = arch->sdl->size.x / 2.0 - (tan(arch->portal.b_right * PI180) * arch->cam->d_screen);
			angle = p->stat.rot.y + arch->portal.b_right;
		}
		arch->shift_txtr.x = wall_clipping(arch, p, &tmp, angle);
		arch->pillar = tmp;
	}
	if (arch->wall->next->frust)
	{
		arch->px.y = fish_bowl_px(arch, *arch->wall->next);
		arch->next.x = distance(*(t_fvct2*)&p->stat.pos, arch->wall->next->p);
		arch->next.y = sin(arch->wall->next->angle * PI180) * arch->next.x;
		arch->next.x = cos(arch->wall->next->angle * PI180) * arch->next.x;

		arch->shift_txtr.y = 0;
	}
	else
	{
		arch->px.y = pillar_polarite(arch, arch->wall->next, arch->wall->pillar);
		if (arch->px.y == 0)
		{
			arch->px.y = arch->sdl->size.x / 2.0 - (tan(arch->portal.b_left * PI180) * arch->cam->d_screen);
			angle = p->stat.rot.y + arch->portal.b_left;
		}
		else
		{
			arch->px.y = arch->sdl->size.x / 2.0 - (tan(arch->portal.b_right * PI180) * arch->cam->d_screen);
			angle = p->stat.rot.y + arch->portal.b_right;
		}
		arch->shift_txtr.y = wall_clipping(arch, p, &tmp, angle);
		arch->next = tmp;
	}
}


/*
**	renvoie la position du mur par rapport au portail de rendu
**		0 si le portail est devant le portail
**		1 si le portail est derriere le portail
*/
int				wall_behind_portal(t_arch *arch)
{
	t_affine	a_wall;
	t_affine	a_portal;

	t_affine	a_pillar;
	t_affine	a_pillar2;
	t_fvct2		inter;
	t_fvct2		inter2;

	a_pillar.a = arch->pillar.y / arch->pillar.x;
	a_pillar.b = 0;
	a_pillar2.a = arch->next.y / arch->next.x;
	a_pillar2.b = 0;

	if (arch->portal.pillar.x == arch->portal.next.x)
	{
		a_portal.lock = 1;
		a_portal.b = arch->portal.pillar.x;
	}
	else
	{
		a_portal.lock = 0;
		a_portal.a = (arch->portal.next.y - arch->portal.pillar.y) / (arch->portal.next.x - arch->portal.pillar.x);
		a_portal.b = arch->portal.pillar.y - a_portal.a * arch->portal.pillar.x;
	}
	inter = interpolation_linear(a_portal, a_pillar);
	inter2 = interpolation_linear(a_portal, a_pillar2);
	if (inter.x > arch->pillar.x && inter2.x > arch->next.x)
		return (0);
	if (arch->pillar.x == arch->next.x)
	{
		a_wall.lock = 1;
		a_wall.b = arch->pillar.x;
	}
	else
	{
		a_wall.lock = 0;
		a_wall.a = (arch->next.y - arch->pillar.y) / (arch->next.x - arch->pillar.x);
		a_wall.b = arch->pillar.y - a_wall.a * arch->pillar.x;
	}
	if (inter.x > arch->pillar.x)
	{
		if (interpolation_linear_secur(a_portal, a_wall, &inter))
			return (0);
		pillar_virtual_move(arch, inter, PILLAR);
		a_pillar.a = arch->pillar.y / arch->pillar.x;
		a_pillar.b = 0;
		arch->px.x = arch->sdl->size.x / 2 - affine_val(a_pillar, arch->cam->d_screen);
	}
	else if (inter2.x > arch->next.x)
	{
		if (interpolation_linear_secur(a_portal, a_wall, &inter))
			return (0);
		pillar_virtual_move(arch, inter, NEXT);
		a_pillar.a = arch->next.y / arch->next.x;
		a_pillar.b = 0;
		arch->px.y = arch->sdl->size.x / 2 - affine_val(a_pillar, arch->cam->d_screen);
	}
	return (1);
}