# TV-B-Gone — IR Blaster Wiring & Setup Guide

## What It Does

Cycles through discrete **Power OFF** codes for 20+ TV brands (Hisense, Samsung, LG, Sony, Panasonic, etc.) via infrared. Fires all codes on startup, then repeats every 5 minutes. Hisense codes fire first.

---

## Parts Needed

| Part | Notes |
|------|-------|
| Arduino Uno (or Nano/Mega) | Uno recommended |
| IR LED (5mm, 940nm) | The clear ones — NOT a photodiode/receiver |
| 100Ω resistor | brown-black-brown — DO NOT use 10K |
| Breadboard + jumper wires | |

> **Important:** Verify your clear LED is an emitter, not a receiver. Test with a phone camera (front camera works best) — you should see it glow purple/white when powered.

---

## Wiring — Single LED (basic)

```
Arduino Pin 3  →  [100Ω resistor]  →  IR LED (+) long leg
                                       IR LED (−) short leg  →  GND
```

```
           Arduino Uno
           ┌─────────┐
   Pin 3 ──┤         │
       GND ──┤         │
           └─────────┘
                │  │
               [R] 100Ω
                │
              IR LED
              (+) long leg
              (−) short leg ── GND
```

**Pin 3 is required** — IRremote library uses Pin 3 for PWM (38kHz carrier) on Uno.

---

## Wiring — Three LEDs (better range)

Wire 3 IR LEDs **in parallel**, all on Pin 3. Give each its own resistor:

```
Pin 3 ──┬──[100Ω]── IR LED 1 (+) → (−) → GND
        ├──[100Ω]── IR LED 2 (+) → (−) → GND
        └──[100Ω]── IR LED 3 (+) → (−) → GND
```

Point the 3 LEDs in slightly different directions to widen the spread.

---

## Library Setup

1. Open Arduino IDE
2. Go to **Tools → Manage Libraries**
3. Search for **IRremote** by `Arduino-IRremote`
4. Install version **4.x** (not v2 or v3)

---

## Upload & Run

1. Copy the sketch from the **TV-B-Gone** section of the Arduino Prank Lab app
2. Set pin number at the top: `#define IR_SEND_PIN 3`
3. Upload to your Uno
4. Open **Serial Monitor (9600 baud)** — you'll see which brand is being sent
5. Point the IR LED at the TV's IR receiver (usually front-center or bottom-left of screen)

---

## Range Tips

| Method | Range |
|--------|-------|
| Single LED, 100Ω | ~1–2 metres |
| 3× LEDs in parallel | ~2–4 metres |
| Smaller resistor (47Ω) | slightly more |
| Transistor driver (2N2222) | ~8–10 metres |

**Transistor wiring for maximum range:**
```
Pin 3 ──[1KΩ]── Base (B) of 2N2222
5V ──[33Ω]── IR LED (+) → LED (−) → Collector (C) of 2N2222
Emitter (E) → GND
```

---

## Troubleshooting

| Symptom | Fix |
|---------|-----|
| No flash visible on phone camera | Check LED isn't backwards — swap legs |
| Camera shows flash but TV doesn't respond | Point closer, aim at TV's IR sensor |
| LED doesn't light even with 5V direct | LED is dead, use a different one |
| Serial Monitor shows nothing | Set baud to 9600, check sketch uploaded |
| Works at 10cm but not 1m | Use 3 LEDs or transistor driver |

---

## Supported Brands (Power OFF only — no toggle, no power ON)

Hisense (3 variants), Samsung, LG, Sony, Panasonic, Philips, Sharp, Toshiba, TCL/Roku, Vizio, Sanyo, RCA, JVC, Haier, Insignia, Magnavox, Emerson, Hitachi, Pioneer, Westinghouse, Funai
