# CardputerMidiMonitor
Basic Midi Monitor for Cardputer

This is a basic Midi Monitor software for your Cardputer. 
It works with the SAM2695 MIDI module, and it should also with any DIY standard MIDI IN port, as long as you connect it to the cardputer the way it is supposed to be connected (with the octocoupler and all).
You can monitor the following midi messages with it:
-Note On
-Note Off
-Control Change
-Program Change
-AfterTouch Poly
-AfterTouch Channel
-Pitch Bend

Pitch Bend messages will show the value on GREEN if positive, and on RED if negative. The rest of the messages will show YELLOW and PURPLE.
It keeps the last SIX received messages on the screen.
There's not much to it. It doesn't do much, but as far as I've tested, it does well what it is supposed to do. A Midi Monitor tool to quickly evaluate what messages are you sending from a Midi device.

Have fun :)
![CardputerMIDI](https://github.com/user-attachments/assets/f72d5d48-adbf-42b6-a576-bef7053e6d0b)
