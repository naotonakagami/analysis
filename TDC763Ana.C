#define TDC763Ana_cxx
#include "TDC763Ana.h"

#include <string>
#include <algorithm>
#include <vector>

#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLatex.h>
#include <TMarker.h>
#include <TEllipse.h>
#include <TROOT.h>
#include <TPad.h>
#include <TApplication.h>

/*
tree->Draw("@hitTime.size()") work
tree->Draw("@hitTime[70].size()") work but results is same as above, not correct
*/

Double_t XTCurve_pol5pol0(Double_t *px, Double_t *par)
{
   // x1 = par[0]
   //  x <= x1 : DCA = par[1]*x^0 + par[2]*x^1 + par[3]*x^2 + par[4]*x^3 + par[5]*x^4 + par[6]*x^5
   //  x >  x1 : DCA = par[7]*x^0 + par[8]*x^1
   Double_t f;
   Double_t x = px[0];
   if (x < 0)
   {
      f = 0.0;
   }
   else if (x <= par[0])
   { // pol5
      f = par[1] + par[2] * x + par[3] * pow(x, 2.0) + par[4] * pow(x, 3.0) + par[5] * pow(x, 4.0) + par[6] * pow(x, 5.0);
   }
   else
   { // pol1
      f = par[7] + par[8] * x;
   }
   return f;
}

Int_t TDC763Ana::SetXTCurve(const Char_t *name, const Char_t *funcType, Double_t *par)
{
   TString strFuncType(funcType);

   if (strFuncType.CompareTo("pol5+pol1") == 0)
   {
      func_XTCurve = new TF1("XTCurve_pol5pol0", XTCurve_pol5pol0, -10.0, 100.0, 9);
      func_XTCurve->SetParameters(par);
   }
   else
   {
      return -1;
   }
   func_XTCurve->SetNpx(1000);
   return 1;
}

TF1 *TDC763Ana::GetXTCurve()
{
   return func_XTCurve;
}

void TDC763Ana::SetDriftTimeOffset(Double_t offset)
{
   driftTimeOffset = offset;
}

Double_t TDC763Ana::GetDriftTimeOffset()
{
   return driftTimeOffset;
}

Double_t TDC763Ana::GetDriftTime(Int_t wireID, Long64_t eventNumber)
{
   return 0.0;
}

void TDC763Ana::SetWireInfo(Int_t wireID, const Char_t *name, Int_t chID, Int_t directionID, Int_t layerID, Double_t xy, Double_t z)
{
   TWireInfo wi;
   wi.chID = chID;
   sprintf(wi.name, "%s", name);
   wi.directionID = directionID;
   wi.layerID = layerID;
   wi.pos[0] = xy;
   wi.pos[1] = z;
   wireInfo.push_back(wi);

   ch2wire.emplace(chID, wireID);
}

void TDC763Ana::PrintWireInfo(Int_t arg_wireID)
{
   Int_t idMin, idMax;
   if (arg_wireID >= 0)
   {
      idMin = arg_wireID;
      idMax = arg_wireID + 1;
   }
   else
   {
      idMin = 0;
      idMax = (Int_t)wireInfo.size();
   }
   for (Int_t wireID = idMin; wireID < idMax; wireID++)
   {
      printf("wire%2d, ch%2d: %-5s, directionID%2d, layer%2d, pos(xy,z)=(%5.1f,%5.1f) mm\n",
             wireID, wireInfo[wireID].chID, wireInfo[wireID].name,
             wireInfo[wireID].directionID,
             wireInfo[wireID].layerID,
             wireInfo[wireID].pos[0], wireInfo[wireID].pos[1]);
   }
}

TWireInfo *TDC763Ana::GetWireInfo(Int_t wireID)
{
   return (&wireInfo[wireID]);
}

Double_t TDC763Ana::GetWirePosXY(Int_t wireID)
{
   return wireInfo[wireID].pos[0];
}

