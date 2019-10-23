1. Features:
    - Output a Morsecode sequence consisting of a maximum of 61 numbers

    - Preservation of the Morsecode sequence through firmware updates

    - Can be triggered by magnets

    - Configurable battery warning

    - Automatic self-test after reset and poweron

    - Low space and power consumption

    - Runs with 2x AA or 2x AAA batteries

    - Perfect for multicaches

1. Usage:

    - trigger the circuit:
        1. Hold the magnet over the magnetic switch (long component).

        2. When Morse sequence is displayed, remove magnet.

        3. After the Morse sequence has finished, the device can be triggered again.

    - Color of the LED:
        - Green: Normal color of the LED.

        - Red: Battery low, should be replaced.
               (should be noted in the listing of the cache)

2. Maintenance:

    - Calibration of the limit for the battery warning:
        1. Connect the circuit to a stable voltage source between 2,2V and 3V.
           (whose voltage sets the new limit, from which the battery is considered "empty")
        
        2. Holding the calibration button while triggering the magnetic switch.
            --> If the Morsecode sequence is displayed, repeat step 2 after the end of the sequence.
                (first hold the button, then trigger the magnetic switch)

        3. Release the button when the LED lights up briefly.
            --> if the LED does not light up briefly, check volatge source and repeat step 2

        4. Device is ready for use.

    - Issuing the self-test:
        1. Press the reset button
            --> The Morse-Cache must not be in the calibration process.

        2. If the LED lights up briefly, the self-test succesful.
           (Else the Morse-Cache may be defective)

            --> A defective Morse-Cache should be repaired before used again.

    - Updating the device firmware:
        --> Only possible with programming adapter
        1. Check www.github.com/Wer-Wolf/Morse-Cache/releases for new version.

        2. Downloading the newest version (.zip).

        3. Extract and flash .hex file with programming adapter.

    - Changing the Morse sequence:

        - Only possible with programming adapter. :-(
            -> See "Developer" for details

3. Developer:

    - Fuses:
        -hfuse: 0xFD

        -lfuse: 0x2A

    - Morsecode:

        - Addresses:

            - Start: 0x00
            - End (maximum): 0x3D

            - The string must be terminated (at 0x3D at the latest) with 0xFF
            - Only numbers between 0x00 and 0x09 (0-9) are accepted data values
            - Invaild data will be ignored

            !When uploading a new morse sequence to the eeprom, the device should be calibrated again!

    - Battery calibration data:

        - Adresses:

            high byte: 0x3F
            low byte: 0x3E

1. Features:
    - Ausgeben einer Morsesequenz aus maximal 61 Nummern.

    - Beibehalten der Morsesequenz nach Firmwareupdates

    - Auslösung durch Magneten

    - Konfigurierbarer Batteriewarner

    - Automatischer Selbsttest nach Einschalten oder einem Reset

    - Niedriger Platz- und Stromverbrauch

    - Läuft mit 2x AA oder 2x AAA Batterien

    - Perfekt für Multicaches

1. Benutzung:

    - Auslösen:
        1. Magneten über den Magnetschalter (langes Bauteil) halten.

        2. Morsesequenz läuft, Magnet entfernen.

        3. Nach Ende der Morsesequenz ist das Gerät wieder einsatzbereit.

    - Farbe der LED:
        - Grün: Normale Farbe der LED.

        - Rot: Batterie niedrig, Batterie sollte gewechselt werden.
               (Am bessten im Listing darauf hinweißen)

2. Wartung:

    - Kalibrierung der Grenze für die Batteriewarnung:
        1. Schaltung mit stabiler Spannungsquelle zwichen 2,2V und 3V verbinden.
           (dessen Spannung stellt die neue Grenze da, ab der die Batterie als "leer" gilt)
        
        2. Halten des Kalibrierungstasters bei gleichzeitigem Auslösen des Magnetschalters.
            --> Sollte ein Morsecode ausgegeben werden, Schritt 2 nach Ende der Ausgabe wiederholen.
                (erst Taster halten, danach Magnetschalter auslösen)

        3. Taster loslassen wenn LED kurz aufleuchted.
           (Schritt 2 wiederholen wenn die LED nicht kurz aufleuchted)

        4. Gerät ist einsatzbereit.

    - Selbsttest starten:
        1. Resetknopf betätigen
            --> Der Morse-Cache darf sich nicht im Kalibrierungsvorgang befinden.

        2. Wenn die LED kurz aufleuchted, war der Selbsttest erfolgreich:
           (Andernfalls könnte der Morse-Cache fehlerhaft sein)

            --> Ein fehlerhafter Morse-Cache sollte nicht ohne Reparatur verwendet werden.

    - Update der Firmware:
        --> Nur mit Programmieradapter möglich
        1. Auf www.github.com/Wer-Wolf/Morse-Cache/releases nach neuerer Version schauen.

        2. Download der neusten Version (.zip).

        3. Extrahieren und .hex-Datei mit Programmieradapter flashen.

    - Ändern der Morsesequenz:

        - Nur mit Programmieradapter möglich. :-(
            --> Genaueres bei "Entwickler"

3. Entwickler:

    - Fuses:
        -hfuse: 0xFD

        -lfuse: 0x2A

    - Morsezeichen:

        - Adressen:

            - Start: 0x00
            - Ende (maximal): 0x3D

            - Die Zeichenkette muss (spätestens bei 0x3D) mit 0xFF abgeschlossen werden
            - Nur die Nummern 0x00 bis 0x09 (0-9) werden aktzeptiert
            - ungültige Daten werden ignoriert

            !Wenn eine neue Morsecode-Sequenz in den EEPROM geladen wurde, muss das Gerät erneut kalibriert werden!

    - Batteriekalibrierungdaten:

        - Adressem:
            Highbyte: 0x3F
            Lowbyte: 0x3E