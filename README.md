# intro
goal: make a small, light, and cheap midi merge/thru device.  

## features
* multiple inputs - merged into one output stream
    * probably 4 inputs - aligns with pic32mx available timers nicely to make input reading easy
* multiple outputs - at least 4
* low latency - try to not be more than 1 byte behind (~32 * 10 = 300 microseconds)
* midi trs sockets instead of din to minimize space requirements. it's what the cool kids use these days anyway.
* per-port setting input/output

## stretch goals
* usb in for:
    * power (need at least 7v input for current circuit)
    * midi-over-usb
    * configuration (? see last point)
* midi logging/debugging. requires screen real estate, so prolly not. 
* advanced filtering/processing of midi commands. this will probably be a separate project.

## tech stuff
the current design is based on a pic32mx, and midi input is handled via the venerable recommended circuit from the midi association based on a 6n135 opto coupler and two not gates in series, in this case a 74hc14 hex inverter.

![circuit diagram](/Docs/circuit.jpeg)

From the midi specification, register and download at https://midi.org/midi-1-0-core-specifications
![source: midi association](/Docs/midi-assoc-circuit.png)]