Double_t TDC763Ana::GetWirePosZ(Int_t wireID)
{
   return wireInfo[wireID].pos[1];
}

Int_t TDC763Ana::GetWireChID(Int_t wireID)
{
   return wireInfo[wireID].chID;
}

Int_t TDC763Ana::GetWireDirectionID(Int_t wireID)
{
   return wireInfo[wireID].directionID;
}

Int_t TDC763Ana::GetWireLayerID(Int_t wireID)
{
   return wireInfo[wireID].layerID;
}

Int_t TDC763Ana::GetWireID(Int_t chID)
{
   return ch2wire[chID];
}

void TDC763Ana::SetADCRangeOfWaveform(Int_t min, Int_t max)
{
   minADCOfWaveform = min;
   maxADCOfWaveform = max;
}

Int_t TDC763Ana::GetMinADCOfWaveform()
{
   return minADCOfWaveform;
}

Int_t TDC763Ana::GetMaxADCOfWaveform()

{
   return maxADCOfWaveform;
}

void TDC763Ana::Plottdcfirsthit(Int_t chbegin, Int_t chend, Int_t tdccut, const Char_t* figname){
   if (fChain == 0)
   return;

   vector<int> hitch; //save the ch info of which had a hit
   Long64_t nbytes=0, nb=0;
   Long64_t nentries = fChain->GetEntries();
   cout << "nentries" <<  nentries << endl;

   TCanvas *c1= new TCanvas("c1", "c1", 800, 1600);
   TCanvas *c2= new TCanvas("c2", "c2", 800, 1600);
   TCanvas *c3= new TCanvas("c3", "c3", 800, 1600);
   TCanvas *c4= new TCanvas("c4", "c4", 800, 1600);
   TCanvas *c5= new TCanvas("c5", "c5", 800, 1600);
   c1->Divide(1,2);
//   c2->Divide(1,2);
//   c3->Divide();
   //TH2F *hthclk = new TH2F("hthclk", "hthclk", 100, -6000., 0., 127, 0., 127.);
   //TH2F *htvsadc = new TH2F("htvsadc", "htvsadc", 100, -6000., 0., 50, 0., 2000.);
   TH1F *hxtX1 = new TH1F("first hit", "first hit", 500, -7000., 0.); //histogram to plot the hTime of the first hClock of each event
   TH1F *hxtX2 = new TH1F("mutihit", "multihit", 500, -7000., 0.); //histogram to plot the hTime of the first hClock of each event
   TH1F *hxtX1_cut = new TH1F("first hit, cell < 3", "first hit, cell<3", 500, -7000., 0.);
   TH1F *hnumcell = new TH1F("hnumcell", "hnumcell", 15, 0, 15); // number of hit cells/layer
   TH1F *hlocation = new TH1F("hlocation", "hlocation", 13, 0, 13); //ch difference when the number of hit cell =>2
   TH2F *hlocvsnumcell = new TH2F("hlocvsnumcell", "hlocvsnumcell",14, 1, 14, 14, 1, 14);
   TH1F *htout = new TH1F("htout", "htout", 15, 0, 15);

   //cout << "htime::" << htime.size() << "  hclock::"<< hc.size()<< endl;

   for(Long64_t jentry=3; jentry<4; jentry++){
         vector<int> hitch;
         Long64_t ientry = LoadTree(jentry);
         int cellcount = 0;
         //cout << "ientry:: " << ientry << endl;
         if(ientry < 0)break;
         auto htime = *hitTime;
         auto hc = *hitClock;
         nb=fChain->GetEntry(jentry);
         //cout << "nb " << nb << endl;
         nbytes += nb;

         int hc_min_clk=10;
         int htime_min = 10;
         int hc_ch_index=-1;

         int hc_clk_index=-1;
         int ht_ch_index=-1;
         int ht_clk_index=-1;

        for(int i=chbegin; i<=chend; i++){
               /* not necessary to loop over j, just look up j==0 => should be suffice */
               if((Int_t)htime[i].size() >= 1){
                  hitch.push_back(i);
                  cellcount++;
                  for(int k=0; k<(Int_t)htime[i].size(); k++){
                  //cout <<"k::" << k <<  "  htime:: " <<  htime[i][k] << endl;
                     if(htime[i][k] <= htime_min){
                     htime_min = htime[i][k];
                     ht_ch_index = i;
                     ht_clk_index = k;
                     }
                  }
               }
        }

         if( (ht_ch_index == -1) && (ht_clk_index == -1) ){
            //cout  << "   ====================================> skipped::" << jentry << endl;
         }else{

               cout << ht_ch_index << "  " << ht_clk_index << endl; 
               
                  hnumcell->Fill(cellcount);

                  if(htime[ht_ch_index][0] > tdccut){
                     htout->Fill(cellcount);
                  }

                  if(cellcount <3){
                     hxtX1_cut->Fill(htime[ht_ch_index][0]);
                  }

                  if(cellcount>0){
                       //cout<< jentry << endl;
                     }

                  if(cellcount > 2){

                     hxtX2->Fill(htime[ht_ch_index][0]);
                     //cout << "cellcount flag" << endl;
                     for(int j=0; j<(int)hitch.size(); j++){
                     int dist=hitch[j] - (ht_ch_index);
                     //cout << "loc. relation::" << j << " " << hitch[j] << " " << ht_ch_index << " " << abs(hitch[j] - (ht_ch_index)) << endl;
                     hlocvsnumcell->Fill(cellcount, dist);
                     }
                  }
                  //htvsadc->Fill(htime[hc_ch_index][hc_clk_index], adcPosSum[hc_ch_index]);
               }
   }
   //printf("%s", figname);
   //  c1->cd(1);
   //  //hxtX1->GetXaxis();
   //  hxtX1->SetXTitle("tdc");
   //  hxtX1->Draw();
   //  //c1->cd(2);
   //  //hnumcell->SetXTitle("number of hit cells/layer");
   //  //hnumcell->Draw();
   //  c1->SaveAs(figname);
   //  c2->cd();
   //  htvsadc->SetMarkerSize(10);
   //  //htvsadc->SetMarkerSyle(5);
   //  htvsadc->Draw("COLZ");
   TFile *fin = new TFile(figname, "RECREATE");
   hxtX1->Write();

   c1->cd(1);
   hxtX1->SetXTitle("tdc");
   hxtX1->SetYTitle("Count");
   hxtX1->Draw();
   hxtX1->Write();
   c1->cd(2);
   gPad->SetLogy();
   hnumcell->SetXTitle("Number of hit cells");
   hnumcell->SetYTitle("Count");
   hnumcell->Draw();
   hnumcell->Write();
   c2->cd();
   hlocvsnumcell->SetXTitle("Number of hit cells");
   hlocvsnumcell->SetYTitle("Abs(first hit cell ch.) - (hit ch.)");
   hlocvsnumcell->Draw("COLZ");
   hlocvsnumcell->Write();
   c3->cd();
   gPad->SetLogy();
   htout->Draw();
   htout->Write();
   c4->cd();
   hxtX2->SetXTitle("tdc");
   hxtX2->SetYTitle("Count");
   hxtX2->Draw();
   hxtX2->Write();

   c5->cd();
   hxtX1_cut->Draw();
   hxtX1_cut->Write();

}



