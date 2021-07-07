// TAKEN FROM 2020 BT script timewindow_comparison.cpp (n.ozawa)

#include <iostream>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TText.h>
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

    int date = 20210611;
    int time = 204632;
    const char* filename = Form("./../-%08d-%06d-Slot1-In1.csv",date,time);
    TTree *tree = new TTree();
    tree->ReadFile(filename,"x:y:z",',');
    TCanvas *c0 = new TCanvas("c0");
    c0->Divide(1,2);

    c0->cd(1);
    tree->Draw("z>>ehisto(4096,0.,4096.)");
    TH1F *ehtree = (TH1F*)gROOT->FindObject("ehisto");
    ehtree->SetTitle(Form("Raw Energy Histogram of Data %s;Energy (Ch);Counts (/%3.0f Ch)",filename,ehtree->GetXaxis()->GetBinWidth(0)));

    double ll = 2200.;
    double ul = 3000.;

    c0->cd(2);

    int timedur = int(908.56*60.); // seconds

    tree->Draw(Form("y>>thisto(%d,0.,%g)",timedur,double(timedur)*TMath::Power(10.,6)));
    TH1F *thtree = (TH1F*)gROOT->FindObject("thisto");
	thtree->SetTitle(Form("Raw Time Histogram of Data %s;Time (#mus);Counts (%3.1f /s)",filename,thtree->GetXaxis()->GetBinWidth(0)*TMath::Power(10.,-6)));

    gStyle->SetLabelSize(0.055,"XY");
    gStyle->SetPadBottomMargin(0.12);
    TCanvas *c1 = new TCanvas("c1");
    TText T;
    T.SetTextFont(42);
    T.SetTextSize(0.07);
    T.SetTextAlign(21);
    c1->Divide(1,2);

	double tcut1 = 0.*TMath::Power(10.,6);
    const char* cuttitle_1 = "Timeframe 1";
	double tcut2 = tcut1 + 60.*60.*TMath::Power(10.,6);
    const char* cuttitle_2 = "Timeframe 2";
	double tcut3 = tcut2 + 60.*60.*TMath::Power(10.,6);
    const char* cuttitle_3 = "Timeframe 3";
//	double tcut4 = tcut3 + 24.*60.*60.*TMath::Power(10.,6);
//    const char* cuttitle_4 = "9/30 0 AM - 2 AM";
//	double tcut5 = tcut4 + 24.*60.*60.*TMath::Power(10.,6);
//    const char* cuttitle_5 = "10/1 0 AM - 2 AM";
    double time_limit = 20.*60.*TMath::Power(10.,6);

    int color_cut1 = 38;
    int color_cut2 = 46;
    int color_cut3 = 30;
//    int color_cut4 = 33;
//    int color_cut5 = 42;

    c1->cd(1);

	THStack *energy_spectrum = new THStack();

	tree->Draw(Form("z>>histo_e_cut1(%d,%3.0f,%3.0f)",int(ul-ll),ll,ul),Form("y>%g && y<%g",tcut1,tcut1+time_limit));
	TH1D *energy_spectrum_cut1 = (TH1D*)gROOT->FindObject("histo_e_cut1");
    energy_spectrum_cut1->SetTitle(cuttitle_1);
    energy_spectrum_cut1->SetLineColor(color_cut1);
    energy_spectrum_cut1->SetFillStyle(0);
	energy_spectrum_cut1->SetLineWidth(2);

    tree->Draw(Form("z>>histo_e_cut2(%d,%3.0f,%3.0f)",int(ul-ll),ll,ul),Form("y>%g && y<%g",tcut2,tcut2+time_limit));
	TH1D *energy_spectrum_cut2 = (TH1D*)gROOT->FindObject("histo_e_cut2");
    energy_spectrum_cut2->SetTitle(cuttitle_2);
    energy_spectrum_cut2->SetLineColor(color_cut2);
    energy_spectrum_cut2->SetFillStyle(0);
	energy_spectrum_cut2->SetLineWidth(2);

	tree->Draw(Form("z>>histo_e_cut3(%d,%3.0f,%3.0f)",int(ul-ll),ll,ul),Form("y>%g && y<%g",tcut3,tcut3+time_limit));
	TH1D *energy_spectrum_cut3 = (TH1D*)gROOT->FindObject("histo_e_cut3");
	energy_spectrum_cut3->SetTitle(cuttitle_3);
	energy_spectrum_cut3->SetLineColor(color_cut3);
    energy_spectrum_cut3->SetFillStyle(0);
	energy_spectrum_cut3->SetLineWidth(2);
