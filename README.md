# Arduino UNO Shield LCD1602 Keypad Menu

ARDUINO IDE 2.0.3 mit Arduino UNO

Das habe ich mal zum Test zusammen kopiert. Aber heilige Scheisse, die Arduino IDE ist Müll, mehr als ein Blinklicht will man da nicht mit machen.

![](https://github.com/OttoMeister/Arduino-Shield-LCD1602-Keypad-Menu/blob/dbdcd88b2f27664ffe7300e0cf62107553492c92/Shield%20LCD1602%20Keypad%20Menu.jpeg)



Anleitung Ubuntu<br> 
#kleiner Fehler beheben<br> 
#"exec: "python": executable file not found in $PATH <br> 
sudo ln -s /usr/bin/python3 /usr/bin/python <br> 
#Arduino download <br> 
wget https://downloads.arduino.cc/arduino-ide/arduino-ide_2.0.3_Linux_64bit.AppImage <br> 
mv arduino-ide_2.0.3_Linux_64bit.AppImage ~/Schreibtisch <br> 
chmod u+x ~/Schreibtisch/arduino-ide_2.0.3_Linux_64bit.AppImage <br> 
~/Schreibtisch/arduino-ide_2.0.3_Linux_64bit.AppImage & disown <br> 
Menü -> File -> Preferences -> Language -> English <br> 
Menü -> File -> Preferences -> Show verbose output during compile und upload -> check <br> 
Menü -> File -> Preferences -> Compiler warnings -> Default <br> 
Menü -> File -> Preferences -> OK <br> 
Tools -> Board -> Bord Manager -> Arduino Uno <br> 
Tools -> Port -> /dev/ttyACM0 auswählen -> OK <br> 
# Weitere Libs installieren <br> 
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "LiquidCrystal by Adafruit" -> INSTALL ALL <br> 
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "LCDMenuLib2 by Niles Feldkämper" -> INSTALL ALL <br> 
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "DHT sensor libary by Adafruit" -> INSTALL ALL <br> 
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "Analog Buttons by AgileWare" -> INSTALL ALL <br> 

