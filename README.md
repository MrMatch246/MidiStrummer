Small VST that strums midi chords. 
Inspired by the Strumming feature of Arturias Keystep 37. No affiliation.
More to come.

Build with JUCE
Assumes that JUCE is located next to this repository. 

Installer for Win avaiable (VST3)
Either use the release or take the one in the installer folder.
Instructions to self-building on request. (im lazy)



# Usage:  
![image](https://github.com/MrMatch246/MidiStrummer/assets/50702646/de1ee4a8-d728-4d58-a996-7b7cf6b4760b)



# Features:
- Sync Mode
  - Free allows using the Delay Knob to set Delay between Notes in sub ms prescison
  - Sync allows using quantized Delay times
- Strum Direction
  - Up/Down
- Time Signature for Quantized Strumming x/1 - x/256 (wip)
  - x as in your daw is set to x/4 (4/4) (wip)
- Triplets
- Order Mode
  - If set to "Enforce Order" the strum will go from lowest to highest note (or highest to lowest)
  - If set to "Unchanged" the order in which the notes are recieved is used. (up and down works here too)
  - Usefull if the chords are Inversions 

![image](https://github.com/MrMatch246/MidiStrummer/assets/50702646/70eb94a6-7d2f-4cf2-8c29-71f52e82d36f)
