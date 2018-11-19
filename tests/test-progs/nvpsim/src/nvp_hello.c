#include <stdio.h>
#include <stdint.h>

uint32_t u32_count;

int main()
{
    u32_count = 0;
    
	printf("Hello Gem5\n");
	
	while (u32_count < 10000)
	{
	    for (uint32_t i = 0; i < 10000; i++);
	    printf("u32_count: %d\n", u32_count++);  
	}
	
	printf("Bye Gem5\n");
	
	return 0;
}
