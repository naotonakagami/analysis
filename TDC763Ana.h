//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 29 14:23:01 2020 by ROOT version 6.20/04
// from TTree tree/64ch Board RP1212 v2
// found on file: data_FW2.5/run_0218_0000.dat.root
//////////////////////////////////////////////////////////

/* 
Todo list: 11 July 2020 
* Make a class TDCTrackerStraight
* Separate TDC763Ana into TRP1212Ana and TDC763Ana 
*/

#ifndef TDC763Ana_h
#define TDC763Ana_h

#include <stdio.h>
#include <iostream>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TVectorD.h>
#include <TVector3.h>

// Header file for the classes stored in the TTree if any.

struct TWireInfo
{
   Int_t chID;
   Char_t name[10];
   Int_t directionID; // X=0, Y=1;
   Int_t layerID;     // 0,1,2 for each X and Y
   Double_t pos[2];   //XorY, Z
};

class TDC763Ana
{
public:
   TTree *fChain;  //!pointer to the analyzed TTree or TChain
   Int_t fCurrent; //!current Tree number in a TChain

   std::vector<TWireInfo> wireInfo;
   std::map<Int_t, Int_t> ch2wire;

   Double_t driftTimeOffset;
   Int_t minADCOfWaveform;
   Int_t maxADCOfWaveform;

   Int_t numOfWaveformSamplings;

   // for Good hit condition
   Int_t adcPosSumThresold;
   Int_t adcThresold;
   Int_t minHitTime;
   Int_t maxHitTime;

   TF1 *func_XTCurve;
   //Double_t        par_XTCurve[20];
   //Double_t        par_err_XTCurve[20];
   //Double_t        XTCurve_pol5pol0(Double_t *x, Double_t *par);

   bool isSmallFigure;

   // Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t boardID;
   Int_t triggerNumber;
   Int_t triggerTime;
   ULong64_t triggerTimeStamp;
   std::vector<std::vector<int>> *waveform;
   std::vector<std::vector<int>> *hitTime;
   std::vector<std::vector<int>> *hitClock;
   Int_t baseline[64];
   Int_t adcSum[64];
   Int_t adcPosSum[64];
   Int_t tot[64];

   // List of branches
   TBranch *b_boardID;          //!
   TBranch *b_triggerNumber;    //!
   TBranch *b_triggerTime;      //!
   TBranch *b_triggerTimeStamp; //!
   TBranch *b_waveform;         //!
   TBranch *b_hitTime;          //!
   TBranch *b_hitClock;         //!
   TBranch *b_baseline;         //!
   TBranch *b_adcSum;           //!
   TBranch *b_adcPosSum;        //!
   TBranch *b_tot;              //!

   TDC763Ana(TTree *tree = 0);
   TDC763Ana(const Char_t *rootData);
   virtual ~TDC763Ana();
   virtual Int_t Cut(Long64_t entry);
   virtual Int_t GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void Init(TTree *tree);

   // wire
   virtual void InitWireInfo();
   virtual void PrintWireInfo(Int_t wireID = -1);
   virtual void SetWireInfo(Int_t wireID, const Char_t *name, Int_t chID, Int_t directionID, Int_t layerID, Double_t xy, Double_t z);
   virtual TWireInfo *GetWireInfo(Int_t wireID = 1);
   virtual Int_t GetWireChID(Int_t wireID);
   virtual Int_t GetWireID(Int_t chID);
   virtual Double_t GetWirePosXY(Int_t wireID);
   virtual Double_t GetWirePosZ(Int_t wireID);
   virtual Int_t GetWireDirectionID(Int_t wireID);
   virtual Int_t GetWireLayerID(Int_t wireID);
   virtual Double_t GetWireEnds(Int_t wireID, Int_t id);
   virtual void GetWireEnds(Int_t wireID, TVector3 &wireEnd0, TVector3 &wireEnd1);

