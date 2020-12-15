# Luminosity and temperature scenario

* Information for a realistic scenario are combined in a single text file using `writeLumiTempScenario.py`
* `MakeSimplifiedLumiTempScenario.C` does the same based on an hypothetic scenario.
* In the output text each line correspond to a periode (a day by default) with the center-of-mass energy; the integrated luminosity and averaged tracker temperature.

## Luminosity

Input with integrated luminosity per day.

## Temperature

* `drawTempHistory.py` allows to draw the various temperatures on a given period and check the algorithm used to compute an average behaviour.
* *Data* directory contains the PLC probes data.
* *Plots* directory to store *per year* trend plots