/*
	tree->Draw(Form("z>>histo_e_cut4(%d,%3.0f,%3.0f)",int(ul-ll),ll,ul),Form("y>%g && y<%g",tcut4,tcut4+time_limit));
	TH1D *energy_spectrum_cut4 = (TH1D*)gROOT->FindObject("histo_e_cut4");
	energy_spectrum_cut4->SetTitle(cuttitle_4);
	energy_spectrum_cut4->SetLineColor(color_cut4);
    energy_spectrum_cut4->SetFillStyle(0);
	energy_spectrum_cut4->SetLineWidth(2);

	tree->Draw(Form("z>>histo_e_cut5(%d,%3.0f,%3.0f)",int(ul-ll),ll,ul),Form("y>%g && y<%g",tcut5,tcut5+time_limit));
	TH1D *energy_spectrum_cut5 = (TH1D*)gROOT->FindObject("histo_e_cut5");
	energy_spectrum_cut5->SetTitle(cuttitle_5);
	energy_spectrum_cut5->SetLineColor(color_cut5);
    energy_spectrum_cut5->SetFillStyle(0);
	energy_spectrum_cut5->SetLineWidth(2);
*/
	energy_spectrum->Add(energy_spectrum_cut1);
    energy_spectrum->Add(energy_spectrum_cut2);
	energy_spectrum->Add(energy_spectrum_cut3);
//	energy_spectrum->Add(energy_spectrum_cut4);
//	energy_spectrum->Add(energy_spectrum_cut5);
    energy_spectrum->Draw("nostack");


    T.DrawTextNDC(.5,.93,Form("Energy Spectrum Comparison (run %08d-%06d)",date,time));
    energy_spectrum->GetXaxis()->SetTitleSize(0.06);
    energy_spectrum->GetXaxis()->SetTitle("Channel (CH)");
    energy_spectrum->GetXaxis()->SetTitleOffset(0.85);
    energy_spectrum->GetYaxis()->SetTitleSize(0.06);
    energy_spectrum->GetYaxis()->SetTitle(Form("Counts (/CH/%3.0f min.)",time_limit*TMath::Power(10.,-6)/60.));
    energy_spectrum->GetYaxis()->SetTitleOffset(0.40);


    c1->cd(1)->BuildLegend();


	c1->cd(2);

	THStack *time_spectrum = new THStack();

	tree->Draw(Form("y>>histo_t(%d,0.,%g)",timedur,double(timedur)*TMath::Power(10.,6)));
	TH1D *time_spectrum_total = (TH1D*)gROOT->FindObject("histo_t");
    time_spectrum_total->SetTitle("Entire Data");
    time_spectrum_total->SetFillStyle(0);

	TH1D *time_spectrum_cut1 = (TH1D*)time_spectrum_total->Clone();
    time_spectrum_cut1->SetTitle(cuttitle_1);
    time_spectrum_cut1->SetLineColor(color_cut1);
    time_spectrum_cut1->SetFillStyle(1001);
    time_spectrum_cut1->SetFillColor(color_cut1);
    time_spectrum_cut1->GetXaxis()->SetRangeUser(tcut1,tcut1+time_limit);
//	TF1 *mean_cut1 = new TF1("Ave. for 1.0 e#muA","[0]",tcut1,tcut1+time_limit);
//	mean_cut1->SetParameter(0,1.0);
//	time_spectrum_cut1->Fit(mean_cut1,"","",tcut1,tcut1+time_limit);

	TH1D *time_spectrum_cut2 = (TH1D*)time_spectrum_total->Clone();
    time_spectrum_cut2->SetTitle(cuttitle_2);
    time_spectrum_cut2->SetLineColor(color_cut2);
    time_spectrum_cut2->SetFillStyle(1001);
    time_spectrum_cut2->SetFillColor(color_cut2);
    time_spectrum_cut2->GetXaxis()->SetRangeUser(tcut2,tcut2+time_limit);
