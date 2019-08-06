#include "sector.h"
#include "debug.h"

t_sector		*sector_new()
{
	t_sector	*new;

	new = (t_sector *)malloc(sizeof(t_sector));
	if (!new)
		return (NULL);
	new->len_sub = 0;
	new->gravity.x = 0;
	new->gravity.y = 0;
	new->gravity.z = G_MOON;
	new->enemys = NULL;
	return (new);
}

void			sector_set_box(t_sector *sector)
{
	int			i;
	t_fvct2		min;
	t_fvct2		max;
	t_wall		*wall;

	i = 1;
	wall = sector->wall;
	if (debug == 6)
		printf("test sector\n");
	fvct2_cmp_affect(&min, &wall[0].pillar->p, &wall[0].next->p, &double_inf);
	fvct2_cmp_affect(&max, &wall[0].pillar->p, &wall[0].next->p, &double_sup);
	if (debug == 6)
	{
		printf("box min .x %f .y %f max .x %f .y %f\n", min.x, min.y, max.x, max.y);
	}
	while (i < sector->len)
	{
		fvct2_cmp_affect2(&min, &wall[i].pillar->p, &wall[i].next->p, &double_inf);
		fvct2_cmp_affect2(&max, &wall[i].pillar->p, &wall[i].next->p, &double_sup);
		i++;
	}
	if (debug == 6)
	{
		describe_sector(*sector);
		printf("box min .x %f .y %f max .x %f .y %f\n", min.x, min.y, max.x, max.y);
	}
	sector->box.start = min;

	sector->box.length = (t_fvct2){max.x - min.x, max.y - min.y};
}

t_vct2		sector_get_floor_texel(t_sector *sector, t_fvct2 pos)
{
	//on recupere le pourcentage
	//on le multiplie a la largeur/longueur du sprite
	t_vct2		texel;

	texel.x = sector->txtrsol.w * ((pos.x - sector->box.start.x) / sector->box.length.x);
	texel.y = sector->txtrsol.h * ((pos.y - sector->box.start.y) / sector->box.length.y);
	if (debug == 9)
	{
		printf("percent %f %f\n", (pos.x - sector->box.start.x) / sector->box.length.x,
			(pos.y - sector->box.start.y) / sector->box.length.y);
		printf("texel .x %d .y %d, texture %d %d\n", texel.x, texel.y, sector->txtrsol.w, sector->txtrsol.h);
	}
	return (texel);
}