void TDC763Ana::PlotWaveForm(Int_t wireID, Int_t firstEventNumber, Int_t numOfEvents)
//=========================================================================
// Plot waveform of a wire with a charge q and tdcHit informations
//  07-Dec-2014 A.Sato
//=========================================================================
{
   if (fChain == 0)
      return;
   Long64_t nentries = fChain->GetEntriesFast();
   cout << "a" << endl;
   //--------------------------------------------------
   // Prepare canvas, histograms,,,
   //--------------------------------------------------
   //TCanvas *c1 = new TCanvas("c1","Event Display",800,600); -> doesn't show any when the PlotAllWaveform function is called
   //gROOT->SetSelectedPad(c1);

   TGraph *gr_waveForm;
   vector<Int_t> vSample;
   cout << "b" << endl;
   //cout << " numOfWaveformSamplings " << numOfWaveformSamplings << endl;
   for (Int_t i = 0; i < numOfWaveformSamplings; i++)
   {
      vSample.push_back(i);
   }
   cout << "c" << endl;
   Double_t titleSize = 0.06;
   Double_t text1Size = 0.04;
   Double_t textTDCSize = 0.04;
   Double_t textAxisTitleSize = 0.04;
   if (isSmallFigure == kTRUE)
   {
      titleSize = 0.1;
      text1Size = 0.07;
      textTDCSize = 0.06;
      textAxisTitleSize = 0.04;
   }
   gStyle->SetTitleFontSize(titleSize);

   //--------------------------------------------------
   //   Event loop
   //--------------------------------------------------
   Long64_t nbytes = 0, nb = 0;
   Long64_t lastEventNumber = firstEventNumber + numOfEvents;
   if (lastEventNumber > nentries)
   {
      lastEventNumber = nentries;
   }
   cout << "d" << endl;
   for (Long64_t jentry = firstEventNumber; jentry < lastEventNumber; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      auto wf = *waveform;
      cout << jentry << endl;
      gr_waveForm = new TGraph(numOfWaveformSamplings, &vSample[0], &wf[wireInfo[wireID].chID][0]);
      gr_waveForm->GetHistogram()->SetTitle(Form("%s: wire%d, ch%d", wireInfo[wireID].name, wireID, wireInfo[wireID].chID));
      gr_waveForm->GetHistogram()->SetMinimum(minADCOfWaveform);
      gr_waveForm->GetHistogram()->SetMaximum(maxADCOfWaveform);
      gr_waveForm->GetHistogram()->SetXTitle("Samplig ID");
      gr_waveForm->GetHistogram()->SetYTitle("ADC(ch)");
      gr_waveForm->SetMarkerStyle(7);
      gr_waveForm->SetMarkerSize(0.5);
      gr_waveForm->GetXaxis()->SetTitleSize(textAxisTitleSize);
      gr_waveForm->GetYaxis()->SetTitleSize(textAxisTitleSize);
      gr_waveForm->Draw("apw");
      cout << "e" << endl;
      TLatex *text1;
      text1 = new TLatex(numOfWaveformSamplings / 15,
                         maxADCOfWaveform - 1 * (maxADCOfWaveform - minADCOfWaveform) / 12,
                         Form("Event:%lld", jentry));
      text1->SetTextSize(text1Size);
      text1->Draw();
      cout << "f" << endl;
      text1 = new TLatex(numOfWaveformSamplings / 15,
                         maxADCOfWaveform - 2 * (maxADCOfWaveform - minADCOfWaveform) / 12,
                         Form("adcPosSum - base: %d", adcPosSum[wireInfo[wireID].chID]));
      text1->SetTextSize(text1Size);
      text1->Draw();

      TLatex *textTDC[32];
      TMarker *markerTDC[32];

      auto htime = *hitTime;
      auto hclock = *hitClock;
      for (Int_t i = 0; i < (Int_t)htime[wireInfo[wireID].chID].size(); i++)
      {
         cout << "i" << i << endl;
         if (isSmallFigure == kTRUE)
         {
            textTDC[i] = new TLatex(13.5 * (numOfWaveformSamplings / 15), maxADCOfWaveform - (4 + i) * (maxADCOfWaveform - minADCOfWaveform) / 15, Form("%d", htime[wireInfo[wireID].chID][i]));
         }
         else
         {
            textTDC[i] = new TLatex(hclock[wireInfo[wireID].chID][i], minADCOfWaveform + (maxADCOfWaveform - minADCOfWaveform) / 20, Form("%d", htime[wireInfo[wireID].chID][i]));
         }
         textTDC[i]->SetTextSize(textTDCSize);

         markerTDC[i] = new TMarker(hclock[wireInfo[wireID].chID][i], wf[wireInfo[wireID].chID][hclock[wireInfo[wireID].chID][i]], 20);
         markerTDC[i]->SetMarkerSize(0.4);
         if (isGoodHit(wireID, i) == kTRUE)
         {
            markerTDC[i]->SetMarkerColor(1);
            textTDC[i]->SetTextColor(4);
         }
         else
         {
            markerTDC[i]->SetMarkerColor(1);
            textTDC[i]->SetTextColor(2);
         }
         textTDC[i]->Draw();
         markerTDC[i]->Draw();
      }
      if (!(lastEventNumber == firstEventNumber + 1))
      {
         //cout << "g" << endl;
         //TPad* pad = (TPad*)gROOT->GetSelectedPad();
         //cout << "g1" << endl;
         //cout << "pad::" << gROOT->GetSelectedPad() << endl;
         //pad->WaitPrimitive();
         //gROOT->GetSelectedPad()->GetCanvas()->Flush();
         //gROOT->GetSelectedPad()->WaitPrimitive(); // Yon need to click the c1 window to look the next waveform.
         //gROOT->GetSelectedPad()->GetX1();
         //cout << "h" << endl;
      }
   } // End of the event loop
}