//	TF1 *mean_cut2 = new TF1("Ave. for 1.5 e#muA","[0]",tcut2,tcut2+time_limit);
//	mean_cut2->SetParameter(0,1.0);
//	time_spectrum_cut2->Fit(mean_cut2,"","",tcut2,tcut2+time_limit);

	TH1D *time_spectrum_cut3 = (TH1D*)time_spectrum_total->Clone();
    time_spectrum_cut3->SetTitle(cuttitle_3);
    time_spectrum_cut3->SetLineColor(color_cut3);
    time_spectrum_cut3->SetFillStyle(1001);
        time_spectrum_cut3->SetFillColor(color_cut3);
    time_spectrum_cut3->GetXaxis()->SetRangeUser(tcut3,tcut3+time_limit);
//	TF1 *mean_cut3 = new TF1("Ave. for 2.0 e#muA","[0]",tcut3,tcut3+time_limit);
//	mean_cut3->SetParameter(0,1.0);
//	time_spectrum_cut3->Fit(mean_cut3,"","",tcut3,tcut3+time_limit);
/*
	TH1D *time_spectrum_cut4 = (TH1D*)time_spectrum_total->Clone();
    time_spectrum_cut4->SetTitle(cuttitle_4);
    time_spectrum_cut4->SetLineColor(color_cut4);
//    time_spectrum_cut4->SetFillColor(color_cut4);
    time_spectrum_cut4->GetXaxis()->SetRangeUser(tcut4,tcut4+time_limit);
//	TF1 *mean_cut4 = new TF1("Ave. for 2.0 e#muA","[0]",tcut4,tcut4+time_limit);
//	mean_cut4->SetParameter(0,1.0);
//	time_spectrum_cut4->Fit(mean_cut4,"","",tcut4,tcut4+time_limit);

	TH1D *time_spectrum_cut5 = (TH1D*)time_spectrum_total->Clone();
    time_spectrum_cut5->SetTitle(cuttitle_5);
    time_spectrum_cut5->SetLineColor(color_cut5);
//    time_spectrum_cut5->SetFillColor(color_cut5);
    time_spectrum_cut5->GetXaxis()->SetRangeUser(tcut5,tcut5+time_limit);
//	TF1 *mean_cut5 = new TF1("Ave. for 2.0 e#muA","[0]",tcut5,tcut5+time_limit);
//	mean_cut5->SetParameter(0,1.0);
//	time_spectrum_cut5->Fit(mean_cut5,"","",tcut5,tcut5+time_limit);
*/
	time_spectrum->Add(time_spectrum_total);
	time_spectrum->Add(time_spectrum_cut1);
    time_spectrum->Add(time_spectrum_cut2);
	time_spectrum->Add(time_spectrum_cut3);
//	time_spectrum->Add(time_spectrum_cut4);
//	time_spectrum->Add(time_spectrum_cut5);
    time_spectrum->Draw("nostack");

    T.DrawTextNDC(.5,.93,Form("Time Spectrum (run %08d-%06d)",date,time));
    time_spectrum->GetXaxis()->SetTitleSize(0.06);
    time_spectrum->GetXaxis()->SetTitle("Time elapsed (#mus)");
    time_spectrum->GetXaxis()->SetTitleOffset(0.85);
    time_spectrum->GetYaxis()->SetTitleSize(0.06);
    time_spectrum->GetYaxis()->SetTitle(Form("Counts (/%3.1f s/4096 CH)",time_spectrum_total->GetXaxis()->GetBinWidth(0)*TMath::Power(10.,-6)));
    time_spectrum->GetYaxis()->SetTitleOffset(0.40);

	c1->cd(2)->BuildLegend();

    c1->Update();
    c1->Modified();

    rootapp.Run();
    return 0;
}
