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

/*char* GetSubdetString(ULong64_t modid)
 {
 char* subdet;
 sprintf(subdet, "");
 int isubdet = GetSubdet(modid);
 if(isubdet==3) sprintf(subdet, "TIB");
 if(isubdet==4) sprintf(subdet, "TID");
 if(isubdet==5) sprintf(subdet, "TOB");
 if(isubdet==6) sprintf(subdet, "TEC");
 
 return subdet;
 }*/

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

TH1F* DrawLayerTemperature(std::string subdet="TIB", int layer=0, bool show=false, bool filter_hotregion=false)
{
    
    TH1F* hT = new TH1F("hT", "average temperature", 50, 0, 50);
    
    TFile *fin = TFile::Open("BasicFiles/TempTree.root");
    if(!fin){
        std::cerr<<"Could not open temperature tree"<<std::endl;
        return 0;
    }
    TTree *treetemp = (TTree*)fin->Get("treetemp");
    Int_t detid;
    Float_t temp;
    treetemp->SetBranchAddress("DETID",&detid);
    treetemp->SetBranchAddress("Temp",&temp);
    Long64_t tempentries = treetemp->GetEntries();
    int stringrod=0;
    
    // Loop on detids
    for(Int_t j = 0; j< tempentries; j++){
        treetemp->GetEntry(j);
        
        // filtering on subdet
        if(GetSubdetString(detid)!=subdet) continue;
        // filtering on layer
        if(layer>0 && GetLayer(detid)!=layer) continue;
        
        stringrod = GetStringRod(detid);
        if(filter_hotregion){
            if(subdet=="TIB"){
                if(layer==3)
                    if((stringrod>=39) || (stringrod<=8 && GetModule(detid)<0)) continue;
                if(layer==1)
                    //if((stringrod>=18 && stringrod<=26 && mod>0) || (stringrod<=5 && mod>0)) continue;
                    if((stringrod>=18 && GetModule(detid)>0) || (stringrod<=5 && GetModule(detid)>0)) continue;
                if(layer==2)
                    //if((stringrod>=25 && GetModule(detid)>0) || (stringrod<=5 && GetModule(detid)>0) || (stringrod==28 && GetModule(detid)==-3) || (stringrod==32 && GetModule(detid)==-1)) continue;
                    if((stringrod>=25 && GetModule(detid)>0) || (stringrod<=5 && GetModule(detid)>0) || (stringrod==28 && GetModule(detid)==-3) || (stringrod>=26 && stringrod<30 && GetModule(detid)==-1)) continue;
           }
            if(subdet=="TOB"){
                if(layer==3)
                    //if((stringrod>=47) || (stringrod<=14 && GetModule(detid)>0)) continue;
                    if((stringrod>=47 && GetModule(detid)>0) || (stringrod<=14 && GetModule(detid)>0)) continue;
                
           }
        }
        hT->Fill(temp);

    }
    fin->Close();
    if(show){
        TCanvas* c1 = new TCanvas();
        hT->Draw();
    }
    return hT;
}

void ShowTemperaturePerLayer(std::string subdet="TIB"){
    TCanvas* c1 = new TCanvas();
    std::vector< TH1F* > htemp;
    for(int i=1; i<5; i++){
        htemp.push_back(DrawLayerTemperature(subdet, i));
        htemp[i-1]->SetLineColor(i);
        if(i==1) htemp[0]->Draw();
        else htemp[i-1]->Draw("same");
    }
    for(int i=1; i<5; i++){
        cout<<"Layer "<<i<<": "<<htemp[i-1]->GetMean()<<endl;
    }
    
}

TH2F* DrawLayerTemperatureVs(std::string subdet="TIB", int layer=0, bool show=false)
{
    
    TH2F* hT = new TH2F("hT", "temperature", 60, 0, 60, 10, -5, 5);
    
    TFile *fin = TFile::Open("BasicFiles/TempTree.root");
    if(!fin){
        std::cerr<<"Could not open temperature tree"<<std::endl;
        return 0;
    }
    TTree *treetemp = (TTree*)fin->Get("treetemp");
    Int_t detid;
    Float_t temp;
    treetemp->SetBranchAddress("DETID",&detid);
    treetemp->SetBranchAddress("Temp",&temp);
    Long64_t tempentries = treetemp->GetEntries();
    
    // Loop on detids
    for(Int_t j = 0; j< tempentries; j++){
        treetemp->GetEntry(j);
        
        // filtering on subdet
        if(GetSubdetString(detid)!=subdet) continue;
        // filtering on layer
        if(layer>0 && GetLayer(detid)!=layer) continue;
        if(temp<50)
        hT->Fill(GetStringRod(detid), GetModule(detid), temp);
        
    }
    fin->Close();
    if(show){
        TCanvas* c1 = new TCanvas();
        hT->Draw("colz");
    }
    return hT;
}

void ShowHotRegionsRemoval(std::string subdet="TIB", int layer=1)
{
    TCanvas* c = new TCanvas();
    TH1F* h1 = DrawLayerTemperature(subdet, layer, false, false);
    TH1F* h2 = DrawLayerTemperature(subdet, layer, false, true);
    h1->Draw();
    h2->SetLineColor(2);
    h2->Draw("same");
    h1->SetTitle(Form("temperature in %s L%i", subdet.c_str(), layer));
    c->Print(Form("temperature_%s_L%i.pdf", subdet.c_str(), layer));
    
    const int nq=20;
    Double_t xq[nq];  // position where to compute the quantiles in [0,1]
    Double_t yq[nq];  // array to contain the quantiles
    for (Int_t i=0;i<nq;i++) xq[i] = Float_t(i+1)/nq;
    h2->GetQuantiles(nq,yq,xq);
    TGraph *gr = new TGraph(nq,xq,yq);
    gr->SetMarkerStyle(21);
    //gr->Draw("alp");
    cout<<"Quantiles:"<<endl;
    gr->Print();
}

void DrawTemperature()
{
    //ShowTemperaturePerLayer();
    //DrawLayerTemperatureVs("TIB", 2, true);
    ShowHotRegionsRemoval("TIB", 1);
    ShowHotRegionsRemoval("TIB", 2);
    ShowHotRegionsRemoval("TIB", 3);
    ShowHotRegionsRemoval("TOB", 3);
}
