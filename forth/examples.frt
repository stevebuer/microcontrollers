\
\ Steve's Forth examples
\

\ define constants that contain the 8051 port addresses
\
\ value CONSTANT name
\
$80 constant P0 
$90 constant P1 
$a0 constant P2 
$b0 constant P3

\ define variable that contains port a counter
\
\ VARIABLE name
\
variable COUNT

\ store a value in the variable
\
\ value varname !
\
55 COUNT !

\ fetch variable value and print 
\
\ varname @
\
COUNT @ .
