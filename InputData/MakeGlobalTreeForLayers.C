#define tecringoffset_   5
#define subdetoffset_   25
#define layerstartbit_  14
#define tecringmask_   0x7
#define subdetmask_    0x7
#define layermask_     0x7

// Detids infos
//--------------

int GetSubdet(ULong64_t modid)
{
    if(modid>999999999) modid/=10; // sensor option for TOB
    // 3->6 : TIB, TID, TOB, TEC
    static const Int_t kSubdetOffset = 25;
    int subdet = (modid>>kSubdetOffset)&0x7;
    
    return subdet;
}

string GetSubdetString(ULong64_t modid)
{
    string subdet;
    int isubdet = GetSubdet(modid);
    if(isubdet==3) subdet = "TIB";
    if(isubdet==4) subdet = "TID";
    if(isubdet==5) subdet = "TOB";
    if(isubdet==6) subdet = "TEC";
    
    return subdet;
}


int GetLayer(ULong64_t modid)
{
    if(modid>999999999) modid/=10; // sensor option for TOB/TEC
    Int_t subdet = GetSubdet(modid);
    
    static const unsigned layerStartBit = 14;
    static const unsigned layerMask = 0x7;
    static const unsigned ringStartBitTID = 9;
    static const unsigned ringMaskTID = 0x3;
    static const unsigned ringStartBitTEC = 5;
    static const unsigned ringMaskTEC = 0x7;
    Int_t layer = 0;
    
    // For TIB and TOB
    if(subdet==3 || subdet==5) layer = ((modid>>layerStartBit) & layerMask);
    // For TID, returns ring
    if(subdet==4) layer = ((modid>>ringStartBitTID) & ringMaskTID);
    // For TEC, returns ring
    if(subdet==6) layer = ((modid>>ringStartBitTEC) & ringMaskTEC);
    
    return layer;
    
}

int GetWheel(ULong64_t modid)
{
    if(modid>999999999) modid/=10; // sensor option for TOB/TEC
    Int_t subdet = GetSubdet(modid);
    
    static const unsigned wheelStartBitTID = 11;
    static const unsigned wheelMaskTID = 0x3;
    static const unsigned wheelStartBitTEC = 14;
    static const unsigned wheelMaskTEC = 0xF;
    Int_t wheel = 0;
    
    // For TID
    if(subdet==4) wheel = ((modid>>wheelStartBitTID) & wheelMaskTID);
    // For TEC
    if(subdet==6) wheel = ((modid>>wheelStartBitTEC) & wheelMaskTEC);
    // For barrel, return 0;
    
    return wheel;
}

int GetStringRod(ULong64_t modid)
{
    if(modid>999999999) modid/=10; // sensor option for TOB/TEC
    Int_t subdet = GetSubdet(modid);
    
    static const unsigned strStartBitTIB = 4;
    static const unsigned strMaskTIB = 0x3F;
    static const unsigned rodStartBitTOB = 5;
    static const unsigned rodMaskTOB = 0x7F;
    Int_t stringRod = 0;
    
    // For TIB
    if(subdet==3) stringRod = ((modid>>strStartBitTIB) & strMaskTIB);
    // For TOB
    if(subdet==5) stringRod = ((modid>>rodStartBitTOB) & rodMaskTOB);
    // For endcap, return 0;
    
    return stringRod;
}

// Modified module number to integrate side
float GetModule(ULong64_t modid)
{
    if(modid>999999999) modid/=10; // sensor option for TOB/TEC
    Int_t subdet = GetSubdet(modid);
    
    static const unsigned modStartBitTIB = 2;
    static const unsigned modMaskTIB = 0x3;
    static const unsigned modStartBitTOB = 2;
    static const unsigned modMaskTOB = 0x7;
    Int_t Mod = 0;
    
    static const unsigned fw_bwStartBit = 12;
    static const unsigned fw_bwMask = 0x3;
    Int_t side = 0;
    
    // For TIB
    if(subdet==3) Mod = ((modid>>modStartBitTIB) & modMaskTIB);
    // For TOB
    if(subdet==5) Mod = ((modid>>modStartBitTOB) & modMaskTOB);
    // For endcap, return 0;
    
    side = ((modid>>fw_bwStartBit) & fw_bwMask);
    // minus side for side=1
    if(side==1) Mod*=-1;
    
    return Mod;
}

