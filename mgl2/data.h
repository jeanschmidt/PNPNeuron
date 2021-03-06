/***************************************************************************
 * data.h is part of Math Graphic Library
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
#ifndef _MGL_DATA_H_
#define _MGL_DATA_H_
/*****************************************************************************/
#include "mgl2/base.h"
#include "mgl2/data_cf.h"
#ifdef __cplusplus
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
//-----------------------------------------------------------------------------
/// Class for working with data array
#if MGL_NO_DATA_A
class mglData
#else
class mglData : public mglDataA
#endif
{
public:

	long nx;		///< number of points in 1st dimensions ('x' dimension)
	long ny;		///< number of points in 2nd dimensions ('y' dimension)
	long nz;		///< number of points in 3d dimensions ('z' dimension)
	mreal *a;		///< data array
	std::string id;	///< column (or slice) names
//	char *id;		///< column (or slice) names
	bool link;		///< use external data (i.e. don't free it)

	/// Initiate by other mglData variable
	inline mglData(const mglData &d)	{	a=0;	mgl_data_set(this,&d);		}
	inline mglData(const mglDataA *d)	{	a=0;	mgl_data_set(this, d);		}
	inline mglData(bool, mglData *d)	// NOTE: Variable d will be deleted!!!
	{	if(d)
		{	nx=d->nx;	ny=d->ny;	nz=d->nz;	a=d->a;	d->a=0;
			id=d->id;	link=d->link;	delete d;	}
		else	{	a=0;	Create(1);	}	}
	/// Initiate by flat array
	inline mglData(int size, const float *d)	{	a=0;	Set(d,size);	}
	inline mglData(int rows, int cols, const float *d)	{	a=0;	Set(d,cols,rows);	}
	inline mglData(int size, const double *d)	{	a=0;	Set(d,size);	}
	inline mglData(int rows, int cols, const double *d)	{	a=0;	Set(d,cols,rows);	}
	inline mglData(const double *d, int size)	{	a=0;	Set(d,size);	}
	inline mglData(const double *d, int rows, int cols)	{	a=0;	Set(d,cols,rows);	}
	/// Read data from file
	inline mglData(const char *fname)			{	a=0;	Read(fname);	}
	/// Allocate the memory for data array and initialize it zero
	inline mglData(long xx=1,long yy=1,long zz=1)	{	a=0;	Create(xx,yy,zz);	}
	/// Delete the array
	virtual ~mglData()	{	if(!link && a)	delete []a;	}
	inline mreal GetVal(long i, long j=0, long k=0)
	{	return mgl_data_get_value(this,i,j,k);}
	inline void SetVal(mreal f, long i, long j=0, long k=0)
	{	mgl_data_set_value(this,f,i,j,k);	}
	/// Get sizes
	inline long GetNx() const	{	return nx;	}
	inline long GetNy() const	{	return ny;	}
	inline long GetNz() const	{	return nz;	}

	/// Link external data array (don't delete it at exit)
	inline void Link(mreal *A, long NX, long NY=1, long NZ=1)
	{	mgl_data_link(this,A,NX,NY,NZ);	}
	inline void Link(mglData *d)	{	Link(d->a,d->nx,d->ny,d->nz);	}
	/// Allocate memory and copy the data from the gsl_vector
	inline void Set(gsl_vector *m)	{	mgl_data_set_vector(this,m);	}
	/// Allocate memory and copy the data from the gsl_matrix
	inline void Set(gsl_matrix *m)	{	mgl_data_set_matrix(this,m);	}

	/// Allocate memory and copy the data from the (float *) array
	inline void Set(const float *A,long NX,long NY=1,long NZ=1)
	{	mgl_data_set_float(this,A,NX,NY,NZ);	}
	/// Allocate memory and copy the data from the (double *) array
	inline void Set(const double *A,long NX,long NY=1,long NZ=1)
	{	mgl_data_set_double(this,A,NX,NY,NZ);	}
	/// Allocate memory and copy the data from the (float **) array
	inline void Set(const float **A,long N1,long N2)
	{	mgl_data_set_float2(this,A,N1,N2);	}
	/// Allocate memory and copy the data from the (double **) array
	inline void Set(const double **A,long N1,long N2)
	{	mgl_data_set_double2(this,A,N1,N2);	}
	/// Allocate memory and copy the data from the (float ***) array
	inline void Set(const float ***A,long N1,long N2,long N3)
	{	mgl_data_set_float3(this,A,N1,N2,N3);	}
	/// Allocate memory and copy the data from the (double ***) array
	inline void Set(const double ***A,long N1,long N2,long N3)
	{	mgl_data_set_double3(this,A,N1,N2,N3);	}
	/// Allocate memory and scanf the data from the string
	inline void Set(const char *str,long NX,long NY=1,long NZ=1)
	{	mgl_data_set_values(this,str,NX,NY,NZ);	}
	/// Import data from abstract type
	inline void Set(HCDT dat)	{	mgl_data_set(this, dat);	}
	inline void Set(const mglData &dat)	{	mgl_data_set(this, &dat);	}
	/// Allocate memory and copy data from std::vector<T>
	inline void Set(const std::vector<int> &d)
	{	if(d.size()<1)	return;
		Create(d.size());	for(long i=0;i<nx;i++)	a[i] = d[i];	}
	inline void Set(const std::vector<float> &d)
	{	if(d.size()<1)	return;
		Create(d.size());	for(long i=0;i<nx;i++)	a[i] = d[i];	}
	inline void Set(const std::vector<double> &d)
	{	if(d.size()<1)	return;
		Create(d.size());	for(long i=0;i<nx;i++)	a[i] = d[i];	}

	/// Create or recreate the array with specified size and fill it by zero
	inline void Create(long mx,long my=1,long mz=1)
	{	mgl_data_create(this,mx,my,mz);	}
	/// Rearange data dimensions
	inline void Rearrange(long mx, long my=0, long mz=0)
	{	mgl_data_rearrange(this,mx,my,mz);	}
	/// Transpose dimensions of the data (generalization of Transpose)
	inline void Transpose(const char *dim="yx")
	{	mgl_data_transpose(this,dim);	}
	/// Extend data dimensions
	inline void Extend(long n1, long n2=0)
	{	mgl_data_extend(this,n1,n2);	}
	/// Reduce size of the data
	inline void Squeeze(long rx,long ry=1,long rz=1,bool smooth=false)
	{	mgl_data_squeeze(this,rx,ry,rz,smooth);	}
	/// Crop the data
	inline void Crop(long n1, long n2,char dir='x')
	{	mgl_data_crop(this,n1,n2,dir);	}
	/// Insert data
	inline void Insert(char dir, long at=0, long num=1)
	{	mgl_data_insert(this,dir,at,num);	}
	/// Delete data
	inline void Delete(char dir, long at=0, long num=1)
	{	mgl_data_delete(this,dir,at,num);	}

	/// Modify the data by specified formula
	inline void Modify(const char *eq,long dim=0)
	{	mgl_data_modify(this, eq, dim);	}
	/// Modify the data by specified formula
	inline void Modify(const char *eq,const mglData &vdat, const mglData &wdat)
	{	mgl_data_modify_vw(this,eq,&vdat,&wdat);	}
	/// Modify the data by specified formula
	inline void Modify(const char *eq,const mglData &vdat)
	{	mgl_data_modify_vw(this,eq,&vdat,0);	}
	/// Modify the data by specified formula assuming x,y,z in range [r1,r2]
	inline void Fill(mglBase *gr, const char *eq, const char *opt="")
	{	mgl_data_fill_eq(gr,this,eq,0,0,opt);	}
	inline void Fill(mglBase *gr, const char *eq, const mglData &vdat, const char *opt="")
	{	mgl_data_fill_eq(gr,this,eq,&vdat,0,opt);	}
	inline void Fill(mglBase *gr, const char *eq, const mglData &vdat, const mglData &wdat,const char *opt="")
	{	mgl_data_fill_eq(gr,this,eq,&vdat,&wdat,opt);	}
	/// Eqidistantly fill the data to range [x1,x2] in direction \a dir
	inline void Fill(mreal x1,mreal x2=NaN,char dir='x')
	{	return mgl_data_fill(this,x1,x2,dir);	}
	/// Set the data by triangulated surface values assuming x,y,z in range [r1,r2]
	inline void Grid(mglBase *gr, const mglData &x, const mglData &y, const mglData &z, const char *opt="")
	{	mgl_data_grid(gr,this,&x,&y,&z,opt);	}
	/// Put value to data element(s)
	inline void Put(mreal val, long i=-1, long j=-1, long k=-1)
	{	mgl_data_put_val(this,val,i,j,k);	}
	/// Put array to data element(s)
	inline void Put(const mglData &dat, long i=-1, long j=-1, long k=-1)
	{	mgl_data_put_dat(this,&dat,i,j,k);	}
	/// Set names for columns (slices)
	inline void SetColumnId(const char *ids)
	{	mgl_data_set_id(this,ids);	}
	/// Make new id
	inline void NewId()	{	id.clear();	}

	/// Read data from tab-separated text file with auto determining size
	inline bool Read(const char *fname)
	{	return mgl_data_read(this,fname); }
	/// Read data from text file with specifeid size
	inline bool Read(const char *fname,long mx,long my=1,long mz=1)
	{	return mgl_data_read_dim(this,fname,mx,my,mz);	}
	/// Save whole data array (for ns=-1) or only ns-th slice to text file
	inline void Save(const char *fname,long ns=-1) const
	{	mgl_data_save(this,fname,ns);	}
	/// Export data array (for ns=-1) or only ns-th slice to PNG file according color scheme
	inline void Export(const char *fname,const char *scheme,mreal v1=0,mreal v2=0,long ns=-1) const
	{	mgl_data_export(this,fname,scheme,v1,v2,ns);	}
	/// Import data array from PNG file according color scheme
	inline void Import(const char *fname,const char *scheme,mreal v1=0,mreal v2=1)
	{	mgl_data_import(this,fname,scheme,v1,v2);	}
	/// Read data from tab-separated text files with auto determining size which filenames are result of sprintf(fname,templ,t) where t=from:step:to
	inline bool ReadRange(const char *templ, double from, double to, double step=1, bool as_slice=false)
	{	return mgl_data_read_range(this,templ,from,to,step,as_slice);	}
	/// Read data from tab-separated text files with auto determining size which filenames are satisfied to template (like "t_*.dat")
	inline bool ReadAll(const char *templ, bool as_slice=false)
	{	return mgl_data_read_all(this, templ, as_slice);	}
	/// Read data from text file with size specified at beginning of the file
	inline bool ReadMat(const char *fname, long dim=2)
	{	return mgl_data_read_mat(this,fname,dim);	}
	/// Read data array from HDF file (parse HDF4 and HDF5 files)
	inline int ReadHDF(const char *fname,const char *data)
	{	return mgl_data_read_hdf(this,fname,data);	}
	/// Save data to HDF file
	inline void SaveHDF(const char *fname,const char *data,bool rewrite=false) const
	{	mgl_data_save_hdf(this,fname,data,rewrite);	}
	/// Put HDF data names into buf as '\t' separated.
	inline int DatasHDF(const char *fname, char *buf, long size) const
	{	return mgl_datas_hdf(fname,buf,size);	}

	/// Get column (or slice) of the data filled by formulas of named columns
	inline mglData Column(const char *eq) const
	{	return mglData(true,mgl_data_column(this,eq));	}
	/// Get momentum (1D-array) of data along direction 'dir'. String looks like "x1" for median in x-direction, "x2" for width in x-dir and so on.
	inline mglData Momentum(char dir, const char *how) const
	{	return mglData(true,mgl_data_momentum(this,dir,how));	}
	/// Get sub-array of the data with given fixed indexes
	inline mglData SubData(long xx,long yy=-1,long zz=-1) const
	{	return mglData(true,mgl_data_subdata(this,xx,yy,zz));	}
	inline mglData SubData(const mglData &xx, const mglData &yy, const mglData &zz) const
	{	return mglData(true,mgl_data_subdata_ext(this,&xx,&yy,&zz));	}
	/// Get trace of the data array
	inline mglData Trace() const
	{	return mglData(true,mgl_data_trace(this));	}
	/// Create n-th points distribution of this data values in range [v1, v2]
	mglData Hist(long n,mreal v1=0,mreal v2=1, long nsub=0) const
	{	return mglData(true,mgl_data_hist(this,n,v1,v2,nsub));	}
	/// Create n-th points distribution of this data values in range [v1, v2] with weight \a w
	mglData Hist(const mglData &w, long n,mreal v1=0,mreal v2=1, long nsub=0) const
	{	return mglData(true,mgl_data_hist_w(this,&w,n,v1,v2,nsub));	}
	/// Get array which is result of summation in given direction or directions
	inline mglData Sum(const char *dir) const
	{	return mglData(true,mgl_data_sum(this,dir));	}
	/// Get array which is result of maximal values in given direction or directions
	inline mglData Max(const char *dir) const
	{	return mglData(true,mgl_data_max_dir(this,dir));	}
	/// Get array which is result of minimal values in given direction or directions
	inline mglData Min(const char *dir) const
	{	return mglData(true,mgl_data_min_dir(this,dir));	}
	/// Get the data which is direct multiplication (like, d[i,j] = this[i]*a[j] and so on)
	inline mglData Combine(const mglData &dat) const
	{	return mglData(true,mgl_data_combine(this,&dat));	}
	/// Resize the data to new size of box [x1,x2]*[y1,y2]*[z1,z2]
	inline mglData Resize(long mx,long my=1,long mz=1, mreal x1=0,mreal x2=1, mreal y1=0,mreal y2=1, mreal z1=0,mreal z2=1) const
	{	return mglData(true,mgl_data_resize_box(this,mx,my,mz,x1,x2,y1,y2,z1,z2));	}
	/// Get array which values is result of interpolation this for coordinates from other arrays
	inline mglData Evaluate(const mglData &idat, bool norm=true) const
	{	return mglData(true,mgl_data_evaluate(this,&idat,0,0,norm));	}
	inline mglData Evaluate(const mglData &idat, const mglData &jdat, bool norm=true) const
	{	return mglData(true,mgl_data_evaluate(this,&idat,&jdat,0,norm));	}
	inline mglData Evaluate(const mglData &idat, const mglData &jdat, const mglData &kdat, bool norm=true) const
	{	return mglData(true,mgl_data_evaluate(this,&idat,&jdat,&kdat,norm));	}

	/// Cumulative summation the data in given direction or directions
	inline void CumSum(const char *dir)	{	mgl_data_cumsum(this,dir);	}
	/// Integrate (cumulative summation) the data in given direction or directions
	inline void Integral(const char *dir)	{	mgl_data_integral(this,dir);	}
	/// Differentiate the data in given direction or directions
	inline void Diff(const char *dir)	{	mgl_data_diff(this,dir);	}
	/// Differentiate the parametrically specified data along direction v1 with v2=const
	inline void Diff(const mglData &v1, const mglData &v2)
	{	mgl_data_diff_par(this,&v1,&v2,0);	}
	/// Differentiate the parametrically specified data along direction v1 with v2,v3=const
	inline void Diff(const mglData &v1, const mglData &v2, const mglData &v3)
	{	mgl_data_diff_par(this,&v1,&v2,&v3);	}
	/// Double-differentiate (like laplace operator) the data in given direction
	inline void Diff2(const char *dir)	{	mgl_data_diff2(this,dir);	}

	/// Swap left and right part of the data in given direction (useful for fourier spectrums)
	inline void Swap(const char *dir)		{	mgl_data_swap(this,dir);	}
	/// Roll data along direction \a dir by \a num slices
	inline void Roll(char dir, long num)	{	mgl_data_roll(this,dir,num);	}
	/// Mirror the data in given direction (useful for fourier spectrums)
	inline void Mirror(const char *dir)		{	mgl_data_mirror(this,dir);	}
	/// Sort rows (or slices) by values of specified column
	inline void Sort(long idx, long idy=-1)	{	mgl_data_sort(this,idx,idy);	}

	/// Set as the data envelop
	inline void Envelop(char dir='x')
	{	mgl_data_envelop(this,dir);	}
	/// Remove phase jump
	inline void Sew(const char *dirs="xyz", mreal da=2*Pi)
	{	mgl_data_sew(this,dirs,da);	}
	/// Smooth the data on specified direction or directions
	inline void Smooth(const char *dirs="xyz",mreal delta=0)
	{	mgl_data_smooth(this,dirs,delta);	}
	/// Normalize the data to range [v1,v2]
	inline void Norm(mreal v1=0,mreal v2=1,bool sym=false,long dim=0)
	{	mgl_data_norm(this,v1,v2,sym,dim);	}
	/// Normalize the data to range [v1,v2] slice by slice
	inline void NormSl(mreal v1=0,mreal v2=1,char dir='z',bool keep_en=true,bool sym=false)
	{	mgl_data_norm_slice(this,v1,v2,dir,keep_en,sym);	}

	/// Hankel transform
	inline void Hankel(const char *dir)	{	mgl_data_hankel(this,dir);	}
	/// Sin-Fourier transform
	inline void SinFFT(const char *dir)	{	mgl_data_sinfft(this,dir);	}
	/// Cos-Fourier transform
	inline void CosFFT(const char *dir)	{	mgl_data_cosfft(this,dir);	}
	/// Fill data by 'x'/'k' samples for Hankel ('h') or Fourier ('f') transform
	inline void FillSample(const char *how)
	{	mgl_data_fill_sample(this,how);	}

	/// Interpolate by qubic splain the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
	inline mreal Spline(mreal x,mreal y=0,mreal z=0) const
	{	return mgl_data_spline(this, x,y,z);	}
	/// Interpolate by qubic splain the data to given point \a x,\a y,\a z which normalized in range [0, 1]
	inline mreal Spline1(mreal x,mreal y=0,mreal z=0) const
	{	return mgl_data_spline(this, x*(nx-1),y*(ny-1),z*(nz-1));	}
	/// Interpolate by linear function the data to given point x=[0...nx-1], y=[0...ny-1], z=[0...nz-1]
	inline mreal Linear(mreal x,mreal y=0,mreal z=0)	const
	{	return mgl_data_linear(this,x,y,z);	}
	/// Interpolate by line the data to given point \a x,\a y,\a z which normalized in range [0, 1]
	inline mreal Linear1(mreal x,mreal y=0,mreal z=0) const
	{	return mgl_data_linear(this,x*(nx-1),y*(ny-1),z*(nz-1));	}

	/// Print information about the data (sizes and momentum) to string
	inline const char *PrintInfo() const	{	return mgl_data_info(this);	}
	/// Print information about the data (sizes and momentum) to FILE (for example, stdout)
	inline void PrintInfo(FILE *fp) const
	{	if(fp)	{	fprintf(fp,"%s",mgl_data_info(this));	fflush(fp);	}	}
	/// Get maximal value of the data
	inline mreal Maximal() const	{	return mgl_data_max(this);	}
	/// Get minimal value of the data
	inline mreal Minimal() const	{	return mgl_data_min(this);	}
	/// Get maximal value of the data and its position
	inline mreal Maximal(long &i,long &j,long &k) const
	{	return mgl_data_max_int(this,&i,&j,&k);	}
	/// Get minimal value of the data and its position
	inline mreal Minimal(long &i,long &j,long &k) const
	{	return mgl_data_min_int(this,&i,&j,&k);	}
	/// Get maximal value of the data and its approximated position
	inline mreal Maximal(mreal &x,mreal &y,mreal &z) const
	{	return mgl_data_max_real(this,&x,&y,&z);	}
	/// Get minimal value of the data and its approximated position
	inline mreal Minimal(mreal &x,mreal &y,mreal &z) const
	{	return mgl_data_min_real(this,&x,&y,&z);	}
	/// Get "energy" and find first (median) and second (width) momentums of data
	inline mreal Momentum(char dir,mreal &m,mreal &w) const
	{	return mgl_data_momentum_val(this,dir,&m,&w,0,0);	}
	/// Get "energy and find 4 momentums of data: median, width, skewness, kurtosis
	inline mreal Momentum(char dir,mreal &m,mreal &w,mreal &s,mreal &k) const
	{	return mgl_data_momentum_val(this,dir,&m,&w,&s,&k);	}
	/// Find position (after specified in i,j,k) of first nonzero value of formula
	inline mreal Find(const char *cond, long &i, long &j, long &k) const
	{	return mgl_data_first(this,cond,&i,&j,&k);	}
	/// Find position (before specified in i,j,k) of last nonzero value of formula
	inline mreal Last(const char *cond, long &i, long &j, long &k) const
	{	return mgl_data_last(this,cond,&i,&j,&k);	}
	/// Find position of first in direction 'dir' nonzero value of formula
	inline long Find(const char *cond, char dir, long i=0, long j=0, long k=0) const
	{	return mgl_data_find(this,cond,dir,i,j,k);	}
	/// Find if any nonzero value of formula
	inline bool FindAny(const char *cond) const
	{	return mgl_data_find_any(this,cond);	}

	/// Copy data from other mglData variable
	inline mglData &operator=(const mglData &d)
	{	if(this!=&d)	Set(d.a,d.nx,d.ny,d.nz);	return *this;	}
	inline mreal operator=(mreal val)
	{	for(long i=0;i<nx*ny*nz;i++)	a[i]=val;	return val;	}
	/// Multiplicate the data by other one for each element
	inline void operator*=(const mglData &d)	{	mgl_data_mul_dat(this,&d);	}
	/// Divide the data by other one for each element
	inline void operator/=(const mglData &d)	{	mgl_data_div_dat(this,&d);	}
	/// Add the other data
	inline void operator+=(const mglData &d)	{	mgl_data_add_dat(this,&d);	}
	/// Substract the other data
	inline void operator-=(const mglData &d)	{	mgl_data_sub_dat(this,&d);	}
	/// Multiplicate each element by the number
	inline void operator*=(mreal d)		{	mgl_data_mul_num(this,d);	}
	/// Divide each element by the number
	inline void operator/=(mreal d)		{	mgl_data_div_num(this,d);	}
	/// Add the number
	inline void operator+=(mreal d)		{	mgl_data_add_num(this,d);	}
	/// Substract the number
	inline void operator-=(mreal d)		{	mgl_data_sub_num(this,d);	}
#ifndef SWIG
	/// Direct access to the data cell
	inline mreal &operator[](long i)	{	return a[i];	}
	// NOTE see 13.10 for operator(), operator[] -- m.b. I should add it ???
#endif
#if MGL_NO_DATA_A
	inline long GetNN() const {	return nx*ny*nz;	}
#else
protected:
#endif
	/// Get the value in given cell of the data without border checking
	inline mreal v(long i,long j=0,long k=0) const
#ifdef DEBUG
	{	if(i<0 || j<0 || k<0 || i>=nx || j>=ny || k>=nz)	printf("Wrong index in mglData");
		return a[i+nx*(j+ny*k)];	}
#else
	{	return a[i+nx*(j+ny*k)];	}
#endif
	inline mreal vthr(long i) const {	return a[i];	}
	// add for speeding up !!!
	inline mreal dvx(long i,long j=0,long k=0) const
	{   register long i0=i+nx*(j+ny*k);
		return i>0? (i<nx-1? (a[i0+1]-a[i0-1])/2:a[i0]-a[i0-1]) : a[i0+1]-a[i0];	}
	inline mreal dvy(long i,long j=0,long k=0) const
	{   register long i0=i+nx*(j+ny*k);
		return j>0? (j<ny-1? (a[i0+nx]-a[i0-nx])/2:a[i0]-a[i0-nx]) : a[i0+nx]-a[i0];}
	inline mreal dvz(long i,long j=0,long k=0) const
	{   register long i0=i+nx*(j+ny*k), n=nx*ny;
		return k>0? (k<nz-1? (a[i0+n]-a[i0-n])/2:a[i0]-a[i0-n]) : a[i0+n]-a[i0];	}
};
//-----------------------------------------------------------------------------
#ifndef SWIG
inline mglData operator*(const mglData &b, const mglData &d)
{	mglData a(b);	a*=d;	return a;	}
inline mglData operator*(mreal b, const mglData &d)
{	mglData a(d);	a*=b;	return a;	}
inline mglData operator*(const mglData &d, mreal b)
{	mglData a(d);	a*=b;	return a;	}
inline mglData operator-(const mglData &b, const mglData &d)
{	mglData a(b);	a-=d;	return a;	}
inline mglData operator-(mreal b, const mglData &d)
{	mglData a(d);	a-=b;	return a;	}
inline mglData operator-(const mglData &d, mreal b)
{	mglData a(d);	a-=b;	return a;	}
inline mglData operator+(const mglData &b, const mglData &d)
{	mglData a(b);	a+=d;	return a;	}
inline mglData operator+(mreal b, const mglData &d)
{	mglData a(d);	a+=b;	return a;	}
inline mglData operator+(const mglData &d, mreal b)
{	mglData a(d);	a+=b;	return a;	}
inline mglData operator/(const mglData &b, const mglData &d)
{	mglData a(b);	a/=d;	return a;	}
inline mglData operator/(const mglData &d, mreal b)
{	mglData a(d);	a/=b;	return a;	}
inline bool operator==(const mglData &b, const mglData &d)
{	if(b.nx!=d.nx || b.ny!=d.ny || b.ny!=d.ny)	return false;
	return !memcmp(b.a,d.a,b.nx*b.ny*b.nz*sizeof(mreal));	}
