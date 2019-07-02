
#ifndef CALCUL_H
# define CALCUL_H

# include "vector.h"

typedef struct	s_affine
{
	double		a;
	double		b;
}				t_affine;

/*
**	coef
*/
double			coef_vct(t_fvct2 quantity, t_vct2 step);
double			coef_diff(double diff, t_vct2 step);
double			coef_vct2(t_fvct2 value, t_fvct2 value2);

/*
**	affine fonctions
*/
double			affine_val(t_affine affine, int x);
t_fvct2			interpolation_linear(t_affine d1, t_affine d2);
t_affine		affine_points(t_fvct2 point1, t_fvct2 point2);
t_affine		affine_def(double a, double b);
double			affine_val_index(t_affine affine, double y);

/*
**	autres
*/
double			hypothenuse(t_fvct2 vct);
int				opacity(int s, int c, double f);

#endif