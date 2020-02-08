/* calcchi.c - Chi Square Test  Version 0.1.0 */
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

void calcchi(int *freqtbl)
   {
   int i;
   double expected;
   double actual;
   double prob;
   double negprob;
   double smpls;
   double chisq;
   double diff;
   double diffsq;
   double df;
   double minchi;
   double maxchi;
   double pvalue;
   printf(" Gap        Actual          Expected      "
      "Difference\n");
   prob = 1.0 / 16.0;
   negprob = 1.0 - prob;
   smpls = (double) SIZE;
   chisq = 0.0;
   for (i=0;i<DF;i++)
      {
      int j;
      expected = prob;
      for (j=1;j<=i;j++)
	 {
         expected *= negprob;
	 } /* for each negative probability */
      expected *= smpls;
      actual = (double) freqtbl[i+1];
      diff = actual - expected;
      diffsq = diff * diff;
      chisq += (diffsq / expected);
      printf("%4d %13.0f %18.5f %14.5f\n",
         i, actual, expected, diff);
      } /* for each gap */
   printf("Sample size is %d\n", SIZE);
   df = (double) (DF - 1);
   minchi = gsl_cdf_chisq_Pinv(0.025,df);
   maxchi = gsl_cdf_chisq_Pinv(0.975,df);
   pvalue = gsl_cdf_chisq_P(chisq,df);
   printf("\n");
   printf("Chi square %f  P-value %18.15f\n", chisq, pvalue);
   printf("\n");
   printf("at 95%c probability and %.0f degrees of freedom,\n"
      "Chi square ranges from %f to %f\n",
      '%', df, minchi, maxchi);
   } /* calcchi */
