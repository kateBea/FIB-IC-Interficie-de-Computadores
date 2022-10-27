L3_extra_G:

    - RE0 has several latency for switching game from IDLE state to RUNNING
    since CPU time is in use (at least in PROTEUS simulation). Solution was
    to switch to game_start() on INT1 interrupt, this way game switches state 
    faster.

    - When GAME_STATUS == RUNNING, press RB1, "PRESS RB1 THEN RE0 TO START GAME" 
    message will not be shown on 7SEG displays another time once it is done. Only then you can 
    press RE0 to start the game functionality, i.e. (RB0 function(+1)).

    - To switch between modes press RB7. Important to point that you can switch between
    fnctionalities, but if the a string is being displayed in the 7SEG displays the 
    functionality will be available only after the string has finished showing