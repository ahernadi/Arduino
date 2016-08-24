# Arduino
Arduino projects

Arduno Sprinkler todo:
- add a burron to cycle trough
- Time displayed is invalid, need to troubleshot
- Add last watered to each zone. thinking add another field to the struct and store time last triggered. loop trough schedules
  and get the max for each zone
- remove case sensitiity so the phone's auto capital will still work  
- add error handling to parser. Currently no feedback
- create a pcb board for all componenct:
  - relay enable transitor
  - shift register
  - headers - should arrange headers so the realy is a straingt connection
  - cycle button
  - esp8266 wireless bridge
  - voltage regulator + all external components
  - maybe a terminal block to connect relay output with solenoids
  - maybe a fuse holder for the 24 volt AC supply
