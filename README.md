# smart_trashcan

making a "smart trashcan" dan can automatically open if there's hand near by or if you kick/knock the trash can, also it can detect if the trashcan is still empty or almost full via Blynk IoT app

Component used:
- Wemos D1 mini
- piezoelectric
- piezoelectric module
- servo motor
- 2x ultrasonic sensor
- AMS1117 (5V regulator)
- 2x 18650 battery (3.7V)
- switch


for the testing I made the trashcan using cardboard, it will look like this:

![smartTrash](https://user-images.githubusercontent.com/105662575/224465232-4e87f00f-9a2a-491c-8eb9-217286939902.jpg)


## schematic:

![schematic new](https://user-images.githubusercontent.com/105662575/224465222-6e38b0c4-1f0a-4fcf-b59c-95ffcd8f3179.png)


In the schematic it used 1 MΩ for the piezoelectric but instead I used piezoelectric module because I found it more sensitive, but you could also try with the resistor connection like the schematic image

the piezo module that I used:

![piezomodul](https://user-images.githubusercontent.com/105662575/223737358-cd96f87f-9d74-49e9-adb6-5efe5d64262e.jpg)

After that it can be soldered into a PCB and used

## Code

I will not explain how to make the Blynk app, but the think you should know is in the code I send the data to V0 virtual pin in Blynk
