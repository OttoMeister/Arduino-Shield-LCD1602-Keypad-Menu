// ============================================================
// Example:     LCDML_serialmonitor
// ============================================================
// Description:
// This example includes the complete functionality over some
// tabs. All Tabs which are started with "LCDML_display_.."
// generates an output on the display / console / ....
// This example is for the author to test the complete functionality
// ============================================================
// *********************************************************************
// special settings
// *********************************************************************
// enable this line when you are not usigng a standard arduino
// for example when your chip is an ESP or a STM or SAM or something else

//#define _LCDML_cfg_use_ram

// *********************************************************************
// includes
// *********************************************************************
#include <LiquidCrystal.h>
#include <LCDMenuLib2.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 13 
#define DHTTYPE    DHT11 
DHT_Unified dht(DHTPIN, DHTTYPE);

// *********************************************************************
// LCDML display settings
// *********************************************************************
// settings for LCD
#define _LCDML_DISP_cols 16
#define _LCDML_DISP_rows 2

// enable this line (set to 1) to show a header above the first menu element
// this function can be changed in LCDML_display_menu tab
#define _LCDML_DSIP_use_header 0

#define _LCDML_DISP_cfg_cursor 0x7E  // cursor Symbol
#define _LCDML_DISP_cfg_scrollbar 1  // enable a scrollbar

// LCD object
// liquid crystal needs (rs, e, dat4, dat5, dat6, dat7)
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const uint8_t scroll_bar[5][8] = {
  { B10001, B10001, B10001, B10001, B10001, B10001, B10001, B10001 },  // scrollbar top
  { B11111, B11111, B10001, B10001, B10001, B10001, B10001, B10001 },  // scroll state 1
  { B10001, B10001, B11111, B11111, B10001, B10001, B10001, B10001 },  // scroll state 2
  { B10001, B10001, B10001, B10001, B11111, B11111, B10001, B10001 },  // scroll state 3
  { B10001, B10001, B10001, B10001, B10001, B10001, B11111, B11111 }   // scrollbar bottom
};

// *********************************************************************
// Prototypes
// *********************************************************************
void lcdml_menu_display();
void lcdml_menu_clear();
void lcdml_menu_control();

// *********************************************************************
// Global variables
// *********************************************************************


// *********************************************************************
// Objects
// *********************************************************************
LCDMenuLib2_menu LCDML_0(255, 0, 0, NULL, NULL);  // root menu element (do not change)
LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows - _LCDML_DSIP_use_header, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);

// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************
// LCDML_0        => layer 0
// LCDML_0_X      => layer 1
// LCDML_0_X_X    => layer 2
// LCDML_0_X_X_X  => layer 3
// LCDML_0_...      => layer ...

// For beginners
// LCDML_add(id, prev_layer, new_num, lang_char_array, callback_function)
LCDML_add(0, LCDML_0, 1, "Information", mFunc_information);                 // this menu function can be found on "LCDML_display_menuFunction" tab
LCDML_add(1, LCDML_0, 2, "Time info", mFunc_timer_info);                    // this menu function can be found on "LCDML_display_menuFunction" tab
LCDML_add(2, LCDML_0, 3, "Special", NULL);                                 // NULL = no menu function
LCDML_add(3, LCDML_0_3, 1, "Go to Root", mFunc_goToRootMenu);              // this menu function can be found on "LCDML_display_menuFunction" tab
LCDML_add(4, LCDML_0_3, 2, "Jump to Time info", mFunc_jumpTo_timer_info);  // this menu function can be found on "LCDML_display_menuFunction" tab
LCDML_add(5, LCDML_0_3, 3, "Back", mFunc_back);                            // this menu function can be found on "LCDML_display_menuFunction" tab


// Example for dynamic content
// 1. set the string to ""
// 2. use type  _LCDML_TYPE_dynParam   instead of    _LCDML_TYPE_default
// this function type can not be used in combination with different parameters
// LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
LCDML_addAdvanced(6, LCDML_0, 4, NULL, "", mDyn_para, 0, _LCDML_TYPE_dynParam);  // NULL = no menu function

