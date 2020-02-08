/* diegap.c - Gap Test, dieharder Generators  Version 0.1.0 */
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

#include "gap.h"

void putstx(char *pgm)
   {
   fprintf(stderr,"Usage: %s Dieharder_RNG_number\n", pgm);
   fprintf(stderr,"Example: %s 208\n", pgm);
   fprintf(stderr,"Where 208 is the kiss generator\n");
   fprintf(stderr,"Example: %s 053\n", pgm);
   fprintf(stderr,"Where 053 is the taus2 generator\n");
   fprintf(stderr,"To see a list of "
      "dieharder generators:\n");
   fprintf(stderr,"dieharder -g -l\n");
   exit(1);
   } /* putstx */

int main(int argc, char **argv)
   {
   int i;
   int gap;
   double dblsize;
   int freqtbl[512];
   int kount;
   char *p,*q;
   char *rawlist;
   xxfmt *xx;
   /*************************************************************/
   /* Allocate memory for the global structure.                 */
   /* This is a re-entrant program.                             */
   /*************************************************************/
   xx = (xxfmt *) malloc(sizeof(xxfmt));
   if (xx == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating xx\n");
      exit(1);
      } /* out of memory */
   /*************************************************************/
   /* initialize global data                                    */
   /*************************************************************/
   xx->dieharder_rngname[0] = '\0';
   xx->rngname = (char *) NULL;
   xx->generator = -1;
   /*************************************************************/
   /* obtain dieharder generator number parameter               */
   /*************************************************************/
   if (argc != 2) putstx(*argv);
   xx->generator = atoi(*(argv+1));
   if (xx->generator < 0 || xx->generator > 405)
      {
      fprintf(stderr,"main: invalid random "
         "number generator %s\n", *(argv+1));
      putstx(*argv);
      } /* invalid generator */
   /*************************************************************/
   /* initialize numeric fields                                 */
   /*************************************************************/
   bld_maxint(xx);  /* build maxint table for dieharder RNGs */
   bld_rngtbl(xx);  /* build name table for dieharder RNGs */
   if (xx->maxint_tbl[xx->generator] == 0.0)
      {
      fprintf(stderr,"main: invalid random "
         "number generator %s\n", *(argv+1));
      putstx(*argv);
      } /* invalid generator */
   xx->modulus = xx->maxint_tbl[xx->generator];
   xx->maxint  = (unsigned int) xx->modulus;
   /*************************************************************/
   /* Allocate memory for the raw sample list.                  */
   /*************************************************************/
   rawlist = (char *) malloc(RAWSIZE);
   if (rawlist == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating rawlist\n");
      exit(1);
      } /* out of memory */
   /********************************************************************/
   /* create a random sample of 1 million 4 bit numbers 0-15           */
   /********************************************************************/
   dblsize = pow(2.0, (double) BITS);          /* 16 */
   xx->eofsw = 0;
   xx->count = 0.0;
   diepfx(xx);
   p = (char *) rawlist;
   q = (char *) rawlist + RAWSIZE;
   while (p < q) *p++ = 0xff;
   p = (char *) rawlist;
   kount = SIZE;
   while (kount--)
      {
      double num;
      int ch;
      num = getdie(xx);
      if (xx->eofsw)
         {
	 fprintf(stderr,"main: end of file "
	    "while building rawlist\n");
	 exit(0);
	 } /* if end of file */
      ch = (int) floor(num * dblsize);
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
   /****************************************************/
   /* calculate the chi square test based on the count */
   /* of gaps in the sampling data.                    */ 
   /****************************************************/
   printf("\t\t    Gap Test\n");
   printf("\t    Dieharder Generator:  %s\n",
      xx->dieharder_rngname);
   printf("\n");
   calcchi(freqtbl);
   /****************************************************/
   /* program has ended, free the heap                 */
   /****************************************************/
   free(rawlist);
   free(xx);
   return(0);
   } /* main */
