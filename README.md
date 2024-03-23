Small VST that strums midi chords. 
Inspired by the Strumming feature of Arturias Keystep 37. No affiliation.
More to come.

Build with JUCE
Assumes that JUCE is located next to this repository. 

Installer for Win avaiable (VST3)
Either use the release or take the one in the installer folder.
Instructions to self-building on request. (im lazy)



USAGE:  
![image](https://github.com/MrMatch246/MidiStrummer/assets/50702646/de1ee4a8-d728-4d58-a996-7b7cf6b4760b)

Features:  
![image](https://github.com/MrMatch246/MidiStrummer/assets/50702646/67295520-e85b-4db6-aa26-71a73417b8f0)

- Strumming Speed
- Strumming Direction
- Quantized Strumming x/1 - x/256 (wip)
  - x as in your daw is set to x/4 (4/4) (wip)
- Triplets

Notice:
Currently strums in the order the messages are received.
If your Keyboard uses inversions and sends the lowest note last it will strum the chord in the wrong order.