void TDC763Ana::PlotAllWaveForm(Int_t firstEventNumber, Int_t numOfEvents, Char_t order)
{
   isSmallFigure = kTRUE;
   TCanvas *c_allWaveForm;
   if (order == 'g')
   {
      c_allWaveForm = new TCanvas("c_allWaveForm", "Waveform of wires in geometoric layout", 1400, 800);
      c_allWaveForm->Divide(13, 6);
   }
   else
   {
      c_allWaveForm = new TCanvas("c_allWaveForm", "Waveform of wires in channel oder", 1200, 1200);
      c_allWaveForm->Divide(8, 8);
   }
   gROOT->SetSelectedPad(c_allWaveForm);
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t lastEventNumber = firstEventNumber + numOfEvents;
   if (lastEventNumber > nentries)
   {
      lastEventNumber = nentries;
   }
   for (Long64_t jentry = firstEventNumber; jentry < lastEventNumber; jentry++)
   {
      Long64_t eventID = firstEventNumber;
      for (Int_t wireID = 0; wireID < (Int_t)wireInfo.size(); wireID++)
      {
         if (order == 'g')
         {
            Int_t padID_offset = wireInfo[wireID].directionID * (13 * 6) / 2; // for X and Y
            padID_offset += wireInfo[wireID].layerID * 13 + 1;                // for layerID
            TString wireName = wireInfo[wireID].name;
            wireName.Remove(0, 3);
            padID_offset += wireName.Atof() - 1;
            if (wireInfo[wireID].directionID == 1 && wireInfo[wireID].layerID == 2)
            {
               padID_offset++;
            }
            c_allWaveForm->cd(padID_offset);
            //printf("ipad=%d\n", padID_offset + int(wireID / 20) * 10 + wireID - wireInfo[wireID].layerID * 10 + 1);
         }
         else
         {
            c_allWaveForm->cd(wireInfo[wireID].chID + 1);
         }
         PlotWaveForm(wireID, jentry, 1);
         cout << jentry << endl;
      }
      if (!(lastEventNumber == firstEventNumber + 1))
      {
         //gROOT->GetSelectedPad()->GetCanvas()->Flush();
          cout << "pad::" << gROOT->GetSelectedPad() << endl;
         gROOT->GetSelectedPad()->WaitPrimitive(); // Yon need to click the c1 window to look the next waveform.
      }
   }
   isSmallFigure = kFALSE;
}

