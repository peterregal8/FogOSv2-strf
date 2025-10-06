#include "kernel/types.h"
#include "user/user.h"
#include "user/time.h"
#include "kernel/stat.h"

int timeTest(void){

	uint64 ts = time();
	struct tm t;
	unix_to_time((long)ts, &t);

	char buf[128];
	strftime(buf, 128, "%Y-%m-%d %H:%M:%S", &t);
	printf("Test 1: %s\n", buf);

	strftime(buf, sizeof(buf), "%H:%M:%S on %d/%m/%Y", &t);
    	printf("Test 2: %s\n", buf);

	strftime(buf, sizeof(buf), "Day %j of the week %w", &t);
    	printf("Test 3: %s\n", buf);

	return 0;
}

int main(){
	timeTest();
	exit(0);
}
