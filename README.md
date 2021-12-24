# 1_Semester_Arduino_Opgave
1 semester - Sundheds- og velfærdsteknologi - SDU Odense

Hovedopgave som danner grundlag for eksaminationen
Denne opgave kan betragtes som en videreførelse af opgave 4:
a)
Skriv et Arduino-program, som indlæser data fra temperatursensoren på det udleverede  eksperiment-shield, via analog input (AO). Temperaturen skal udskrives på monitor.
Hvis temperaturen stiger med mere end 4 oC over rumtemperatur, skal dette alarmeres med blink på LED D1, D2, D3 og D4.

b)
Udvid programmet således at de fire LED først holder op med at blinke når temperaturen er faldet til 1 oC over rumtemperatur (det har betegnelsen en hysterese på 3 oC).

c)
Udvid programmet, således at en LED tilsluttet port 9 vil lyse med en variabel intensitet som følger den målte temperatur inden for et passende område, f.eks. slukket eller meget svagt lysende ved rumtemperatur og stigende op til max. lysintensitet ved en temperaturstigning på 4 oC.

d)
Udvid programmet, således at det sampler temperatur, med en frekvens på 5 Hertz (200 ms interval). Samplede værdier lægges løbende i et array med en konstant længde 50 elementer.
Programmet skal kontinuerligt udskrive følgende på monitor:
-	Middelværdien af temperaturen for de seneste 10 sekunder.
-	Maksimal og minimal værdi som har optrådt i de seneste 10 sekunder. 
-	Standardafvigelse for de seneste 10 sekunder.

