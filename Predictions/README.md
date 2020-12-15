# Code for predicting full depletion voltage evolution

Precise input files names in `ComputeVdepEvolution.C` before compiling and executing.

## include

*include* directory contains the main codes.
*HamburgModel* is the class containing all the methods used for computing leakage current and charge carriers evolution.
*HamburgModelFactory* is the main class. There are method to read the various inputs, call the HamburgModel methods to do predictions for a given module, draw and save curves.

## Inputs

Directory to store localy the inputs: the global data tree and the luminosity/temperature scenario.
The scenario file format must be:
`StepInYearIndex GlobalIndex CollisionEnergy Luminosity Temperature`

## perLayer

A special version allows to predict evolutions for virtual modules representative of each layer. The file is `ComputeVdepEvolution_perlayer.C` and can be compiled with this option: `make perlayer`
