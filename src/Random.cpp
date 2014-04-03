/*
 * Random.cpp
 *
 *  Created on: Feb 22, 2013
 *      Author: Thibaut Metivet
 */

#include "Random.hpp"
#include <ctime>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

namespace LQCDA {

#ifdef HAVE_SSE

#define STEP(pi,pj) \
  __asm__ __volatile__ ("movaps %4, %%xmm4 \n\t" \
                        "movaps %%xmm2, %%xmm3 \n\t" \
                        "subps %2, %%xmm4 \n\t" \
                        "movaps %%xmm1, %%xmm5 \n\t" \
                        "cmpps $0x6, %%xmm4, %%xmm2 \n\t" \
                        "andps %%xmm2, %%xmm5 \n\t" \
                        "subps %%xmm3, %%xmm4 \n\t" \
                        "andps %%xmm0, %%xmm2 \n\t" \
                        "addps %%xmm4, %%xmm5 \n\t" \
                        "movaps %%xmm5, %0 \n\t" \
                        "movaps %5, %%xmm6 \n\t" \
                        "movaps %%xmm2, %%xmm3 \n\t" \
                        "subps %3, %%xmm6 \n\t" \
                        "movaps %%xmm1, %%xmm7 \n\t" \
                        "cmpps $0x6, %%xmm6, %%xmm2 \n\t" \
                        "andps %%xmm2, %%xmm7 \n\t" \
                        "subps %%xmm3, %%xmm6 \n\t" \
                        "andps %%xmm0, %%xmm2 \n\t" \
                        "addps %%xmm6, %%xmm7 \n\t" \
                        "movaps %%xmm7, %1" \
                        : \
                        "=m" ((*pi).c1), \
                        "=m" ((*pi).c2) \
                        : \
                        "m" ((*pi).c1), \
                        "m" ((*pi).c2), \
                        "m" ((*pj).c1), \
                        "m" ((*pj).c2) \
                        : \
                        "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7")


static void error(int no)
{
   switch(no)
   {
      case 1:
         std::cerr<<"Error in subroutine rlxd_init\n";
         std::cerr<<"Bad choice of luxury level (should be 1 or 2)\n";
         break;
      case 2:
    	 std::cerr<<"Error in subroutine rlxd_init\n";
    	 std::cerr<<"Bad choice of seed (should be between 1 and 2^31-1)\n";
         break;
      case 3:
    	  std::cerr<<"Error in rlxd_get\n";
    	  std::cerr<<"Undefined state (ranlxd is not initialized\n";
         break;
      case 5:
    	  std::cerr<<"Error in rlxd_reset\n";
    	  std::cerr<<"Unexpected input data\n";
         break;
   }
   std::cerr<<"Program aborted\n";
   std::exit(0);
}


void RandGen::update(void)
{
   int k,kmax;
   dble_vec_t *pmin,*pmax,*pi,*pj;

   kmax=pr;
   pmin=&x.vec[0];
   pmax=pmin+12;
   pi=&x.vec[ir];
   pj=&x.vec[jr];

   __asm__ __volatile__ ("movaps %0, %%xmm0 \n\t"
                         "movaps %1, %%xmm1 \n\t"
                         "movaps %2, %%xmm2"
                         :
                         :
                         "m" (one_bit),
                         "m" (one),
                         "m" (carry)
                         :
                         "xmm0", "xmm1", "xmm2");

   for (k=0;k<kmax;k++)
   {
      STEP(pi,pj);
      pi+=1;
      pj+=1;
      if (pi==pmax)
         pi=pmin;
      if (pj==pmax)
         pj=pmin;
   }

   __asm__ __volatile__ ("movaps %%xmm2, %0"
                         :
                         "=m" (carry));

   ir+=prm;
   jr+=prm;
   if (ir>=12)
      ir-=12;
   if (jr>=12)
      jr-=12;
   is=8*ir;
   is_old=is;
}


void RandGen::define_constants(void)
{
   int k;
   float b;

   one.c1=1.0f;
   one.c2=1.0f;
   one.c3=1.0f;
   one.c4=1.0f;

   b=(float)(ldexp(1.0,-24));
   one_bit.c1=b;
   one_bit.c2=b;
   one_bit.c3=b;
   one_bit.c4=b;

   for (k=0;k<96;k++)
   {
      next[k]=(k+1)%96;
      if ((k%4)==3)
         next[k]=(k+5)%96;
   }
}


void RandGen::init(int level,int seed)
{
   int i,k,l;
   int ibit,jbit,xbit[31];
   int ix,iy;

   define_constants();

   if (level==1)
      pr=202;
   else if (level==2)
      pr=397;
   else
      error(1);

   i=seed;

   for (k=0;k<31;k++)
   {
      xbit[k]=i%2;
      i/=2;
   }

   if ((seed<=0)||(i!=0))
      error(2);

   ibit=0;
   jbit=18;

   for (i=0;i<4;i++)
   {
      for (k=0;k<24;k++)
      {
         ix=0;

         for (l=0;l<24;l++)
         {
            iy=xbit[ibit];
            ix=2*ix+iy;

            xbit[ibit]=(xbit[ibit]+xbit[jbit])%2;
            ibit=(ibit+1)%31;
            jbit=(jbit+1)%31;
         }

         if ((k%4)!=i)
            ix=16777215-ix;

         x.num[4*k+i]=(float)(ldexp((double)(ix),-24));
      }
   }

   carry.c1=0.0f;
   carry.c2=0.0f;
   carry.c3=0.0f;
   carry.c4=0.0f;

   ir=0;
   jr=7;
   is=91;
   is_old=0;
   prm=pr%12;
}


void RandGen::ranlxd(double r[],int n)
{
   int k;

   for (k=0;k<n;k++)
   {
      is=next[is];
      if (is==is_old)
         update();
      r[k]=(double)(x.num[is+4])+(double)(one_bit.c1*x.num[is]);
   }
}


void RandGen::getState(int state[])
{
   int k;
   float base;

   base=(float)(ldexp(1.0,24));
   state[0]=size();

   for (k=0;k<96;k++)
      state[k+1]=(int)(base*x.num[k]);

   state[97]=(int)(base*carry.c1);
   state[98]=(int)(base*carry.c2);
   state[99]=(int)(base*carry.c3);
   state[100]=(int)(base*carry.c4);

   state[101]=pr;
   state[102]=ir;
   state[103]=jr;
   state[104]=is;
}


void RandGen::setState(int state[])
{
   int k;

   define_constants();

   if (state[0]!=size())
      error(5);

   for (k=0;k<96;k++)
   {
      if ((state[k+1]<0)||(state[k+1]>=167777216))
         error(5);

      x.num[k]=(float)(ldexp((double)(state[k+1]),-24));
   }

   if (((state[97]!=0)&&(state[97]!=1))||
       ((state[98]!=0)&&(state[98]!=1))||
       ((state[99]!=0)&&(state[99]!=1))||
       ((state[100]!=0)&&(state[100]!=1)))
      error(5);

   carry.c1=(float)(ldexp((double)(state[97]),-24));
   carry.c2=(float)(ldexp((double)(state[98]),-24));
   carry.c3=(float)(ldexp((double)(state[99]),-24));
   carry.c4=(float)(ldexp((double)(state[100]),-24));

   pr=state[101];
   ir=state[102];
   jr=state[103];
   is=state[104];
   is_old=8*ir;
   prm=pr%12;

   if (((pr!=202)&&(pr!=397))||
       (ir<0)||(ir>11)||(jr<0)||(jr>11)||(jr!=((ir+7)%12))||
       (is<0)||(is>91))
      error(5);
}

#else	// HAVE_SSE

#define BASE 0x1000000
#define MASK 0xffffff

#define STEP(pi,pj) \
      d=(*pj).c1.c1-(*pi).c1.c1-carry.c1; \
      (*pi).c2.c1+=(d<0); \
      d+=BASE; \
      (*pi).c1.c1=d&MASK; \
      d=(*pj).c1.c2-(*pi).c1.c2-carry.c2; \
      (*pi).c2.c2+=(d<0); \
      d+=BASE; \
      (*pi).c1.c2=d&MASK; \
      d=(*pj).c1.c3-(*pi).c1.c3-carry.c3; \
      (*pi).c2.c3+=(d<0); \
      d+=BASE; \
      (*pi).c1.c3=d&MASK; \
      d=(*pj).c1.c4-(*pi).c1.c4-carry.c4; \
      (*pi).c2.c4+=(d<0); \
      d+=BASE; \
      (*pi).c1.c4=d&MASK; \
      d=(*pj).c2.c1-(*pi).c2.c1; \
      carry.c1=(d<0); \
      d+=BASE; \
      (*pi).c2.c1=d&MASK; \
      d=(*pj).c2.c2-(*pi).c2.c2; \
      carry.c2=(d<0); \
      d+=BASE; \
      (*pi).c2.c2=d&MASK; \
      d=(*pj).c2.c3-(*pi).c2.c3; \
      carry.c3=(d<0); \
      d+=BASE; \
      (*pi).c2.c3=d&MASK; \
      d=(*pj).c2.c4-(*pi).c2.c4; \
      carry.c4=(d<0); \
      d+=BASE; \
      (*pi).c2.c4=d&MASK


static void error(int no)
{
	switch(no)
	{
		case 0:
		std::cerr<<"Error in rlxd_init\n";
		std::cerr<<"Arithmetic on this machine is not suitable for ranlxd\n";
		break;
		case 1:
		std::cerr<<"Error in subroutine rlxd_init\n";
		std::cerr<<"Bad choice of luxury level (should be 1 or 2)\n";
		break;
		case 2:
		std::cerr<<"Error in subroutine rlxd_init\n";
		std::cerr<<"Bad choice of seed (should be between 1 and 2^31-1)\n";
		break;
		case 3:
		std::cerr<<"Error in rlxd_get\n";
		std::cerr<<"Undefined state (ranlxd is not initialized)\n";
		break;
		case 4:
		std::cerr<<"Error in rlxd_reset\n";
		std::cerr<<"Arithmetic on this machine is not suitable for ranlxd\n";
		break;
		case 5:
		std::cerr<<"Error in rlxd_reset\n";
		std::cerr<<"Unexpected input data\n";
		break;
	}
	std::cerr<<"Program aborted\n";
	std::exit(0);
}


void RandGen::update(void)
{
   int k,kmax,d;
   dble_vec_t *pmin,*pmax,*pi,*pj;

   kmax=pr;
   pmin=&x.vec[0];
   pmax=pmin+12;
   pi=&x.vec[ir];
   pj=&x.vec[jr];

   for (k=0;k<kmax;k++)
   {
      STEP(pi,pj);
      pi+=1;
      pj+=1;
      if (pi==pmax)
         pi=pmin;
      if (pj==pmax)
         pj=pmin;
   }

   ir+=prm;
   jr+=prm;
   if (ir>=12)
      ir-=12;
   if (jr>=12)
      jr-=12;
   is=8*ir;
   is_old=is;
}


void RandGen::define_constants(void)
{
   int k;

   one_bit=ldexp(1.0,-24);

   for (k=0;k<96;k++)
   {
      next[k]=(k+1)%96;
      if ((k%4)==3)
         next[k]=(k+5)%96;
   }
}


void RandGen::init(int level,int seed)
{
   int i,k,l;
   int ibit,jbit,xbit[31];
   int ix,iy;

   if ((INT_MAX<2147483647)||(FLT_RADIX!=2)||(FLT_MANT_DIG<24)||
       (DBL_MANT_DIG<48))
      error(0);

   define_constants();

   if (level==1)
      pr=202;
   else if (level==2)
      pr=397;
   else
      error(1);

   i=seed;

   for (k=0;k<31;k++)
   {
      xbit[k]=i%2;
      i/=2;
   }

   if ((seed<=0)||(i!=0))
      error(2);

   ibit=0;
   jbit=18;

   for (i=0;i<4;i++)
   {
      for (k=0;k<24;k++)
      {
         ix=0;

         for (l=0;l<24;l++)
         {
            iy=xbit[ibit];
            ix=2*ix+iy;

            xbit[ibit]=(xbit[ibit]+xbit[jbit])%2;
            ibit=(ibit+1)%31;
            jbit=(jbit+1)%31;
         }

         if ((k%4)!=i)
            ix=16777215-ix;

         x.num[4*k+i]=ix;
      }
   }

   carry.c1=0;
   carry.c2=0;
   carry.c3=0;
   carry.c4=0;

   ir=0;
   jr=7;
   is=91;
   is_old=0;
   prm=pr%12;
}


void RandGen::ranlxd(double r[],int n)
{
   int k;

   for (k=0;k<n;k++)
   {
      is=next[is];
      if (is==is_old)
         update();
      r[k]=one_bit*((double)(x.num[is+4])+one_bit*(double)(x.num[is]));
   }
}

void RandGen::getState(int state[])
{
   int k;

   state[0]=size();

   for (k=0;k<96;k++)
      state[k+1]=x.num[k];

   state[97]=carry.c1;
   state[98]=carry.c2;
   state[99]=carry.c3;
   state[100]=carry.c4;

   state[101]=pr;
   state[102]=ir;
   state[103]=jr;
   state[104]=is;
}


void RandGen::setState(int state[])
{
   int k;

   if ((INT_MAX<2147483647)||(FLT_RADIX!=2)||(FLT_MANT_DIG<24)||
       (DBL_MANT_DIG<48))
      error(4);

   define_constants();

   if (state[0]!=size())
      error(5);

   for (k=0;k<96;k++)
   {
      if ((state[k+1]<0)||(state[k+1]>=167777216))
         error(5);

      x.num[k]=state[k+1];
   }

   if (((state[97]!=0)&&(state[97]!=1))||
       ((state[98]!=0)&&(state[98]!=1))||
       ((state[99]!=0)&&(state[99]!=1))||
       ((state[100]!=0)&&(state[100]!=1)))
      error(5);

   carry.c1=state[97];
   carry.c2=state[98];
   carry.c3=state[99];
   carry.c4=state[100];

   pr=state[101];
   ir=state[102];
   jr=state[103];
   is=state[104];
   is_old=8*ir;
   prm=pr%12;

   if (((pr!=202)&&(pr!=397))||
       (ir<0)||(ir>11)||(jr<0)||(jr>11)||(jr!=((ir+7)%12))||
       (is<0)||(is>91))
      error(5);
}

#endif	// HAVE_SSE

RandGen::RandGen(const int seed, const int level)
{
	init(level, seed);
}

RandGen::RandGen()
{
	int t = (int)time(NULL);
	init(1, t);
}

double RandGen::getUniform(double a, double b)
{
	double r;
	ranlxd(&r, 1);
	return (b-a)*r + a;
}

unsigned int RandGen::getUniformInt(const unsigned int max)
{
	double r;
	ranlxd(&r, 1);
	return ((unsigned int)(r*(double)(max)));
}

double RandGen::getNormal(const double mean, const double sigma)
{
	double rx,ry,sqnrm;

	do
	{
		rx = getUniform(-1.0,1.0);
		ry = getUniform(-1.0,1.0);
		sqnrm = rx*rx + ry*ry;
	}while ((sqnrm > 1.0)||(sqnrm == 0.0));

	return sigma*rx*sqrt(-2.0*log(sqnrm)/sqnrm) + mean;
}

}	// namespace LQCDA


