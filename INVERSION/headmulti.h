#include "fftw3.h"

struct MultiGPU
{
	float *seismogram_obs;
	float *seismogram_syn;
	float *seismogram_rms;
	float *seismogram;
	float *seismograms;
	float *seismogram_tmpobs;
	float *seismogram_tmpsyn;

	float *p_borders_up,   *p_borders_bottom;
	float *p_borders_left, *p_borders_right;

	float *image_vp,*image_rho;
	float *image_sources,*image_receivers;

	float *psdptx,*psdpty;
	float *psdptamp,*psdpttheta;
	
	float *psdvxx,*psdvxy;
	float *psdvxamp,*psdvxtheta;

	float *psdvzx,*psdvzy;
	float *psdvzamp,*psdvztheta;

	float *psdpx,*psdpy;
	float *psdpamp,*psdptheta;

	fftw_complex *vxff,*vzff;
	fftw_complex *ptff,*pff;

	float *rick;
	// vectors for the devices
	int *d_s_ix;
	int *d_r_ix;
	
	float *d_rick;
	float *d_rc;
	float *d_asr;

	int *d_randnum;

	float *d_vp, *d_rho;

	float *d_a_x, *d_a_x_half;
	float *d_a_z, *d_a_z_half;
	float *d_b_x, *d_b_x_half;
	float *d_b_z, *d_b_z_half;

	float *d_vx,*d_vz;
	float *d_p;

	float *d_vx_inv,*d_vz_inv;
	float *d_p_inv;  

	float *d_phi_vx_x,*d_phi_vz_z;
	float *d_phi_p_x,*d_phi_p_z;

	float *d_seismogram;
	float *d_seismogram_rms;

	float *d_p_borders_up,*d_p_borders_bottom;
	float *d_p_borders_left,*d_p_borders_right;

	float *dp_dt;
	float *pt_max;
	int *it_max;

	float *d_image_vp,*d_image_rho;
	float *d_image_sources,*d_image_receivers;
	
	float *d_fs;

	float *d_psdpx,*d_psdpy;
	float *d_psdpamp,*d_psdptheta;

	float *d_psdvxx,*d_psdvxy;
	float *d_psdvxamp,*d_psdvxtheta;

	float *d_psdvzx,*d_psdvzy;
	float *d_psdvzamp,*d_psdvztheta;
	
	float *d_psdptx,*d_psdpty;
	float *d_psdptamp,*d_psdpttheta;
};

#ifndef __HEADMULTI_H__

#define __HEADMULTI_H__ 

