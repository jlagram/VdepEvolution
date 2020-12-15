# Global tree with various input data

All the data are merged into a single .root file using `MakeGlobalTree.C` with one entry per module.
An adapted version `MakeGlobalTree_perlayer.C` exists for computing and using average representative fluences and temperatures per layer. Fictive modules with average behaviour are used.

# BasicFiles

The various inputs are:
- module positions and powergroups
- fluence, temperature and leakage current maps
- initial full depletion voltages
- dT/dP coefficients for simulating self-warming

# For specific studies

`DrawTemperature.C` can be used for checking temperature distributions in the tracker, in particular per layer.
