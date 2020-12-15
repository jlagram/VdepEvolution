#include "include/HamburgModelFactory.h"
#include "include/HamburgModelFactory.cc"

#define tecringoffset_   5
#define subdetoffset_   25
#define layerstartbit_  14
#define tecringmask_   0x7
#define subdetmask_    0x7
#define layermask_     0x7


// Computation for each detid

//void ComputeVdepEvolution_v2(){
int main(){

    HamburgModelFactory factory;
    factory.setGlobalTree("Inputs/GlobalTree_perlayer_stdinterpol_flu35-70.root");
    factory.readLumiTempScenario("Inputs/run3_projection_scenario_step1day_repl-5oC.txt");
    factory.runSimuForAvgModules(true); // drawNeff
    factory.drawLumiTempScenario();
    
}
