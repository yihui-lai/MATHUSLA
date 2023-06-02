#ifdef CMSSW_GIT_HASH
#include "MATHUSLA/Tile/interface/LYSimAnalysis.hh"
#include "MATHUSLA/Tile/interface/LYSimDetectorConstruction.hh"
#include "MATHUSLA/Tile/interface/LYSimPMTSD.hh"
#include "MATHUSLA/Tile/interface/LYSimSteppingAction.hh"
#include "MATHUSLA/Tile/interface/LYSimSteppingMessenger.hh"
#else
#include "LYSimAnalysis.hh"
#include "LYSimDetectorConstruction.hh"
#include "LYSimPMTSD.hh"
#include "LYSimSteppingAction.hh"
#include "LYSimSteppingMessenger.hh"
#endif

#include "G4EventManager.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4UnitsTable.hh"

using namespace CLHEP;

LYSimSteppingAction::LYSimSteppingAction(LYSimAnalysis *ana)
    : G4UserSteppingAction(),
      maxtracklength(100000. * mm),
      messenger(new LYSimSteppingMessenger(this)),
      analysis(ana)
{
}

LYSimSteppingAction::~LYSimSteppingAction()
{
  delete messenger;
}

void LYSimSteppingAction::UserSteppingAction(const G4Step *step)
{
  G4OpBoundaryProcessStatus boundaryStatus = Undefined;
  static G4OpBoundaryProcess *boundary = NULL;

  // find the boundary process only once
  if (!boundary)
  {
    G4ProcessManager *pm = step->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector *pv = pm->GetProcessList();
    G4int i;

    for (i = 0; i < nprocesses; i++)
    {
      if ((*pv)[i]->GetProcessName() == "OpBoundary")
      {
        boundary = (G4OpBoundaryProcess *)(*pv)[i];
        break;
      }
    }
  }

  // Stop if boundary is not found
  if (boundary)
  {
    boundaryStatus = boundary->GetStatus();

    switch (boundaryStatus)
    {
    case Detection:
      // Note, this assumes that the volume causing detection is the photocathode
      // because it is the only one with non-zero efficiency. Trigger sensitive
      // detector manually since photon is absorbed but status was Detection
      {
        G4EventManager::GetEventManager()->KeepTheCurrentEvent();
        G4SDManager *SDman = G4SDManager::GetSDMpointer();
        LYSimPMTSD *pmtSD = (LYSimPMTSD *)SDman->FindSensitiveDetector("/LYSimPMT");
        if (pmtSD)
        {
          pmtSD->ProcessHits_constStep(step, NULL);
        }
        break;
        step->GetTrack()->SetTrackStatus(fStopAndKill);
      }
    default:
      break;
    }

    G4double tracklength = step->GetTrack()->GetTrackLength();
    if (tracklength > maxtracklength)
    {
      G4cout << "Track length exceeded limit of " << maxtracklength / mm
             << "mm" << G4endl;
      step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
  }

  G4StepPoint *thePrePoint = step->GetPreStepPoint();
  G4StepPoint *thePostPoint = step->GetPostStepPoint();
  G4VPhysicalVolume *thePrePV = thePrePoint->GetPhysicalVolume();
  G4VPhysicalVolume *thePostPV = thePostPoint->GetPhysicalVolume();
  G4String thePrePVName = "";
  if (thePrePV)
    thePrePVName = thePrePV->GetName();
  G4String thePostPVName = "";
  if (thePostPV)
    thePostPVName = thePostPV->GetName();

  G4Track *theTrack = step->GetTrack();

  if (theTrack)
  {
    G4ParticleDefinition *particleType = theTrack->GetDefinition();
    // if (particleType == G4OpticalPhoton::OpticalPhotonDefinition() ){
    // if(thePrePVName.contains("PhyhWLSfiber_cald")){
    //     std::cout<<thePrePVName<<" -> "<<thePostPVName<<std::endl;
    // }
    // if(thePostPVName.contains("Hole") && thePrePVName.contains("PhyhWLSfiber_cald")){
    // std::cout<<thePrePVName<<" -> "<<thePostPVName<<std::endl;
    // std::cout<<"kill a photon from PhyhWLSfiber_cald to TileHole "<<std::endl;
    // theTrack->SetTrackStatus(fKillTrackAndSecondaries);
    //}
    //}
    if (particleType == G4OpticalPhoton::OpticalPhotonDefinition() && theTrack->GetCurrentStepNumber() == 1)
    {
      if (thePrePVName.contains("PhyhWLSfiber_core"))
      {
        LYSimAnalysis::GetInstance()->addwlsphoton();
        // std::cout<<"generate a wls from "<<theTrack->GetCreatorProcess()->GetProcessName()<<std::endl;
        // theTrack->SetTrackStatus(fKillTrackAndSecondaries);
      }
      else
      {
        LYSimAnalysis::GetInstance()->addgenphoton();
      }
    }
  }
}
