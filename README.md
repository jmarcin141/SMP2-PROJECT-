## SMP2-PROJECT-Engine steering by IRDA remote control. 
<p align="center">
PROJECT - CONSPECT
</p>

## 	QUICK INTRO<p>[EN]</p>
## => Design Assumptions:
- 	IRDA Remote control.
	4 buttons. 
	Rotate - Left, Right
	Speed adjustment - Up, Down.

- 	KL05 board.
	configuration Ports, PWM, UART.

- Engine.
Universal DC - Power supply 5V,
H-bridge integrated Circuit.

## => Implementation:
	Universal assembly board, pinout for controls, 
	
	
## 	<p>[PL]</p>	
## Opis szczegółowy:
	Celem naszego projektu jest rozbudowa silnika prądu stałego sterowanego przez pilot IRDA.
	Regulacja prędkości obrotów za pomocą PWM i kilku przycisków na pilocie IR.

## Analiza problemu:
	
	Nasz projekt będzie modyfikacją i dalszą rozbudową sterowania silnikiem prądu stałego. Początkiem
	naszej pracy będzie modyfikacja działania układu z ATmegi32 na Kl05.

<p align="center">
	<img src="https://github.com/jmarcin141/SMP2-PROJECT-/blob/master/attachments/rys1.jpg" width="85%" />
</p>

## Plan realizacji:
	Pierwszym naszym zadaniem będzie odpowiednia konfiguracja PWM-a na płytce KL05 i sprawdzenie
	poprawności działania silnika prądu stałego(w oparciu o mostek H).Kolejnym krokiem będzie
	wykonanie sterowania prędkości obrotów wykorzystując pilot IRDA, odbiornik podczerwieni
	TSOP2236 - 36kHz, oraz w planach komunikacja poprzez interfejs szeregowy (UART)-wysyłając dane z
	programu o interfejsie graficznym (Labview).

<p align="center">
	<img src="https://github.com/jmarcin141/SMP2-PROJECT-/blob/master/attachments/rys2.jpg" width="70%" />
</p>

## Authors:
	Grzegorz Jurek,
	Jakub Marcinkowski
		 
