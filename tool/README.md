# **eeptool:**

* Read/Create EEPROM images for the Morse-Cache

* Creates a binary for burning into the EEPROM of the ATiny13/A

***You need to have Python 3 installed for running eeptool.py!***

# **Usage:**

* Read a morse-code sequence from a given binary file:
    ```eeptool.py file```

* Write a morse-code sequence to a binary file:
    ```eeptool.py -w NUMBERS file```
        
    * When finished, upload the binary file to the ATtiny13/A's **EEPROM**

* Display help page:
    ```eeptool.py --help```

## **Built with:**

* ```argparse``` - argument parsing library

## **Authors:**

* **Armin W.** [Wer-Wolf](https://github.com/Wer-Wolf)

