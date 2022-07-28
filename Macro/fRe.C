//#include <boost/algorithm/string.hpp>
void set_root_style();
void toString(double arr[], int length){
    cout << "toString called" << endl;
    for (int i = 0; i < length; i++){
        cout << arr[i] << endl;
    }

}
void toString(string arr[], int length){
    cout << "toString called" << endl;
    for (int i = 0; i < length; i++){
        cout << arr[i] << endl;
    }

}

void scan_epoxy(){

    set_root_style();
    
    TCanvas *c1=new TCanvas();
    TChain *f[4];
    TH2F *htemp[4];
    TH1F *h[4];
    const char *name[4] = {"0","0.2", "0.5","1"};
    const char *wid[4]={"0","0.2","0.5","1"};
    for(int i=0;i<4;i++){
        f[i]=new TChain("LYSim");
        f[i]->Add(Form("%s_jul24.root",name[i]));
        htemp[i] = new TH2F(Form("htemp%s",name[i]),";X position [mm];",1,-1,1,100,0,500);
        f[i]->Draw(Form("detectphotons3+detectphotons4:BeamX>>htemp%s",name[i]));
        htemp[i]->Draw("colz");
        h[i] =(TH1F*)htemp[i]->ProfileX();
        h[i]->SetLineColor(i+1);
        h[i]->SetMarkerColor(i+1);
        h[i]->SetLineWidth(1);
    }
    
    auto leg2 = new TLegend(0.25,0.7,0.5,0.85);
    h[0]->GetXaxis()->SetRangeUser(-1,1);
    h[0]->GetYaxis()->SetTitle("Average LY (p.e.)");
    h[0]->GetYaxis()->SetRangeUser(0,300);
    h[0]->GetYaxis()->SetTitleSize(0.045);
    h[0]->Draw("e");
    leg2->AddEntry(h[0],Form("Attenuation length = %smm ",wid[0]), "l");
    for(int i=1;i<4;i++){
        h[i]->Draw(" same e");
        leg2->AddEntry(h[i],Form("Attenuation length = %smm ",wid[i]), "l");
    }

    leg2->SetLineWidth(0);
    leg2->SetTextSize(0.03);
    leg2->Draw();
    
    
}
//----------------------new method--------------------------------------
void fReDraw(){
    namespace fs = std::filesystem;
    //using namespace std::string::find;
    set_root_style();
    std::string path = ".";
    
    int fileNum = 0;
    
    for (const auto & entry : fs::directory_iterator(path)){
        std::string fileName = entry.path();
        if (fileName.find(".root") != string::npos){
            fileNum++;
        }
    }
    string fileName[fileNum];
    int counter1 = 0;
    for (const auto & entry : fs::directory_iterator(path)){
        std::string fileNameTmp = entry.path();
        const char *fN = fileNameTmp.c_str();
        if (fileNameTmp.find(".root") != string::npos){
            fileName[counter1] = fileNameTmp;
            counter1++;
           // cout << entry.path() << endl;
        }
    }
    //sort fileName array;
    toString(fileName, fileNum);
    std::sort(fileName, fileName + fileNum);
    toString(fileName, fileNum);


     TChain *fileChain[fileNum];
    for (int i = 0; i < fileNum; i++){
        const char *fN = fileName[i].c_str();
        fileChain[i] = new TChain("LYSim");
        fileChain[i] -> Add(fN);
        //std::cout << entry.path() << std::endl;
    }
    double xAxis [fileNum];
    double yAxis [fileNum];
    for (int i = 0; i < fileNum; i++){
        double xTmp = stod(fileName[i].substr(2, 3));
        xAxis[i] = xTmp;
    }
    TH1F * histoTmp=new TH1F("h","h",1000,0,1000);
    toString(xAxis, fileNum);
    for (int i = 0; i < fileNum; i++){
        fileChain[i]->Draw("detectphotons3+detectphotons4>>h");
        yAxis[i] = histoTmp -> GetMean();
    }
    toString(yAxis, fileNum);
    auto g = new TGraph(fileNum, xAxis, yAxis);
    g -> GetXaxis() -> SetTitle("Refraction");
    g -> GetYaxis() -> SetTitle("PE");
    g -> Draw();


}


void fRe(){
    //scan_(epoxy();
    fReDraw();
}


//


// Improve styling
void set_root_style()
{
    gStyle->SetTextFont(42);

    // Canvas
    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderSize(10);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasDefH(800);
    gStyle->SetCanvasDefW(1000);
    gStyle->SetCanvasDefX(10);
    gStyle->SetCanvasDefY(10);

    // Pads
    gStyle->SetPadColor(0);
    gStyle->SetPadBorderSize(10);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadBottomMargin(0.2);
    gStyle->SetPadTopMargin(0.1);
    gStyle->SetPadLeftMargin(0.2);
    gStyle->SetPadRightMargin(0.2);
    gStyle->SetPadGridX(0);
    gStyle->SetPadGridY(0);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    // Frames
    gStyle->SetFrameFillStyle(0);
    gStyle->SetFrameFillColor(kWhite);
    gStyle->SetFrameLineColor(kBlack);
    gStyle->SetFrameLineStyle(0);
    gStyle->SetFrameLineWidth(1);
    gStyle->SetFrameBorderSize(10);
    gStyle->SetFrameBorderMode(0);

    // Markers
    gStyle->SetMarkerStyle(3);
    gStyle->SetMarkerSize(3);

    // Errors
    gStyle->SetEndErrorSize(4);
    gStyle->SetErrorX(0.);

    // Various
    gStyle->SetTickLength(0.03, "XYZ");
    gStyle->SetTitleSize(0.055, "XYZ");
    gStyle->SetTitleFont(42, "XYZ");
    gStyle->SetLabelFont(42, "XYZ");
    gStyle->SetNdivisions(405, "XYZ");

    gStyle->SetTitleOffset(1.1, "X");
    gStyle->SetTitleOffset(1.5, "Y");
    gStyle->SetTitleOffset(1.15, "Z");

    gStyle->SetLabelOffset(0.004, "X");
    gStyle->SetLabelOffset(0.015, "Y");
    gStyle->SetLabelOffset(0.015, "Z");

    gStyle->SetLabelSize(0.055, "X");
    gStyle->SetLabelSize(0.055, "Y");
    gStyle->SetLabelSize(0.055, "Z");

    gStyle->SetStatFont(42);
    gStyle->SetTitleFont(42);

    // Options
    gStyle->SetOptFit(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetPalette(kCMYK);

    gROOT->ForceStyle();

    return;
}
