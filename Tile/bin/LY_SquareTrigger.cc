#include "MATHUSLA/Tile/interface/LYSimAnalysis.hh"
#include "MATHUSLA/Tile/interface/LYSimDetectorConstruction.hh"
#include "MATHUSLA/Tile/interface/LYSimPhysicsList.hh"
#include "MATHUSLA/Tile/interface/LYSimPrimaryGeneratorAction.hh"
#include "MATHUSLA/Tile/interface/LYSimSteppingAction.hh"
#include "MATHUSLA/Tile/interface/LYSimTrackingAction.hh"
#include "MATHUSLA/Tile/interface/LYSimProtonGeneratorAction.hh"
#include "UserUtils/Common/interface/ArgumentExtender.hpp"

#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

int main(int argc, char **argv)
{
  usr::po::options_description desc("Running a run with a certain geometry");
  desc.add_options()                                                                                //
      ("beamx,x", usr::po::defvalue<double>(0), "x center of beam [mm]")                            //
      ("beamz,z", usr::po::defvalue<double>(0), "z center of beam [mm]")                            //
      ("tileZ,l", usr::po::defvalue<double>(200), "Length of tile [mm]")                            //
      ("tileX,X", usr::po::defvalue<double>(50), "X of tile [mm]")                                  //
      ("tileY,Y", usr::po::defvalue<double>(20), "Y of tile [mm]")                                  //
      ("beamwidth,w", usr::po::defvalue<double>(30), "width of beam [mm]")                          //
      ("fiberZ,f", usr::po::defvalue<float>(5), "fiber length [m]")                                 //
      ("fiberZshift,s", usr::po::defvalue<double>(0), "fiber shift [m]")                            //
      ("absmult,a", usr::po::defvalue<double>(1000), "absorption length at 425nm, unit mm")         //
      ("tiledecay,td", usr::po::defvalue<double>(5), "tile decay time, unit ns, 11.5 ms overall")   //
      ("tilerise,tr", usr::po::defvalue<double>(0.5), "tile rise time, unit ns")                    //
      ("absY11,b", usr::po::defvalue<double>(5000), "absorption length of Y11, unit mm")            //
      ("yield,y", usr::po::defvalue<double>(10000), "light yield / MeV")                            //
      ("wrapreflect,m", usr::po::defvalue<float>(0.985), "Wrap reflectivity")                       //
      ("sipmeff,e", usr::po::defvalue<double>(1), "SiPM eff")                                       //
      ("Y11decayTime,d", usr::po::defvalue<double>(7.193), "Y11 WLS time constant")                 //
      ("NEvents,N", usr::po::defvalue<unsigned>(1), "Number of events to run")                      //
      ("useProton,P", usr::po::defvalue<int>(1), "Flag to switch the source to a true muon source") //
      ("handwrap,H", usr::po::defvalue<int>(0), "Flag to switch to handwrap")                       //
      ("cladlayer,c", usr::po::defvalue<unsigned>(1), "Number of cladlayers")                       //
      ("fiberR,r", usr::po::defvalue<double>(0.7), "Fiber radius")                                  //
      ("HoleR,R", usr::po::defvalue<double>(1), "Hole radius")                                      //
      ("output,o", usr::po::defvalue<std::string>("test.root"), "output file")                      //
      ("spare,S", usr::po::defvalue<double>(1.0), "Spare Parameter")                                //
      ;

  usr::ArgumentExtender args;
  args.AddOptions(desc);
  args.ParseOptions(argc, argv);

  const double x_center = args.Arg<double>("beamx");
  const double z_center = args.Arg<double>("beamz");
  const double tileZ = args.Arg<double>("tileZ");
  const double tileX = args.Arg<double>("tileX");
  const double tileY = args.Arg<double>("tileY");
  const double width = args.Arg<double>("beamwidth");
  const double fiberZ = args.Arg<float>("fiberZ");
  const double fiberZshift = args.Arg<double>("fiberZshift");
  const double absmult = args.Arg<double>("absmult");
  const double absY11 = args.Arg<double>("absY11");
  const double yield = args.Arg<double>("yield");
  const double wrapref = args.Arg<float>("wrapreflect");
  const double Y11decayTime = args.Arg<double>("Y11decayTime");
  const double sipmeff = args.Arg<double>("sipmeff");
  const unsigned N = args.Arg<unsigned>("NEvents");
  const bool useProton = args.Arg<int>("useProton");
  const bool handwrap = args.Arg<int>("handwrap");
  const unsigned cladlayer = args.Arg<unsigned>("cladlayer");
  const double fiberR = args.Arg<double>("fiberR");
  const double HoleR = args.Arg<double>("HoleR");
  std::string filename = args.Arg<std::string>("output");
  filename.insert(filename.length() - 5, "_" + usr::RandomString(6));
  const double tilerise = args.Arg<double>("tilerise");
  const double tiledecay = args.Arg<double>("tiledecay");
  const double spare = args.Arg<double>("spare");

  // Must initialize Run Manager first
  G4RunManager *runManager = new G4RunManager;
  LYSimPhysicsList *physlist = new LYSimPhysicsList();
  // Overriding the detector parameters
  LYSimDetectorConstruction *detector = new LYSimDetectorConstruction();
  detector->SetTileZ(tileZ);
  detector->SetTileX(tileX);
  detector->SetTileY(tileY);
  detector->SetFiberZ(fiberZ);
  detector->SetFiberXoff(0);
  detector->SetFiberZoff(fiberZshift);
  detector->SetFiberClad(cladlayer);
  detector->SetHoleShape(0); // 0 circle; 1 square; 2 el
  detector->SetTileAbsMult(absmult);
  detector->SetTileDecaytime(tiledecay);
  detector->SetTileRisetime(tilerise);
  // detector->SetClad_refrac_index(tilerise,tiledecay);  //clad1, clad2

  detector->SetTileScintillation(yield);
  detector->SetY11decaytime(Y11decayTime);
  detector->SetY11attenu(absY11);
  detector->SetFiberR(fiberR);
  detector->SetHoleRadius(HoleR);

  detector->SetGaprefrac_index(spare); // Gap material refraction index
  detector->Setcladdirt(0);          // dirt on the fiber clad
  detector->SetWrapReflect(wrapref);
  detector->SetSiPMReflect(sipmeff);
  detector->Set_handwrap(handwrap);

  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(physlist);

  // Overriding the generator parameters
  if (!useProton)
  {
    LYSimPrimaryGeneratorAction *genaction = new LYSimPrimaryGeneratorAction(detector);
    genaction->SetBeamX(x_center);
    genaction->SetBeamY(z_center);
    genaction->SetWidth(width);
    LYSimAnalysis::GetInstance()->SetGeneratorAction(genaction);
    runManager->SetUserAction(genaction);
  }
  else
  {
    LYSimProtonGeneratorAction *genaction = new LYSimProtonGeneratorAction();
    genaction->SetBeamX(x_center);
    genaction->SetBeamZ(z_center);
    genaction->SetWidth(width);
    LYSimAnalysis::GetInstance()->SetProtonGeneratorAction(genaction);
    runManager->SetUserAction(genaction);
  }

  // Construct LYSimAnalysis class
  LYSimAnalysis::GetInstance()->SetDetector(detector);
  LYSimAnalysis::GetInstance()->SetOutputFile(filename);

  runManager->SetUserAction(new LYSimAnalysis::RunAction());
  runManager->SetUserAction(new LYSimAnalysis::EventAction());
  runManager->SetUserAction(new LYSimTrackingAction());
  runManager->SetUserAction(
      new LYSimSteppingAction(LYSimAnalysis::GetInstance()));

  // Preparing the LYSimAnalysis
  LYSimAnalysis::GetInstance()->PrepareExperiment();

  // Initialize G4 kernel
  runManager->Initialize();
  G4VisManager *visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
  G4UImanager *UIManager = G4UImanager::GetUIpointer();

  char cmd[1024];
  UIManager->ApplyCommand("/control/verbose 1");
  UIManager->ApplyCommand("/run/verbose 0");
  UIManager->ApplyCommand("/process/setVerbose 0");
  UIManager->ApplyCommand("/tracking/verbose 0");

  srand(time(NULL));

  sprintf(cmd, "/random/setSeeds %d %d", rand(), rand());
  UIManager->ApplyCommand(cmd);

  std::cout << N << std::endl;
  runManager->BeamOn(N);

  LYSimAnalysis::GetInstance()->EndOfExperiment();

  // Job termination Free the store: user actions, physics_list and
  // detector_description are owned and deleted by the run manager, so they
  // should not be deleted in the main() program !
  delete visManager;
  delete runManager;

  return 0;
}
