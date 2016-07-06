
#include <stdio.h>
#include <stdint.h>
#include "fixedbuffer.h"



define_fixed_buffer_queue(short,short);

int main(int argc, char** argv) {
	
	printf("point A\n");
	fixed_short_buffer_queue* q = new_fixed_short_buffer_queue(10,20);
	
	printf("sizeof q: %u\n",sizeof_fixed_short_buffer_queue(q));
	fixed_short_buffer_queue_dq(q);
	printf("sizeof q: %u\n",sizeof_fixed_short_buffer_queue(q));
	
	printf("point B\n");
	
	del_fixed_short_buffer_queue(q,DEL_STRUCT);
	printf("point C\n");
}

