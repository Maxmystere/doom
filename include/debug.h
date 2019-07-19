
#ifndef DEBUG_H
# define DEBUG_H


# include "doom_struct.h"

# define WEND		"\e[0m"

# define WRED		"\e[31m\e[1m"
# define WGREEN		"\e[32m\e[1m"
# define WYELLOW 	"\e[33m\e[1m"
# define WBLUE 		"\e[34m\e[1m"
# define WMAGENTA 	"\e[35m\e[1m"
# define WCYAN 		"\e[36m\e[1m"
# define WGREY		"\e[38;5;250;250;250m"

# define WBRED		"\e[41m"
# define WBGREEN	"\e[42m"
# define WBYELLOW 	"\e[43m"
# define WBBLUE 	"\e[44m"
# define WBMAGENTA 	"\e[45m"
# define WBCYAN 	"\e[46m"

# define RED		0xff0000ff
# define BLUE		0x0000ffff
# define GREEN		0x00ff00ff
# define BROWN		0xff00ffff
# define YELLOW		0xffff00ff
# define PURPLE		0xff00ffff
# define WHITE		0xffffffff

# define BLUE_SOFT	0x138fa2ff
# define RED_SOFT	0x831a1aff
# define GREEN_SOFT	0x548528ff

void					sector_recursif(t_sector sector, int sub);
void		d_wall(t_wall *wall);
void		draw_borne(t_arch *arch, uint32_t color);
void		draw_wall(t_arch *arch, uint32_t color);
void		fill_line_debug(t_arch *arch, t_sdl *sdl, t_vct2 pos0, t_vct2 pos1, Uint32 color);
void		debug_screen_copy(t_arch *arch);
void		draw_affine(t_arch *arch, t_affine affine, uint32_t color);
void		p_debug(t_fvct2 a, Uint32 color, t_arch *arch);
void		b_point_d(t_arch *arch, t_fvct2 v, Uint32 color);

#endif
