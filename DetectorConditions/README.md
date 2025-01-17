# LV and HV information

## Script to format channel info

`python computeTimeFractionOn.py inputfile [outputfile]`
The output file will contain for each day the fraction of time the channel was ON.


## LV-Info

Default files to use:
*LV-Info/Data/TIB_minus_1.1.1.1_LV_Analog_Mar2010-2020.csv*
*LV-Info/Data/TID_plus_1.1.1.1_LV_Analog_Mar2010-2020.csv*
*LV-Info/Data/TID_plus_1.1.1.1_LV_Analog_Mar2010-2020.csv*
*LV-Info/Data/TEC_minus_1.1.1.1.1_LV_Analog_Mar2010-2020.csv*
Outputs saved in `FractionOn/`

## HV-Info

Default files to use:
*HV-Info/Data/TIB_minus_1.1.1.1_HV1_Mar2010-2020.csv*
*HV-Info/Data/TOB_plus_1.1.1.1_HV1_Mar2010-2020.csv*
*HV-Info/Data/TEC_minus_1.1.1.1.1_HV1_Mar2010-2020.csv*
Outputs saved in `FractionOn/`

## Script to merge info of several channels

`python writeCommonHistory.py LV/HV`
Merging info and saving median of the time fraction ON of several channels. Helps in correcting in case a data is missing in DB for a channel and the time computation is wrong.

## Script for comparing history files

`python compareHistory.py file1 file2 [threshold]`
