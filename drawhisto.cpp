#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <TROOT.h>
#include <TFile.h>
#include <TStyle.h>
#include <TText.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TRint.h>
using namespace std;

int main (int argc, char** argv){
    TRint rootapp("app",&argc,argv);

    // Default input file parameters
    int exp_date = 20210611;
    int slot_no = 1;
    int input_no = 1;

    // Input file parameters passed by argument
    if (argc != 5){
        cout << "usage: ./drawhisto <date> <time> <slot> <input>" << endl;
        cout << "enter 'd' to use the default value" << endl;
        exit(1);
    }
    if (argv[1] != "d") exp_date = stoi(argv[1]);
    int run_time = stoi(argv[2]);
    if (argv[3] != "d") slot_no = stoi(argv[3]);
    if (argv[4] != "d") input_no = stoi(argv[4]);

    // CSV file to read data from
    const char* filename = Form("./-%08d-%06d-Slot%01d-In%01d.csv",exp_date,run_time,slot_no,input_no);

    // Read live-time of measurement (cell B37)
    double timedur = -9999.;
    ifstream readcell(filename);
    if (!readcell){
        cout << "Cannot open datafile " << filename << "!" << endl;
        exit(1);
    }else{
        cout << "Analyzing data file " << filename << "..." << endl;
    }
    string line;
    int row = 0;
    int col = 0;
    while ( getline(readcell,line) ){ // read loop for entire CSV file
        ++row;
        istringstream linestream(line);
        string item;
        while ( getline(linestream,item,',') ){ // read loop for each line
            ++col;
            if (row==37 && col==2){
                timedur = stod(item); // live-time measurement duration in seconds
                cout << "Measurement live-time = " << timedur << " seconds." << endl;
                break;
            }
        }
        if (timedur != -9999.) break;
    }

    // Read CSV file into TTree
    TTree *tree = new TTree("rawtree","Raw data");
    int rawnum;
    double rawtime, rawchannel;
    tree->Branch("rawnum",&rawnum,"rawnum/I");
    tree->Branch("rawtime",&rawtime,"rawtime/D");
    tree->Branch("rawchannel",&rawchannel,"rawchannel/D");
    tree->ReadFile(filename,"rawnum/I:rawtime/D:rawchannel/D",',');

    // Set title font
    gStyle->SetLabelSize(0.07,"XY");
    gStyle->SetPadBottomMargin(0.13);
    gStyle->SetTitleFontSize(0.1);
    TText T;
    T.SetTextFont(42);
    T.SetTextSize(0.08);
    T.SetTextAlign(21);

    // Create TCanvas (split vertically in half)
    TCanvas *c_raw = new TCanvas("c_raw");
    c_raw->Divide(1,2);

    // Energy histogram
    c_raw->cd(1);

    tree->Draw("rawchannel>>ehisto(4096,0.,4096.)");
    TH1F *ehtree = (TH1F*)gROOT->FindObject("ehisto");
    ehtree->SetTitle("");
    ehtree->Draw();
    ehtree->GetXaxis()->SetRangeUser(ll,ul);

    T.DrawTextNDC(.5,.93,Form("Raw energy histogram %s",filename));
    ehtree->GetXaxis()->SetTitleSize(0.07);
    ehtree->GetXaxis()->SetTitle("Energy (CH)");
    ehtree->GetXaxis()->SetTitleOffset(0.85);
    ehtree->GetYaxis()->SetTitleSize(0.07);
    ehtree->GetYaxis()->SetTitle(Form("Counts (/%3.0f CH/%3.2f min.)",ehtree->GetXaxis()->GetBinWidth(0),timedur/60.));
    ehtree->GetYaxis()->SetTitleOffset(0.4);


    // Time histogram
    c_raw->cd(2);

    tree->Draw(Form("rawtime>>thtree(%d,0.,%3.0f)",timedur,double(timedur)*TMath::Power(10.,6)));
    TH1D *thtree = (TH1D*)gROOT->FindObject("thtree");
    thtree->SetTitle("");
    thtree->Draw();

    T.DrawTextNDC(.5,.93,Form("Raw time histogram %s",filename));
    thtree->GetXaxis()->SetTitleSize(0.07);
    thtree->GetXaxis()->SetTitle("Time (#mus)");
    thtree->GetXaxis()->SetTitleOffset(0.85);
    thtree->GetYaxis()->SetTitleSize(0.07);
    thtree->GetYaxis()->SetTitle(Form("Counts (/%3.1f s)",thtree->GetXaxis()->GetBinWidth(0)*TMath::Power(10.,-6)));
    thtree->GetYaxis()->SetTitleOffset(0.35);


    // Update canvas and finish
    c_raw->Update();
    c_raw->Modified();

    rootapp.Run();
    return 0;
}