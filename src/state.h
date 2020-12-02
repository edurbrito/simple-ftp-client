#include <stdlib.h>
#include "utils.h"

#ifndef STATE_H
#define STATE_H

/**
 * Server Response State Machine - Init State
 * @param input to be evaluated
 * @param res server response code struct
 * @return new state after evaluation
*/
state init_state(char input, res_code * res);

/**
 * Server Response State Machine - N1 State
 * @param input to be evaluated
 * @param res server response code struct
 * @return new state after evaluation
*/
state n1_state(char input, res_code * res);

/**
 * Server Response State Machine - N2 State
 * @param input to be evaluated
 * @param res server response code struct
 * @return new state after evaluation
*/
state n2_state(char input, res_code * res);

/**
 * Server Response State Machine - N3 State
 * @param input to be evaluated
 * @return new state after evaluation
*/
state n3_state(char input);

/**
 * Server Response State Machine - Last State
 * @param input to be evaluated
 * @return new state after evaluation
*/
state last_state(char input);

/**
 * Server Response State Machine - Main Switch
 * @param input to be evaluated
 * @param current_state to call the corresponding function
 * @param res server response code struct
 * @return new state after evaluation
*/
state getState(char input, state current_state, res_code * res);

#endif