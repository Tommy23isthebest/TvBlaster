How it works: You wire an IR LED to an Arduino (Uno, Nano, or Mega) and point it at any TV. The sketch cycles through power-off codes for 20+ TV brands — Samsung, LG, Sony, Panasonic, Vizio, Philips, Sharp, Toshiba, TCL, Hisense, RCA, JVC, Hitachi, Pioneer, and more.
Protocols covered: NEC, Sony, Samsung, RC5, RC6 — covers basically every TV made in the last 20 years.
What you need: Just an IR LED (940nm), a 100Ω resistor, and the IRremote library installed in Arduino IDE.
How it runs: Sends all codes on startup, then loops every 5 seconds so you can sweep a room. Serial monitor shows which code it's sending.
Super simple wiring — only 3 connections:

What you need:

Arduino (Uno, Nano, or Mega)
1x IR LED (940nm — the clear/purple ones)
1x 100Ω resistor
A couple of jumper wires
Connections:

IR LED long leg (anode) → 100Ω resistor → Pin 3 on the Arduino
IR LED short leg (cathode) → GND on the Arduino
That's it. Two wires and a resistor.

Tips:

The IR LED looks like it's not doing anything because the light is invisible to your eyes, but your phone camera can see it — use that to verify it's blinking
Point it directly at the TV's IR sensor (usually bottom-center of the bezel)
For more range, you can add a second IR LED in parallel, or use a transistor to drive it with more current
Keep it within about 5-10 feet of the TV for best results
Upload steps:

Install the IRremote library in Arduino IDE (Sketch → Include Library → Manage Libraries → search "IRremote")
Open the sketch from the app (filter to IR category, copy the code)
Select your board (Uno/Nano/Mega)
Upload
Point at your Hisense and watch it go dark
