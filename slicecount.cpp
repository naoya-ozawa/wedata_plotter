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
#include <THStack.h>
#include <TF1.h>
#include <TLatex.h>
#include <TRint.h>
using namespace std;

int main (int argc, char** argv){

    // Input file parameters passed by argument
    if (argc != 9){
        cout << "usage: ./slicecount <date> <time> <slot> <input> <e_ll (ch)> <e_ul (ch)> <t_ll (s)> <t_ul (s)>" << endl;
        exit(1);
    }

    string exp_date = argv[1];
    string run_time = argv[2];
    string slot_no = argv[3];
    string input_no = argv[4];
    string e_ll = argv[5];
    string e_ul = argv[6];
    string t_ll = argv[7];
    string t_ul = argv[8];

    // CSV file to read data from
    string filelabel = "-" + exp_date + "-" + run_time + "-Slot" + slot_no + "-In" + input_no;
//    string filelocation = "./../" + filelabel + ".csv";
    string filelocation = "./" + filelabel + ".csv";
    const char* filename = filelocation.c_str();

    // // Create output directory (if it does not exist yet)
    // string outdir = "./../analysis/" + filelabel;
    // struct stat statBuf;
    // if (stat(outdir.c_str(),&statBuf) != 0) mkdir(outdir.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH);

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
    TRint rootapp("app",&argc,argv);
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

	THStack *energy_spectrum = new THStack();

    tree->Draw("rawchannel>>ehisto(4096,0.,4096.)");
    TH1F *ehtree = (TH1F*)gROOT->FindObject("ehisto");
    ehtree->SetTitle("Raw Data");

	tree->Draw(Form("rawchannel>>ehisto_cut(%d,%3.0f,%3.0f)",stoi(e_ul)-stoi(e_ll),stod(e_ll),stod(e_ul)),Form("rawtime>%g && rawtime<%g",stod(t_ll)*TMath::Power(10.,6),stod(t_ul)*TMath::Power(10.,6)));
	TH1D *energy_spectrum_cut = (TH1D*)gROOT->FindObject("ehisto_cut");
    energy_spectrum_cut->SetTitle("Counted Data");
    energy_spectrum_cut->SetLineColor(38);
    energy_spectrum_cut->SetFillColor(38);
    energy_spectrum_cut->SetFillStyle(1001);

    energy_spectrum->Add(ehtree);
    energy_spectrum->Add(energy_spectrum_cut);
    energy_spectrum->Draw("nostack");

    T.DrawTextNDC(.5,.93,Form("Raw and sliced energy histogram %s (e: %d-%d ch, t: %d-%d s)",filename,stoi(e_ll),stoi(e_ul),stoi(t_ll),stoi(t_ul)));
    energy_spectrum->GetXaxis()->SetTitleSize(0.07);
    energy_spectrum->GetXaxis()->SetTitle("Energy (CH)");
    energy_spectrum->GetXaxis()->SetTitleOffset(0.85);
    energy_spectrum->GetYaxis()->SetTitleSize(0.07);
    energy_spectrum->GetYaxis()->SetTitle(Form("Counts (/%3.0f CH/%3.2f min.)",ehtree->GetXaxis()->GetBinWidth(0),timedur/60.));
    energy_spectrum->GetYaxis()->SetTitleOffset(0.45);
    c_raw->cd(1)->SetLogy();

    TLatex *l_espectrum = new TLatex();
    l_espectrum->DrawLatex(stoi(e_ll),energy_spectrum_cut->GetMaximum()/10.,Form("%3.2f counts/min.",energy_spectrum_cut->Integral()*60./(stod(t_ul)-stod(t_ll))));

    // Time histogram
    c_raw->cd(2);

    THStack *time_spectrum = new THStack();

    tree->Draw(Form("rawtime>>thtree(%d,0.,%3.0f)",TMath::Nint(timedur),double(TMath::Nint(timedur))*TMath::Power(10.,6)));
    TH1D *thtree = (TH1D*)gROOT->FindObject("thtree");
    thtree->SetTitle("");
    thtree->Draw();    

	tree->Draw(Form("rawtime>>thisto_cut(%d,%3.0f,%3.0f)",stoi(t_ul)-stoi(t_ll),stod(t_ll)*TMath::Power(10.,6),stod(t_ul)*TMath::Power(10.,6)),Form("rawchannel>%g && rawchannel<%g",stod(e_ll),stod(e_ul)));
	TH1D *time_spectrum_cut = (TH1D*)gROOT->FindObject("thisto_cut");
    time_spectrum_cut->SetTitle("Counted Data");
    time_spectrum_cut->SetLineColor(38);
    time_spectrum_cut->SetFillColor(38);
    time_spectrum_cut->SetFillStyle(1001);
    TF1 *f_decay = new TF1("f_decay","[0]*TMath::Exp(-x/[1])+[2]",stod(t_ll)*TMath::Power(10.,6),stod(t_ul)*TMath::Power(10.,6));
    f_decay->SetParameters(1000.,5.0*60.*TMath::Power(10.,6),10.);
    time_spectrum_cut->Fit("f_decay");

    time_spectrum->Add(thtree);
    time_spectrum->Add(time_spectrum_cut);
    time_spectrum->Draw("nostack");

    filename = Form("./-%s-%s-Slot%s-In%s.csv",exp_date.c_str(),run_time.c_str(),slot_no.c_str(),input_no.c_str()); // re-set
    T.DrawTextNDC(.5,.93,Form("Raw and sliced time histogram %s (e: %d-%d ch, t: %d-%d s)",filename,stoi(e_ll),stoi(e_ul),stoi(t_ll),stoi(t_ul)));
    time_spectrum->GetXaxis()->SetTitleSize(0.07);
    time_spectrum->GetXaxis()->SetTitle("Time (#mus)");
    time_spectrum->GetXaxis()->SetTitleOffset(0.85);
    time_spectrum->GetYaxis()->SetTitleSize(0.07);
    time_spectrum->GetYaxis()->SetTitle(Form("Counts (/%3.1f s)",thtree->GetXaxis()->GetBinWidth(0)*TMath::Power(10.,-6)));
    time_spectrum->GetYaxis()->SetTitleOffset(0.40);

    TLatex *l_tspectrum = new TLatex();
    l_tspectrum->DrawLatex(stoi(t_ll)*TMath::Power(10.,6),time_spectrum_cut->GetMaximum()/10.,Form("#tau = %3.2f#pm%3.2f s",f_decay->GetParameter(1)*TMath::Power(10.,-6),f_decay->GetParError(1)*TMath::Power(10.,-6)));

    // Update canvas and finish
    c_raw->Update();
    c_raw->Modified();

    rootapp.Run();

    return 0;
}
