# Arduino UNO Shield LCD1602 Keypad Menu

ARDUINO IDE 2.0.3 mit Arduino UNO

Das habe ich mal zum Test zusammen kopiert. Aber heilige Scheisse, die Arduino IDE ist Müll, mehr als ein Blinklicht will man da nicht mit machen.

![](https://github.com/OttoMeister/Arduino-Shield-LCD1602-Keypad-Menu/blob/dbdcd88b2f27664ffe7300e0cf62107553492c92/Shield%20LCD1602%20Keypad%20Menu.jpeg)



Anleitung Ubuntu
#kleiner Fehler beheben
#"exec: "python": executable file not found in $PATH 
sudo ln -s /usr/bin/python3 /usr/bin/python
#Arduino download
wget https://downloads.arduino.cc/arduino-ide/arduino-ide_2.0.3_Linux_64bit.AppImage
mv arduino-ide_2.0.3_Linux_64bit.AppImage ~/Schreibtisch
chmod u+x ~/Schreibtisch/arduino-ide_2.0.3_Linux_64bit.AppImage
~/Schreibtisch/arduino-ide_2.0.3_Linux_64bit.AppImage & disown
Menü -> File -> Preferences -> Language -> English
Menü -> File -> Preferences -> Show verbose output during compile und upload -> check
Menü -> File -> Preferences -> Compiler warnings -> Default
Menü -> File -> Preferences -> OK
Tools -> Board -> Bord Manager -> Arduino Uno
Tools -> Port -> /dev/ttyACM0 auswählen -> OK
# Weitere Libs installieren
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "LiquidCrystal by Adafruit" -> INSTALL ALL
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "LCDMenuLib2 by Niles Feldkämper" -> INSTALL ALL
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "DHT sensor libary by Adafruit" -> INSTALL ALL
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "Analog Buttons by AgileWare" -> INSTALL ALL