#endif
//-----------------------------------------------------------------------------
#ifndef SWIG
mreal mglLinear(const mreal *a, long nx, long ny, long nz, mreal x, mreal y, mreal z);
mreal mglSpline3(const mreal *a, long nx, long ny, long nz, mreal x, mreal y, mreal z,mreal *dx=0, mreal *dy=0, mreal *dz=0);
#endif
//-----------------------------------------------------------------------------
/// Integral data transformation (like Fourier 'f' or 'i', Hankel 'h' or None 'n') for amplitude and phase
inline mglData mglTransformA(const mglDataA &am, const mglDataA &ph, const char *tr)
{	return mgl_transform_a(&am,&ph,tr);	}
/// Integral data transformation (like Fourier 'f' or 'i', Hankel 'h' or None 'n') for real and imaginary parts
inline mglData mglTransform(const mglDataA &re, const mglDataA &im, const char *tr)
{	return mgl_transform(&re,&im,tr);	}
/// Apply Fourier transform for the data and save result into it
inline void mglFourier(mglData &re, mglData &im, const char *dir)
{	mgl_data_fourier(&re,&im,dir);	}
/// Short time Fourier analysis for real and imaginary parts. Output is amplitude of partial Fourier (result will have size {dn, floor(nx/dn), ny} for dir='x'
inline mglData mglSTFA(const mglDataA &re, const mglDataA &im, long dn, char dir='x')
{	return mglData(true, mgl_data_stfa(&re,&im,dn,dir));	}
//-----------------------------------------------------------------------------
/// Saves result of PDE solving (|u|^2) for "Hamiltonian" \a ham with initial conditions \a ini
inline mglData mglPDE(mglBase *gr, const char *ham, const mglData &ini_re, const mglData &ini_im, mreal dz=0.1, mreal k0=100,const char *opt="")
{	return mglData(true, mgl_pde_solve(gr,ham, &ini_re, &ini_im, dz, k0,opt));	}
/// Saves result of PDE solving for "Hamiltonian" \a ham with initial conditions \a ini along a curve \a ray (must have nx>=7 - x,y,z,px,py,pz,tau or nx=5 - x,y,px,py,tau)
inline mglData mglQO2d(const char *ham, const mglData &ini_re, const mglData &ini_im, const mglData &ray, mreal r=1, mreal k0=100)
{	return mglData(true, mgl_qo2d_solve(ham, &ini_re, &ini_im, &ray, r, k0, 0, 0));	}
inline mglData mglQO2d(const char *ham, const mglData &ini_re, const mglData &ini_im, const mglData &ray, mglData &xx, mglData &yy, mreal r=1, mreal k0=100)
{	return mglData(true, mgl_qo2d_solve(ham, &ini_re, &ini_im, &ray, r, k0, &xx, &yy));	}
/// Prepares ray data for mglQO_PDE with starting point \a r0, \a p0
inline mglData mglRay(const char *ham, mglPoint r0, mglPoint p0, mreal dt=0.1, mreal tmax=10)
{	return mglData(true, mgl_ray_trace(ham, r0.x, r0.y, r0.z, p0.x, p0.y, p0.z, dt, tmax));	}
/// Calculate Jacobian determinant for D{x(u,v), y(u,v)} = dx/du*dy/dv-dx/dv*dy/du
inline mglData mglJacobian(const mglData &x, const mglData &y)
{	return mglData(true, mgl_jacobian_2d(&x, &y));	}
/// Calculate Jacobian determinant for D{x(u,v,w), y(u,v,w), z(u,v,w)}
inline mglData mglJacobian(const mglData &x, const mglData &y, const mglData &z)
{	return mglData(true, mgl_jacobian_3d(&x, &y, &z));	}
/// Do something like Delone triangulation
inline mglData mglTriangulation(const mglData &x, const mglData &y, const mglData &z)
{	return mglData(true,mgl_triangulation_3d(&x,&y,&z));	}
inline mglData mglTriangulation(const mglData &x, const mglData &y)
{	return mglData(true,mgl_triangulation_2d(&x,&y));	}
//-----------------------------------------------------------------------------
#endif
#endif
