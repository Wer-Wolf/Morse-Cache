# **The Morse-Cache**



## **Description:**

Make your geocache more interesting with the **Morse-Cache**!. When triggered, it outputs a sequence of numbers in morse code. The **Morse-Cache** also indicates via an adjustable battery warning when his batteries should be replaced. Besides this, the **Morse-Cache** is low power and fits even in small cases. Perfect for multicaches!

## **Features:**

* Output a morse code sequence

    * Sequence consists of a maximum of 61 numbers

    * Preservation through firmware updates

* Triggered by magnets (through a reed switch)

* Configurable battery warning (See **Usage**)

* Automatic self-test after reset and poweron (See **Usage**)

* Low space and power consumption

* Runs with 2x AA or 2x AAA batteries (recommended)

## **Usage:**

### **Trigger the Morse-Cache:**

1. Hold the magnet over the magnetic switch (long component).

2. When Morse sequence is displayed, remove magnet.

3. After the Morse sequence has finished, the device can be triggered again.

* Color of the LED (should be noted in the listing of the geocache):

    * **Green**: Normal color of the LED

    * **Red**: Battery low, should be replaced

### **Adjust the battery warning:**

1. Connect the circuit to a stable voltage source between 2,2V and 3V.
   (whose voltage sets the new limit, from which the battery is considered "empty")
    
2. Holding the calibration button while triggering the magnetic switch.

***If the morse code sequence is displayed, repeat step 2 after the end of the sequence (first hold the button, then trigger the magnetic switch)***

3. Release the calibration button when the LED lights up briefly.

    * If the LED does not light up briefly, check volatge source and repeat step 2.

***Releasing the magnetic switch after the LED goes out will display the morse code sequence***

4. Device is ready for use.

### **Issuing the self-test:**

1. Press the reset button.

***The Morse-Cache must not be in the calibration process***

2. If the LED lights up briefly, the self-test succesful (else the internal EEPROM memory of the Morse-Cache may be defective).

***A defective Morse-Cache should be repaired before used again!***
        
### **Updating the device firmware:**
        
***Only possible with programming adapter***

1. Check [Releases](www.github.com/Wer-Wolf/Morse-Cache/releases) for new version.

2. Downloading the newest version (*.elf*).

3. Extract and flash the *.elf* file with a programming adapter (USBasp, ...).

### **Changing the Morse sequence:**

***Only possible with programming adapter***

1. Use the **eeptool** for creating the desired morse code sequence.

2. Burn the resulting *.bin* file in the EEPROM of the **Morse-Cache**.

***You may readjust the baterry warning***

## **Additional notes:**

### **Supported Devices:**

* **ATtiny13**

* **ATtiny13A** *recommended*

### **Fuses (ATiny13/A):**

* **hfuse:** ```0xFD```

* **lfuse:** ```0x2A```

### **Morse code characteristics:**

#### **EEPROM addresses:**

* **Start:** ```0x00```
    
* **End (maximum):** ```0x3D```

#### **Characteristics:**

* The string must be terminated (at ```0x3D``` at the latest) with ```0xFF```.
* Only numbers between ```0x00``` and ```0x09``` (0-9) are accepted data values.
    
***Invaild data will be ignored!***

## **Battery warning mechanism:**

* The current ADC output is compared with the value inside the EEPROM. 

#### **EEPROM adresses:**

* high byte: ```0x3F```
* low byte: ```0x3E```

## **Built with:**

* ```avr-gcc``` - compiler
* ```avr-libc``` - runtime libary
* ```avrdude``` - programming software

## **License:**
This project is licensed under the GPL License - see the [LICENSE](LICENSE) file for details

## **Authors:**

* **[Wer-Wolf]**(https://github.com/Wer-Wolf)

## **Acknowledgments:**

* The idea how to store the morse code was discovered in the [Mikrokontroller.net Forum](https://www.mikrocontroller.net/topic/171196).