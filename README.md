# Teensy based arpeggiator for DAW-based workflow
Possibly overkill.

See http://unsilent.art blog for details

This code uses the MIDI functionality of the Teensy microcontroller. It syncs to the MIDI clock and song position pointer, as well as listening for MIDI note data. In time with the beat, it will play back a note from the set of held notes. 

The hardware is based on a custom dev board I made, but it should run happily on a raw Teensy if you strip out the UI code. I'll add the C++ files for this when I get the chance.

This code is not thing of beauty, but it did solve a problem I had in finding a MIDI arpeggiator to behave the way I wanted.

Hope it's useful to you.

Kind regards,
Nick.
