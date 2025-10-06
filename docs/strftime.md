# STRFTIME 

## Description
strftime added to our OS can be called by any file that includes the time.h header file. The header file contains two functions at the moment but more can be added later. 

It contains a unix to time function which converts a unix timestamp into a struct that includes all different versions of a date and time. It takes in a unix timestamp in the form or a long, and a the struct thats declared in the header file. 

It also contains a function called strftime. This takes in a buffer char array, the max number of spaces for the char array buffer, the format you want the date and/or time printed out in, and again the time struct created in header file. 

## How to use
``1`` First get your unix timestamp then create a new struct to use.
``2`` Next translate the unix timestamp to the struct using the ``unix_to_struct()`` function.
``3`` Then pass in all the variables to ``strftime()`` which will put the formatted time/date into your buffer that you pass in.

## Example workflow

````bash
    
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


````

This should print out;

````bash
Test 1: 2025-10-06 18:40:26
Test 2: 18:40:26 on 06/10/2025
Test 3: Day 78 of the week October	
````

## Different types of format options

``%Y`` -> Year
``%m`` -> Month in numeral format
``%d`` -> Day in numeral format
``%H`` -> Hour
``%M`` -> Minute
``%S`` -> Second
``%j`` -> Day of the year as in day 36 out of 365
``%w`` -> Month as a word i.e. October