// differentiate internal/external strings for TIB
bool IsInternal(ULong64_t modid)
{
    if(modid>999999999) modid/=10; // sensor option for TOB/TEC
    Int_t subdet = GetSubdet(modid);
    
    static const unsigned int str_int_extStartBit= 10;
    static const unsigned int str_int_extMask= 0x3;
    Int_t order = 0;
    
    // For TIB
    if(subdet!=3) return false;
    else {
        order = ((modid>>str_int_extStartBit) & str_int_extMask);
        // external string
        if(order==1) return true;
        else return false;
    }
    
}

//---------------------------------------------------------------------------------------------------------

float GetQuantile(vector<float> v, float p){
    
    vector<float>::iterator b = v.begin();
    vector<float>::iterator e = v.end();
    vector<float>::iterator nth = b;
    
    const std::size_t pos = p * std::distance(b, e);
    advance(nth, pos);
    nth_element(b, nth, e);
    
    return *nth;
    
}


//---------------------------------------------------------------------------------------------------------

void MakeGlobalTreeForLayers()
{
    
    // use quantile
    // filter hot regions
    bool filter_hot_regions=true;
    

    //-------------------------------
    // Declaration of the input tree
    //-------------------------------
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

    fin = new TFile("GlobalTree_stdinterpol_flu35-70.root");
    tree = (TTree*) fin->Get("globaltree");

    tree->SetBranchAddress("Partition",&partition);
    tree->SetBranchAddress("Struct",&struc);
    tree->SetBranchAddress("StructPos",&structpos);
    tree->SetBranchAddress("DETID",&detid);
    tree->SetBranchAddress("X",&x);
    tree->SetBranchAddress("Y",&y);
    tree->SetBranchAddress("Z",&z);
    tree->SetBranchAddress("L",&l);
    tree->SetBranchAddress("W1",&w1);
    tree->SetBranchAddress("D",&d);
    tree->SetBranchAddress("W2",&w2);

    tree->SetBranchAddress("PG",pg);
    tree->SetBranchAddress("CHANNEL",&channel);
    tree->SetBranchAddress("PGINDEX",&pgindex);
    tree->SetBranchAddress("NOMODULES",&nomod);

    tree->SetBranchAddress("fluence_3500TeV",&fluence_3500TeV);
    tree->SetBranchAddress("fluence_3500TeV_err",&fluenceerror_3500TeV);
    tree->SetBranchAddress("fluence_7000TeV",&fluence_7000TeV);
    tree->SetBranchAddress("fluence_7000TeV_err",&fluenceerror_7000TeV);
    tree->SetBranchAddress("Temp",&temp);
    tree->SetBranchAddress("VDEP", &ini_vdep);
    tree->SetBranchAddress("DTDP",&dtdp);
    tree->SetBranchAddress("IniIleak", &ini_leak);

    nentries = tree->GetEntries();

    
    
    //--------------------------------
    // Declaration of the output tree
    //--------------------------------
    
    TFile *fout = new TFile("GlobalTree_perlayer.root" ,"recreate");
    TTree *globaltree = new TTree("globaltree", "");
    
    Int_t detid_o,structpos_o;
    Float_t x_o,y_o,z_o,l_o,w1_o,w2_o,d_o;
    Char_t partition_o[4],struc_o[5];
    
    Char_t pg_o[30];
    Int_t channel_o,pgindex_o,lastindex_o,nomod_o;
    
    //Float_t fluence, fluenceerror;
    Float_t fluence_3500TeV_o, fluenceerror_3500TeV_o;
    Float_t fluence_7000TeV_o, fluenceerror_7000TeV_o;
    Float_t temp_o, ini_vdep_o, ini_leak_o, dtdp_o;
    
    globaltree->Branch("Partition", &partition_o, "Partition/C");
    globaltree->Branch("Struct", &struc_o, "Struct/C");
    globaltree->Branch("StructPos", &structpos_o, "StructPos/I");
    globaltree->Branch("DETID", &detid_o, "DETID/I");
    globaltree->Branch("X", &x_o, "X/F");
    globaltree->Branch("Y", &y_o, "Y/F");
    globaltree->Branch("Z", &z_o, "Z/F");
    globaltree->Branch("L", &l_o, "L/F");
    globaltree->Branch("W1", &w1_o, "W1/F");
    globaltree->Branch("D", &d_o, "D/F");
    globaltree->Branch("W2", &w2_o, "W2/F");
    
    globaltree->Branch("PG", &pg_o, "PG/C");
    globaltree->Branch("CHANNEL", &channel_o, "L/I");
    globaltree->Branch("PGINDEX", &pgindex_o, "L/I");
    globaltree->Branch("NOMODULES", &nomod_o, "L/I");
    
    Float_t tree_fluence_35_o, tree_fluence_err_35_o;
    globaltree->Branch("fluence_3500TeV", &tree_fluence_35_o, "fluence_3500_TeV/F");
    globaltree->Branch("fluence_3500TeV_err", &tree_fluence_err_35_o, "fluence_3500TeV_err/F");
    Float_t tree_fluence_70_o, tree_fluence_err_70_o;
    globaltree->Branch("fluence_7000TeV", &tree_fluence_70_o, "fluence_7000TeV/F");
    globaltree->Branch("fluence_7000TeV_err", &tree_fluence_err_70_o, "fluence_7000TeV_err/F");
    globaltree->Branch("Temp", &temp_o, "Temp/F");
    globaltree->Branch("VDEP", &ini_vdep_o, "VDEP/F");
    globaltree->Branch("DTDP", &dtdp_o, "DTDP/F");
    globaltree->Branch("IniIleak", &ini_leak_o, "IniIleak/F");


    //------------------------
    // Containers for a layer
    //------------------------

    //const int nlay=10;
    //Float_t x[nlay],y[nlay],z[nlay],l[nlay],w1[nlay],w2[nlay],d[nlay];

    vector<float> l_l, w1_l, w2_l, d_l;
    vector<float> fluence_3500TeV_l, fluenceerror_3500TeV_l, fluence_7000TeV_l, fluenceerror_7000TeV_l;
    vector<float> temp_l, ini_vdep_l, ini_leak_l, dtdp_l;
    
    

    // Loop over layers
    for (int ilay = 1; ilay<21; ilay++){
        
        int subdet_l = 0;
        int layer_l = 0;
        
        // barrel layers
        if(ilay<11)
        {
            subdet_l = ilay>=5 ? 5 : 3;
            layer_l = ilay>=5 ? ilay-4 : ilay;
        }
        // endcap rings
        else
        {
            subdet_l = ilay>=6 ? 6 : 4;
            layer_l = ilay>=14 ? ilay-13 : ilay-10;
        }
        
        l_l.clear();
        w1_l.clear();
        w2_l.clear();
        d_l.clear();
        
        fluence_3500TeV_l.clear();
        fluenceerror_3500TeV_l.clear();
        fluence_7000TeV_l.clear();
        fluenceerror_7000TeV_l.clear();
        
        temp_l.clear();
        ini_vdep_l.clear();
        ini_leak_l.clear();
        dtdp_l.clear();
        
        bool is_hot = false;

        
        // Loop over detids
        for (int idet = 0; idet< nentries; idet++) {
            
            tree->GetEntry(idet);
            
            int subdet = GetSubdet(detid);
            int layer = GetLayer(detid);
            
            //cout<<"TEMP: "<<detid<<" "<<temp<<endl;
            if(subdet!=subdet_l || layer!=layer_l) continue;
            //cout<<detid<<" "<<subdet<<" "<<layer<<endl;
            
            is_hot=false;
            
            // Remove hot regions
            if(filter_hot_regions){
                
                int stringrod = GetStringRod(detid);
                int mod = GetModule(detid);
                
                //if(temp<0 || temp>50) continue;

                if(subdet==3){
                    if(layer==3)
                        if((stringrod>=39) || (stringrod<=8 && mod<0)) is_hot=true;
                    if(layer==1)
                        //if((stringrod>=18 && stringrod<=26 && mod>0) || (stringrod<=5 && mod>0)) is_hot=true;
                        if((stringrod>=18 && mod>0) || (stringrod<=5 && mod>0)) is_hot=true;
                    if(layer==2)
                        //if((stringrod>=25 && mod>0) || (stringrod<=5 && mod>0) || (stringrod==28 && mod==-3) || (stringrod==32 && mod==-1)) is_hot=true;
                        if((stringrod>=25 && mod>0) || (stringrod<=5 && mod>0) || (stringrod==28 && mod==-3) || (stringrod>=26 && stringrod<=30 && mod==-1)) is_hot=true;
                }
                if(subdet==5){
                    if(layer==3)
                        //if((stringrod>=47) || (stringrod<=14 && mod>0)) is_hot=true;
                        if((stringrod>=47 && mod>0) || (stringrod<=14 && mod>0)) is_hot=true;
                }
                //if(is_hot) cout<<"HOT DETID: "<<detid<<endl;
                //if(is_hot) continue;
                if(((subdet==3 && layer<=3) || (subdet==5 && layer==3)) && !is_hot) continue;
                //cout<<"TEMP: "<<detid<<" "<<temp<<endl;
            }
            
            sprintf(partition_o, "%s", GetSubdetString(detid).c_str());
            if(subdet_l==3 || subdet_l==5) sprintf(struc_o, "L%i", layer);
            else sprintf(struc_o, "R%i", layer);

            l_l.push_back(l);
            w1_l.push_back(w1);
            w2_l.push_back(w2);
            d_l.push_back(d);
            
            fluence_3500TeV_l.push_back(fluence_3500TeV);
            fluenceerror_3500TeV_l.push_back(fluenceerror_3500TeV);
            fluence_7000TeV_l.push_back(fluence_7000TeV);
            fluenceerror_7000TeV_l.push_back(fluenceerror_7000TeV);
            
            temp_l.push_back(temp);
            ini_vdep_l.push_back(ini_vdep);
            ini_leak_l.push_back(ini_leak);
            dtdp_l.push_back(dtdp);
        }


        
        // Set quantile values
        float p = 0.5;
        
        structpos_o = layer_l;
        detid_o = ilay;
        
        x_o = 0;
        y_o = 0;
        z_o = 0;
        
        l_o = GetQuantile(l_l, p);
        w1_o = GetQuantile(w1_l, p);
        w2_o = GetQuantile(w2_l, p);
        d_o = GetQuantile(d_l, p);
        
        sprintf(pg_o, "L");
        channel_o = 0;
        pgindex_o = 0;
        nomod_o = 0;
        
        tree_fluence_35_o = GetQuantile(fluence_3500TeV_l, p);
        //tree_fluence_35_o = GetQuantile(fluence_3500TeV_l, 0.9);
        tree_fluence_err_35_o = GetQuantile(fluenceerror_3500TeV_l, p);
        tree_fluence_70_o = GetQuantile(fluence_7000TeV_l, p);
        //tree_fluence_70_o = GetQuantile(fluence_7000TeV_l, 0.9);
        tree_fluence_err_70_o = GetQuantile(fluenceerror_7000TeV_l, p);
        
        temp_o = GetQuantile(temp_l, p);
        ini_vdep_o = GetQuantile(ini_vdep_l, p);
        //ini_vdep_o = GetQuantile(ini_vdep_l, .9);
        dtdp_o = GetQuantile(dtdp_l, p);
        ini_leak_o = GetQuantile(ini_leak_l, p);
        
        //cout<<detid_o<<" "<<layer_l<<" "<<temp_o<<" "<<ini_vdep_o<<" "<<dtdp_o<<" "<<ini_leak_o<<endl;
        cout<<endl<<detid_o<<" "<<struc_o<<endl;
        cout<<"var: Q10 Q50 Q90"<<endl;
        float flu_Q10=GetQuantile(fluence_7000TeV_l, .1);
        float flu_Q90=GetQuantile(fluence_7000TeV_l, .9);
        float max_variation=TMath::Abs((flu_Q10-tree_fluence_70_o)/tree_fluence_70_o);
        if(TMath::Abs((flu_Q90-tree_fluence_70_o)/tree_fluence_70_o) > max_variation) max_variation=TMath::Abs((flu_Q90-tree_fluence_70_o)/tree_fluence_70_o);
        cout<<"Flu: "<<flu_Q10<<" "<<tree_fluence_70_o<<" "<<flu_Q90<<" -> +/- "<<max_variation<<"%"<<endl;
        cout<<"T: "<<GetQuantile(temp_l, .1)<<" "<<temp_o<<" "<<GetQuantile(temp_l, .9)<<endl;
        cout<<"Vdep: "<<GetQuantile(ini_vdep_l, .1)<<" "<<ini_vdep_o<<" "<<GetQuantile(ini_vdep_l, .9)<<endl;
        
        globaltree->Fill();

    }


    // Save tree
    globaltree->Write();
    fout->Close();

}

