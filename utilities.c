/**
 File
   utilities.c

 Author
   Jerod Weinman
   Box - Noyce Science Division Office
   CSC261.01
   Henry Fisher - 3537
   Jimin Tan - 4465

 Summary
   Implements calculations of utility used in policy and value iteration.

 Provides
   double calc_eu( const mdp*  p_mdp, unsigned int state, const double* utilities,
        const unsigned int action)
   void calc_meu( const mdp* p_mdp, unsigned int state, const double* utilities,
         double *meu, unsigned int *action )
*/

#include "mdp.h"
#include "utilities.h"
#include <stdio.h>


/*A function that calculate the expected utility for all possible 
actions*/
double calc_eu( const mdp*  p_mdp, unsigned int state, const double* utilities,
	      const unsigned int action)
{
  unsigned int numAvailActions = p_mdp->numAvailableActions[state];
  unsigned int *availActions = p_mdp->actions[state];
  int i;
  //This is a boolean that store whether an action is available
  int available = 0;
  //Check all available actions are reasonable
  for (i = 0; i < numAvailActions; i++) {
    if (action == availActions[i]) {
      available = 1;
    }
  }
  //If there are action that is not reasonable, return 0 for eu
  if (!available) {
    return 0;
  }

  double eu = 0;   // Expected utility
  double ***transitionProb = p_mdp->transitionProb;
  unsigned int sPrime;
  // Calculate expected utility: sum_{s'} P(s'|s,a)*U(s')
  for (sPrime = 0; sPrime < p_mdp->numStates; sPrime++) {
    //Calculating eu using transition probability
    eu += transitionProb[sPrime][state][action] * utilities[sPrime];
  }
  return eu;
}

/* calc_meu calculate the maximum expected utility among actions in a
given state*/
void calc_meu( const mdp* p_mdp, unsigned int state, const double* utilities,
	       double *meu, unsigned int *action )
{
  unsigned int numAvailActions = p_mdp->numAvailableActions[state];
  unsigned int *availActions = p_mdp->actions[state];

  int i;
  //loop through all actions to update meu
  for (i = 0; i < numAvailActions; i++) {
  	double eu = calc_eu(p_mdp, state, utilities, availActions[i]);
    /*If the expected utility is bigger than the maximum expected 
      utility we replace meu with eu. If this is the first time we
      run this loop, we want to give eu to meu since we don't want it
      to be 0.*/
  	if (eu > *meu || i == 0) {
  		*meu = eu;
  		*action = availActions[i];
  	}
  }
}
