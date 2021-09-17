// Set up ROOT and run the code with
// root -l cumulantLooping.C
// The input file is from a toy model with 200 events and 100 particles per event
#include <stdio.h>
#include <complex>
#include <iostream>
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
using namespace std; 

void cumulantLooping(){

     long nEvt; 
     Int_t nParticle;
     Float_t phi[500];
     Float_t pt[500];
     Float_t eta[500];

     TH1D *histEvtCosPhi2 = new TH1D("histEvtCosPhi2", "histEvtCosPhi2", 220,-1.1,1.1);
     TH1D *histEvtCosPhi4 = new TH1D("histEvtCosPhi4", "histEvtCosPhi4", 220,-1.1,1.1);
     TH1D *histCorrelator2 = new TH1D("histCorrelator2", "histCorrelator2", 220,-1.1,1.1);
     TH1D *histCorrelator4 = new TH1D("histCorrelator4", "histCorrelator4", 220,-1.1,1.1);

     float gapSize = 0.8; 
     TChain t1("t1");
     t1.Add("./toy_mult100_200events.root/tree");
     t1.SetBranchAddress("nTrk", &nParticle);
     t1.SetBranchAddress("pt", &pt);
     t1.SetBranchAddress("eta", &eta);
     t1.SetBranchAddress("phi", &phi);

     nEvt=t1.GetEntries();
     //nEvt=20;
     cout<<endl<<"nEvt = "<<nEvt<<endl<<endl;
     for(long iEvt=0; iEvt<nEvt; iEvt++){
       if(iEvt%2==0)  cout<<"iEvt = "<<iEvt<<endl;
       t1.GetEntry(iEvt);

       histEvtCosPhi2->Reset();
       histEvtCosPhi4->Reset();

       for(int i1=0; i1<nParticle; i1++){

         //for(int i2=0; i2<nParticle; i2++){
         for(int i2=i1+1; i2<nParticle; i2++){

           if( fabs(eta[i1] - eta[i2])<gapSize ) continue;
           histEvtCosPhi2->Fill( cos(2*(phi[i1] - phi[i2])) );

           //for(int i3=0; i3<nParticle; i3++){
           for(int i3=i1+1; i3<nParticle; i3++){

             if( fabs(eta[i1] - eta[i3])<gapSize || fabs(eta[i2] - eta[i3])<gapSize ) continue;
             //for(int i4=0; i4<nParticle; i4++){
             for(int i4=i3+1; i4<nParticle; i4++){
               if( fabs(eta[i1] - eta[i4])<gapSize || fabs(eta[i2] - eta[i4])<gapSize || fabs(eta[i3] - eta[i4])<gapSize ) continue;
               histEvtCosPhi4->Fill( cos(2*(phi[i1] + phi[i2] - phi[i3] - phi[i4])) );
             }
           }
         } 
       }
       if(histEvtCosPhi2->GetEntries()>0){
         histCorrelator2->Fill(histEvtCosPhi2->GetMean(), histEvtCosPhi2->GetEntries());
       }
       if(histEvtCosPhi4->GetEntries()>0){
          histCorrelator4->Fill(histEvtCosPhi4->GetMean(), histEvtCosPhi4->GetEntries());
       }

     }

     ofstream outText;
     outText.open("loopingc24_output.txt");
     outText<<nEvt<<"  "<<histCorrelator2->GetMean()<<"   "<<histCorrelator4->GetMean()<<"     "<<histCorrelator4->GetMean() - 2*histCorrelator2->GetMean()*histCorrelator2->GetMean()<<endl;

     cout<<nEvt<<"  "<<histCorrelator2->GetMean()<<"   "<<histCorrelator4->GetMean()<<"     "<<histCorrelator4->GetMean() - 2*histCorrelator2->GetMean()*histCorrelator2->GetMean()<<endl;
     outText.close();

     TFile *outFile = new TFile("saved_hist_looping_cumulant.root","recreate");
     outFile->cd();
     histCorrelator2->Write();
     histCorrelator4->Write();
     outFile->Write();
     outFile->Close();

}