   // waveform
   virtual void PlotWaveForm(Int_t wireID, Int_t firstEventNumber = 1, Int_t numOfEvents = 10);
   virtual void PlotAllWaveForm(Int_t firstEventNumber = 1, Int_t numOfEvents = 10, Char_t order = 'g');
   virtual void SetADCRangeOfWaveform(Int_t min = 0, Int_t max = 1024);
   virtual Int_t GetMinADCOfWaveform();
   virtual Int_t GetMaxADCOfWaveform();
  
  
  // xt curve from the data
  virtual void Plottdcfirsthit(Int_t chbegin, Int_t chend, Int_t tdccut, const Char_t *figname);
   

   // adc
   //virtual Int_t PlotADCSum(Int_t wireID)
   //virtual Int_t PlotADCPosSum(Int_t wireID)
   //virtual Int_t PlotBaseline(Int_t wireID)

   // tot
   //virtual Int_t PlotToT(Int_t wireID)

   // drifttime
   virtual void SetDriftTimeOffset(Double_t offset = 0.0);
   virtual Double_t GetDriftTimeOffset();
   virtual Double_t GetDriftTime(Int_t wireID, Long64_t eventNumber = -1);
   //virtual Int_t PlotDriftTime(Int_t wireID, bool selectGoodHits = kTRUE)
   //virtual Int_t PlotHitTime(Int_t wireID, bool selectGoodHits = kTRUE)

   // XTcurve
   virtual Int_t SetXTCurve(const Char_t *name, const Char_t *funcType, Double_t *par);
   virtual TF1 *GetXTCurve();
   virtual Double_t GetDriftLength(Double_t dt); // Get drift length from drift time

   // hit judgement
   virtual bool isGoodHit(Int_t wireID, Int_t hitID);
   virtual void SetAdcPosSumThresold(Int_t thre);
   virtual void SetAdcThresold(Int_t thre);
   virtual void SetMinHitTime(Int_t min);
   virtual void SetMaxHitTime(Int_t max);
   virtual void SetGoodDriftTimeRange(Int_t min, Int_t max);
   virtual Int_t GetAdcPosSumThresold();
   virtual Int_t GetAdcThresold();
   virtual Int_t GetMinHitTime();
   virtual Int_t GetMaxHitTime();
   //virtual void PrintHitWireIDs(bool selectGoodHits = kTRUE);

   virtual Int_t GetFirstHitInLayer(const Char_t *layerName, bool selectGoodHits); 


   // Multiplicity
   virtual Int_t GetNumOfCellHitsInLayer(const Char_t *layerName, bool selectGoodHits = kTRUE);
   virtual Int_t PlotNumOfCellHitsInLayer(const Char_t *layerName, bool selectGoodHits = kTRUE);
   virtual Int_t GetNumOfHitsInLayer(const Char_t *layerName, bool selectGoodHits = kTRUE);
   virtual Int_t GetNumOfHitsInCell(Int_t wireID, bool selectGoodHits = kTRUE);
   virtual Int_t PlotNumOfHitsInCell(Int_t wireID, bool selectGoodHits = kTRUE);

   // Event display
   virtual void DisplayEvents(Int_t firstEventNumber = 1, Int_t numOfEvents = 10);

   virtual void Loop();
   virtual Bool_t Notify();
   virtual void Show(Long64_t entry = -1);
   virtual void Test(Int_t ev, Int_t ch, Int_t sampleID);
};

#endif

#ifdef TDC763Ana_cxx
TDC763Ana::TDC763Ana(TTree *tree) : fChain(0)
{
   // if parameter tree is not specified (or zero), connect the file
   // used to generate this class and read the Tree.

   //const Char_t *rootData = "data_FW2.5/run_0241_0000.dat.root";
   //const Char_t *rootData = "data_FW2.5/run_0242_0000.dat.root";
   //const Char_t *rootData = "data_FW2.5/run_0242_0018.dat.root";
   const Char_t *rootData = "./data_kumatori/run_0121_0000.dat.root";
   if (tree == 0)
   {
      TFile *f = (TFile *)gROOT->GetListOfFiles()->FindObject(rootData);
      if (!f || !f->IsOpen())
      {
         f = new TFile(rootData);
      }
      f->GetObject("tree", tree);
   }
   Init(tree);
}

