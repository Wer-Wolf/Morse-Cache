# **eeptool:**

* Read/Write EEPROM files for the Morse-Cache

* Creates a binary file (*.bin*)

***The binary provided with the release is comiled and tested for Linux!***

# **Usage:**

* Read a morse-code sequence from a given binary file:
    ```./eeptool <FILE> READ```

* Write a morse-code sequence to a binary file:
    ```./eeptool <FILE> WRITE <DATA>```
        
    * When finished, upload the binary file to the ATtiny13/A's **EEPROM**

## **Built with:**

* ```gcc``` - compiler

## **Authors:**

* **Armin W.** [Wer-Wolf](https://github.com/Wer-Wolf)

