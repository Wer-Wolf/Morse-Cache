# **eeptool:**

* Read/Create EEPROM files for the Morse-Cache

* Creates a binary file for burning into the eeprom of the ATiny13/A

***The binary provided with the release is compiled and tested for Linux and Windows 10!***

# **Usage:**

* Read a morse-code sequence from a given binary file:
    ```eeptool read <file>```

* Write a morse-code sequence to a binary file:
    ```eeptool write <file> <data>```
        
    * When finished, upload the binary file to the ATtiny13/A's **eeprom**

* Display a simple summary about the eeptool:
    ```eeptool help```

### **Building from source:**

1. Clone the Git Repository.

2. Type ```cd tool```.

3. Building the binary with **gcc** using ```make all```.

4. Use ```make clean``` to clean up.

## **Built with:**

* ```gcc``` - compiler

## **Authors:**

* **Armin W.** [Wer-Wolf](https://github.com/Wer-Wolf)

