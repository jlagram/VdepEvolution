void MakeOld500fbScenario(){
    
    bool debug=false;
    const int periodes = 41; //until 2023
    
    // periodes
    //---------
    
    Int_t t_at_T[periodes] = {
        18, 49, 6, //2010
        18, 49, 6, //2011
        18, 49, 6, //2012
        31, 18, 24, //2013
        18, 49, 6, //2014
        18, 49, 6, //2015
        18, 49, 6, //2016
        18, 49, 6, //2017
        18, 49, 6, //2018
        31, 18, 24, //2019
        31, 18, 24, //2020
        18, 49, 6, //2021
        18, 49, 6, //2022
        18, 49 //, 6, //2023
    }; // 5 days periods
    
    

    // center-of-mass enery
    //---------------------
    
    Float_t *SqrtS = new Float_t[periodes]; // in TeV
    SqrtS[0] = 0.;
    SqrtS[2] = 0.;
    SqrtS[3] = 0.;
    SqrtS[5] = 0.;
    SqrtS[6] = 0.;
    SqrtS[8] = 0.;
    SqrtS[9] = 0.;
    SqrtS[10] = 0.;
    SqrtS[11] = 0.;
    SqrtS[12] = 0.;
    SqrtS[13] = 0.;
    SqrtS[14] = 0.;
    SqrtS[15] = 0.;
    SqrtS[17] = 0.;
    SqrtS[18] = 0.;
    SqrtS[20] = 0.;
    SqrtS[21] = 0.;
    SqrtS[23] = 0.;
    SqrtS[24] = 0.;
    SqrtS[26] = 0.;
    SqrtS[27] = 0.;
    SqrtS[29] = 0.;
    SqrtS[30] = 0.;
    SqrtS[32] = 0.;
    SqrtS[33] = 0.;
    SqrtS[35] = 0.;
    SqrtS[36] = 0.;
    SqrtS[38] = 0.;
    SqrtS[39] = 0.;
    
    SqrtS[1] = 7; //2010
    SqrtS[4] = 7; //2011
    SqrtS[7] = 8; //2012
    SqrtS[16] = 13; //2015
    SqrtS[19] = 13; //2016
    SqrtS[22] = 13; //2017
    SqrtS[25] = 13; //2018
    SqrtS[28] = 0.; //2019
    SqrtS[31] = 0.; //2020
    SqrtS[34] = 13; //2021
    SqrtS[37] = 13; //2022
    SqrtS[40] = 13; //2022
    //Sum: 500.2 fb-1
    
    
    
    // Lumi
    //-----
    
    Float_t *Lumi = new Float_t[periodes]; // in fb-1
    Lumi[0] = 0.;
    Lumi[2] = 0.;
    Lumi[3] = 0.;
    Lumi[5] = 0.;
    Lumi[6] = 0.;
    Lumi[8] = 0.;
    Lumi[9] = 0.;
    Lumi[10] = 0.;
    Lumi[11] = 0.;
    Lumi[12] = 0.;
    Lumi[13] = 0.;
    Lumi[14] = 0.;
    Lumi[15] = 0.;
    Lumi[17] = 0.;
    Lumi[18] = 0.;
    Lumi[20] = 0.;
    Lumi[21] = 0.;
    Lumi[23] = 0.;
    Lumi[24] = 0.;
    Lumi[26] = 0.;
    Lumi[27] = 0.;
    Lumi[29] = 0.;
    Lumi[30] = 0.;
    Lumi[32] = 0.;
    Lumi[33] = 0.;
    Lumi[35] = 0.;
    Lumi[36] = 0.;
    Lumi[38] = 0.;
    Lumi[39] = 0.;
    
    Lumi[1] = 0.046; //2010
    Lumi[4] = 6.2; //2011
    Lumi[7] = 23.7; //2012
    Lumi[16] = 4.1; //2015
    Lumi[19] = 41.4; //2016
    Lumi[22] = 51.7; //2017
    Lumi[25] = 67.8; //2018
    Lumi[28] = 0.; //2019
    Lumi[31] = 0.; //2020
    Lumi[34] = 102; //2021
    Lumi[37] = 102; //2022
    Lumi[40] = 102; //2022
    //Sum: 500.2 fb-1
    
    
    
    // temperature
    //------------
    
    float Ton=4;
    float Toff = 6;
    float Toff2 = 0;
    float RT = 20;

    // temperature shift
    float T1=19; // In Run 2
    float T2=24; // In 2018
    float T3=29; // In Run 3
    
    // in C
    Double_t T_95[periodes] = {
        Toff, Ton, Toff, //2010
        Toff, Ton, Toff, //2011
        Toff, Ton, Toff, //2012
        RT, Toff, Toff2, //2013
        Toff2, Toff2, Toff2, //2014
        Toff2, Ton-T1, Toff2, //2015
        Toff2, Ton-T1, Toff2, //2016
        Toff2, Ton-T1, Toff2, //2017
        Toff2, Ton-T2, Toff2, //2018
        Toff2, Toff2, Toff2, //2019
        Toff2, Toff2, RT, //2020
        Toff2, Ton-T3, Toff2, //2021
        Toff2, Ton-T3, Toff2, //2022
        Toff2, Ton-T3//, Toff2, //2023
    };
    
    
    
    // Generate scenario description file
    //-----------------------------------
    
    Int_t maxTime=0;
    Int_t stepInYear=0;
    Int_t startingYear=2010;
    Int_t dayNumber=1;
    std::cout<<"#define StepInDays 1"<<std::endl;
    std::cout<<"Index dayInYear sqrt(s) lumi temperature"<<std::endl;
    for(Int_t i=0; i<periodes; i++){
        if(i%3==0) {stepInYear=0; std::cout << "# Starting new year: " << startingYear+i/3 << std::endl;}
        maxTime += t_at_T[i];
        // Loop over periodes steps
        for(Int_t j=0; j<t_at_T[i]; j++){
            //std::cout << dayNumber <<" "<< stepInYear+1 <<" "<< SqrtS[i] <<" "<< Lumi[i]/(t_at_T[i]) <<" "<< T_95[i] << std::endl;
            // Loop over days
            for(Int_t k=0; k<5; k++){
                std::cout << dayNumber <<" "<< stepInYear*5+k+1 <<" "<< SqrtS[i] <<" "<< Lumi[i]/(5.*t_at_T[i]) <<" "<< T_95[i] << std::endl;
                dayNumber++;
            }
            stepInYear++;
        }
    }
    if(debug) std::cout << " maxtime "<< maxTime << std::endl;



}


