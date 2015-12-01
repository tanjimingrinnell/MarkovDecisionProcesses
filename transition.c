/* start.c
 *
 * A small test program to load and print info on an MDP file
 *
 * Jerod Weinman
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <inttypes.h>

#include "mdp.h"



////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    fprintf(stderr,"Usage: %s mdpfile state action\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  // Pointer for the MDP struct
  mdp *p_mdp;

  // Read the MDP file
  p_mdp = mdp_read(argv[1]);

  if (NULL == p_mdp)
    // mdp_read prints a message upon failure
    exit(EXIT_FAILURE);
  
  unsigned int state = (unsigned int)strtol(argv[2], NULL, 10); // Loop variable over states
  unsigned int action = (unsigned int)strtol(argv[3], NULL, 10);

  printf("s'    P(s' | s=%u,a=%u", state, action);

  unsigned int currentState = 0;
  
  for (currentState=0 ; currentState < p_mdp->numStates ; currentState++)
  {    
    printf("%u:%9.2f", currentState, p_mdp->transitionProb[currentState][state][action]);
    printf("\n");
  }
   
  // Clean up
  mdp_free(p_mdp);

}
