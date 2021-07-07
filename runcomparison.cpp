// TAKEN FROM 2020 BT SCRIPT runcomparison.cpp (n.ozawa)

#include <iostream>
#include <TROOT.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TTree.h>
#include <TH1F.h>
#include <THStack.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TLatex.h>
#include <TRint.h>
using namespace std;

int main (int argc, char** argv){
    TRint rootapp("app",&argc,argv);

    const char* comparison_title = "Effect of IR heater";

    const char* filename_1 = "./../-20210611-183359-Slot1-In1.csv";
    int color_1 = 38;
    int timedur_1 = int(94.70*60.); // seconds
    const char* runtitle_1 = "Before heating (run 183359)";
    double tll1 = 0.;

    const char* filename_2 = "./../-20210611-204106-Slot1-In1.csv";
    int color_2 = 46;
    int timedur_2 = int(5.03*60.); // seconds
    const char* runtitle_2 = "During heating SV=80 (run 204106)";
    double tll2 = 0.;

    const char* filename_3 = "./../-20210611-204632-Slot1-In1.csv";
    int color_3 = 30;
    int timedur_3 = int(908.56*60.); // seconds
    const char* runtitle_3 = "After heating (run 204632)";
    double tll3 = 0.;

//    int color_cut4 = 33;
//    int color_cut5 = 42;

    double ll = 1500.;
    double ul = 3000.;

    double tll = 0.;
//    double tul = 1130.;
    double t_dur = 5.*60.;

    TTree *tree_1 = new TTree();
    tree_1->ReadFile(filename_1,"x:y:z",',');
    TCanvas *c1 = new TCanvas("c1");
    c1->Divide(1,2);

    c1->cd(1);
    tree_1->Draw("z>>ehisto_1(4096,0.,4096.");
    TH1F *ehtree_1 = (TH1F*)gROOT->FindObject("ehisto_1");
    ehtree_1->SetTitle(Form("Raw Energy Histogram of Data %s;Energy (Ch);Counts (/Ch)",filename_1));

    c1->cd(2);

    tree_1->Draw(Form("y>>thisto_1(%d,0.,%g)",timedur_1,double(timedur_1)*TMath::Power(10.,6)));
    TH1F *thtree_1 = (TH1F*)gROOT->FindObject("thisto_1");
    thtree_1->SetTitle(Form("Raw Time Histogram of Data %s;Time (#mus);Counts (/%3.1f s)",filename_1,thtree_1->GetXaxis()->GetBinWidth(0)*TMath::Power(10.,-6)));



    TTree *tree_2 = new TTree();
    tree_2->ReadFile(filename_2,"x:y:z",',');
    TCanvas *c2 = new TCanvas("c2");
    c2->Divide(1,2);

    c2->cd(1);
    tree_2->Draw("z>>ehisto_2(4096,0.,4096.");
    TH1F *ehtree_2 = (TH1F*)gROOT->FindObject("ehisto_2");
    ehtree_2->SetTitle(Form("Raw Energy Histogram of Data %s;Energy (Ch);Counts (/Ch)",filename_2));

    c2->cd(2);

    tree_2->Draw(Form("y>>thisto_2(%d,0.,%g)",timedur_2,double(timedur_2)*TMath::Power(10.,6)));
    TH1F *thtree_2 = (TH1F*)gROOT->FindObject("thisto_2");
    thtree_2->SetTitle(Form("Raw Time Histogram of Data %s;Time (#mus);Counts (/%3.1f s)",filename_2,thtree_2->GetXaxis()->GetBinWidth(0)*TMath::Power(10.,-6)));




    TTree *tree_3 = new TTree();
    tree_3->ReadFile(filename_3,"x:y:z",',');
    TCanvas *c3 = new TCanvas("c3");
    c3->Divide(1,2);

    c3->cd(1);
    tree_3->Draw("z>>ehisto_3(4096,0.,4096.");
    TH1F *ehtree_3 = (TH1F*)gROOT->FindObject("ehisto_3");
    ehtree_3->SetTitle(Form("Raw Energy Histogram of Data %s;Energy (Ch);Counts (/Ch)",filename_3));

    c3->cd(2);

    tree_3->Draw(Form("y>>thisto_3(%d,0.,%g)",timedur_3,double(timedur_3)*TMath::Power(10.,6)));
    TH1F *thtree_3 = (TH1F*)gROOT->FindObject("thisto_3");
    thtree_3->SetTitle(Form("Raw Time Histogram of Data %s;Time (#mus);Counts (/%3.1f s)",filename_3,thtree_3->GetXaxis()->GetBinWidth(0)*TMath::Power(10.,-6)));


    
    TCanvas *cstack = new TCanvas("cstack");
    cstack->Divide(1,2);

    cstack->cd(1);

	THStack *energy_spectrum = new THStack();
	energy_spectrum->SetTitle(Form("%s;Channel (CH);Counts (/CH/%3.0f s)",comparison_title,(t_dur)/(1.)));

	tree_1->Draw(Form("z>>histo_e_1(%d,%3.0f,%3.0f)",int(ul-ll),ll,ul),Form("y>%g && y<%g",tll1*TMath::Power(10.,6),(tll1+t_dur)*TMath::Power(10.,6)));
	TH1D *energy_spectrum_1 = (TH1D*)gROOT->FindObject("histo_e_1");
    energy_spectrum_1->SetTitle(runtitle_1);
    energy_spectrum_1->SetLineColor(color_1);
    energy_spectrum_1->SetFillStyle(0);
	energy_spectrum_1->SetLineWidth(2);

    tree_2->Draw(Form("z>>histo_e_2(%d,%3.0f,%3.0f)",int(ul-ll),ll,ul),Form("y>%g && y<%g",tll2*TMath::Power(10.,6),(tll2+t_dur)*TMath::Power(10.,6)));
	TH1D *energy_spectrum_2 = (TH1D*)gROOT->FindObject("histo_e_2");
    energy_spectrum_2->SetTitle(runtitle_2);
    energy_spectrum_2->SetLineColor(color_2);
    energy_spectrum_2->SetFillStyle(0);
	energy_spectrum_2->SetLineWidth(2);

    tree_3->Draw(Form("z>>histo_e_3(%d,%3.0f,%3.0f)",int(ul-ll),ll,ul),Form("y>%g && y<%g",tll3*TMath::Power(10.,6),(tll3+t_dur)*TMath::Power(10.,6)));
	TH1D *energy_spectrum_3 = (TH1D*)gROOT->FindObject("histo_e_3");
    energy_spectrum_3->SetTitle(runtitle_3);
    energy_spectrum_3->SetLineColor(color_3);
    energy_spectrum_3->SetFillStyle(0);
	energy_spectrum_3->SetLineWidth(2);

	energy_spectrum->Add(energy_spectrum_1);
    energy_spectrum->Add(energy_spectrum_2);
    energy_spectrum->Add(energy_spectrum_3);
    energy_spectrum->Draw("nostack");

    cstack->cd(1)->BuildLegend();


	cstack->cd(2);

	THStack *time_spectrum = new THStack();

	TH1D *time_spectrum_1 = (TH1D*)thtree_1->Clone();
    time_spectrum_1->SetTitle(runtitle_1);
    time_spectrum_1->SetLineColor(color_1);
    time_spectrum_1->SetFillStyle(0);
    time_spectrum_1->GetXaxis()->SetRangeUser(tll1*TMath::Power(10.,6),(tll1+t_dur)*TMath::Power(10.,6));

	TH1D *time_spectrum_2 = (TH1D*)thtree_2->Clone();
    time_spectrum_2->SetTitle(runtitle_2);
    time_spectrum_2->SetLineColor(color_2);
    time_spectrum_2->SetFillStyle(0);
    time_spectrum_2->GetXaxis()->SetRangeUser(tll2*TMath::Power(10.,6),(tll2+t_dur)*TMath::Power(10.,6));

	TH1D *time_spectrum_3 = (TH1D*)thtree_3->Clone();
    time_spectrum_3->SetTitle(runtitle_3);
    time_spectrum_3->SetLineColor(color_3);
    time_spectrum_3->SetFillStyle(0);
    time_spectrum_3->GetXaxis()->SetRangeUser(tll3*TMath::Power(10.,6),(tll3+t_dur)*TMath::Power(10.,6));

	time_spectrum->SetTitle(Form("Time Spectrum;Time Elapsed (#mus);Counts (/%3.1f s/4096 CH)",time_spectrum_1->GetXaxis()->GetBinWidth(0)*TMath::Power(10.,-6)));

	time_spectrum->Add(time_spectrum_1);
    time_spectrum->Add(time_spectrum_2);
    time_spectrum->Add(time_spectrum_3);
    time_spectrum->Draw("nostack");
	cstack->cd(2)->BuildLegend();

    rootapp.Run();
    return 0;
}