TDC763Ana::TDC763Ana(const Char_t *rootData) : fChain(0)
{
   TTree *tree;
   TFile *f = (TFile *)gROOT->GetListOfFiles()->FindObject(rootData);
   if (!f || !f->IsOpen())
   {
      f = new TFile(rootData);
   }
   f->GetObject("tree", tree);
   Init(tree);
}

TDC763Ana::~TDC763Ana()
{
   if (!fChain)
      return;
   delete fChain->GetCurrentFile();
}

Int_t TDC763Ana::GetEntry(Long64_t entry)
{
   // Read contents of entry.
   if (!fChain)
      return 0;
   return fChain->GetEntry(entry);
}

Long64_t TDC763Ana::LoadTree(Long64_t entry)
{
   // Set the environment to read one entry
   if (!fChain)
      return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0)
      return centry;
   if (fChain->GetTreeNumber() != fCurrent)
   {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void TDC763Ana::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   waveform = 0;
   hitTime = 0;
   hitClock = 0;

   // Set branch addresses and branch pointers
   if (!tree)
      return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("boardID", &boardID, &b_boardID);
   fChain->SetBranchAddress("triggerNumber", &triggerNumber, &b_triggerNumber);
   fChain->SetBranchAddress("triggerTime", &triggerTime, &b_triggerTime);
   fChain->SetBranchAddress("triggerTimeStamp", &triggerTimeStamp, &b_triggerTimeStamp);
   fChain->SetBranchAddress("waveform", &waveform, &b_waveform);
   fChain->SetBranchAddress("hitTime", &hitTime, &b_hitTime);
   fChain->SetBranchAddress("hitClock", &hitClock, &b_hitClock);
   fChain->SetBranchAddress("baseline", baseline, &b_baseline);
   fChain->SetBranchAddress("adcSum", adcSum, &b_adcSum);
   fChain->SetBranchAddress("adcPosSum", adcPosSum, &b_adcPosSum);
   fChain->SetBranchAddress("tot", tot, &b_tot);

   driftTimeOffset = -2131.0;
   adcPosSumThresold = 100; //
   adcThresold = 137; // adc - baseline
   minHitTime = (Int_t)(-5000);
//   minHitTime = (Int_t)(driftTimeOffset - 10);
   maxHitTime = -10;
//   maxHitTime = minHitTime + 120;

   InitWireInfo();

   // Set default XT curve
   Double_t par[9];
   par[0] = 10.0;
   par[1] = 0.0;
   par[2] = 0.2425;
   par[3] = -0.062;
   par[4] = 0.008921;
   par[5] = -0.0005862;
   par[6] = 1.413e-05;
   par[7] = 0.443771;
   par[8] = 0.028421;
   cout << "Default XT curve is set to XTCurve_pol5pol0: " << SetXTCurve("default_pol5pol0", "pol5+pol1", par) << endl;

   minADCOfWaveform = -20;
   maxADCOfWaveform = 800;

   isSmallFigure = kFALSE;

   Notify();

   GetEntry(0);
   auto wf = *waveform;
   numOfWaveformSamplings = wf[1].size();
}

Bool_t TDC763Ana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TDC763Ana::Show(Long64_t entry)
{
   // Print contents of entry.
   // If entry is not specified, print current entry
   if (!fChain)
      return;
   fChain->Show(entry);
}

Int_t TDC763Ana::Cut(Long64_t entry)
{
   // This function may be called from Loop.
   // returns  1 if entry is accepted.
   // returns -1 otherwise.
   return 1;
}

