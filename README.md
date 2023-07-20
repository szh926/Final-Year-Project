Link to University Git Repository (HTTS): https://git.cs.bham.ac.uk/projects-2022-23/szh926

Link to University Git Reository (SSH): git@git.cs.bham.ac.uk:projects-2022-23/szh926.git 

# Final-Year-Project :- RFID Tag Cloner
This repository contains all code created for my final year project. This is an Arduino-based project.

This project can be used to create and RFID Cloner and does the following:
  - Reads data off an RFID card presented
  - Stores the data to a file on an SD card
  - Writes a selected clone to another presented RFID tag
  - Renames a selected saved clone
  - Deletes a selected saved clone

## Development
  **This project is was completed on April 13th.**

## Compatibility
This project was created using ``Arduino IDE version 2.0.3`` and cannot be guaranteed to be supported on any older versions.

## Components
  - 1 x Arduino UNO (plus USB A-to USB B cable for uploading code)
  - 1 x LCD1602 Module (with pin header)
  - 1 x MFRC522 RFID Reader Module
  - 4 x Pushbuttons
  - 1 x MicroSD Card Adater
  - 1 x MicroSD card (minimum 1GB)
  - 1 x 330 Ohm Resistor
  - 1 x 830 Tie-Points Breadboard
  - 7 x Female-to-Male Dupont Wire
  - 35 x Breadboard Jumper Wire
  - 1 x Potentiometer 10k (optional for controlling LCD screen)
  - 1 x 9V battery with Snap-on Connector (optional for powering board when not connected to computer)

## Hardware Setup
The following is the hardware layout for setup (also available in main):

![Hardware Layout](Hardware Layout.pdf)

### Pin Layout
The following table shows the typical pin layout used:

|                            |              | Arduino UNO Pin |
| ------------               | ------------ | ------------    |
| **MFRC522 (RFID Reader)**  | 3.3V         | 3.3V            |
|                            | RST          | 9               |
|                            | GND          | GND             |
|                            | MISO         | 12              |
|                            | MOSI         | 11              |
|                            | SCK          | 13              |
|                            | SDA          | 10              |
| **MicroSD Card Adpater**   | CS           | 8               |
|                            | SCK          | 13              |
|                            | MOSI         | 11              |
|                            | MISO         | 12 (via 330 Ohm Reistor)|
|                            | VCC          | 5V              |
|                            | GND          | GND             |
| **Liquid Crystal Display** | VSS (GND)    | GND             |
|                            | VDD (5V)     | 5V              | 
|                            | V0           | 5               |
|                            | RS           | A5              |
|                            | RW           | GND             |
|                            | E            | A4              |
|                            | D4           | A3              |
|                            | D5           | A2              |
|                            | D6           | A1              |
|                            | D7           | A0              |
|                            | A            | 5V              |
|                            | K            | GND             |
| **Left Button**            | Left Pin     | GND             |
|                            | Right Pin    | 0               | 
| **Select Button**          | Left Pin     | GND             |
|                            | Right Pin    | 1               | 
| **Right Button**           | Left Pin     | GND             |
|                            | Right Pin    | 2               | 
| **Back Button**            | Left Pin     | GND             |
|                            | Right Pin    | 3               | 

Important: If your micro controller supports multiple SPI interfaces, the library only uses the default (first) SPI of the Arduino framework.

### Technical Issues
  Test files have been created for each component used in the project to test its functionality and any potential faults with the hardware.
  
  When using a test file, follow the Hardware setup given in the comments at the top of the file. The project should work if the setup has been followed correctly. If not, it is likely the hardware you are trying to use is broken or faulty, **and must be replaced.**

  If a component must be replaced, please folow the Hardware Setup section of this README.md file.

## Software Setup

### Installing Libraries
Any libraires that haven't been included in this file should be bundled with every Arduino Platform. 

To use the external libraries provided, please add them to the "library" folder in ``C:\Users\username\Documents\Arduino``

Alternatively, download them manually from the Arduino IDE Library Manager.

### MicroSD card Setup
To allow the MicroSD card to perform properly for this project, please format it to the FAT16 or FAT32 file format. Once this is complete, create an empty csv file in the main directory named "CLONES.csv" or "CLONES2.csv"

## Running Code
Once setup is complete, run the code by uploading ``main.ino`` to the connected Arduino board. This can be done using the Arduino IDE.

### Using the Project
If everything is set up according to the Hardware Setup and Software Setup Sections, the project will be operable. To control the menu presented on the display, use the corresponding buttons and when promtped to use present a card, present it in close proxitmity with the RC522 RFID Reader. If any errors occur, please refer back to this README and any comments within the testPrograms.
