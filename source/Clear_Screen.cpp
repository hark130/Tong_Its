#include "Clear_Screen.h"
#include <unistd.h>
#include <term.h>

void clear_the_screen(void)
{
	if (!cur_term)
    {
    	int result;
    	setupterm(NULL, STDOUT_FILENO, &result);
    	if (result <= 0) 
    	{
    		return;
    	}
    }

  	putp(tigetstr("clear"));

  	return;
}
