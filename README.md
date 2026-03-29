How it works: You wire an IR LED to an Arduino (Uno, Nano, or Mega) and point it at any TV. The sketch cycles through power-off codes for 20+ TV brands — Samsung, LG, Sony, Panasonic, Vizio, Philips, Sharp, Toshiba, TCL, Hisense, RCA, JVC, Hitachi, Pioneer, and more.
Protocols covered: NEC, Sony, Samsung, RC5, RC6 — covers basically every TV made in the last 20 years.
What you need: Just an IR LED (940nm), a 100Ω resistor, and the IRremote library installed in Arduino IDE.
How it runs: Sends all codes on startup, then loops every 5 seconds so you can sweep a room. Serial monitor shows which code it's sending.