void MakeRun3Scenario(){
    
    bool debug=false;
    const int periodes = 15; //until 2023
    
    // periodes
    //---------
    
    Int_t t_at_T[periodes] = {
        12, 61, 0, //2019
        0, 73, 0,//2020
        18, 49, 6, //2021
        18, 49, 6, //2022
        18, 49, 6 //2023
    }; // 5 days periods
    

    
    // temperature
    //------------
    
    float Ton=4;
    float Toff = 6;
    float Toff2 = 0;
    float RT = 20;
    
    // temperature shift
    float T1=19; // In Run 2
    float T2=24; // In 2018
    float T3=29; // In Run 3
    
    // in C
    Double_t T_95[periodes] = {
        RT, Toff2, Toff2,//2019
        Toff2, Toff2, Toff2,//2020
        Toff2, Ton-T3, Toff2, //2021
        Toff2, Ton-T3, Toff2, //2022
        Toff2, Ton-T3, Toff2 //2023
    };
    
    
    // center-of-mass enery
    //---------------------
    
    Float_t *SqrtS = new Float_t[periodes]; // in TeV
    SqrtS[0] = 0.;
    SqrtS[1] = 0.;
    SqrtS[2] = 0.;
    SqrtS[3] = 0.;
    SqrtS[4] = 0.;
    SqrtS[5] = 0.;
    SqrtS[6] = 0.;
    SqrtS[7] = 0.;
    SqrtS[8] = 0.;
    SqrtS[9] = 0.;
    SqrtS[10] = 0.;
    SqrtS[11] = 0.;
    SqrtS[12] = 0.;
    SqrtS[13] = 0.;
    SqrtS[14] = 0.;

    SqrtS[7] = 13; //2021
    SqrtS[10] = 13; //2022
    SqrtS[13] = 13; //2022
    
    
    // Lumi
    //-----
    
    Float_t *Lumi = new Float_t[periodes]; // in fb-1
    Lumi[0] = 0.;
    Lumi[1] = 0.;
    Lumi[2] = 0.;
    Lumi[3] = 0.;
    Lumi[4] = 0.;
    Lumi[5] = 0.;
    Lumi[6] = 0.;
    Lumi[7] = 0.;
    Lumi[8] = 0.;
    Lumi[9] = 0.;
    Lumi[10] = 0.;
    Lumi[11] = 0.;
    Lumi[12] = 0.;
    Lumi[13] = 0.;
    Lumi[14] = 0.;

    Lumi[7] = 60; //2021
    Lumi[10] = 70; //2022
    Lumi[13] = 70; //2022
    

    
    // Generate scenario description file
    //-----------------------------------
    
    Int_t maxTime=0;
    Int_t stepInYear=0;
    Int_t startingYear=2019;
    Int_t dayNumber=1;
    std::cout<<"#define StepInDays 1"<<std::endl;
    std::cout<<"Index dayInYear sqrt(s) lumi temperature"<<std::endl;
    for(Int_t i=0; i<periodes; i++){
        if(i%3==0) {stepInYear=0; std::cout << "# Starting new year: " << startingYear+i/3 << std::endl;}
        maxTime += t_at_T[i];
        // Loop over periodes steps
        for(Int_t j=0; j<t_at_T[i]; j++){
            //std::cout << dayNumber <<" "<< stepInYear+1 <<" "<< SqrtS[i] <<" "<< Lumi[i]/(t_at_T[i]) <<" "<< T_95[i] << std::endl;
            // Loop over days
            for(Int_t k=0; k<5; k++){
                std::cout << dayNumber <<" "<< stepInYear*5+k+1 <<" "<< SqrtS[i] <<" "<< Lumi[i]/(5.*t_at_T[i]) <<" "<< T_95[i] << std::endl;
                dayNumber++;
            }
            stepInYear++;
        }
    }
    if(debug) std::cout << " maxtime "<< maxTime << std::endl;
    
    
    
}


void MakeSimplifiedLumiTempScenario(){
    
    //MakeOld500fbScenario();
    MakeRun3Scenario();
    return;
}