// Example for conditions (for example for a screensaver)
// 1. define a condition as a function of a boolean type -> return false = not displayed, return true = displayed
// 2. set the function name as callback (remove the braces '()' it gives bad errors)
// LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
LCDML_addAdvanced(7, LCDML_0, 5, COND_hide, "screensaver", mFunc_screensaver, 0, _LCDML_TYPE_default);  // this menu function can be found on "LCDML_display_menuFunction" tab

// ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

// menu element count - last element id
// this value must be the same as the last menu element
#define _LCDML_DISP_cnt 7



// create menu
LCDML_createMenu(_LCDML_DISP_cnt);

// *********************************************************************
// SETUP
// *********************************************************************
void setup() {
  // serial init; only be needed if serial control is used
  // start serial
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("Booting...");

  Serial.println(F(_LCDML_VERSION));  // only for examples
 // Initialize device.
  dht.begin();
  Serial.println(F("DHT Unified Sensor"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));

  

  // LCD Begin
  lcd.begin(_LCDML_DISP_cols, _LCDML_DISP_rows);
  // set special chars for scrollbar
  lcd.createChar(0, (uint8_t*)scroll_bar[0]);
  lcd.createChar(1, (uint8_t*)scroll_bar[1]);
  lcd.createChar(2, (uint8_t*)scroll_bar[2]);
  lcd.createChar(3, (uint8_t*)scroll_bar[3]);
  lcd.createChar(4, (uint8_t*)scroll_bar[4]);

  // LCDMenuLib Setup
  LCDML_setup(_LCDML_DISP_cnt);

  // Some settings which can be used

  // Enable Menu Rollover
  LCDML.MENU_enRollover();

  // Enable Screensaver (screensaver menu function, time to activate in ms)
  LCDML.SCREEN_enable(mFunc_screensaver, 10000);  // set to 10 seconds
  //LCDML.SCREEN_disable();

  // Some needful methods

  // You can jump to a menu function from anywhere with
  //LCDML.OTHER_jumpToFunc(mFunc_p2); // the parameter is the function name
}


// *********************************************************************
boolean COND_hide()  // hide a menu element
// *********************************************************************
{
  return false;  // hidden
}


#include <AnalogButtons.h>  // 1.2.0by AgileWare
#define ANALOG_PIN A0


AnalogButtons analogButtons(ANALOG_PIN, INPUT);
//RIGHT=0,UP=133,DOWN=309,LEFT=481,SELECT=721,NONE=1023
Button b1 = Button(0, &RightClick, &RightHold);
Button b2 = Button(133, &UpClick, &UpHold);
Button b3 = Button(309, &DownClick, &DownHold);
Button b4 = Button(481, &LeftClick, &LeftHold);
Button b5 = Button(721, &SelectClick, &SelectHold);
void RightClick() {
  LCDML.BT_right();
  Serial.println("button Right clicked");
}
void RightHold() {
  LCDML.BT_right();
  Serial.println("button Right held");
}
void UpClick() {
  LCDML.BT_up();
  Serial.println("button UP clicked");
}
void UpHold() {
  LCDML.BT_up();
  Serial.println("button UP held");
}
void DownClick() {
  LCDML.BT_down();
  Serial.println("button DOWN clicked");
}
void DownHold() {
  LCDML.BT_down();
  Serial.println("button DOWN held");
}
void LeftClick() {
  LCDML.BT_left();
  Serial.println("button Left clicked");
}
void LeftHold() {
  LCDML.BT_left();
  Serial.println("button Left held");
}
void SelectClick() {
  LCDML.BT_enter();
  Serial.println("button Select clicked");
}
void SelectHold() {
  LCDML.BT_enter();
  Serial.println("button Select held");
}

void lcdml_menu_control(void) {
  if (LCDML.BT_setup()) {
    analogButtons.add(b1);
    analogButtons.add(b2);
    analogButtons.add(b3);
    analogButtons.add(b4);
    analogButtons.add(b5);
  }
  analogButtons.check();
}
/* ===================================================================== *
 *                                                                       *
 * Dynamic content                                                       *
 *                                                                       *
 * ===================================================================== *
 */
#include <EEPROM.h>

