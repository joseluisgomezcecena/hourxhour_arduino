# Arduino based sensor

This will describe how to use program the Arduino circuits in duralock site A and B

## Installation

First install arduino IDE (integrated development enviroment) found here: 
[Download Arduino IDE](https://www.arduino.cc/en/software)

1. Open the file for the needed circuit (duralock side a or duralock side b)

2. Then connect a serial cable to the arduino 

3. Click upload sketch and wait for upload to be done

4. All Done!

## Known bug

if for some reason the arduino wont connect to the network after 3 or attepmts you will have to edit this line just change one letter of this string and it will work.

```bash
byte mac[] = { 0xDE, 0xAD, 0xDB, 0xEF, 0xFE, 0xFD }; //if it doesnt have a sticker just make it up as long no device on your network has this same mac you should be fine

```


