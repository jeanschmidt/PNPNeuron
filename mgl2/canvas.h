/***************************************************************************
 * canvas.h is part of Math Graphic Library
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
#ifndef MGL_CANVAS_H
#define MGL_CANVAS_H
#include "mgl2/base.h"
//-----------------------------------------------------------------------------
#ifndef MGL_STACK_ENTRY
#define MGL_STACK_ENTRY	10
#endif
//-----------------------------------------------------------------------------
#if MGL_HAVE_GIF
#include <gif_lib.h>
#else
struct GifFileType;
#endif
//-----------------------------------------------------------------------------
/// Structure for transformation matrix
struct mglMatrix
{
	mreal b[9];
	mreal x,y,z,pf;
	mglMatrix()	{	clear();	}
	inline void clear()	{	x=y=z=0;	memset(b,0,9*sizeof(mreal));	b[0]=b[4]=b[8]=1;	}
	inline mglMatrix &operator=(const mglMatrix &a)
	{	x=a.x;	y=a.y;	z=a.z;	pf=a.pf;	memcpy(b,a.b,9*sizeof(mreal));	return *this;	}
};
//-----------------------------------------------------------------------------
/// Structure for drawing axis and ticks
struct mglAxis
{
	mglAxis()	{	dv=ds=d=v0=v1=v2=o=sh=0;	ns=f=ch=*t=0;	}
	mglAxis(const mglAxis &aa)
	{	dv=aa.dv;	ds=aa.ds;	d=aa.d;		dir=aa.dir;	sh=aa.sh;
		v0=aa.v0;	v1=aa.v1;	v2=aa.v2;	o=aa.o;
		a = aa.a;	b = aa.b;	org=aa.org;	txt=aa.txt;
		ns=aa.ns;	f=aa.f;		ch=aa.ch;	wcscpy(t,aa.t);	}
	inline void AddLabel(const wchar_t *lbl, mreal v)
	{	txt.push_back(mglText(lbl,"",v));	}
	inline void AddLabel(const std::wstring &lbl, mreal v)
	{	txt.push_back(mglText(lbl,v));	}

	mreal dv,ds;		///< Actual step for ticks and subticks.
	mreal d;			///< Step for axis ticks (if positive) or its number (if negative).
	int ns;			///< Number of axis subticks.
	wchar_t t[256];	///< Tick template (set NULL to use default one ("%.2g" in simplest case))
	mglPoint dir;	///< Axis direction
	mglPoint a,b;	///< Directions of over axis
	mglPoint org;
	mreal v0;		///< Center of axis cross section
	mreal v1;		///< Minimal axis range.
	mreal v2;		///< Maximal axis range.
	mreal o;			///< Point of starting ticks numbering (if NAN then Org is used).
	int f;			///< Flag 0x1 - time, 0x2 - manual, 0x4 - fixed dv
	std::vector<mglText> txt;	///< Axis labels
	char ch;			///< Character of axis (like 'x','y','z','c')
	mreal sh;		///< Extra shift of ticks and axis labels
};
//-----------------------------------------------------------------------------
/// Structure for light source
struct mglLight
{
	mglLight()	{	n=false;	a=b=0;	}
	bool n;			///< Availability of light sources
	mglPoint d;		///< Direction of light sources
	mglPoint r;		///< Position of light sources (NAN for infinity)
	mglPoint q;		///< Actual position of light sources (filled by LightScale() function)
	mglPoint p;		///< Actual direction of light sources (filled by LightScale() function)
	mreal a;		///< Aperture of light sources
	mreal b;		///< Brightness of light sources
	mglColor c;		///< Color of light sources
};
//-----------------------------------------------------------------------------
class mglCanvas;
/// Structure for light source
struct mglDrawReg
{
	int x1,x2,y1,y2;
	void set(mglCanvas *gr, int nx, int ny, int m);
};
//-----------------------------------------------------------------------------
/// Structure contains everything for drawing
struct mglDrawDat
{
	std::vector<mglGroup> Grp;	///< List of groups with names -- need for export
	std::vector<mglPnt> Pnt;	///< Internal points
	std::vector<mglPrim> Prm;	///< Primitives (lines, triangles and so on) -- need for export
	std::vector<mglPrim> Sub;	///< InPlot regions {n1=x1,n2=x2,n3=y1,n4=y2,id}
	std::vector<mglText> Ptx;	///< Text labels for mglPrim
	std::vector<mglText> Leg;	///< Text labels for legend
	std::vector<mglTexture> Txt;///< Pointer to textures
};
//-----------------------------------------------------------------------------
/// Class contains all functionality for creating different mathematical plots
class mglCanvas : public mglBase
{
friend struct mglPrim;
public:
using mglBase::Light;

	mglCanvas(int w=800, int h=600);
	virtual ~mglCanvas();

	/// Set default parameter for plotting
	void DefaultPlotParam();

	/// Set angle of view indepently from mglCanvas::Rotate()
	virtual void View(mreal tetx,mreal tetz,mreal tety=0);
	/// Zoom in or zoom out (if Zoom(0, 0, 1, 1)) a part of picture
	virtual void Zoom(mreal x1, mreal y1, mreal x2, mreal y2);
	/// Restore image after View() and Zoom()
	virtual void Restore()	{	Bp.clear();	Bp.pf=0;	}

	/// Clear transformation matrix.
	inline void Identity(bool rel=false)	{	InPlot(0,1,0,1,rel);	}
	/// Push transformation matrix into stack
	inline void Push()	{	stack.push_back(B);	}
	/// Set PlotFactor
	inline void SetPlotFactor(mreal val)
	{	if(val<=0)	{B.pf=1.55;	set(MGL_AUTO_FACTOR);}	else {B.pf=val;	clr(MGL_AUTO_FACTOR);}	}
	/// Get PlotFactor
	inline mreal GetPlotFactor()	{	return B.pf;	}
	/// Pop transformation matrix from stack
	inline void Pop()	{	B = stack.back(); stack.pop_back();	}
	/// Clear up the frame
	virtual void Clf(mglColor back=WC);

	/// Put further plotting in cell of stick rotated on angles tet, phi
	void StickPlot(int num, int i, mreal tet, mreal phi);
	/// Put further plotting in some region of whole frame surface.
	void InPlot(mreal x1,mreal x2,mreal y1,mreal y2,bool rel=true);
	void InPlot(mreal x1,mreal x2,mreal y1,mreal y2, const char *style);
	/// Add title for current subplot/inplot
	void Title(const char *title,const char *stl="#",mreal size=-2);
	void Title(const wchar_t *title,const char *stl="#",mreal size=-2);
	/// Set aspect ratio for further plotting.
	void Aspect(mreal Ax,mreal Ay,mreal Az);
	/// Rotate a further plotting.
	void Rotate(mreal TetX,mreal TetZ,mreal TetY=0);
	/// Rotate a further plotting around vector {x,y,z}.
	void RotateN(mreal Tet,mreal x,mreal y,mreal z);
	/// Set perspective (in range [0,1)) for plot. Set to zero for switching off.
	void Perspective(mreal a)	{	Bp.pf = fabs(a);	}

	/// Set size of frame in pixels. Normally this function is called internaly.
	virtual void SetSize(int w,int h);
	/// Get ratio (mreal width)/(mreal height).
	mreal GetRatio() const;
	/// Get bitmap data prepared for saving to file
	virtual unsigned char **GetRGBLines(long &w, long &h, unsigned char *&f, bool alpha=false);
	/// Get RGB bitmap of current state image.
	virtual const unsigned char *GetBits();
	/// Get RGBA bitmap of current state image.
	const unsigned char *GetRGBA()	{	Finish();	return G4;	}
	/// Get width of the image
	int GetWidth() const	{	return Width;	}
	/// Get height of the image
	int GetHeight() const	{	return Height;	}
	/// Combine plots from 2 canvases. Result will be saved into this.
	void Combine(const mglCanvas *gr);
	/// Send graphical information to node id using MPI
	void MPI_Send(int id);
	/// Receive graphical information from node id using MPI
	void MPI_Recv(int id);
	inline mreal GetDelay() const	{	return Delay;	}
	inline void SetDelay(mreal d)	{	Delay=d;	}

	/// Calculate 3D coordinate {x,y,z} for screen point {xs,ys}
	mglPoint CalcXYZ(int xs, int ys) const;
	/// Calculate screen point {xs,ys} for 3D coordinate {x,y,z}
	void CalcScr(mglPoint p, int *xs, int *ys) const;
	mglPoint CalcScr(mglPoint p) const;
	/// Set object/subplot id
	inline void SetObjId(long id)	{	ObjId = id;	}
	/// Get object id
	inline int GetObjId(long x,long y) const	{	return OI[x+Width*y];	}
	/// Get subplot id
	int GetSplId(long x,long y) const;

	/// Create new frame.
	virtual int NewFrame();
	/// Finish frame drawing
	virtual void EndFrame();
	/// Get the number of created frames
	inline int GetNumFrame() const	{	return CurFrameId;	}
	/// Reset frames counter (start it from zero)
	inline void ResetFrames()	{	CurFrameId=0;	DrwDat.clear();	}

	/// Start write frames to cinema using GIF format
	void StartGIF(const char *fname, int ms=100);
	/// Stop writing cinema using GIF format
	void CloseGIF();
	/// Finish plotting. Normally this function is called internaly.
	virtual void Finish(bool fast=true);
	/// Export points and primitives in file using MGLD format
	bool ExportMGLD(const char *fname, const char *descr=0);
	/// Import points and primitives from file using MGLD format
	bool ImportMGLD(const char *fname, bool add=false);

	/// Set the transparency type
	inline void SetTranspType(int val)
	{	Flag=(Flag&(~3)) + (val&3);	SetAxisStl(val==2?"w-":"k-");	}
	/// Set the fog distance or switch it off (if d=0).
	virtual void Fog(mreal d, mreal dz=0.25);
	/// Switch on/off the specified light source.
	virtual void Light(int n, bool enable);
	/// Add a light source.
	virtual void AddLight(int n,mglPoint r, mglPoint d, char c='w', mreal bright=0.5, mreal ap=0);
	inline void AddLight(int n,mglPoint d, char c='w', mreal bright=0.5, mreal ap=0)
	{	AddLight(n,mglPoint(NAN),d,c,bright,ap);	}

	/// Set ticks position and text (\n separated). Use n=0 to disable this feature.
	void SetTicksVal(char dir, const char *lbl, bool add=false);
	void SetTicksVal(char dir, HCDT v, const char *lbl, bool add=false);
	void SetTicksVal(char dir, HCDT v, const char **lbl, bool add=false);
	void SetTicksVal(char dir, const wchar_t *lbl, bool add=false);
	void SetTicksVal(char dir, HCDT v, const wchar_t *lbl, bool add=false);
	void SetTicksVal(char dir, HCDT v, const wchar_t **lbl, bool add=false);
	/// Set templates for ticks
	void SetTickTempl(char dir, const wchar_t *t);
	void SetTickTempl(char dir, const char *t);
	/// Set time templates for ticks
	void SetTickTime(char dir, mreal d=0, const char *t="");
	/// Set the ticks parameters
	void SetTicks(char dir, mreal d=0, int ns=0, mreal org=NAN);
	/// Auto adjust ticks
	void AdjustTicks(const char *dir="xyzc", bool force=false);
	/// Tune ticks
	inline void SetTuneTicks(int tune, mreal pos=1.15)
	{	TuneTicks = tune;	FactorPos = pos;	};
	/// Set ticks styles
	void SetAxisStl(const char *stl="k", const char *tck=0, const char *sub=0);
	/// Set ticks length
	void SetTickLen(mreal tlen, mreal stt=1.);

	/// Draws bounding box outside the plotting volume with color \a c.
	void Box(const char *col=0, bool ticks=true);
	/// Draw axises with ticks in directions determined by string parameter \a dir.
	void Axis(const char *dir="xyzt", const char *stl="");
	/// Draw grid lines perpendicular to direction determined by string parameter \a dir.
	void Grid(const char *dir="xyzt",const char *pen="B-");
	/// Print the label \a text for axis \a dir.
	void Label(char dir, const char *text, mreal pos=0, mreal shift=0);
	void Labelw(char dir, const wchar_t *text, mreal pos=0, mreal shift=0);

	/// Draw colorbar at edge of axis
	void Colorbar(const char *sch=0);
	void Colorbar(const char *sch, mreal x, mreal y, mreal w, mreal h);
	/// Draw colorbar at edge of axis for manual colors
	void Colorbar(HCDT v, const char *sch=0);
	void Colorbar(HCDT v, const char *sch, mreal x, mreal y, mreal w, mreal h);

	/// Draw legend of accumulated strings at position (x, y) by \a font with \a size
	inline void Legend(mreal x, mreal y, const char *font="#", mreal size=-0.8, mreal llen=0.1)
	{	Legend(Leg,x,y,font,size,llen);	}
	/// Draw legend of accumulated strings by \a font with \a size
	inline void Legend(int where=0x3, const char *font="#", mreal size=-0.8, mreal llen=0.1)
	{	Legend(Leg,(where&1)?1:0,(where&2)?1:0,font,size,llen);	}
	/// Draw legend of accumulated strings by \a font with \a size
	inline void Legend(const std::vector<mglText> &leg, int where=3, const char *font="#", mreal size=-0.8, mreal llen=0)
	{	Legend(leg,(where&1)?1:0,(where&2)?1:0,font,size,llen);	}
	/// Draw legend strings \a text at position (x, y) by \a font with \a size
	void Legend(const std::vector<mglText> &leg, mreal x, mreal y, const char *font="#", mreal size=-0.8, mreal llen=0);
	/// Number of marks in legend sample
	inline void SetLegendMarks(int num=1)	{	LegendMarks = num>0?num:1;	};

	void StartAutoGroup (const char *);
	void EndGroup();
	/// Retur color for primitive depending lighting
	mglColor GetColor(const mglPrim &p);
	/// Set extra shift for tick and axis labels
	inline void SetTickShift(mglPoint p)
	{	ax.sh = p.x;	ay.sh = p.y;	az.sh = p.z;	ac.sh = p.c;	}

protected:
	mreal Delay;		///< Delay for animation in seconds
	// NOTE: Z should be float for reducing space and for compatibility reasons
	float *Z;			///< Height for given level in Z-direction
	unsigned char *C;	///< Picture for given level in Z-direction
	int *OI;			///< ObjId arrays
	unsigned char *G4;	///< Final picture in RGBA format. Prepared in Finish().
	unsigned char *G;	///< Final picture in RGB format. Prepared in Finish().
	std::vector<mglDrawDat> DrwDat;	///< Set of ALL drawing data for each frames
#if MGL_HAVE_PTHREAD
	pthread_mutex_t mutexSub, mutexPrm, mutexPtx, mutexStk, mutexGrp;
#endif

	int LegendMarks;	///< Number of marks in the Legend
	unsigned char BDef[4];	///< Background color
	mglAxis ax,ay,az,ac;///< Axis parameters

	int TuneTicks;		///< Draw tuned ticks with extracted common component
	mreal FactorPos;	///< Position of axis ticks factor (0 at Min, 1 at Max, 1.1 is default)
	mreal TickLen;		///< Length of tiks (subticks length is sqrt(1+st_t)=1.41... times smaller)
	char AxisStl[32];	///< Axis line style. Default is "k"
	char TickStl[32];	///< Tick line style. Default is "k"
	char SubTStl[32];	///< Subtick line style. Default is "k"
	mreal st_t;			///< Subtick-to-tick ratio (ls=lt/sqrt(1+st_t)). Default is 1.

	int CurFrameId;		///< Number of automaticle created frames
	int Width;			///< Width of the image
	int Height;			///< Height of the image
	int Depth;			///< Depth of the image
	mglMatrix Bp;		///< Transformation matrix for View() and Zoom()
	mglMatrix B;		///< Transformation matrix
	mglMatrix B1;		///< Transformation matrix for colorbar
	mreal inW, inH;		///< Width and height of last InPlot
	mglLight light[10];	///< Light sources
	mreal FogDist;		///< Inverse fog distance (fog ~ exp(-FogDist*Z))
	mreal FogDz;		///< Relative shift of fog

	/// Auto adjust ticks
	void AdjustTicks(mglAxis &aa, bool ff);
	/// Prepare labels for ticks
	void LabelTicks(mglAxis &aa);
	/// Draw axis
	void DrawAxis(mglAxis &aa, bool text=true, char arr=0,const char *stl="");
	/// Draw axis grid lines
	void DrawGrid(mglAxis &aa);
	/// Update axis ranges
	inline void UpdateAxis()
	{	ax.v0=Org.x;	ay.v0=Org.y;	az.v0=Org.z;	ac.v0=Org.c;
		ax.v1=Min.x;	ay.v1=Min.y;	az.v1=Min.z;	ac.v1=Min.c;
		ax.v2=Max.x;	ay.v2=Max.y;	az.v2=Max.z;	ac.v2=Max.c;	}

	/// Clear ZBuffer only
	void ClfZB();
	/// Scale coordinates and cut off some points
	bool ScalePoint(mglPoint &p, mglPoint &n, bool use_nan=true) const;
	void LightScale();	///< Additionally scale positions of light sources

	/// Push drawing data (for frames only). NOTE: can be VERY large
	long PushDrwDat();
	/// Get drawing data for i-th frame.
	void GetDrwDat(long i);

	mreal GetOrgX(char dir) const;	///< Get Org.x (parse NAN value)
	mreal GetOrgY(char dir) const;	///< Get Org.y (parse NAN value)
	mreal GetOrgZ(char dir) const;	///< Get Org.z (parse NAN value)

	void mark_plot(long p, char type, mreal size=1);	// position in pntC
	void arrow_plot(long p1, long p2, char st);			// position in pntC
	void line_plot(long p1, long p2);	// position in pntC
	void trig_plot(long p1, long p2, long p3);			// position in pntN
	void quad_plot(long p1, long p2, long p3, long p4);	// position in pntN
	void Glyph(mreal x, mreal y, mreal f, int style, long icode, mreal col);
	mreal text_plot(long p,const wchar_t *text,const char *fnt,mreal size=-1,mreal sh=0,mreal  col=-('k'), bool rot=true);	// position in pntN

	void add_prim(mglPrim &a);	///< add primitive to list
	void mark_draw(long p, char type, mreal size, mglDrawReg *d);
	void arrow_draw(long p1, long p2, char st, mreal size, mglDrawReg *d);
	virtual void line_draw(long p1, long p2, mglDrawReg *d);
	virtual void trig_draw(long p1, long p2, long p3, bool anorm, mglDrawReg *d);
	virtual void quad_draw(long p1, long p2, long p3, long p4, mglDrawReg *d);
	virtual void pnt_draw(long p, mglDrawReg *d);
	void glyph_draw(const mglPrim *P, mglDrawReg *d);
	bool IsSame(const mglPrim &pr,mreal wp,mglColor cp,int st);

	// functions for multi-threading
	void PreparePrim(bool fast);
	void pxl_combine(unsigned long id, unsigned long n, const void *);
	void pxl_memcpy(unsigned long id, unsigned long n, const void *);
	void pxl_backgr(unsigned long id, unsigned long n, const void *);
	void pxl_primdr(unsigned long id, unsigned long n, const void *);
	void pxl_transform(unsigned long id, unsigned long n, const void *);
	void pxl_setz(unsigned long id, unsigned long n, const void *);
	void pxl_setz_adv(unsigned long id, unsigned long n, const void *);
	void pxl_other(unsigned long id, unsigned long n, const void *p);
	/// Put drawing from other mglCanvas (for multithreading, like subplots)
	void PutDrawReg(mglDrawReg *d, const mglCanvas *gr);
	
private:
//	mreal _tetx,_tety,_tetz;		// extra angles
	std::vector<mglMatrix> stack;	///< stack for transformation matrixes
	int dr_nx1, dr_nx2, dr_ny1, dr_ny2;	// Allowed drawing region
	GifFileType *gif;
	mreal fscl,ftet;	///< last scale and rotation for glyphs

	/// Draw generic colorbar
	void colorbar(HCDT v, const mreal *s, int where, mreal x, mreal y, mreal w, mreal h);
	/// Draw labels for ticks
	void DrawLabels(mglAxis &aa);
	/// Draw tick
	void tick_draw(mglPoint o, mglPoint d1, mglPoint d2, int f, const char *stl);
	/// Plot point \a p with color \a c
	void pnt_plot(long x,long y,mreal z,const unsigned char c[4]);
	mreal FindOptOrg(char dir, int ind) const;
	/// Transform mreal color and alpha to bits format
	unsigned char* col2int(const mglPnt &p, unsigned char *r);
	/// Combine colors in 2 plane.
	void combine(unsigned char *c1,unsigned char *c2);
	/// Fast drawing of line between 2 points
	void fast_draw(long p1, long p2, mglDrawReg *d);

	/// Additionally scale points \a p for positioning in image
	void PostScale(mglPoint &p) const;
	/// Scale points \a p for projection to the face number \a nface in image
	long ProjScale(int nface, long p);
	inline void PostScale(mglPoint *p,long n) const	{	for(long i=0;i<n;i++)	PostScale(p[i]);	}

	// functions for glyph drawing
	void glyph_fill(const mglPnt &p, mreal f, int nt, const short *trig, mglDrawReg *d);
	void glyph_wire(const mglPnt &p, mreal f, int nl, const short *line, mglDrawReg *d);
	void glyph_line(const mglPnt &p, mreal f, bool solid, mglDrawReg *d);
};
//-----------------------------------------------------------------------------
struct mglThreadG
{
	mglCanvas *gr;		// grapher
	void (mglCanvas::*f)(unsigned long i, unsigned long n, const void *);
	unsigned id;		// thread id
	unsigned long n;	// total number of iteration
	const void *p;		// external parameter
};
/// Start several thread for the task
void mglStartThread(void (mglCanvas::*func)(unsigned long i, unsigned long n), mglCanvas *gr, unsigned long n);
//-----------------------------------------------------------------------------
#endif