uint8_t g_dynParam = 100;  // when this value comes from an EEPROM, load it in setup
           

void mDyn_para(uint8_t line)
// *********************************************************************
{
  //lesenn von EEPROM
  g_dynParam = EEPROM.read(0);
  // check if this function is active (cursor stands on this line)
  if (line == LCDML.MENU_getCursorPos()) {
    // make only an action when the cursor stands on this menu item
    //check Button
    if (LCDML.BT_checkAny()) {
      if (LCDML.BT_checkEnter()) {
        // this function checks returns the scroll disable status (0 = menu scrolling enabled, 1 = menu scrolling disabled)
        if (LCDML.MENU_getScrollDisableStatus() == 0) {
          // disable the menu scroll function to catch the cursor on this point
          // now it is possible to work with BT_checkUp and BT_checkDown in this function
          // this function can only be called in a menu, not in a menu function
          LCDML.MENU_disScroll();
        } else {
          // enable the normal menu scroll function
          LCDML.MENU_enScroll();
        }
      }
      // This check have only an effect when MENU_disScroll is set
      if (LCDML.BT_checkUp()) {
        g_dynParam++;
      }
      // This check have only an effect when MENU_disScroll is set
      if (LCDML.BT_checkDown()) {
        g_dynParam--;
      }
      if (LCDML.BT_checkRight()) {
        if (g_dynParam > 19)
          g_dynParam = g_dynParam + 10;
        else g_dynParam++;
      }
      if (LCDML.BT_checkLeft()) {
        if (g_dynParam > 20)
          g_dynParam = g_dynParam - 10;
        else g_dynParam--;
      }
      // Display Helligkeit
      analogWrite(10, g_dynParam);
    }
  }
  char buf[20];
  sprintf(buf, "Dim: %d", g_dynParam);
  // use the line from function parameters
  lcd.setCursor(1, (line + _LCDML_DSIP_use_header));
  lcd.print(buf);
  // Speichern nach EEPROM
  EEPROM.write(0, g_dynParam);
}
// =====================================================================
//
// Output function
//
// =====================================================================

/* ******************************************************************** */
void lcdml_menu_clear()
/* ******************************************************************** */
{
  lcd.clear();
  lcd.setCursor(0, 0);
  analogWrite(10, EEPROM.read(0));   
}

