#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <rs232.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main()
{    
	int state = 0;
    int com_port = 6;       /* COM7 */
    int baudrate = 115200;  /* 115200 */
    char mode[]  = {'8', 'N', '1', 0};
    state = RS232_OpenComport(com_port, baudrate, mode);
    if(!state)
        printf("Open COM Port Success\n\n");
    clock_t t1, t2;
    t1 = clock();
    Sleep(1234);
    t2 = clock();
    printf("%lf\n", (t2-t1)/(double)(CLOCKS_PER_SEC));
    return 0;
}
