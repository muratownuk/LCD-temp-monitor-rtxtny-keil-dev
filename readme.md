# LCD-temp-monitor-rtxtny-keil-dev 

**Google-Drive URL:**
```
 https://drive.google.com/drive/folders/1ewdTUWSF-nkjTB1uiyQlwgvCL1-rwyUh?usp=sharing  
```

**Demonstration Video URL:**
```
 https://www.youtube.com/watch?v=rb25G0KUSpk 
``` 

### Project Function  
This project monitors the temperature of the C8051F005 MCU via the on-board 
temperature sensor. If the temperature is between 20 degrees celsius and 40 
degrees celsius, an indication of temperature in \"OK\" region is portrayed 
through a green LED. If the temperature is below or above this region (min, 
max), a red LED is used to indicate it. These min and max temperatures can be 
modified through the program via keypad. 

This project is coded with RTX51 Tiny RTOS Tasks. The tasks in the program are 
INIT, ADC, LCD and KEYPAD. 

INIT task is run automatically due to RTOS functions and is used to initialize 
and configure the MCU peripherals used in the project and initialize the LCD. 
Once these are intialized, the OS will create and run the rest of the tasks 
and delete itself. 

The ADC task will convert the analog temperature measurement to digital. The 
resulting temperature will be calculated using the temperature transfer 
function found in the MCU datasheet. 

The LCD task will write the resulting temperature to the LCD and indicate if 
it is in the \"OK\" region or exceeds it. 

The KEYPAD tasks function is to modify the min/max temperature range of the 
project. The min of 20 and max of 40 can be set to different values, so the 
programs reaction will be for these different set values.

The RTOS in this program uses signaling method. 