/* ******************************************************************** */
void lcdml_menu_display()
/* ******************************************************************** */
{
  // update content
  // ***************
  if (LCDML.DISP_checkMenuUpdate()) {
    // clear menu
    // ***************
    LCDML.DISP_clear();

    // declaration of some variables
    // ***************
    // content variable
    char content_text[_LCDML_DISP_cols];  // save the content text of every menu element
    // menu element object
    LCDMenuLib2_menu *tmp;
    // some limit values
    uint8_t i = LCDML.MENU_getScroll();
    uint8_t maxi = (_LCDML_DISP_rows - _LCDML_DSIP_use_header) + i;
    uint8_t n = 0;

    // Display a header with the parent element name
    if(_LCDML_DSIP_use_header > 0)
    {
      // only one line
      if(LCDML.MENU_getLayer() == 0)
      {
        // this text is displayed when no header is available
        lcd.setCursor(0,0);
        lcd.print(F("Root Menu"));       
      }
      else
      {
        // Display parent name
        LCDML_getContent(content_text, LCDML.MENU_getParentID());
        lcd.setCursor(0,0);
        lcd.print(content_text);       
      }        
    }      

    // check if this element has children
    if ((tmp = LCDML.MENU_getDisplayedObj()) != NULL)
    {
      // loop to display lines
      do
      {
        // check if a menu element has a condition and if the condition be true
        if (tmp->checkCondition())
        {
          // check the type off a menu element
          if(tmp->checkType_menu() == true)
          {
            // display normal content
            LCDML_getContent(content_text, tmp->getID());
            lcd.setCursor(1, (n+_LCDML_DSIP_use_header));
            lcd.print(content_text);
          }
          else
          {
            if(tmp->checkType_dynParam()) {
              tmp->callback(n);
            }
          }
          // increment some values
          i++;
          n++;
        }
      // try to go to the next sibling and check the number of displayed rows
      } while (((tmp = tmp->getSibling(1)) != NULL) && (i < maxi));
    }
  }

  if(LCDML.DISP_checkMenuCursorUpdate())
  {
    // init vars
    uint8_t n_max             = (LCDML.MENU_getChilds() >= (_LCDML_DISP_rows - _LCDML_DSIP_use_header)) ? (_LCDML_DISP_rows - _LCDML_DSIP_use_header) : (LCDML.MENU_getChilds());
    uint8_t scrollbar_min     = 0;
    uint8_t scrollbar_max     = LCDML.MENU_getChilds();
    uint8_t scrollbar_cur_pos = LCDML.MENU_getCursorPosAbs();
    uint8_t scroll_pos        = ((1.*n_max * (_LCDML_DISP_rows - _LCDML_DSIP_use_header)) / (scrollbar_max - 1) * scrollbar_cur_pos);


    // display rows
    for (uint8_t n = 0; n < n_max; n++)
    {
      //set cursor
      lcd.setCursor(0, (n+_LCDML_DSIP_use_header));

      //set cursor char
      if (n == LCDML.MENU_getCursorPos()) {
        lcd.write(_LCDML_DISP_cfg_cursor);
      } else {
        lcd.write(' ');
      }

      // delete or reset scrollbar
      if (_LCDML_DISP_cfg_scrollbar == 1) {
        if (scrollbar_max > n_max) {
          lcd.setCursor((_LCDML_DISP_cols - 1), (n+_LCDML_DSIP_use_header));
          lcd.write((uint8_t)0);
        }
        else {
          lcd.setCursor((_LCDML_DISP_cols - 1), (n+_LCDML_DSIP_use_header));
          lcd.print(' ');
        }
      }
    }

    // display scrollbar
    if (_LCDML_DISP_cfg_scrollbar == 1) {
      if (scrollbar_max > n_max) {
        //set scroll position
        if (scrollbar_cur_pos == scrollbar_min) {
          // min pos
          lcd.setCursor((_LCDML_DISP_cols - 1), (0+_LCDML_DSIP_use_header));
          lcd.write((uint8_t)1);
        } else if (scrollbar_cur_pos == (scrollbar_max - 1)) {
          // max pos
          lcd.setCursor((_LCDML_DISP_cols - 1), (n_max - 1 + _LCDML_DSIP_use_header));
          lcd.write((uint8_t)4);
        } else {
          // between
          lcd.setCursor((_LCDML_DISP_cols - 1), (scroll_pos / n_max + _LCDML_DSIP_use_header));
          lcd.write((uint8_t)(scroll_pos % n_max) + 1);
        }
      }
    }
  }
}


// *********************************************************************
void mFunc_information(uint8_t param)
// *********************************************************************
{
  if (LCDML.FUNC_setup())  // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup function
    lcd.setCursor(0, 0);
    lcd.print(F("DHT11 Sensor"));
    lcd.setCursor(0, 1);
    lcd.print(F("Press any button"));
  }

  if (LCDML.FUNC_loop())  // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if (LCDML.BT_checkAny()) {  // check if any button is pressed (enter, up, down, left, right)
      // LCDML_goToMenu stops a running menu function and goes to the menu
      LCDML.FUNC_goBackToMenu();
    }
  }

  if (LCDML.FUNC_close())  // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}


// *********************************************************************
uint8_t g_func_timer_info = 0;  // time counter (global variable)
unsigned long g_timer_1 = 0;    // timer variable (global variable)
void mFunc_timer_info(uint8_t param)
// *********************************************************************
{
  if (LCDML.FUNC_setup())  // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    lcd.print(F("5 sec wait"));       // print some content on first row
    g_func_timer_info = 5;            // reset and set timer
    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds

    LCDML.TIMER_msReset(g_timer_1);
  }


  if (LCDML.FUNC_loop())  // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function

    // reset screensaver timer
    LCDML.SCREEN_resetTimer();

    // this function is called every 100 milliseconds

    // this method checks every 1000 milliseconds if it is called
    if (LCDML.TIMER_ms(g_timer_1, 1000)) {
      g_func_timer_info--;  // increment the value every second
      lcd.setCursor(0, 0);  // set cursor pos
      lcd.print(F("  "));
      lcd.setCursor(0, 0);           // set cursor pos
      lcd.print(g_func_timer_info);  // print the time counter value
    }
    lcd.setCursor(0, 1);  // set cursor
    lcd.print(millis());
    // this function can only be ended when quit button is pressed or the time is over
    // check if the function ends normally
    if (g_func_timer_info <= 0) {
      // leave this function
      LCDML.FUNC_goBackToMenu();
    }
  }

  if (LCDML.FUNC_close())  // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}