Double_t TDC763Ana::GetDriftLength(Double_t dt)
{
   return func_XTCurve->Eval(dt);
}

bool TDC763Ana::isGoodHit(Int_t wireID, Int_t hitID)
{
   Int_t ch = wireInfo[wireID].chID;
   auto wf = *waveform;
   auto htime = *hitTime;
   auto hclock = *hitClock;

   Int_t status = 0;
   Int_t ht = htime[ch][hitID];
   Int_t hc= hclock[ch][hitID];
   
   if (adcPosSum[ch] < adcPosSumThresold)
   {
      status += 1;
   }
   if (!(wf[ch][hclock[ch][hitID]] > adcThresold || wf[ch][1 + hclock[ch][hitID]] > adcThresold))
   {
      status += 3;
   }
   if (!(ht > minHitTime && ht < maxHitTime))
   {
      status += 5;
   }
   if (status == 0)
   {
      cout <<"hittime " <<  ht <<  "  " << hc <<  "  ch" << ch <<  endl;
      return kTRUE;
   }
   else
   {
      return kFALSE;
   }
}

void TDC763Ana::SetAdcPosSumThresold(Int_t thre)
{
   adcPosSumThresold = thre;
}
void TDC763Ana::SetAdcThresold(Int_t thre)
{
   adcThresold = thre;
}
void TDC763Ana::SetMinHitTime(Int_t min)
{
   minHitTime = min;
}
void TDC763Ana::SetMaxHitTime(Int_t max)
{
   maxHitTime = max;
}
void TDC763Ana::SetGoodDriftTimeRange(Int_t min, Int_t max)
{
   minHitTime = min;
   maxHitTime = max;
}
Int_t TDC763Ana::GetAdcPosSumThresold()
{
   return adcPosSumThresold;
}
Int_t TDC763Ana::GetAdcThresold()
{
   return adcThresold;
}
Int_t TDC763Ana::GetMinHitTime()
{
   return minHitTime;
}

