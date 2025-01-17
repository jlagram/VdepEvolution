#!/bin/sh

python3 computeTimeFractionOn.py HV HV-Info/Data/TIB_minus_1.1.1.1_HV1_Mar2010-2022.csv  HV-Info/FractionOn/HV-history-TIB.txt  > HV-Info/FractionOn/log-HV-TIB.txt
python3 computeTimeFractionOn.py HV HV-Info/Data/TIB_minus_2.1.1.1_HV1_Mar2010-2022.csv  HV-Info/FractionOn/HV-history-TIB2.txt  > HV-Info/FractionOn/log-HV-TIB2.txt
python3 computeTimeFractionOn.py HV HV-Info/Data/TOB_plus_1.1.1.1_HV1_Mar2010-2022.csv  HV-Info/FractionOn/HV-history-TOB.txt  > HV-Info/FractionOn/log-HV-TOB.txt
python3 computeTimeFractionOn.py HV HV-Info/Data/TOB_plus_2.1.1.1_HV1_Mar2010-2022.csv  HV-Info/FractionOn/HV-history-TOB2.txt  > HV-Info/FractionOn/log-HV-TOB2.txt
python3 computeTimeFractionOn.py HV HV-Info/Data/TID_plus_1.1.1.1_HV1_Mar2010-2022.csv  HV-Info/FractionOn/HV-history-TID.txt  > HV-Info/FractionOn/log-HV-TID.txt
python3 computeTimeFractionOn.py HV HV-Info/Data/TEC_minus_1.1.1.1.1_HV1_Mar2010-2022.csv  HV-Info/FractionOn/HV-history-TEC.txt  > HV-Info/FractionOn/log-HV-TEC.txt

python3 mergeChannelsHistory.py HV
