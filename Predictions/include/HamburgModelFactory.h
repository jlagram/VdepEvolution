#ifndef Hamburg_Model_Factory
#define Hamburg_Model_Factory

#include "HamburgModel.h"
#include "TFile.h"
#include "TTree.h"

#define tecringoffset_   5
#define subdetoffset_   25
#define layerstartbit_  14
#define tecringmask_   0x7
#define subdetmask_    0x7
#define layermask_     0x7


class HamburgModelFactory {
    
  public:
    HamburgModelFactory(){

        periodeInDays = 1;
        debug=true;
        
        // files
        fin = 0;
        fout = new TFile("Graph_new.root", "recreate");
    
        total_Lumi=0;
        total_Feq=0;
    
    };
    ~HamburgModelFactory(){
        if(fin) fin->Close();
        if(fout) fout->Close();
    };

    void setGlobalTree(std::string filename); // mandatory before calling runSimuForAllModules()
    void readLumiTempScenario(std::string filename);
    void runSimuForAllModules(int option);
    void runSimuForAvgModules(bool drawNeff); // Idem, but for avg module per layer simu
    void drawLumiTempScenario(); // plots filled in runSimuForAllModules() and not in readLumiTempScenario()

    
  private:
    
    void simulateSensorEvolution(int detid_); // to be called only in runSimuForAllModules() after having loaded tree infos
    void drawSaveSensorSimu(bool print_plots);
    
    
    // data members
    // -------------
    
    Int_t periodeInDays;
    Bool_t debug;

    // for input tree reading
    
    TFile *fin;
    TTree *tree;
    
    Int_t detid,structpos;
    Float_t x,y,z,l,w1,w2,d;
    Char_t partition[4],struc[5];
    
    Char_t pg[30];
    Int_t channel,pgindex,lastindex,nomod;

    //Float_t fluence, fluenceerror;
    Float_t fluence_3500TeV, fluenceerror_3500TeV;
    Float_t fluence_7000TeV, fluenceerror_7000TeV;
    Float_t temp, ini_vdep, ini_leak, dtdp;

    Int_t nentries;
    
    
    // containers for lumi/temp scenario
    
    std::vector< int > Step;
    std::vector< int > StepInYear;
    std::vector< float > SqrtS;
    std::vector< float > Lumi;
    std::vector< float > Temp;
    std::vector< int > Active;
    unsigned int Nperiods;
    
    
    // containers for Hamburg model (simu over time)

    std::vector< double > Feq;
    std::vector< double > Temp_evol;
    std::vector< double > I_leak;
    std::vector< double > Na;
    std::vector< double > NY;
    std::vector< double > Nc;
    std::vector< double > N;
    std::vector< double > U;
    
    std::vector< double > intLumi;
    std::vector< double > intFeq;
    Float_t total_Lumi;
    Float_t total_Feq;
    
    // File to save graphs
    TFile* fout;


    
};

#endif