Int_t TDC763Ana::GetMaxHitTime()
{
   return maxHitTime;

}

Int_t TDC763Ana::GetNumOfCellHitsInLayer(const Char_t *layerName, bool selectGoodHits)
{
   if (fChain->GetReadEntry() < 0)
   {
      fChain->GetEntry(0);
   }
   Int_t counter = 0;
   auto htime = *hitTime;
   for (Int_t wireID = 0; wireID < (Int_t)wireInfo.size(); wireID++)
   {
      string wireName = wireInfo[wireID].name;
      if (wireName.find(layerName) == 0)
      {
         if (GetNumOfHitsInCell(wireID, selectGoodHits) > 0)
         {
            counter++;
         }
      }
   }
   return counter;
}

Int_t TDC763Ana::PlotNumOfCellHitsInLayer(const Char_t *layerName, bool selectGoodHits)
{
   if (fChain == 0)
      return 0;

   TH1F *h1 = new TH1F("h1", Form("Number of Hit Cells in %s (GoodHit=%d)", layerName, selectGoodHits), 10, 0, 10);
   h1->SetXTitle("Number of Hit Cells");
   h1->SetYTitle("(counts)");

   Long64_t nentries = fChain->GetEntriesFast();
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      fChain->GetEntry(jentry);
      // if (Cut(ientry) < 0) continue;
      h1->Fill(GetNumOfCellHitsInLayer(layerName, selectGoodHits), 1);
   }
   h1->Draw();
   return h1->GetEntries();
}

