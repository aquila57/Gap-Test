/* etausgap.c - Gap Test, etaus Generator  Version 0.1.0 */
/* Copyright (C) 2020 aquila57 at github.com */

/* This program is free software; you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License as    */
/* published by the Free Software Foundation; either version 2 of    */
/* the License, or (at your option) any later version.               */

/* This program is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of    */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the      */
/* GNU General Public License for more details.                      */

/* You should have received a copy of the GNU General Public License */
/* along with this program; if not, write to:                        */

   /* Free Software Foundation, Inc.                                 */
   /* 59 Temple Place - Suite 330                                    */
   /* Boston, MA 02111-1307, USA.                                    */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <gsl/gsl_cdf.h>
#include "etaus.h"

#define RAWSIZE 1001000

#define SIZE 1000000

#define BITS 4

#define DF 128

void calcchi(int *freqtbl);

int main(void)
   {
   int i;
   int gap;
   etfmt *et;
   int freqtbl[512];
   int kount;
   char *p,*q;
   char *rawlist;
   rawlist = (char *) malloc(RAWSIZE);
   if (rawlist == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating rawlist\n");
      exit(1);
      } /* out of memory */
   /****************************************************/
   /* initialize the etaus random number generator     */
   /****************************************************/
   et = (etfmt *) etausinit();
   /********************************************************************/
   /* create a random sample of 1 million 4 bit numbers 0-15           */
   /********************************************************************/
   p = (char *) rawlist;
   q = (char *) rawlist + RAWSIZE;
   while (p < q) *p++ = 0xff;
   p = (char *) rawlist;
   kount = SIZE;
   while (kount--)
      {
      int ch;
      ch = etauspwr(et,BITS);
      *p++ = (char) ch;
      } /* gen loop */
   /********************************************************************/
   /* now count the gaps in the raw list of numbers                    */
   /* the sublist, 1234512345, contains 5 gaps of 4                    */
   /* freqtbl[4] will be incremented by 5                              */
   /* if the sublist is 1212123134512345,                              */
   /* freqtbl[1] is incremented by 5                                   */
   /* freqtbl[2] is incremented by 1                                   */
   /* freqtbl[3] is incremented by 1                                   */
   /* freqtbl[4] is incremented by 3                                   */
   /********************************************************************/
   for (i=0;i<512;i++) freqtbl[i] = 0;
   gap = 0;
   while (gap <= DF)       /* for each gap distance */
      {
      p = (char *) rawlist;
      i = SIZE;
      while (i--)        /* for all the sample numbers 0-15 */
	 {
	 int j;
	 int bypass;
	 /***********************************************************/
	 /* if testing a gap of n numbers, bypass any gap less      */
	 /* than n numbers.  That gap has already been counted.     */
	 /* Only count a gap of n numbers.                          */
	 /***********************************************************/
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
	 /* if a gap of n numbers has been found, count it */
         if (!bypass && *p == *(p+gap+1))
	    {
	    freqtbl[gap]++;
	    /* debugging */
	    // printf("gap %d dgt %02x\n", gap, *p);
	    } /* if gap match */
	 p++;
	 } /* for each 4-bit number in rawlist */
      gap++;
      } /* for each gap size */
   /*****************************************/
   /* debugging routine                     */
   /* change -DF to DF to activate          */
   /*****************************************/
   for (i=0;i<=-DF;i++)
      {
      if (freqtbl[i])
         {
         // printf("gap length %d freq %d\n", i, freqtbl[i]);
	 } /* print non-zero frequencies */
      } /* for each distance */
   printf("\t\t   Gap Test\n");
   printf("\t\tetaus Generator\n");
   printf("\n");
   /****************************************************/
   /* calculate the chi square test based on the count */
   /* of gaps in the sampling data.                    */ 
   /****************************************************/
   calcchi(freqtbl);
   /****************************************************/
   /* program has ended, free the heap                 */
   /****************************************************/
   free(rawlist);
   free(et->state);
   free(et);
   return(0);
   } /* main */