void TDC763Ana::Test(Int_t ev, Int_t ch, Int_t sampleID)
{
   fChain->GetEntry(ev);

   std::cout << "1: " << waveform->at(ch)[sampleID] << std::endl;
   std::cout << "1: " << waveform->at(ch).size() << std::endl;

   auto wf = *waveform;
   std::cout << "2: " << wf[ch][sampleID] << std::endl;
   std::cout << "2: " << wf[ch].size() << std::endl;

   // std::cout << "3: " << (*waveform)[ch]][sampleID] << std::endl; // Dose not work
   // std::cout << "3: " << (*waveform)[ch] << std::endl; // Dose not work
   // std::cout << "4: " << (*waveform)[ch] << std::endl; // Dose not work
   //auto value = waveform[0][0];
   //std::cout << "5: " << value << std::endl; // Dose not work
}

Double_t TDC763Ana::GetWireEnds(Int_t wireID, Int_t id)
{
   TVectorD wireEnds(6);
   Double_t wireLength = 50.0; //mm
   if (wireInfo[wireID].directionID == 0)
   {
      wireEnds[0] = -wireLength / 2.0;       // X0
      wireEnds[1] = wireInfo[wireID].pos[0]; // Y0
      wireEnds[2] = wireInfo[wireID].pos[1]; // Z0
      wireEnds[3] = wireLength / 2.0;        // X1
      wireEnds[4] = wireInfo[wireID].pos[0]; // Y1
      wireEnds[5] = wireInfo[wireID].pos[1]; // Z1
   }
   else
   {
      wireEnds[0] = wireInfo[wireID].pos[0]; // X0
      wireEnds[1] = -wireLength / 2.0;       // Y0
      wireEnds[2] = wireInfo[wireID].pos[1]; // Z0
      wireEnds[3] = wireInfo[wireID].pos[0]; // X1
      wireEnds[4] = wireLength / 2.0;        // Y1
      wireEnds[5] = wireInfo[wireID].pos[1]; // Z1
   }
   return (wireEnds[id]);
}

void TDC763Ana::GetWireEnds(Int_t wireID, TVector3 &wireEnd0, TVector3 &wireEnd1)
{
   for (Int_t i = 0; i < 3; i++)
   {
      wireEnd0(i) = GetWireEnds(wireID, i);
      wireEnd1(i) = GetWireEnds(wireID, i + 3);
   }
}

