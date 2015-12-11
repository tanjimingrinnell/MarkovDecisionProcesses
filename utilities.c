#include "mdp.h"
#include "utilities.h"
#include <stdio.h>

double calc_eu( const mdp*  p_mdp, unsigned int state, const double* utilities,
	      const unsigned int action)
{
  
  unsigned int numAvailActions = p_mdp->numAvailableActions[state];
  unsigned int *availActions = p_mdp->actions[state];
  int i;
  int available = 0;
  for (i = 0; i < numAvailActions; i++) {
    if (action == availActions[i]) {
      available = 1;
    }
  }
  if (!available) {
    return 0;
  }

  double eu = 0;   // Expected utility
  double ***transitionProb = p_mdp->transitionProb;
  unsigned int sPrime;
  if (state == 60) {
    printf("utilities[60]: %f\n", utilities[60]);
  }
  // Calculate expected utility: sum_{s'} P(s'|s,a)*U(s')
  for (sPrime = 0; sPrime < p_mdp->numStates; sPrime++) {
    
  	eu += transitionProb[sPrime][state][action] * utilities[sPrime];
  }
 
  return eu;
}

void calc_meu( const mdp* p_mdp, unsigned int state, const double* utilities,
	       double *meu, unsigned int *action )
{
  unsigned int numAvailActions = p_mdp->numAvailableActions[state];
  unsigned int *availActions = p_mdp->actions[state];

  int i;
  for (i = 0; i < numAvailActions; i++) {
  	double eu = calc_eu(p_mdp, state, utilities, availActions[i]);
    if (state == 60) {
      printf("eu in meu: %f\n", eu);
    }
    
  	if (eu > *meu || i == 0) {
  		*meu = eu;
  		*action = availActions[i];
  	}
  }
}
