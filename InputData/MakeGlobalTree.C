// This code read the different input trees and compute a global one with one entry per detid
// It avoids matching detids of the different trees each time the simu is run

#define tecringoffset_   5
#define subdetoffset_   25
#define layerstartbit_  14
#define tecringmask_   0x7
#define subdetmask_    0x7
#define layermask_     0x7


// interpolation for fluence
bool GetFluence(TTree* treefluence, Float_t Z, Float_t R, Float_t deltaZ, Float_t deltaR, Float_t &flu, Float_t &flu_err){

    Bool_t value_found=false;
    
    Float_t zpos,rpos;
    Float_t fluence, fluenceerror;
    treefluence->SetBranchAddress("Z",&zpos);
    treefluence->SetBranchAddress("R",&rpos);
    treefluence->SetBranchAddress("fluence",&fluence);
    treefluence->SetBranchAddress("fluence_err",&fluenceerror);
    Long64_t fluentries = treefluence->GetEntries();

    Float_t zdiff;
    Float_t dislocate, flu1,flu2, searchpos, finalfluence;
    Int_t sumcase;
    
    // closest point  // Up to 17% lower values (TIB L1) than with interpolation
    /*for(Int_t n =0; n<fluentries; n++){
     treefluence->GetEntry(n);
     zdiff = 100*z - zpos;
     if(zdiff>=-1*deltaZ && zdiff<deltaZ){
     dislocate = R-rpos;
     if(dislocate>=-1*deltaR && dislocate<+deltaR){
     //std::cout<<"closest "<<zpos<<" "<<rpos<<" "<<fluence<<std::endl;
     std::cout<<detid<<" "<<fluence<<std::endl;
     break;
     }
     }
     }*/
    
    // std interpolation for R only (variations with Z are small)
    Float_t zpos_prev, rpos_prev;
    Float_t flu1_err, flu2_err;
    for(Int_t n =0; n<fluentries; n++){
        treefluence->GetEntry(n);
        zdiff = 100*Z - zpos;
        if(zdiff>=-1*deltaZ && zdiff<deltaZ){
            dislocate = R-rpos;
            if(dislocate>=0 && dislocate<2*deltaR){
                flu1 = fluence;
                flu1_err = fluenceerror;
                //std::cout<<"classical interpol "<<zpos<<" "<<rpos<<" "<<flu1<<std::endl;
                
                // Get next closest value as they are ordered
                zpos_prev = zpos;
                rpos_prev = rpos;
                n++;
                treefluence->GetEntry(n);
                // Check it is indeed the desired value
                //std::cout<<" Z "<<zpos-zpos_prev<<" R "<<rpos-rpos_prev<<std::endl;
                if(zpos_prev!=zpos || rpos-rpos_prev<0 || rpos-rpos_prev>2*deltaR)
                    std::cerr<<" ERROR: expected an other position for the fluence value"<<std::endl;
                
                dislocate = R-rpos;  // IS IT RIGHT ?????????????????????????????????????????????????????????????????????????????????????????????
                //std::cout<<dislocate<<std::endl;
                //if(dislocate>0 || dislocate<-2*deltaR) std::cout<<" Error: not within fluence input boundaries"<<std::endl;
                flu2 = fluence;
                flu2_err = fluenceerror;
                
                value_found=true;
                flu = dislocate/deltaR/2.*(flu2-flu1)+flu1;
                flu_err = max(flu1_err, flu2_err);
                //std::cout<<fluence<<" "<<fluenceerror<<" "<<flu1_err<<" "<<flu2_err<<endl;
                //std::cout<<"classical interpol "<<zpos<<" "<<rpos<<" "<<flu2<<std::endl;
                //std::cout<<"classical interpol "<<zpos<<" "<<rpos<<" "<<dislocate/deltaR/2.*(flu2-flu1)+flu1<<std::endl;
                //std::cout<<detid<<" "<<dislocate/deltaR/2.*(flu2-flu1)+flu1<<std::endl;
                break;
            }
        }
        
    }
    
    // old matching // between -7% and +5% difference with my interpolation
    /*for(Int_t n =0; n<fluentries; n++){
     treefluence->GetEntry(n);
     if((zpos>(100*z))&&(zpos<(100*z+2*deltaZ))){
     //std::cout <<" Zpos "<<zpos<<" "<<100*Z<<" "<<2*deltaZ<<" "<<rpos<<" "<<R<<" "<<fluence<<std::endl;
     if(R-rpos>2*deltaR) {
     continue;
     }
     dislocate= R-rpos;
     if(dislocate>=deltaR && dislocate<=2*deltaR){
     flu2=fluence;
     searchpos=rpos+2*deltaR;
     sumcase =1;
     }
     else if(dislocate>=0 && dislocate<=deltaR){
     flu2=fluence;
     searchpos=rpos-2*deltaR;
     sumcase =0;
     }
     //std::cout <<" Searchpos "<<searchpos<<" "<<dislocate<<"  case "<<sumcase<<std::endl;
     for(Int_t n2=0; n2<fluentries; n2++){
     treefluence->GetEntry(n2);
     if((zpos>(100*z-deltaZ))&&(zpos<(100*z+deltaZ))){
     if(rpos==searchpos){
     flu1= fluence;
     //std::cout<<"flu1: "<<fluence<<" z:"<<zpos<<" r:"<<searchpos<<std::endl;
     break;
     }
     }
     }
     //std::cout <<" sumcase: "<<sumcase<<" dislocate: " << dislocate <<std::endl;
     finalfluence=((1-2*sumcase-dislocate/deltaR+2*dislocate/deltaR*sumcase)*flu1 + (1+2*sumcase + dislocate/deltaR-2*dislocate/deltaR*sumcase)*flu2)/2;
     // WRONG - was used for 2018 results
     // between -12% and +4% difference with my interpolation
     // between -7% and +1% difference with this oldmatching when no error with delta
     //finalfluence=((1-2*sumcase-dislocate/deltaZ+2*dislocate/deltaZ*sumcase)*flu1 + (1+2*sumcase + dislocate/deltaZ-2*dislocate/deltaZ*sumcase)*flu2)/2;
     value_found=true;
     //if(!sumcase) std::cout<<"myflu "<<dislocate/deltaR/2.*(flu2-flu1)+flu2<<std::endl;
     //if(sumcase==1) std::cout<<"myflu "<<dislocate/deltaR/2.*(flu1-flu2)+flu2<<std::endl;
     if(debug) std::cout<< "   found 2 " << found << " (fluence) " << std::endl;
     if(debug) std::cout <<" got fluence: " << finalfluence << " as a mix of " << flu1 << " " << flu2 << std::endl;
     //std::cerr<<detid<<" "<<finalfluence<<std::endl;
     flu = finalfluence;
     break;
     }
     flu_err=0;
     }*/
    
    //std::cout<<" Fluence: "<<flu<<" at "<<Z<<" "<<R<<std::endl;

    return value_found;
}



