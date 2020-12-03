#include <ctype.h>
#include "state.h"

state init_state(char input, res_code *res)
{
    if (isdigit(input))
    {
        if (res != NULL)
            res->n1 = input;
        return N1;
    }
    else
    {
        return INIT;
    }
}

state n1_state(char input, res_code *res)
{
    if (isdigit(input))
    {
        if (res != NULL)
            res->n2 = input;
        return N2;
    }
    else
    {
        return INIT;
    }
}

state n2_state(char input, res_code *res)
{
    if (isdigit(input))
    {
        if (res != NULL)
            res->n3 = input;
        return N3;
    }
    else
    {
        return INIT;
    }
}

state n3_state(char input)
{
    if (input == ' ')
    {
        return LAST;
    }
    else
    {
        return INIT;
    }
}

state last_state(char input)
{
    if (input == '\n' || input == '\r')
    {
        return STOP;
    }
    else
    {
        return LAST;
    }
}

state getState(char input, state current_state, res_code *res)
{
    switch (current_state)
    {
    case INIT:
        return init_state(input, res);
    case N1:
        return n1_state(input, res);
    case N2:
        return n2_state(input, res);
    case N3:
        return n3_state(input);
    case LAST:
        return last_state(input);
    case STOP:
        return STOP;
    default:
        return INIT;
    }
}