Int_t TDC763Ana::GetNumOfHitsInLayer(const Char_t *layerName, bool selectGoodHits)
{
   if (fChain->GetReadEntry() < 0)
   {
      fChain->GetEntry(0);
   }
   Int_t counter = 0;
   auto htime = *hitTime;
   for (Int_t wireID = 0; wireID < (Int_t)wireInfo.size(); wireID++)
   {
      string wireName = wireInfo[wireID].name;
      if (wireName.find(layerName) == 0)
      {
         counter += GetNumOfHitsInCell(wireID, selectGoodHits);
      }
   }
   return counter;
}

Int_t TDC763Ana::GetNumOfHitsInCell(Int_t wireID, bool selectGoodHits)
{
   if (fChain->GetReadEntry() < 0)
   {
      fChain->GetEntry(0);
   }
   Int_t ch = wireInfo[wireID].chID;
   auto htime = *hitTime;
   Int_t counter = 0;
   if (selectGoodHits == kTRUE)
   {
      for (Int_t i = 0; i < (Int_t)htime[ch].size(); i++)
      {
         if (isGoodHit(wireID, i) == kTRUE)
         {
            counter++;
         }
      }
   }
   else
   {
      counter = (Int_t)htime[ch].size();
   }

   return counter;
}

Int_t TDC763Ana::PlotNumOfHitsInCell(Int_t wireID, bool selectGoodHits)
{
   if (fChain == 0)
      return 0;

   TH1F *h1 = new TH1F("h1", Form("Number of Hits of wire%d (GoodHit=%d)", wireID, selectGoodHits), 10, 0, 10);
   h1->SetXTitle("Number of Hits");
   h1->SetYTitle("(counts)");

   Long64_t nentries = fChain->GetEntriesFast();
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      fChain->GetEntry(jentry);
      // if (Cut(ientry) < 0) continue;
      h1->Fill(GetNumOfHitsInCell(wireID, selectGoodHits), 1);
   }
   h1->Draw();
   return h1->GetEntries();
}

Int_t TDC763Ana::GetFirstHitInLayer(const Char_t *layerName, bool selectGoodHits){
   if(fChain->GetReadEntry() < 0)
   {
      fChain->GetEntry(3 ); 
   }
   vector<int> hitch; //save the ch infor of which had a hit 
   Int_t counter = 0; 
   //Int_t ch = wireInfo[wireID].chID; 
   auto htime = *hitTime; 
  
   Int_t hclk_min=10;
   Int_t htime_min = 10;

   Int_t hc_ch_index=-1;
   Int_t hc_clk_index=-1;
   Int_t ht_ch_index=-1;
   Int_t ht_clk_index=-1;

   for(Int_t wireID = 0; wireID < (Int_t)wireInfo.size(); wireID++)
   {
      Int_t ch = wireInfo[wireID].chID;
      string wireName = wireInfo[wireID].name;
      if(wireName.find(layerName)==0)
      {
         if((Int_t)htime[wireID].size() >=1){
            hitch.push_back((Int_t)wireID);
            cout << "============= wireID=====================" << wireID << endl; 
            for(int k=0; k<(Int_t)htime[(Int_t)wireID].size(); k++){
               cout << "k " << k << "  htime_min " << htime_min << endl; 
               
               if(selectGoodHits == kTRUE){
                  counter++; 
                  if(isGoodHit(wireID, k)== kTRUE){
                     if(htime[(Int_t)wireID][k] <= htime_min){
                     htime_min = htime[(Int_t)wireID][k];
                     ht_ch_index = (Int_t)wireID; 
                     ht_clk_index = k;      
                     cout << "ht_clk_index " << ht_clk_index << " ht_ch_index " << ht_ch_index << endl;
                     }
                  }              
               }            
            }
         }
      }
   }

   if((ht_ch_index == -1) && (ht_clk_index == -1)){
     cout  << "   ====================================> skipped::" << "jentry" << endl;
   }else{
      cout << ht_ch_index << "  " << ht_clk_index <<  "    " << htime[ht_ch_index][ht_clk_index] << endl;
   }   
   return counter; 
}



