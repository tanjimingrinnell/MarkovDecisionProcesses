#include "mdp.h"
#include "utilities.h"
#include <stdio.h>

double calc_eu( const mdp*  p_mdp, unsigned int state, const double* utilities,
	      const unsigned int action)
{
  double eu = 0;   // Expected utility

  
  double ***transitionProb = p_mdp->transitionProb;
  unsigned int sPrime;

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
  	if (eu > *meu) {
  		*meu = eu;
  		*action = availActions[i];
  	}
  }
}
