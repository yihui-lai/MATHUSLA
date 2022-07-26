
void set_root_style();

void PrintArray(double array[]){
    for (int k = 0; k < 50; k++)
        cout<<Form("   At Min PE = %d, SD =  ",k+1) + to_string(array[k]) <<endl;
}
//double[] convertArray(double array[][]){
//    double tmp
    
    
//    return tmp;
//}

void sigp()
{
    //set_root_style();
    TCanvas*c=new TCanvas();
    const char *name[4] = {"1.7ns","2.5ns","5.0ns","7.0ns"};
    TH1F *histo[4];
    TChain *chain[4];
    //TH1F *histo[4];
    double sigma[4][50];
    double sigma_e[4][50];
    
    double x[4][50];
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 50; j++){
            x[i][j] = j+1;
        }
    }
    
    //TH1F *histo = new TH1F("histo", "Histogram", 50, -10, 10);
    //TChain *chain[4] = new TChain("LYSim");
    
    for(int k = 0; k < 4; k++){
        chain[k] = new TChain("LYSim");
        chain[k] -> Add(Form("%sscp.root",name[k]));
        histo[k] = new TH1F(Form("histo%s",name[k]), "Histogram", 50, -10, 10);
        //Form("detectphotons3+detectphotons4:BeamX>>htemp%s",name[i])
        //chain -> Draw(Form("(dt_rms3-dt_rms4)/2>>histo%s",name[k]),Form("min(detectphotons3,detectphotons4)==%d",i));
        for (int i = 1; i <= 50; i++){
            chain[k] -> Draw(Form("(dt_rms3-dt_rms4)/2>>histo%s",name[k]),Form("min(detectphotons3,detectphotons4)==%d",i));
            TF1 *fit = new TF1("fit", "gaus");
            if (histo[k] ->Integral()>10){
            histo[k] -> Fit("fit","Q");
            sigma[k][i-1] = fit -> GetParameter(2);
            sigma_e[k][i-1] = fit -> GetParError(2);
            }else{
                sigma[k][i-1] =-1;
                sigma_e[k][i-1] = 0;
            }
        }
        
        
    }
    
    //PrintArray(sigma[3]);
    
    
    
    TGraphErrors *sca[4];
    TMultiGraph *gg=new TMultiGraph();
    for (int c = 0; c < 4; c++){
        sca[c] = new TGraphErrors(50,x[c],sigma[c],0,sigma_e[c]);
        sca[c] -> SetMarkerColor(c+1);
        sca[c] -> SetMarkerStyle(20+c);
        sca[c] -> SetMarkerSize(0.5);
        //sca[c]->SetLineWidth(4);
        sca[c] -> SetLineColor(c+1);
        if (c == 0){
            sca[c] -> Draw("AP");
        }else{
            sca[c] -> Draw("AP same");
        }
        gg->Add(sca[c]);
        //sca[c] -> Draw("AC*");
    }
    //sca[0]->SetLineWidth(4);
    sca[0] -> GetXaxis() -> SetRangeUser(0,25);
    sca[0] -> GetYaxis() -> SetRangeUser(0,8);
    sca[0] -> GetXaxis() -> SetTitle("Minimum PE");
    sca[0] -> GetYaxis() -> SetTitle("Timing Resolution (ns)");
    //auto sca[c] = new TGraph(50,x,sigma);
    sca[0] -> SetTitle("1.7 ns");
    sca[1] -> SetTitle("2.5 ns");
    sca[2] -> SetTitle("5.0 ns");
    sca[3] -> SetTitle("7.0 ns");
    
    TF1 *ffit[4];
    for(int k = 0; k < 4; k++){
        ffit[k] = new TF1(Form("ffit_%d",k), "[0]/sqrt(x)");
        ffit[k] -> SetLineColor(k+1);
        sca[k] ->Fit(Form("ffit_%d",k));
    }
    
    gg -> GetXaxis() -> SetRangeUser(0,25);
    gg -> GetYaxis() -> SetRangeUser(0,8);
    gg -> GetXaxis() -> SetTitle("Minimum PE");
    gg -> GetYaxis() -> SetTitle("Timing Resolution (ns)");
    gg->Draw("AP E");
    
    //sca->SetMarkerColor(1);
    //sca -> Draw("AC*");
    
    c->BuildLegend();
     
     
    /*auto leg2 = new TLegend(0.25,0.7,0.5,0.85);
    leg2->AddEntry(sca,"1.7 ns", "l");
    leg2->SetLineWidth(0);
    leg2->SetTextSize(0.03);
    leg2->Draw();
    */
    
}





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
    gStyle->SetMarkerSize(0.5);

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
