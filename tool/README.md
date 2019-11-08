# **eeptool:**

* Read/Create EEPROM files for the Morse-Cache

* Creates a binary file (*.bin*)

***The binary provided with the release is compiled and tested for Linux!***

# **Usage:**

* Read a morse-code sequence from a given binary file:
    ```./eeptool <FILE> READ```

* Write a morse-code sequence to a binary file:
    ```./eeptool <FILE> WRITE <DATA>```
        
    * When finished, upload the binary file to the ATtiny13/A's **EEPROM**

### **Building from source:**

1. Clone the Git Repository.

2. Type ```cd tool```.

3. Building the binary with **gcc** using ```make all```.

4. Use ```make clean``` to clean up.

## **Built with:**

* ```gcc``` - compiler

## **Authors:**

* **Armin W.** [Wer-Wolf](https://github.com/Wer-Wolf)

