#include "doom_nukem.h"

void	ui_free(t_ui *ui)
{
	int i;

	i = -1;
	while (ui->btnarr[++i].txture)
		SDL_DestroyTexture(ui->btnarr[i].txture);
	i = -1;
	while (ui->btnmap[++i].txture)
	{
		SDL_DestroyTexture(ui->btnmap[i].txture);
		if (i > 1)
			free(ui->btnmap[i].data);
	}
	if (ui->fonts.s64)
		TTF_CloseFont(ui->fonts.s64);
	if (ui->fonts.s32)
		TTF_CloseFont(ui->fonts.s32);
	if (ui->fonts.s128)
		TTF_CloseFont(ui->fonts.s128);
}

int		ui_by_sdl(t_doom *doom, t_ui *ui)
{
	ui->btnarr[0] = add_doom_button(doom);
	ui->btnarr[1] = add_start_button(doom);
	ui->btnarr[2] = add_opt_button(doom);
	ui->btnarr[3] = add_editor_button(doom);
	ui->btnarr[4] = add_quit_button(doom, " Quit ", &doom_exit);
	ui->btnmap[0] = add_quit_button(doom, " Return ", &return_button);
	ui->btnmap[1] = add_mapmenu_button(doom);
	ui->btnopt[0] = ui->btnmap[0];
	ui->btnopt[1] = ui->btnarr[0];
	ui->slidopt[0] = add_fov_slider(doom);
	ui->slidopt[0].loc.parent = &(ui->btnopt[1].loc);
	return (1);
}


int		ui_init(t_ui *ui)
{
	ui->m_status = 1;
	if (!(ui->fonts.s64 = TTF_OpenFont(TTFWOLF, 64)))
		return (0);
	if (!(ui->fonts.s128 = TTF_OpenFont(TTFWOLF, 128)))
		return (0);
	if (!(ui->fonts.s32 = TTF_OpenFont(TTFIMPACT, 32)))
		return (0);
	return (1);
}