/**
 File
   value_iteration.c

 Author
   Jerod Weinman
   Box - Noyce Science Division Office
   CSC261.01
   Henry Fisher - 3537
   Jimin Tan - 4465

 Summary
   Provides value iteration and driver function.

 Provides
   void value_iteration( const mdp* p_mdp, double epsilon, double gamma,
          double *utilities)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "utilities.h"
#include "mdp.h"


void value_iteration( const mdp* p_mdp, double epsilon, double gamma,
		      double *utilities)
{
  // Declare and malloc utilities prime ('successor' utilities), then set all
  //  entries to 0.
  double * utilitiesPrime;
  utilitiesPrime = malloc( sizeof(double) * p_mdp->numStates );
  memset(utilitiesPrime, 0, sizeof(double) * p_mdp->numStates);

  double delta;
  do {
    delta = 0;
    // Copy latest learned utilities over to current utilities.
    memcpy(utilities, utilitiesPrime, sizeof(double) * p_mdp->numStates);

    // Loop over all states. At each state, calculate the maximum expected
    //  utility of that state, and then update latest learned utilities with
    //  that value times the discount factor, plus the reward of the state.
    unsigned int state;
    for (state = 0; state < p_mdp->numStates; state++) {  
      if (p_mdp->terminal[state]) {
        utilitiesPrime[state] = p_mdp->rewards[state];
      } else {
        double meu = 0;
        unsigned int action = 0;
        calc_meu(p_mdp, state, utilities, &meu, &action);
        utilitiesPrime[state] = p_mdp->rewards[state] + gamma*meu;
      }      

      // We check to see how much our utilities changed, keeping a running max
      //  of the change. If this exceeds a certain threshhold, we end the
      //  iteration. See termination condition for the threshhold.
      double utilChange = fabs(utilitiesPrime[state] - utilities[state]);
      if (utilChange > delta) {
        delta = utilChange;
      }
    }
  } while (delta >= epsilon*(1-gamma)/gamma);

  free(utilitiesPrime);
}


/*
 * Main: value_iteration gamma epsilon mdpfile
 *
 * Runs value_iteration algorithm using gamma and with max
 * error of epsilon on utilities of states using MDP in mdpfile.
 *
 * Author: Jerod Weinman
 */
int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    fprintf(stderr,"Usage: %s gamma epsilon mdpfile\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  // Read and process configurations
  double gamma, epsilon;
  char* endptr; // String End Location for number parsing
  mdp *p_mdp;

  // Read gamma, the discount factor, as a double
  gamma = strtod(argv[1], &endptr);

  if ( (endptr - argv[1])/sizeof(char) < strlen(argv[1]) )
  {
    fprintf(stderr, "%s: Illegal non-numeric value in argument gamma=%s\n",
            argv[0],argv[1]);
      exit(EXIT_FAILURE);
  }

  // Read epsilon, maximum allowable state utility error, as a double
  epsilon = strtod(argv[2], &endptr); 

  if ( (endptr - argv[2])/sizeof(char) < strlen(argv[2]) )
  {
    fprintf(stderr, "%s: Illegal non-numeric value in argument epsilon=%s\n",
            argv[0],argv[2]);
      exit(EXIT_FAILURE);
  }

  // Read the MDP file (exits with message if error)
  p_mdp = mdp_read(argv[3]);

  if (NULL == p_mdp)
    { // mdp_read prints a message
      exit(EXIT_FAILURE);
    }

  // Allocate utility array
  double * utilities;

  utilities = malloc( sizeof(double) * p_mdp->numStates );

  // Verify we have memory for utility array
  if (NULL == utilities)
  {
    fprintf(stderr,
	    "%s: Unable to allocate utilities (%s)",
	    argv[0],
	    strerror(errno));
    exit(EXIT_FAILURE);
  }

  // Run value iteration!
  value_iteration ( p_mdp, epsilon, gamma, utilities);

  // Print utilities
  unsigned int state;
  for ( state=0 ; state < p_mdp->numStates ; state++)
    printf("%f\n",utilities[state]);
  
  // Clean up
  free (utilities);
  mdp_free(p_mdp);

}


