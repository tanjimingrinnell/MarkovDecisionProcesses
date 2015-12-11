/**
 File
   policy_evaluation.c

 Author
   Jerod Weinman
   Box - Noyce Science Division Office
   CSC261.01
   Henry Fisher - 3537
   Jimin Tan - 4465

 Summary
   Provides policy evaluation, used in policy iteration

 Provides
   void policy_evaluation( const unsigned int* policy, const mdp* p_mdp,
			double epsilon, double gamma,
			double* utilities)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "utilities.h"
#include "mdp.h"

/*  Procedure
 *    policy_evaluation
 *
 *  Purpose
 *    Iteratively estimate state utilities under a fixed policy
 *
 *  Parameters
 *   policy
 *   p_mdp
 *   epsilon
 *   gamma
 *   utilities
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    policy points to a valid array of length p_mdp->numStates
 *    Each policy entry respects 0 <= policy[s] < p_mdp->numActions
 *       and policy[s] is an entry in p_mdp->actions[s]
 *    p_mdp is a pointer to a valid, complete mdp
 *    epsilon > 0
 *    0 < gamma < 1
 *    utilities points to a valid array of length p_mdp->numStates
 *
 *  Postconditions
 *    utilities[s] has been updated according to the simplified Bellman update
 *    so that no update is larger than epsilon
 */
void policy_evaluation( const unsigned int* policy, const mdp* p_mdp,
			double epsilon, double gamma,
			double* utilities)
{
	// Declare and malloc utilities prime ('successor' utilities), then set all
	//  entries to 0.
	double * utilitiesPrime;
  utilitiesPrime = malloc( sizeof(double) * p_mdp->numStates );
  memset(utilitiesPrime, 0, sizeof(double) * p_mdp->numStates);
	
	double delta;
	// Need to run this at least once.
	do {
		delta = 0;
		// Loop over states, each time calculating the expected utility of our
		//  current policy, and learning new utilities from the reward of the
		//  currend state plus expected utility times a discount factor.
		unsigned int state;
		for(state = 0; state < p_mdp->numStates; state++) {
			// Note that terminal states get slightly different treatment.
			if (p_mdp->terminal[state]) {
				utilitiesPrime[state] = p_mdp->rewards[state]; 
			} else {
				double eu = calc_eu(p_mdp, state, utilities, policy[state]);
        utilitiesPrime[state] = p_mdp->rewards[state] + gamma*eu;
			}
			// We keep track of the highest change in utilities. If this exceeds a
			//  certain threshhold (see condition for while loop), we end iteration.
			double utilChange = fabs(utilitiesPrime[state] - utilities[state]);
			if (utilChange > delta) {
				delta = utilChange;
			}			
		}
		// Latest learned utilities are copied over to old utilities before repeat.
		memcpy(utilities, utilitiesPrime, sizeof(double) * p_mdp->numStates);
	} while (delta > epsilon);

	free(utilitiesPrime);
}