// *********************************************************************
uint8_t g_button_value = 0;  // button value counter (global variable)
void mFunc_p2(uint8_t param)
// *********************************************************************
{
  if (LCDML.FUNC_setup())  // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup function
    // print LCD content
    // print LCD content
    lcd.setCursor(0, 0);
    lcd.print(F("press left or up"));
    lcd.setCursor(0, 1);
    lcd.print(F("count: 0 of 3"));
    // Reset Button Value
    g_button_value = 0;

    // Disable the screensaver for this function until it is closed
    LCDML.FUNC_disableScreensaver();
  }

  if (LCDML.FUNC_loop())  // ****** LOOP *********
  {
    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if (LCDML.BT_checkAny())  // check if any button is pressed (enter, up, down, left, right)
    {
      if (LCDML.BT_checkLeft() || LCDML.BT_checkUp())  // check if button left is pressed
      {
        LCDML.BT_resetLeft();  // reset the left button
        LCDML.BT_resetUp();    // reset the left button
        g_button_value++;

        // update LCD content
        // update LCD content
        lcd.setCursor(7, 1);        // set cursor
        lcd.print(g_button_value);  // print change content
      }
    }

    // check if button count is three
    if (g_button_value >= 3) {
      LCDML.FUNC_goBackToMenu();  // leave this function
    }
  }

  if (LCDML.FUNC_close())  // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}



// *********************************************************************
void mFunc_screensaver(uint8_t param)
// *********************************************************************
{
  if (LCDML.FUNC_setup())  // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds
  }
  if (LCDML.FUNC_loop()) {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    sensors_event_t event;
    lcd.setCursor(0, 0);  // set cursor
    dht.temperature().getEvent(&event);  // Get temperature event and print its value.
    if (isnan(event.temperature)) {
      lcd.println(F("Error temp.!    "));
    } else {
      lcd.print(F("Temp.:     "));
      lcd.print(event.temperature,1);
      lcd.println(F("C"));
    }
    lcd.setCursor(0, 1);  // set cursor
    dht.humidity().getEvent(&event);    // Get humidity event and print its value.
    if (isnan(event.relative_humidity)) {
      lcd.println(F("Error humidity! "));
    } else {
      lcd.print(F("Humidity:    "));
      lcd.print(event.relative_humidity,0);
      lcd.println(F("%"));
    }
    if (LCDML.BT_checkAny())      // check if any button is pressed (enter, up, down, left, right)
      LCDML.FUNC_goBackToMenu();  // leave this function
  }
  if (LCDML.FUNC_close()) {
    LCDML.MENU_goRoot();
  }
}



// *********************************************************************
void mFunc_back(uint8_t param)
// *********************************************************************
{
  if (LCDML.FUNC_setup())  // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);
    // end function and go an layer back
    LCDML.FUNC_goBackToMenu(1);  // leave this function and go a layer back
  }
}


// *********************************************************************
void mFunc_goToRootMenu(uint8_t param)
// *********************************************************************
{
  if (LCDML.FUNC_setup())  // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // go to root and display menu
    LCDML.MENU_goRoot();
  }
}

// *********************************************************************
void mFunc_jumpTo_timer_info(uint8_t param)
// *********************************************************************
{
  if (LCDML.FUNC_setup())  // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    LCDML.OTHER_jumpToFunc(mFunc_timer_info);
  }
}


// *********************************************************************
// LOOP
// *********************************************************************
void loop() {
  LCDML.loop();
}