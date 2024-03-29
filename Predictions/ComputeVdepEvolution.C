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
    factory.setGlobalTree("Inputs/GlobalTree.root");
    factory.readLumiTempScenario("Inputs/realistic_scenario_2022.txt");
    //factory.readLumiTempScenario("Inputs/test_scenario.txt");
    //factory.runSimuForAllModules(1);
    factory.runSimuForAllModules(2, true);
    factory.drawLumiTempScenario();    
}
