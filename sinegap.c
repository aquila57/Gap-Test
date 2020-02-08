#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <gsl/gsl_cdf.h>
#include "eegl.h"

#define RAWSIZE 1001000

#define SIZE 1000000

#define DF 128

void calcchi(int *freqtbl);

int main(void)
   {
   int i;
   int gap;
   int freqtbl[512];
   int kount;
   double onepi;
   double dblbits;
   char *p,*q;
   char *rawlist;
   rawlist = (char *) malloc(RAWSIZE);
   if (rawlist == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating rawlist\n");
      exit(1);
      } /* out of memory */
   eefmt *ee;
   ee = (eefmt *) eeglinit();
   p = (char *) rawlist;
   q = (char *) rawlist + RAWSIZE;
   while (p < q) *p++ = 0xff;
   onepi = M_PI;
   dblbits = (double) 16;       /* 2^4 */
   p = (char *) rawlist;
   kount = SIZE;
   while (kount--)
      {
      int ch;
      double sinx;
      double exx;
      exx = eeglunif(ee) * onepi;
      sinx = sin(exx) * dblbits;
      ch = (int) sinx;
      *p++ = (char) ch;
      } /* gen loop */
   for (i=0;i<512;i++) freqtbl[i] = 0;
   gap = 0;
   while (gap <= DF)
      {
      p = (char *) rawlist;
      i = SIZE;
      while (i--)
	 {
	 int j;
	 int bypass;
	 bypass = 0;
	 j = 1;
	 while (j < gap)
	    {
	    if (*p == *(p + j))
	       {
	       bypass = 1;
	       break;
	       } /* if intermediate gap */
	    j++;
	    } /* for each interval up to gap */
         if (!bypass && *p == *(p+gap+1))
	    {
	    freqtbl[gap]++;
	    // printf("gap %d dgt %02x\n", gap, *p);
	    } /* if gap match */
	 p++;
	 } /* for each dgt */
      gap++;
      } /* for each dgt */
   /* change -DF to DF to activate */
   for (i=0;i<=-DF;i++)
      {
      if (freqtbl[i])
         {
         // printf("gap length %d freq %d\n", i, freqtbl[i]);
	 } /* print non-zero frequencies */
      } /* for each distance */
   printf("\t\t     Gap Test\n");
   printf("\t\tsine wave Ganerator\n");
   printf("\n");
   calcchi(freqtbl);
   free(ee->state);
   free(ee);
   return(0);
   } /* main */