void MakeGlobalTree()
{
    string inputdirname = "./BasicFiles/";
    string outputfilename = "GlobalTree.root";
    
    bool debug = false;
    
    
    //---------------------
    // Opening input files
    //---------------------
    
    // module positions and dimensions
    
    TFile *f1 = TFile::Open(Form("%s/TrackMap.root", inputdirname.c_str()));
    if(!f1){
        std::cerr << "Could not open tracker map" << std::endl;
        return;
    }
    TTree *treemap = (TTree*)f1->Get("treemap");
    Int_t detid,structpos;
    Float_t x,y,z,l,w1,w2,d;
    Char_t partition[4],struc[5];
    treemap->SetBranchAddress("Partition",&partition);
    treemap->SetBranchAddress("Struct",&struc);
    treemap->SetBranchAddress("StructPos",&structpos);
    treemap->SetBranchAddress("DETID",&detid);
    treemap->SetBranchAddress("X",&x);
    treemap->SetBranchAddress("Y",&y);
    treemap->SetBranchAddress("Z",&z);
    treemap->SetBranchAddress("L",&l);
    treemap->SetBranchAddress("W1",&w1);
    treemap->SetBranchAddress("D",&d);
    treemap->SetBranchAddress("W2",&w2);
    Long64_t mapentries = treemap->GetEntries();
    if(debug) {std::cout << "mapentries " << mapentries << std::endl;}

    
    // Fluence map
    
    TFile *f2_35 = TFile::Open(Form("%s/fluenceTree_3500.root", inputdirname.c_str())); //JLA
    //TFile *f2 = TFile::Open("/Users/jlagram/StripMon_ForErik/BasicFiles/FluenceTree_1MeV_7TeV.root");
    if(!f2_35){
        std::cerr<<"Could not open fluence map"<<std::endl;
        return;
    }
    /*TTree *treefluence = (TTree*)f2->Get("treefluence");
     Float_t zpos,rpos;
     Double_t fluence, fluenceerror;
     treefluence->SetBranchAddress("Zpos",&zpos);
     treefluence->SetBranchAddress("Rpos",&rpos);
     treefluence->SetBranchAddress("Fluence",&fluence);
     treefluence->SetBranchAddress("FluenceError",&fluenceerror);*/
    TTree *treefluence_35 = (TTree*)f2_35->Get("fluence");
    Long64_t fluentries_35 = treefluence_35->GetEntries();
    if(debug) {std::cout << "fluentries_35 " << fluentries_35 << std::endl;}
    
    // second map
    TFile *f2_70 = TFile::Open(Form("%s/fluenceTree_7000.root", inputdirname.c_str()));
    if(!f2_70){
        std::cerr<<"Could not open fluence map"<<std::endl;
        return;
    }
    TTree *treefluence_70 = (TTree*)f2_70->Get("fluence");
    Long64_t fluentries_70 = treefluence_70->GetEntries();
    if(debug) {std::cout << "fluentries_70 " << fluentries_70 << std::endl;}
    
    
    // initial temperature map

    TFile *f3 = TFile::Open(Form("%s/TempTree.root", inputdirname.c_str()));
    if(!f3){
        std::cerr<<"Could not open temperature tree"<<std::endl;
        return;
    }
    TTree *treetemp = (TTree*)f3->Get("treetemp");
    Int_t tdet;
    Float_t temp;
    treetemp->SetBranchAddress("DETID",&tdet);
    treetemp->SetBranchAddress("Temp",&temp);
    Long64_t tempentries = treetemp->GetEntries();
    if(debug) {std::cout << "tempentries " << tempentries << std::endl;}


    // initial full depletion voltage
    
    TTree * treevdep = new TTree("VDEP","vdep");
    Int_t detid_vdep;
    Float_t ini_vdep;
    treevdep->ReadFile(Form("%s/Ini_Vdep.dat", inputdirname.c_str()),"DETID/I:VDEP/F" );
    treevdep->SetBranchAddress("DETID", &detid_vdep);
    treevdep->SetBranchAddress("VDEP", &ini_vdep);
    Long64_t vdepentries = treevdep->GetEntries();
    
    if(debug) {std::cout << "vdepentries " << vdepentries << std::endl;}

    
    // dP/dT values for self-heating

    TFile *f4 = TFile::Open(Form("%s/Erik_dPdT.root", inputdirname.c_str()));
    if(!f4){
        std::cerr << "could not open thermal contact map " << std::endl;
        return;
    }
    TTree *treedpdt = (TTree*)f4->Get("tree");
    Long64_t detiddpdt;
    Float_t dtdp;
    treedpdt->SetBranchAddress("DETID",&detiddpdt);
    treedpdt->SetBranchAddress("DTDP",&dtdp);
    Long64_t dpdtentries = treedpdt->GetEntries();
    if(debug) {std::cout << "dpdtentries " << dpdtentries << std::endl;} 


    // initial leakage current values

    TFile *f5 = TFile::Open(Form("%s/IleakTree.root", inputdirname.c_str()));
    if(!f5){
        std::cerr<<"Could not open ini leakage file" << std::endl;
        return;
    }
    TTree * treeileak = (TTree*)f5->Get("treeileak");
    Int_t detid_leak;
    Float_t ini_leak;
    treeileak->SetBranchAddress("DETID", &detid_leak);
    treeileak->SetBranchAddress("IniIleak", &ini_leak);
    Long64_t ileakentries = treeileak->GetEntries();
    if(debug) {std::cout << "ileakentries " << ileakentries << std::endl;} 

    
    // powergroup informations
                            
    TFile *f6 = new TFile(Form("%s/DETID_numberofmodules.root", inputdirname.c_str()));
    if(!f6){
        std::cerr<<"Could not open pg file" << std::endl;
        return;
    }
    TTree *pgtree = (TTree*)f6->Get("treeDETnomods");
    Char_t pg[30];
    Int_t detidpg, channel, pgindex, lastindex, nomod, nomodlast, missing;
    pgtree->SetBranchAddress("DETID",&detidpg);
    pgtree->SetBranchAddress("PG",pg);
    pgtree->SetBranchAddress("CHANNEL",&channel);
    pgtree->SetBranchAddress("PGINDEX",&pgindex);
    pgtree->SetBranchAddress("NOMODULES",&nomod);
    Int_t pgentries = pgtree->GetEntries();
    if(debug) {std::cout << " pgentries: " << pgentries << std::endl;}

    

    //// Looping over all trees to compute an inclusive list of detids
    
    //--------------------------------
    // Declaration of the output tree
    //--------------------------------
    
    TFile *fout = new TFile(outputfilename.c_str() ,"recreate");
    TTree *globaltree = new TTree("globaltree", "");
    
    globaltree->Branch("Partition", &partition, "Partition/C");
    globaltree->Branch("Struct", &struc, "Struct/C");
    globaltree->Branch("StructPos", &structpos, "StructPos/I");
    globaltree->Branch("DETID", &detid, "DETID/I");
    globaltree->Branch("X", &x, "X/F");
    globaltree->Branch("Y", &y, "Y/F");
    globaltree->Branch("Z", &z, "Z/F");
    globaltree->Branch("L", &l, "L/F");
    globaltree->Branch("W1", &w1, "W1/F");
    globaltree->Branch("D", &d, "D/F");
    globaltree->Branch("W2", &w2, "W2/F");
    
    globaltree->Branch("PG", &pg, "PG/C");
    globaltree->Branch("CHANNEL", &channel, "L/I");
    globaltree->Branch("PGINDEX", &pgindex, "L/I");
    globaltree->Branch("NOMODULES", &nomod, "L/I");

    Float_t tree_fluence_35, tree_fluence_err_35;
    globaltree->Branch("fluence_3500TeV", &tree_fluence_35, "fluence_3500_TeV/F");
    globaltree->Branch("fluence_3500TeV_err", &tree_fluence_err_35, "fluence_3500TeV_err/F");
    Float_t tree_fluence_70, tree_fluence_err_70;
    globaltree->Branch("fluence_7000TeV", &tree_fluence_70, "fluence_7000TeV/F");
    globaltree->Branch("fluence_7000TeV_err", &tree_fluence_err_70, "fluence_7000TeV_err/F");
    globaltree->Branch("Temp", &temp, "Temp/F");
    globaltree->Branch("VDEP", &ini_vdep, "VDEP/F");
    globaltree->Branch("DTDP", &dtdp, "DTDP/F");
    globaltree->Branch("IniIleak", &ini_leak, "IniIleak/F");
    

    
    //-------------------------------------------------
    // Looping over detids to associate all the values
    //-------------------------------------------------

    Int_t found=0;
    Float_t tempr;
    // Need to know the step used for the FLUKA FILE (consider positions ordered)
    Float_t delta_z= 1.59575/2;
    Float_t delta_r= 2.5/2;

    for (int i = 0; i< mapentries; i++) { //mapentries
        found=0;
        treemap->GetEntry(i);
        tempr = 100*TMath::Sqrt(x*x+y*y);

        if(debug) std::cout << "  detid " << detid << " x " << x << " y " << y << " z " << z << std::endl;
        //std::cout << " pg " << pg << std::endl;
        
        // Getting powergroup infos
        Bool_t found_pg=false;
        for (int pgiter = 0; pgiter< pgentries; pgiter++) {
            pgtree->GetEntry(pgiter);
            if(detid==detidpg){
                found+=1;
                found_pg=true;
                if(debug) std::cout<< "   found 1 " << found << " (pg) " << std::endl;
                break;
            }
        }
        if(!found_pg) {sprintf(pg, ""); channel=-1; pgindex=-1; nomod=-1;}

        
        //// fluence matching
        Bool_t found_fluence_35 = GetFluence(treefluence_35, z, tempr, delta_z, delta_r, tree_fluence_35, tree_fluence_err_35);
        if(!found_fluence_35) {tree_fluence_35=0; tree_fluence_err_35=0;}
        Bool_t found_fluence_70 = GetFluence(treefluence_70, z, tempr, delta_z, delta_r, tree_fluence_70, tree_fluence_err_70);
        if(!found_fluence_70) {tree_fluence_70=0; tree_fluence_err_70=0;}
        if(found_fluence_35 && found_fluence_70) found+=1;
        if(debug) std::cout << "   continue after the fluence matching" << std::endl;

        
        // Getting temperature
        Bool_t found_temp=false;
        for(Int_t j = 0; j< tempentries; j++){
            treetemp->GetEntry(j);
            if(detid==tdet){
                found+=1;
                found_temp=true;
                if(debug) std::cout<< "   found 3 " << found << " (temperature) " << std::endl;
                break;
            } 
        }
        if(!found_temp) {temp=999;}
        
        
        // Getting initial value of leakage current
        Bool_t found_ileak=false;
        for(Int_t p = 0 ; p<ileakentries ; p++){
            treeileak->GetEntry(p);
            if(detid==detid_leak){
                found+=1;
                found_ileak=true;
                if(debug) std::cout<< "   found 4 " << found << " (ileak) " << std::endl;
                break;
            }
        }
        if(!found_ileak) {temp=999;}
        
        
        // Getting initial value of depl voltage
        Bool_t found_vdep=false;
        for(Int_t q=0; q<vdepentries ; q++){
            treevdep->GetEntry(q);
            if(detid==detid_vdep){
                found+=1;
                found_vdep=true;
                if(debug) std::cout<< "   found 5 " << found << " (vdep) " << std::endl;
                break;
            }
        }
        if(!found_vdep) {ini_vdep=999;}
        
        
        // Getting thermal contact
        Bool_t found_dpdt=false;
        for(Int_t iterdpdt=0;iterdpdt<dpdtentries;iterdpdt++){
            treedpdt->GetEntry(iterdpdt);
            if(detiddpdt==detid){
                found+=1;
                found_dpdt=true;
                // treat outliers
                if(dtdp<2) dtdp=8;
                if(dtdp>22) dtdp=8;
                if(debug) std::cout<< "   found 6 " << found << " (dpdt) " << std::endl;
                break;
            } 
        }

        
        Int_t subdet = ((detid >> subdetoffset_) & subdetmask_);
        Int_t position;

        // Complete missing thermal contacts
        if(!found_dpdt){
            found +=1;
            if(subdet==3) { //TIB
                position = ((detid >> layerstartbit_) & layermask_);
                (position < 3) ? (dtdp=12) : (dtdp=8);
            }
            if(subdet==4) { //TID
                dtdp=8;
            }
            if(subdet==5) { //TOB
                dtdp=5;
            }
            if(subdet==6){ //TEC
                dtdp=8;
            }
            std::cout << "replaced dpdt for " << detid << std::endl;
        }


        if(found!=6) {
            std::cout << "    CONTINUE detid = " << detid << std::endl;
            continue;
        }
        if(debug)   std::cout<< "    found all mappings " << found << std::endl;
        
        // filling output tree
        globaltree->Fill();

    } // end loop over detids
    
    
    globaltree->Write();
    fout->Close();
}
