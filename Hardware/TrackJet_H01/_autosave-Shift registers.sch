EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L 74xx:74HC595 U17
U 1 1 60222614
P 3300 3000
F 0 "U17" H 3100 3700 50  0000 C CNN
F 1 "74HC595" H 3050 3600 50  0000 C CNN
F 2 "Package_SO:SOIC-16_3.9x9.9mm_P1.27mm" H 3300 3000 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 3300 3000 50  0001 C CNN
	1    3300 3000
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 6022B3C3
P 3300 2400
F 0 "#PWR?" H 3300 2250 50  0001 C CNN
F 1 "+3V3" H 3315 2573 50  0000 C CNN
F 2 "" H 3300 2400 50  0001 C CNN
F 3 "" H 3300 2400 50  0001 C CNN
	1    3300 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6022C2F9
P 3300 3700
F 0 "#PWR?" H 3300 3450 50  0001 C CNN
F 1 "GND" H 3305 3527 50  0000 C CNN
F 2 "" H 3300 3700 50  0001 C CNN
F 3 "" H 3300 3700 50  0001 C CNN
	1    3300 3700
	1    0    0    -1  
$EndComp
Text HLabel 2900 2600 0    50   Input ~ 0
REG_DAT
Text HLabel 2900 2800 0    50   Input ~ 0
REG_CLK
Text HLabel 2900 3200 0    50   Input ~ 0
REG_OE
Text HLabel 2900 3100 0    50   Input ~ 0
REG_LAT
$Comp
L power:+3V3 #PWR?
U 1 1 6022D229
P 2450 2900
F 0 "#PWR?" H 2450 2750 50  0001 C CNN
F 1 "+3V3" H 2465 3073 50  0000 C CNN
F 2 "" H 2450 2900 50  0001 C CNN
F 3 "" H 2450 2900 50  0001 C CNN
	1    2450 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 2900 2450 2900
$Comp
L 74xx:74HC595 U?
U 1 1 60230575
P 3300 4900
F 0 "U?" H 3100 5600 50  0000 C CNN
F 1 "74HC595" H 3050 5500 50  0000 C CNN
F 2 "Package_SO:SOIC-16_3.9x9.9mm_P1.27mm" H 3300 4900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 3300 4900 50  0001 C CNN
	1    3300 4900
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 6023057B
P 3300 4300
F 0 "#PWR?" H 3300 4150 50  0001 C CNN
F 1 "+3V3" H 3315 4473 50  0000 C CNN
F 2 "" H 3300 4300 50  0001 C CNN
F 3 "" H 3300 4300 50  0001 C CNN
	1    3300 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60230581
P 3300 5600
F 0 "#PWR?" H 3300 5350 50  0001 C CNN
F 1 "GND" H 3305 5427 50  0000 C CNN
F 2 "" H 3300 5600 50  0001 C CNN
F 3 "" H 3300 5600 50  0001 C CNN
	1    3300 5600
	1    0    0    -1  
$EndComp
Text HLabel 2900 4700 0    50   Input ~ 0
REG_CLK
Text HLabel 2900 5100 0    50   Input ~ 0
REG_OE
Text HLabel 2900 5000 0    50   Input ~ 0
REG_LAT
$Comp
L power:+3V3 #PWR?
U 1 1 6023058B
P 2450 4800
F 0 "#PWR?" H 2450 4650 50  0001 C CNN
F 1 "+3V3" H 2465 4973 50  0000 C CNN
F 2 "" H 2450 4800 50  0001 C CNN
F 3 "" H 2450 4800 50  0001 C CNN
	1    2450 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 4800 2450 4800
Wire Wire Line
	2900 4500 2650 4500
Wire Wire Line
	2650 4500 2650 4000
Wire Wire Line
	2650 4000 3700 4000
Wire Wire Line
	3700 4000 3700 3500
Text HLabel 3700 2600 2    50   Input ~ 0
REG_OUT_1
Text HLabel 3700 2700 2    50   Input ~ 0
REG_OUT_2
Text HLabel 3700 2800 2    50   Input ~ 0
REG_OUT_3
Text HLabel 3700 2900 2    50   Input ~ 0
REG_OUT_4
Text HLabel 3700 3000 2    50   Input ~ 0
REG_OUT_5
Text HLabel 3700 3100 2    50   Input ~ 0
REG_OUT_6
Text HLabel 3700 3200 2    50   Input ~ 0
REG_OUT_7
Text HLabel 3700 3300 2    50   Input ~ 0
REG_OUT_8
Text HLabel 3700 4500 2    50   Input ~ 0
REG_OUT_9
Text HLabel 3700 4600 2    50   Input ~ 0
REG_OUT_10
Text HLabel 3700 4700 2    50   Input ~ 0
REG_OUT_11
Text HLabel 3700 4800 2    50   Input ~ 0
REG_OUT_12
Text HLabel 3700 4900 2    50   Input ~ 0
REG_OUT_13
Text HLabel 3700 5000 2    50   Input ~ 0
REG_OUT_14
Text HLabel 3700 5100 2    50   Input ~ 0
REG_OUT_15
Text HLabel 3700 5200 2    50   Input ~ 0
REG_OUT_16
NoConn ~ 3700 5400
$EndSCHEMATC
