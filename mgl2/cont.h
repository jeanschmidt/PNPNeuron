/***************************************************************************
 * cont.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _MGL_CONT_H_
#define _MGL_CONT_H_
#include "mgl2/base.h"
/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
void mgl_textw_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, const wchar_t *text, const char *font, const char *opt);
void mgl_textw_xy(HMGL gr, HCDT x, HCDT y, const wchar_t *text, const char *font, const char *opt);
void mgl_textw_y(HMGL gr, HCDT y, const wchar_t *text, const char *font, const char *opt);
void mgl_text_xyz(HMGL gr, HCDT x, HCDT y, HCDT z, const char *text, const char *font, const char *opt);
void mgl_text_xy(HMGL gr, HCDT x, HCDT y, const char *text, const char *font, const char *opt);
void mgl_text_y(HMGL gr, HCDT y, const char *text, const char *font, const char *opt);

void mgl_cont_gen(HMGL gr, mreal val, HCDT a, HCDT x, HCDT y, HCDT z, const char *stl);
void mgl_contf_gen(HMGL gr, mreal v1, mreal v2, HCDT a, HCDT x, HCDT y, HCDT z, const char *stl);

void mgl_cont_xy_val(HMGL graph, HCDT v, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);
void mgl_cont_val(HMGL graph, HCDT v, HCDT z, const char *sch, const char *opt);
void mgl_cont_xy(HMGL graph, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);
void mgl_cont(HMGL graph, HCDT z, const char *sch, const char *opt);

void mgl_contf_xy_val(HMGL graph, HCDT v, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);
void mgl_contf_val(HMGL graph, HCDT v, HCDT z, const char *sch, const char *opt);
void mgl_contf_xy(HMGL graph, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);
void mgl_contf(HMGL graph, HCDT z, const char *sch, const char *opt);

void mgl_contd_xy_val(HMGL graph, HCDT v, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);
void mgl_contd_val(HMGL graph, HCDT v, HCDT z, const char *sch, const char *opt);
void mgl_contd_xy(HMGL graph, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);
void mgl_contd(HMGL graph, HCDT z, const char *sch, const char *opt);

void mgl_contv_xy_val(HMGL graph, HCDT v, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);
void mgl_contv_val(HMGL graph, HCDT v, HCDT z, const char *sch, const char *opt);
void mgl_contv_xy(HMGL graph, HCDT x, HCDT y, HCDT z, const char *sch, const char *opt);
void mgl_contv(HMGL graph, HCDT z, const char *sch, const char *opt);

void mgl_axial_xy_val(HMGL graph, HCDT v, HCDT x, HCDT y, HCDT a, const char *sch, const char *opt);
void mgl_axial_val(HMGL graph, HCDT v, HCDT a, const char *sch, const char *opt);
void mgl_axial_xy(HMGL graph, HCDT x, HCDT y, HCDT a, const char *sch, const char *opt);
void mgl_axial(HMGL graph, HCDT a, const char *sch, const char *opt);

void mgl_torus(HMGL graph, HCDT r, HCDT z, const char *col, const char *opt);

void mgl_grid3_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT a, const char *stl, mreal sVal, const char *opt);
void mgl_grid3(HMGL graph, HCDT a, const char *stl, mreal sVal, const char *opt);

void mgl_dens3_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT a, const char *stl, mreal sVal, const char *opt);
void mgl_dens3(HMGL graph, HCDT a, const char *stl, mreal sVal, const char *opt);

void mgl_cont3_xyz_val(HMGL graph, HCDT v, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, mreal sVal, const char *opt);
void mgl_cont3_val(HMGL graph, HCDT v, HCDT a, const char *sch, mreal sVal, const char *opt);
void mgl_cont3_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, mreal sVal, const char *opt);
void mgl_cont3(HMGL graph, HCDT a, const char *sch, mreal sVal, const char *opt);

void mgl_contf3_xyz_val(HMGL graph, HCDT v, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, mreal sVal, const char *opt);
void mgl_contf3_val(HMGL graph, HCDT v, HCDT a, const char *sch, mreal sVal, const char *opt);
void mgl_contf3_xyz(HMGL graph, HCDT x, HCDT y, HCDT z, HCDT a, const char *sch, mreal sVal, const char *opt);
void mgl_contf3(HMGL graph, HCDT a, const char *sch, mreal sVal, const char *opt);

/*****************************************************************************/
void mgl_text_xyz_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, uintptr_t *z,const char *text,const char *font, const char *opt,int,int l,int n);
void mgl_text_xy_(uintptr_t *gr, uintptr_t *x, uintptr_t *y, const char *text, const char *font, const char *opt,int, int l,int n);
void mgl_text_y_(uintptr_t *gr, uintptr_t *y, const char *text, const char *font, const char *opt,int, int l,int n);

void mgl_cont_xy_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_cont_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_cont_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_cont_(uintptr_t *graph, uintptr_t *z, const char *sch, const char *opt,int,int);

void mgl_contf_xy_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_contf_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_contf_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_contf_(uintptr_t *graph, uintptr_t *z, const char *sch, const char *opt,int,int);

void mgl_contd_xy_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_contd_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_contd_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_contd_(uintptr_t *graph, uintptr_t *z, const char *sch, const char *opt,int,int);

void mgl_contv_xy_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_contv_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_contv_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, const char *sch, const char *opt,int,int);
void mgl_contv_(uintptr_t *graph, uintptr_t *z, const char *sch, const char *opt,int,int);

void mgl_axial_xy_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch, const char *opt,int,int);
void mgl_axial_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *sch, const char *opt,int,int);
void mgl_axial_xy_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *a, const char *sch, const char *opt,int,int);
void mgl_axial_(uintptr_t *graph, uintptr_t *a, const char *sch, const char *opt,int,int);

void mgl_torus_(uintptr_t *graph, uintptr_t *r, uintptr_t *z, const char *pen, const char *opt,int,int);

void mgl_grid3_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch, mreal *sVal, const char *opt,int,int);
void mgl_grid3_(uintptr_t *graph, uintptr_t *a, const char *sch, mreal *sVal, const char *opt,int,int);

void mgl_dens3_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch, mreal *sVal, const char *opt,int,int);
void mgl_dens3_(uintptr_t *graph, uintptr_t *a, const char *sch, mreal *sVal, const char *opt,int,int);

void mgl_cont3_xyz_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch, mreal *sVal, const char *opt,int,int);
void mgl_cont3_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *sch, mreal *sVal, const char *opt,int,int);
void mgl_cont3_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch, mreal *sVal, const char *opt,int,int);
void mgl_cont3_(uintptr_t *graph, uintptr_t *a, const char *sch, mreal *sVal, const char *opt,int,int);

void mgl_contf3_xyz_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch, mreal *sVal, const char *opt,int,int);
void mgl_contf3_val_(uintptr_t *graph, uintptr_t *v, uintptr_t *a, const char *sch, mreal *sVal, const char *opt,int,int);
void mgl_contf3_xyz_(uintptr_t *graph, uintptr_t *x, uintptr_t *y, uintptr_t *z, uintptr_t *a, const char *sch, mreal *sVal, const char *opt,int,int);
void mgl_contf3_(uintptr_t *graph, uintptr_t *a, const char *sch, mreal *sVal, const char *opt,int,int);

/*****************************************************************************/
#ifdef __cplusplus
}
#endif
/*****************************************************************************/
#endif
