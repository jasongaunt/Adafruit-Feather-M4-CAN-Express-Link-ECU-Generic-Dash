# Introduction
This project is a demonstration of how to parse the "Generic Dash" CAN BUS frames from any ViPEC / Link ECU on the Arduino-compatible [Adafruit Feather M4 CAN Express](https://learn.adafruit.com/adafruit-feather-m4-can-express).

I have personally developed this using a Link G4+ PnP ECU but it should work with any of the following (either wire-in or Plug-and-Play);

* ViPEC V Series ie. V44 / V88
* ViPEC iSeries ie. i44 / i88
* Link G4 Series
* Link G4+ Series
* Link G4X Series

This code may work with other ECUs in the ViPEC / Link series however I cannot confirm. As such they are *not* supported.

This code performs the following;

* Correctly initialises the hardware on the Adafruit M4 CAN Express Feather
* Listens for CAN messages at 500 kbit (you can change this in the header file)
* Listens for Generic Dash sent as 11-bit CAN frames with header ID **200** (also changeable in the header file)
* Regularly prints the Engine RPM, Engine Coolant Temperature, Intake Air Temperature and Battery Voltage to serial
* Flashes the on-board NeoPixel red when a CAN frame is received and green when printing to serial

# Assumptions
It is assumed that you...

* Already know how to work with the Arduino IDE, adding boards and installing libraries
* Are using the Adafruit M4 CAN Express Feather
* Have correctly wired up the CAN HIGH output to your ECU's CAN1H or CAN2H output
* Have correctly wired up the CAN LOW output to your ECU's CAN1L or CAN2L output
* Have correctly wired up the CAN GND to your ECU's GND

# Setup

### ECU Configuration

For the ECU, you will need to enable the Generic Dash output on (one of) your CAN outputs with the same header and baud rate as configured in the header file (you *can* change the values in the header file).

To set up the Link G4+ to emit the Generic Dash output over CAN;

 1. Connect to your ECU with the relevant software (**PCLink G4+** in my case)
 2. Go to the **ECU Controls** menu and click **CAN Setup**
 3. If relevant, select the correct **CAN Module** (*CAN 1* or *CAN 2*, wherever you connected the Feather)
 4. Set **Mode** to **User Defined** and the **Bit Rate** to **500 kbit/s**
 5. Select a free channel below (**Channel 1** for example) and set **Mode** to **Transmit Generic Dash**
 6. Set the **Transmit Rate** to something suitable (20 Hz should be fine)
 7. Set the **CAN ID** to whatever is in your header file (default is 200) and the **Format** to **Normal**
 8. Click **Apply** and then **Ok**
 9. Press **CTRL+S** to store the settings to your ECU

### Arduino IDE Configuration

For the Feather, you will most likely need to set up the board libraries if you haven't done already, [here are the instructions](https://learn.adafruit.com/adafruit-feather-m4-can-express/arduino-ide-setup).

You will then need to install the following libraries:

* CAN Adafruit Fork by adafruit (tested with v1.2.1)
* Adafruit NeoPixel by Adafruit (tested with v2.0.2)

Next, load the project in your Arduino IDE and upload it.

# Usage

When the program is running, you should see similar output to the following over serial;

```
19:46:15.717 -> BAT: 13.53 volts
19:46:16.288 -> RPM: 849 RPM
19:46:16.288 -> ECT: 86 °C
19:46:16.288 -> IAT: 35 °C
19:46:16.288 -> BAT: 13.53 volts
19:46:16.906 -> RPM: 851 RPM
19:46:16.906 -> ECT: 86 °C
19:46:16.906 -> IAT: 35 °C
19:46:16.906 -> BAT: 13.53 volts
19:46:17.477 -> RPM: 853 RPM
19:46:17.477 -> ECT: 86 °C
19:46:17.477 -> IAT: 35 °C
19:46:17.477 -> BAT: 13.53 volts
```

If you look in the header file for `GenericDashParameters` you will get a list of the fields you can get. These correspond to [the Generic Dash output](https://www.akao.co.uk/manuals/Link%20G4+%20Manual/device_specific_can_informatio.htm#genericdash).

In your code, calling... 
```cpp
float BatteryVoltage = getGenericDashValue(GenericDash, ECU_BATTERY_VOLTAGE);
```
...for example will return the battery voltage that the ECU sees.

Most of these dash values are floats, so the above `getGenericDashValue` function always returns a float. That said, you likely need to *cast* these to what you need them to be. 

For example, engine RPM is not a float, it is an integer ranging from 0-15000 (RPM), so to get that value you'd do the following;

```cpp
int EngineRPM = (int)getGenericDashValue(GenericDash, ECU_ENGINE_SPEED_RPM);
```

And for the Limits Flags, these are an *unsigned int* bit field which you can grab with the following;

```cpp
unsigned int LimitsFlags = (unsigned int)getGenericDashValue(GenericDash, ECU_LIMIT_FLAGS_BITFIELD);
```

And to then read those Limits Flags...

```cpp
bool RPMLimitActive = bitRead(LimitsFlags, LIMITS_FLAG_RPM_LIMIT);
```

You can find those `LIMITS_FLAG_*` bit fields in the header file too.

Good luck!

# Author
Jason Gaunt - 2021

# License
MIT License, see `LICENSE`