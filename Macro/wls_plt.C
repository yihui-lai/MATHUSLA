//FNALNew_trial220516_testcladdirt
//scp -r ug_xyc@hepcms.umd.edu:~/CMSSW_11_3_0_pre4/src/HGCalTileSim/condor/4_1_50/*.png /Users/yucun_xie/Desktop
void set_root_style();


//single plot
void scan_epoxy(){

    set_root_style();
    
    TCanvas *c1=new TCanvas();
    TChain *f=new TChain("LYSim");
    f->Add("./hole.root");
    TH2F *htemp = new TH2F("htemp",";x position;LY;",20,-20,20,100,0,500);
    f->Draw("detectphotons3+detectphotons4:BeamX>>htemp");
    htemp->Draw("colz");
    
    TCanvas *c2=new TCanvas();
    TH1F *h1 = new TH1F("h1",";LY;",20,-20,20);
    h1=(TH1F*)htemp->ProfileX();
    h1->Draw("e");
}


void wls_plt(){
    scan_epoxy();
}





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
    gStyle->SetMarkerStyle(8);
    gStyle->SetMarkerSize(1.35);

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