void TDC763Ana::InitWireInfo()
{
   //------------------------------------------------------------------------
   // wireID(0-59), name, chID of RP1212(1-64), directionID(X=0, Y=1), layerID(0,1,2 for each X and Y), posXY(mm), posZ(mm)
   //------------------------------------------------------------------------
   // for DC763
   SetWireInfo(0, "X1-01", 0, 0, 0, -160, -123.923);
   SetWireInfo(1, "X1-02", 1, 0, 0, -120, -123.923);
   SetWireInfo(2, "X1-03", 2, 0, 0, -80, -123.923);
   SetWireInfo(3, "X1-04", 3, 0, 0, -40, -123.923);
   SetWireInfo(4, "X1-05", 4, 0, 0, 0, -123.923);
   SetWireInfo(5, "X1-06", 5, 0, 0, 40, -123.923);
   SetWireInfo(6, "X1-07", 6, 0, 0, 80, -123.923);
   SetWireInfo(7, "X1-08", 7, 0, 0, 120, -123.923);
   SetWireInfo(8, "X1-09", 8, 0, 0, 160, -123.923);
   SetWireInfo(9, "X2-01", 9, 0, 1, -180, -89.282);
   SetWireInfo(10, "X2-02", 10, 0, 1, -140, -89.282);
   SetWireInfo(11, "X2-03", 11, 0, 1, -100, -89.282);
   SetWireInfo(12, "X2-04", 12, 0, 1, -60, -89.282);
   SetWireInfo(13, "X2-05", 13, 0, 1, -20, -89.282);
   SetWireInfo(14, "X2-06", 14, 0, 1, 20, -89.282);
   SetWireInfo(15, "X2-07", 15, 0, 1, 60, -89.282);
   SetWireInfo(16, "X2-08", 16, 0, 1, 100, -89.282);
   SetWireInfo(17, "X2-09", 17, 0, 1, 140, -89.282);
   SetWireInfo(18, "X2-10", 18, 0, 1, 180, -89.282);
   SetWireInfo(19, "X3-01", 19, 0, 2, -160, -54.641);
   SetWireInfo(20, "X3-02", 20, 0, 2, -120, -54.641);
   SetWireInfo(21, "X3-03", 21, 0, 2, -80, -54.641);
   SetWireInfo(22, "X3-04", 22, 0, 2, -40, -54.641);
   SetWireInfo(23, "X3-05", 23, 0, 2, 0, -54.641);
   SetWireInfo(24, "X3-06", 24, 0, 2, 40, -54.641);
   SetWireInfo(25, "X3-07", 25, 0, 2, 80, -54.641);
   SetWireInfo(26, "X3-08", 26, 0, 2, 120, -54.641);
   SetWireInfo(27, "X3-09", 27, 0, 2, 160, -54.641);
   SetWireInfo(28, "Y1-01", 28, 1, 0, -240, 54.641);
   SetWireInfo(29, "Y1-02", 29, 1, 0, -200, 54.641);
   SetWireInfo(30, "Y1-03", 30, 1, 0, -160, 54.641);
   SetWireInfo(31, "Y1-04", 31, 1, 0, -120, 54.641);
   SetWireInfo(32, "Y1-05", 32, 1, 0, -80, 54.641);
   SetWireInfo(33, "Y1-06", 33, 1, 0, -40, 54.641);
   SetWireInfo(34, "Y1-07", 34, 1, 0, 0, 54.641);
   SetWireInfo(35, "Y1-08", 35, 1, 0, 40, 54.641);
   SetWireInfo(36, "Y1-09", 36, 1, 0, 80, 54.641);
   SetWireInfo(37, "Y1-10", 37, 1, 0, 120, 54.641);
   SetWireInfo(38, "Y1-11", 38, 1, 0, 160, 54.641);
   SetWireInfo(39, "Y1-12", 39, 1, 0, 200, 54.641);
   SetWireInfo(40, "Y1-13", 40, 1, 0, 240, 54.641);
   SetWireInfo(41, "Y2-01", 41, 1, 1, -220, 89.282);
   SetWireInfo(42, "Y2-02", 42, 1, 1, -180, 89.282);
   SetWireInfo(43, "Y2-03", 43, 1, 1, -140, 89.282);
   SetWireInfo(44, "Y2-04", 44, 1, 1, -100, 89.282);
   SetWireInfo(45, "Y2-05", 45, 1, 1, -60, 89.282);
   SetWireInfo(46, "Y2-06", 46, 1, 1, -20, 89.282);
   SetWireInfo(47, "Y2-07", 47, 1, 1, 20, 89.282);
   SetWireInfo(48, "Y2-08", 48, 1, 1, 60, 89.282);
   SetWireInfo(49, "Y2-09", 49, 1, 1, 100, 89.282);
   SetWireInfo(50, "Y2-10", 50, 1, 1, 140, 89.282);
   SetWireInfo(51, "Y2-11", 51, 1, 1, 180, 89.282);
   SetWireInfo(52, "Y2-12", 52, 1, 1, 220, 89.282);
   SetWireInfo(53, "Y3-01", 53, 1, 2, -200, 123.923);
   SetWireInfo(54, "Y3-02", 54, 1, 2, -160, 123.923);
   SetWireInfo(55, "Y3-03", 55, 1, 2, -120, 123.923);
   SetWireInfo(56, "Y3-04", 56, 1, 2, -80, 123.923);
   SetWireInfo(57, "Y3-05", 57, 1, 2, -40, 123.923);
   SetWireInfo(58, "Y3-06", 58, 1, 2, 0, 123.923);
   SetWireInfo(59, "Y3-07", 59, 1, 2, 40, 123.923);
   SetWireInfo(60, "Y3-08", 60, 1, 2, 80, 123.923);
   SetWireInfo(61, "Y3-09", 61, 1, 2, 120, 123.923);
   SetWireInfo(62, "Y3-10", 62, 1, 2, 160, 123.923);
   SetWireInfo(63, "Y3-11", 63, 1, 2, 200, 123.923);
}

#endif // #ifdef TDC763Ana_cxx