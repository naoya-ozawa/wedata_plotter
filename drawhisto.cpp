#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <TROOT.h>
#include <TMath.h>
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

    // Input file parameters passed by argument
    if (argc != 5){
        cout << "usage: ./drawhisto <date> <time> <slot> <input>" << endl;
        exit(1);
    }
    string exp_date = argv[1];
    string run_time = argv[2];
    string slot_no = argv[3];
    string input_no = argv[4];

    // CSV file to read data from
    string filelabel = "-" + exp_date + "-" + run_time + "-Slot" + slot_no + "-In" + input_no;
    string filelocation = "./../" + filelabel + ".csv";
    const char* filename = filelocation.c_str();

    // Create output directory (if it does not exist yet)
    string outdir = "./../analysis/" + filelabel;
    struct stat statBuf;
    if (stat(outdir.c_str(),&statBuf) != 0) mkdir(outdir.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH);

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
    int k = 0;
    while ( getline(readcell,line) ){ // read loop for entire CSV file
        istringstream linestream(line);
        string item;
        while ( getline(linestream,item,',') ){ // read loop for each line
            ++k; // item counter
            if (k==75){
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
    TRint rootapp("app",&argc,argv);
    TCanvas *c_raw = new TCanvas("c_raw");
    c_raw->Divide(1,2);

    // Energy histogram
    c_raw->cd(1);

    double ll = 0.;
    double ul = 4096.;
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
    ehtree->GetYaxis()->SetTitleOffset(0.45);
    c_raw->cd(1)->SetLogy();

    // Time histogram
    c_raw->cd(2);

    tree->Draw(Form("rawtime>>thtree(%d,0.,%3.0f)",TMath::Nint(timedur),double(TMath::Nint(timedur))*TMath::Power(10.,6)));
    TH1D *thtree = (TH1D*)gROOT->FindObject("thtree");
    thtree->SetTitle("");
    thtree->Draw();
    
    filename = Form("./-%s-%s-Slot%s-In%s.csv",exp_date.c_str(),run_time.c_str(),slot_no.c_str(),input_no.c_str()); // re-set
    T.DrawTextNDC(.5,.93,Form("Raw time histogram %s",filename));
    thtree->GetXaxis()->SetTitleSize(0.07);
    thtree->GetXaxis()->SetTitle("Time (#mus)");
    thtree->GetXaxis()->SetTitleOffset(0.85);
    thtree->GetYaxis()->SetTitleSize(0.07);
    thtree->GetYaxis()->SetTitle(Form("Counts (/%3.1f s)",thtree->GetXaxis()->GetBinWidth(0)*TMath::Power(10.,-6)));
    thtree->GetYaxis()->SetTitleOffset(0.40);


    // Update canvas and finish
    c_raw->Update();
    c_raw->Modified();

    rootapp.Run();
    return 0;
}