void TDC763Ana::DisplayEvents(Int_t firstEventNumber, Int_t numOfEvents)
{
   TCanvas *c_EventDisplay = (TCanvas *)gROOT->FindObjectAny("c_EventDisplay");
   if (!c_EventDisplay)
   {
      c_EventDisplay = new TCanvas("c_EventDisplay", "Event Display", 1600, 800);
   }
   gStyle->SetOptStat(0);
   c_EventDisplay->Divide(2, 1);

   TH2F *frameXZ = new TH2F("frameXZ", "Event Display XZ; x (mm); z(mm)", 10, -30, 30, 10, 0, 60);
   TH2F *frameYZ = new TH2F("frameYZ", "Event Display YZ; y (mm); z(mm)", 10, -30, 30, 10, 0, 60);
   TMarker *hitWireMarker = new TMarker();
   hitWireMarker->SetMarkerStyle(7);
   hitWireMarker->SetMarkerColor(kBlack);
   TEllipse *driftCircle = new TEllipse();
   driftCircle->SetFillColorAlpha(0, 0.0); // colorID, alpha

   if (fChain == 0)
      return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   Long64_t lastEventNumber = firstEventNumber + numOfEvents;
   if (lastEventNumber > nentries)
   {
      lastEventNumber = nentries;
   }
   Int_t IsHitExist = 0;
   for (Long64_t jentry = firstEventNumber; jentry < lastEventNumber; jentry++)
   {
      IsHitExist = 0;
      c_EventDisplay->Clear();
      c_EventDisplay->Divide(2, 1);
      c_EventDisplay->cd(1);
      frameXZ->Draw();
      c_EventDisplay->cd(2);
      frameYZ->Draw();
      c_EventDisplay->Update();
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;

      auto htime = *hitTime;
      auto hclock = *hitClock;

      for (Int_t wireID = 0; wireID < (Int_t)wireInfo.size(); wireID++)
      {
         if (adcPosSum[GetWireChID(wireID)] > 190)
         {
            for (Int_t i = 0; i < (Int_t)htime[wireInfo[wireID].chID].size(); i++)
            {
               c_EventDisplay->cd(GetWireDirectionID(wireID) + 1);
               Double_t dt = htime[wireInfo[wireID].chID][i] - GetDriftTimeOffset();
               if (isGoodHit(wireID, i) == kTRUE)
               {
                  Double_t driftLengh = GetDriftLength(htime[wireInfo[wireID].chID][i] - GetDriftTimeOffset());
                  driftCircle->DrawEllipse(GetWirePosXY(wireID), GetWirePosZ(wireID), driftLengh, driftLengh, 0.0, 360.0, 1.0);
                  hitWireMarker->DrawMarker(GetWirePosXY(wireID), GetWirePosZ(wireID));
                  c_EventDisplay->Update();
                  IsHitExist = 1;
               }
            }
         }
      }
      printf("Event=%lld,HitExist=%d\n", jentry, IsHitExist);
      if (!(lastEventNumber == firstEventNumber + 1) && IsHitExist == 1)
      {
         // for this case a double click is needed.
         c_EventDisplay->Update();
         c_EventDisplay->WaitPrimitive(); // Yon need to click the c1 window to look the next waveform.
      }
   }
}

void TDC763Ana::Loop()
{
   if (fChain == 0)
      return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}

// //= for standalone ===========================================================
// int main(int argc, char *argv[])
// {
//     if (argc < 2)
//     {
//         std::string message = "Usage: ./binary2root_RP1212 <rawDataFileName> <rootFileName>";
//         std::cerr << message << std::endl;
//         return -1;
//     }

//     TDC763Ana aa(argv[1]);
//     if (argc == 3)
//     {
//         aa.Plo(argv[2]);
//         std::cout << "Number of events: " << event << std::endl;
//     }
//     else if (argc == 2)
//     {
//         aa.MakeRootData();
//     }
//     return 0;
// }