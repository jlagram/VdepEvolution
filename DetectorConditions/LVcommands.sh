#!/bin/sh

python3 computeTimeFractionOn.py LV LV-Info/Data/TIB_minus_1.1.1.1_LV_Analog_Mar2010-2022.csv  LV-Info/FractionOn/LV-history-TIB.txt  > LV-Info/FractionOn/log-LV-TIB.txt
python3 computeTimeFractionOn.py LV LV-Info/Data/TIB_minus_2.1.1.1_LV_Analog_Mar2010-2022.csv  LV-Info/FractionOn/LV-history-TIB2.txt  > LV-Info/FractionOn/log-LV-TIB2.txt
python3 computeTimeFractionOn.py LV LV-Info/Data/TOB_plus_1.1.1.1_LV_Analog_Mar2010-2022.csv  LV-Info/FractionOn/LV-history-TOB.txt  > LV-Info/FractionOn/log-LV-TOB.txt
python3 computeTimeFractionOn.py LV LV-Info/Data/TOB_plus_2.1.1.1_LV_Analog_Mar2010-2022.csv  LV-Info/FractionOn/LV-history-TOB2.txt  > LV-Info/FractionOn/log-LV-TOB2.txt
python3 computeTimeFractionOn.py LV LV-Info/Data/TID_plus_1.1.1.1_LV_Analog_Mar2010-2022.csv  LV-Info/FractionOn/LV-history-TID.txt  > LV-Info/FractionOn/log-LV-TID.txt
python3 computeTimeFractionOn.py LV LV-Info/Data/TEC_minus_1.1.1.1.1_LV_Analog_Mar2010-2022.csv  LV-Info/FractionOn/LV-history-TEC.txt  > LV-Info/FractionOn/log-LV-TEC.txt

python3 mergeChannelsHistory.py LV

