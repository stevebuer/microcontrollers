/* 
 * AT89S52 Hello World
 *
 */

#include <mcs51/at89x52.h>
 
static void delay(unsigned int t) 
{
        while (t--)
		;
}

void main()
{
        while (1) {

                /* Toggle LED at P2_0 */

                P2_0 = 0;
                delay(30000);
                P2_0 = 1;
                delay(30000);
        }
}