#ifdef __cplusplus
extern "C" {
#endif

	void getdevice(int *GPU_N);

	struct Source
	{
		int s_iz,s_ix,r_iz,r_n;
		int *r_ix;
	};

	struct Encode
	{
		int num,offset,r_n;
		int s_iz,r_iz;
		int *s_ix,*r_ix;
	};
	////////////////////////////////////////

	void fdtd_2d_GPU_forward(int ntx, int ntz, int ntp, int nx, int nz,
			int pml, int Lc, float *rc, float dx, float dz,
			float *rick, int itmax, float dt, int iter, int ifreq, int freqintv, int Nf, float *fs, int *randnum,
			int is, struct Encode es[], int NN, struct MultiGPU plan[], int GPU_N, int rnmax, float *rho,
			float *vp,
			float *k_x, float *k_x_half,
			float *k_z, float *k_z_half,
			float *a_x, float *a_x_half,
			float *a_z, float *a_z_half,
			float *b_x, float *b_x_half,
			float *b_z, float *b_z_half, int inv_flag);

	//////////////////////////////////////////////////////////
	void fdtd_2d_GPU_backward(int ntx, int ntz, int ntp, int nx, int nz,
			int pml, int Lc, float *rc, float dx, float dz,
			float *rick, int itmax, float dt, int iter, int ifreq, int freqintv, int Nf, float *fs, int *randnum,
			int is, struct Encode es[], int NN, struct MultiGPU plan[], int GPU_N, int rnmax, float *rho,
			float *vp,
			float *k_x, float *k_x_half,
			float *k_z, float *k_z_half,
			float *a_x, float *a_x_half,
			float *a_z, float *a_z_half,
			float *b_x, float *b_x_half,
			float *b_z, float *b_z_half);

	void decongpu_fre(float *seismogram_syn, float *seismogram_obs, float *seismogram_rms, float *Misfit, int i, 
			int itmax, float dt, float dx, int is, int nx, struct Encode es[], struct Source ss[], int ifreq, int freqintv, float *fs, int *randnum, int pml);

	void gpu_fre(float *seismogram_rms,int tmax,int nx,float dt);

	void laplacegpu_filter(float *pp_pre,float *pp_f,int nxx,
			int nzz,float dx,float dz);

	void variables_malloc(int ntx, int ntz, int ntp, int nx, int nz,
			int pml, int Lc, float dx, float dz, int itmax,
			struct MultiGPU plan[], int GPU_N, int rnmax, int Nf, int NN
			);

	void variables_free(int ntx, int ntz, int ntp, int nx, int nz,
			int pml, int Lc, float dx, float dz, int itmax,
			struct MultiGPU plan[], int GPU_N, int rnmax, int Nf, int NN
			);

	void ricker_fre(float *rick, int is, struct Encode es[], int GPU_N, struct MultiGPU plan[], int ifreq, int freqintv, float *fs,  int *randnum,
			int itmax, float dt, float dx, int nx, int pml, int ricker_flag);

	void seismgobs_fre(float *seismogram_obs, float *seismogram_tmp, int ii, int ifreq, int freqintv, float *fs, int *randnum, int i,
			int itmax, float dt, float dx, int nx, int pml);

	void seismgsyn_fre(float *seismogram_syn, float *seismogram_tmp, int is, struct Encode es[], int ifreq, int freqintv, float *fs, int *randnum, int i,
			int itmax, float dt, float dx, int nx, int pml);

	void seismgrms_fre(float *seismogram_syn, float *seismogram_obs, float *seismogram_rms, int is, struct Encode es[], struct Source ss[], int ifreq, int freqintv, float *fs, int *randnum, int i,
			int itmax, float dt, float dx, int nx, int pml);

	void conjugate_fre(float *seismogram_rms, int i,
			int itmax, float dt, float dx, int nx, int pml);

#ifdef __cplusplus
}
#endif

#endif

void get_acc_model(float *vp, float *rho, int ntp, int ntx, int ntz, int pml);

void ini_model_mine(float *vp, float *rho, int ntp, int ntz, int ntx, int pml, int flag);

void get_ini_model(float *vp, float *rho, int ntp, int ntx, int ntz, int pml, int iterb);
	
void get_absorbing_parameters(
		float *d_x, float *d_x_half, 
		float *d_z, float *d_z_half,
		float *a_x, float *a_x_half,
		float *a_z, float *a_z_half,
		float *b_x, float *b_x_half,
		float *b_z, float *b_z_half,
		float *k_x, float *k_x_half,
		float *k_z, float *k_z_half,
		int ntz, int ntx, int nz, int nx,
		int pml, float dx, float f0, float t0, float dt, float vp_max);

void maximum_vector(float *vector, int n, float *maximum_value);

void get_lame_constants( 
		float *vp_two, float *vp, 
		float * rho, int ntp);

void ini_step(float *dn, int np, float *un0, float max);

void ricker_wave(float *rick, int itmax, float f0, float t0, float dt, int flag);

void update_model(float *vp, float *vp_n,
		float *dn_vp, float *un_vp,
		int ntp, int ntz, int ntx, int pml, float vpmin, float vpmax);

void Preprocess(int nz, int nx, float dx, float dz, float *P, int flag);

void cal_xishu(int Lx,float *rx);

void input_parameters(int *nx,int *nz,int *pml,int *Lc,float *dx,float *dz,float *rectime,float *dt,float *f0, int *Nf,int *freqintv,float *freq0,int *ns,float *sx0,
		float *shotdx,float *shotdep,float *recdep,float *moffsetx,int *itn,int *iterb,int *ifreqb,int *Ns,int *ricker_flag,int *noise_flag
		//,int *GPU_N
		);
