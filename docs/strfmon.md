## strfmon

### Description
This function -- added to the user library (user/ulib.c) -- takes a user-inputted string including a monetary value and optional conversation specifications and places a formatted version into a new string buffer. By default, the amount is placed within brackets, and a .00 is added if the amount is a whole number.

### Flags/specifications implemented
``#n`` - number of digits to format to the left of the decimal point. If the value of ``n`` is less than the number of digits of the money amount, then nothing happens. Otherwise, unused positions will be filled according to the ``=f`` flag.

``=f`` - fill character to use. If this flag is not given, then the empty characters will be filled with spaces. 

``^`` - disable the grouping character. By default, monetary values will be separated with commas.

``!`` - disable the currency symbol. By default, monetary values will have the $ sign.

Every specification starts with ``%`` and ends with ``n``.

### Build/testing
To build, run ``make qemu`` in the top-level OS directory, and then run ``montest``. You can then edit the strfmon function call in user/montest.c to test different combinations of monetary values and specifications. Note that the specifications can be given in any order.

### Example input/outputs
``strfmon(buf, 64, "1234%n")`` -> buf should contain ``[$1,234.00]``

``strfmon(buf, 64, "1234.01%n")`` -> buf should contain ``[$1,234.01]``

``strfmon(buf, 64, "1234.56%!n")`` -> buf should contain ``[1,234.56]``

``strfmon(buf, 64, "1234%^#7n")`` -> buf should contain ``[$   1234.00]``

``strfmon(buf, 64, "1234.56%#7=&^!n")`` -> buf should contain ``[&&&1234.56]``
