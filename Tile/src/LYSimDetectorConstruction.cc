#ifdef CMSSW_GIT_HASH
#include "MATHUSLA/Tile/interface/LYSimDetectorConstruction.hh"
#include "MATHUSLA/Tile/interface/LYSimDetectorMessenger.hh"
#include "MATHUSLA/Tile/interface/Materials.hh"
#include "MATHUSLA/Tile/interface/SurfaceProperty.hh"
#else
#include "LYSimDetectorConstruction.hh"
#include "LYSimDetectorMessenger.hh"
#include "Materials.hh"
#include "SurfaceProperty.hh"
#endif

#include <math.h>

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Ellipsoid.hh"
#include "G4GeometryManager.hh"
#include "G4IntersectionSolid.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Orb.hh"
#include "G4Orb.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4RegionStore.hh"
#include "G4RotationMatrix.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Torus.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "G4EllipticalTube.hh"

using std::cos;
using std::sin;
using std::tan;
using std::atan;
using std::exp;
using namespace CLHEP;

LYSimPMTSD* LYSimDetectorConstruction::fPMTSD = NULL;

LYSimDetectorConstruction::LYSimDetectorConstruction()
  : G4VUserDetectorConstruction()
{
  fdetectorMessenger = new LYSimDetectorMessenger( this );

  _tilex        = 50*mm;
  _tiley        = 20*mm;
  _tilez        = 200*mm;
  _tile_x1      = 0.0*mm;
  _tile_x2      = 0.0*mm;
  wrapgap       = 0.1*mm;
  wrapthickness = 0.1*mm;

  _absmult      = 1000; // mm
  _tiledecay    = 2.5; //ns
  _tilerise     = 0.5; //ns
  _sipm_x          = 10*mm;
  _sipm_y          = 10*mm;
  _sipm_z          = 0.1*mm;
  _sipm_eff = 1;

  //wls
  _handwrap   = false;
  _cladlayer  = 1; //number of fiber clad layers 
  _holeshape = 0; //0 circle; 1 square; 2 el
  _WLSfiberR = 0.7*mm;
  _WLSfiber_clad_thick = 0.05*mm;
  _WLSfiber_clad2_thick = 0.05*mm;
  
  _hole_radius = 1*mm;//
  _hole_x1 = 0*mm; // hole position 
  _hole_x2 = _hole_x1; // a second hole position
  
  _WLSfiberZ = 3.2*m;
  _WLS_zoff = 1.7*m;
  
  _WLSfiberZ = _tilez*2.5;
  _WLS_zoff = 0;
  _WLS_xoff = 0;
  _claddirt = 0.0;

  //---------------
  //material
  //---------------
  fBialkali = Make_Bialkali();
  fEpoxy    = Make_Epoxy();
  fAir      = Make_Custom_Air();
  fEJ200    = Make_EJ200();
  fResin    = Make_Resin();
  SetTileAbsMult( _absmult );
  _ScintiN      = 100;
  SetTileScintillation( _ScintiN );
  SetTileDecaytime(_tiledecay);
  SetTileRisetime(_tilerise);
  mfiber  = Make_sgc();
  mfiber_clad = Make_acrylic(); //cald 1
  mfiber_clad2 = Make_Fluor_acrylic(); //clad 2
  fcoating = Make_Coating();
  
  _y11_decaytime = 11.5; //ns
  SetY11decaytime( _y11_decaytime );

  //---------------
  //surface
  //---------------
  _tile_alpha   = 0.01;
  fTileBulkSurface        = MakeS_RoughInterface( _tile_alpha );
  fSiPMSurface3            = MakeS_SiPM();
  fSiPMSurface4            = MakeS_SiPM();
  fTiO2Surface = MakeS_TiO2Surface();   // Extrusion (TiO2 Coating) Surface
  //opSurface =  MakeS_IdealPolished();
  _wrap_reflect = 0.985;
  fESROpSurface           = MakeS_RoughMirror();
  SetWrapReflect( _wrap_reflect );  // Set fESROpSurface to be flat reflection

}

void
LYSimDetectorConstruction::UpdateGeometry()
{
  // clean-up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();

  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4LogicalSkinSurface::CleanSurfaceTable();
  G4LogicalBorderSurface::CleanSurfaceTable();

  G4RunManager::GetRunManager()->DefineWorldVolume( Construct() );
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

LYSimDetectorConstruction::~LYSimDetectorConstruction()
{
  if( fdetectorMessenger ){ delete fdetectorMessenger; }
}

G4VPhysicalVolume*
LYSimDetectorConstruction::Construct()
{
  static const bool checkOverlaps = true;
  double verbose=11;
  ///////////////////////////////////////////////////////////////////////////////
  // World volume
  ///////////////////////////////////////////////////////////////////////////////
  G4Box* solidWorld = new G4Box( "World", WorldHalfX() , WorldHalfY(), WorldHalfZ() );
  G4LogicalVolume* logicWorld = new G4LogicalVolume( solidWorld, fAir, "World" );
  G4VPhysicalVolume* physWorld = new G4PVPlacement( 0
                                                  , G4ThreeVector( 0, 0, 0 )
                                                  , logicWorld
                                                  , "World"
                                                  , 0
                                                  , false
                                                  , 0
                                                  , checkOverlaps );

  ///////////////////////////////////////////////////////////////////////////////
  // extruded scintillator
  ///////////////////////////////////////////////////////////////////////////////
  // create hole boundary 
  G4VSolid* solidHoleBound;
  G4LogicalVolume* logicTileBulk;
  if(_holeshape==0) solidHoleBound = new G4Tubs( "TileHole", 0, _hole_radius, _tilez, 0, 2*pi  );
  else if(_holeshape==1) solidHoleBound = new G4Box( "TileHole", _hole_radius, _hole_radius, _tilez  );
  else if (_holeshape==2) solidHoleBound = new G4EllipticalTube( "TileHole", _hole_radius, _hole_radius*2, _tilez );
  if(verbose>10) std::cout<<"---> hole shape set to be "<<_holeshape<<" radius is "<<_hole_radius << std::endl;
  
  G4VSolid* solidTile = ConstructTrapazoidSolid( "TileTrap"
                                               , _tilex
                                               , _tiley
                                               , _tilez
                                               , _tile_x1
                                               , _tile_x2 );
  G4VSolid* tileBulk1 = new G4SubtractionSolid( "TileSolid_Bulk"
                            , solidTile, solidHoleBound
                            , 0, G4ThreeVector( _hole_x1, 0, 0 ) );
  if(_hole_x1!=_hole_x2){
    // if _hole_x1!=_hole_x2, set another hole 
    G4VSolid* tileBulk = new G4SubtractionSolid( "TileSolid_Bulk", tileBulk1, solidHoleBound, 0, G4ThreeVector( _hole_x2, 0, 0 ) );
    logicTileBulk = new G4LogicalVolume( tileBulk, fEJ200, "TileBulkLogic" );
  }else{
    logicTileBulk = new G4LogicalVolume( tileBulk1, fEJ200, "TileBulkLogic" ); 
  }
  G4VPhysicalVolume* physTileBulk = new G4PVPlacement( 0
                                                     , G4ThreeVector( 0, 0, 0 )
                                                     , logicTileBulk
                                                     , "TileBulkPhysic"
                                                     , logicWorld
                                                     , false
                                                     , 0
                                                     , checkOverlaps );

  ///////////////////////////////////////////////////////////////////////////////
  // wrapper or clad, nothing at both ends
  ///////////////////////////////////////////////////////////////////////////////
  G4LogicalVolume* logicWrap;
  if(_handwrap){
    // Hand wrap, a thin air gap between scnitillator and wrapper
    G4VSolid* solidExtrusion = ConstructHollowWrapSolid();
    logicWrap = new G4LogicalVolume(solidExtrusion, fEpoxy, "Extrusion");
    G4VPhysicalVolume* physWrap = new G4PVPlacement( 0
                                                 , G4ThreeVector( 0, 0, 0 )
                                                 , logicWrap
                                                 , "Wrap"
                                                 , logicWorld
                                                 , false
                                                 , 0
                                                 , checkOverlaps );
    G4LogicalSkinSurface* WrapSurface = new G4LogicalSkinSurface( "WrapSurface" , logicWrap, fESROpSurface ); // Set wrapper surface to be ESR
  }else{
    // No air gap between scnitillator and wrapper
    G4VSolid* solidExtrusion = ConstructHollowWrapCladSolid();
    logicWrap = new G4LogicalVolume(solidExtrusion, fcoating, "Extrusion");
    G4VPhysicalVolume* physWrap = new G4PVPlacement( 0
                                                 , G4ThreeVector( 0, 0, 0 )
                                                 , logicWrap
                                                 , "Wrap"
                                                 , logicWorld
                                                 , false
                                                 , 0
                                                 , checkOverlaps );
    G4LogicalSkinSurface* WrapSurface = new G4LogicalSkinSurface( "WrapSurface", logicWrap, fTiO2Surface ); // Set wrapper surface to be TiO2
  }

  ///////////////////////////////////////////////////////////////////////////////
  // wrap ends, if cover ends with wrapper
  ///////////////////////////////////////////////////////////////////////////////
  /*
  G4VPhysicalVolume* physScintillatorEndWrap3;
  G4VPhysicalVolume* physScintillatorEndWrap4;
  G4LogicalVolume* logicWrap_ends;
  G4VSolid* solidWrapface1;
  G4VSolid* wrapface = ConstructTrapazoidSolid( "Wrapface", _tilex, _tiley, wrapthickness, 0, 0 );
  G4VSolid* solidWrapface0 = new G4SubtractionSolid( "solidWrapface0", wrapface, solidHoleBound, 0, G4ThreeVector( _hole_x1, 0, 0 ) );
  if(_hole_x1!=_hole_x2){  
    if(verbose>10) std::cout<<"---> Add a second fiber hole "<< std::endl;
    solidWrapface1 = new G4SubtractionSolid( "solidWrapface1", solidWrapface0, solidHoleBound, 0, G4ThreeVector( _hole_x2, 0, 0 ) );
    logicWrap_ends = new G4LogicalVolume( solidWrapface1, fEpoxy,  "Wrapface" );
  }
  else{
    logicWrap_ends = new G4LogicalVolume( solidWrapface0, fEpoxy,  "Wrapface" );
  }
  G4LogicalSkinSurface* WrapfaceSurface = new G4LogicalSkinSurface( "WrapfaceSurface", logicWrap_ends, MakeS_NoAbsorbing() );
  physScintillatorEndWrap3 = new G4PVPlacement( 0
                                                , G4ThreeVector( 0, 0, _tilez*0.5 + wrapthickness*0.5 )
                                                , logicWrap_ends
                                                , "Wrapface3"
                                                , logicWorld
                                                , false
                                                , 0
                                                , checkOverlaps );
  physScintillatorEndWrap4 = new G4PVPlacement( 0
                                                , G4ThreeVector( 0, 0, -_tilez*0.5 - wrapthickness*0.5 )
                                                , logicWrap_ends
                                                , "Wrapface4"
                                                , logicWorld
                                                , false
                                                , 0
                                                , checkOverlaps );
  */


  /*
  //interface between wrap ends and scintillator
  if(!_handwrap){
      new G4LogicalBorderSurface("surfacewrapout1", physScintillatorEndWrap3, physTileBulk, MakeS_Absorbing());
      new G4LogicalBorderSurface("surfacewrapout2", physTileBulk, physScintillatorEndWrap3, MakeS_Absorbing());
      new G4LogicalBorderSurface("surfacewrapout3", physTileBulk, physScintillatorEndWrap4, MakeS_Absorbing());
      new G4LogicalBorderSurface("surfacewrapout4", physScintillatorEndWrap4, physTileBulk, MakeS_Absorbing());
  }
  */


  ///////////////////////////////////////////////////////////////////////////////
  // fiber. single layer clad
  ///////////////////////////////////////////////////////////////////////////////
  assert(_hole_radius>=_WLSfiberR+_WLSfiber_clad_thick); 
  G4VSolid* solidWLSfiber = new G4Tubs("WLSFiber", 0., _WLSfiberR, _WLSfiberZ*0.5, 0., 2*pi);
  G4VSolid* solidWLSfiber_clad = new G4Tubs("WLSFiber_clad", _WLSfiberR, _WLSfiberR+_WLSfiber_clad_thick, _WLSfiberZ*0.5, 0., 2*pi);
  G4LogicalVolume* logicWLSfiber = new G4LogicalVolume( solidWLSfiber , mfiber,  "logicWLSfiber" );
  G4LogicalVolume* logicWLSfiber_clad = new G4LogicalVolume( solidWLSfiber_clad , mfiber_clad,  "logicWLSfiber_clad" );

  G4VPhysicalVolume* physWLSfiber = new G4PVPlacement( 0, G4ThreeVector(_hole_x1+_WLS_xoff, 0, -_WLS_zoff)
                                                      , logicWLSfiber
                                                      , "PhyhWLSfiber_core"
                                                      , logicWorld
                                                      , false
                                                      , 0
                                                      , checkOverlaps );

  G4VPhysicalVolume* physWLSfiber_clad = new G4PVPlacement( 0, G4ThreeVector(_hole_x1+_WLS_xoff, 0, -_WLS_zoff)
                                                      , logicWLSfiber_clad
                                                      , "PhyhWLSfiber_cald"
                                                      , logicWorld
                                                      , false
                                                      , 0
                                                      , checkOverlaps );

  ///////////////////////////////////////////////////////////////////////////////
  // fiber. second layer clad
  ///////////////////////////////////////////////////////////////////////////////
  G4LogicalVolume* logicWLSfiber_clad2;
  G4VSolid* solidWLSfiber_clad2;
  std::cout<<"============  add a second cladding! ================="<<std::endl;
  solidWLSfiber_clad2 = new G4Tubs("WLSFiber_clad2", _WLSfiberR+_WLSfiber_clad_thick, _WLSfiberR+_WLSfiber_clad_thick+_WLSfiber_clad2_thick, _WLSfiberZ*0.5, 0., 2*pi);
  logicWLSfiber_clad2 = new G4LogicalVolume( solidWLSfiber_clad2 , mfiber_clad2,  "logicWLSfiber_clad2" );
  if(_cladlayer==2){
     G4VPhysicalVolume* physWLSfiber_clad2 = new G4PVPlacement( 0, G4ThreeVector(_hole_x1+_WLS_xoff, 0, -_WLS_zoff)
                                                      , logicWLSfiber_clad2
                                                      , "PhyhWLSfiber_cald2"
                                                      , logicWorld
                                                      , false
                                                      , 0
                                                      , checkOverlaps );
   }

  ///////////////////////////////////////////////////////////////////////////////
  // gap between fiber and hole
  ///////////////////////////////////////////////////////////////////////////////
  double holeinner = _WLSfiberR+_WLSfiber_clad_thick;
  if(_cladlayer==2) holeinner+=_WLSfiber_clad2_thick;
  G4VSolid* solidHole;
  if(_holeshape==0) solidHole = new G4Tubs( "TileHole", 0, _hole_radius, _tilez*0.5, 0, 2*pi  );
  else if(_holeshape==1) solidHole = new G4Box( "TileHole", _hole_radius, _hole_radius, _tilez*0.5  );
  else if (_holeshape==2) solidHole = new G4EllipticalTube( "TileHole", _hole_radius, 2, _tilez*0.5 );

  G4VSolid* solidWLSfiberFrame = new G4Tubs("WLSFiberFrame", 0., holeinner, _WLSfiberZ*0.5, 0., 2*pi);
  G4VSolid* solidHole_subs= new G4SubtractionSolid( "TileHole_Subs", solidHole, solidWLSfiberFrame, 0, G4ThreeVector( _WLS_xoff, 0, 0 ) );
  G4LogicalVolume* logicHole = new G4LogicalVolume( solidHole_subs, fAir,  "Hole" );
  //G4LogicalVolume* logicHole = new G4LogicalVolume( solidHole_subs, fResin,  "Hole" );
  G4VPhysicalVolume* physHole = new G4PVPlacement( 0
                                                 , G4ThreeVector( _hole_x1, 0, 0 )
                                                 , logicHole
                                                 , "Hole"
                                                 , logicWorld
                                                 , false
                                                 , 0
                                                 , checkOverlaps );
  /*
  if(_cladlayer==2){
        new G4LogicalBorderSurface("surfacefiberOut", physWLSfiber,
                                   physWLSfiber_clad, opSurface);
        new G4LogicalBorderSurface("surfacefiberin", physWLSfiber_clad,
                                   physWLSfiber, opSurface);
        new G4LogicalBorderSurface("surfaceclad1Out", physWLSfiber_clad,
                                   physWLSfiber_clad2, opSurface);
        new G4LogicalBorderSurface("surfaceclad1in", physWLSfiber_clad2,
                                   physWLSfiber_clad, opSurface);
        new G4LogicalBorderSurface("surfaceClad2Out", physWLSfiber_clad2,
                                   physHole, opSurface);
        new G4LogicalBorderSurface("surfaceClad2In", physHole, physWLSfiber_clad2,
                                   opSurface);
  }else{
        new G4LogicalBorderSurface("surfacefiber1Out", physWLSfiber,
                                   physWLSfiber_clad, opSurface);
        new G4LogicalBorderSurface("surfacefiber1in", physWLSfiber_clad,
                                   physWLSfiber, opSurface);
        new G4LogicalBorderSurface("surfaceClad1Out", physWLSfiber_clad,
                                   physHole, opSurface);
        new G4LogicalBorderSurface("surfaceClad1In",  physHole, physWLSfiber_clad,
                                   opSurface);

  }
  */

  /*
  G4OpticalSurface* surface_IdealnonPolished = new G4OpticalSurface("IdealnonPolished", glisur, ground, dielectric_dielectric, 1);
  double surface_IdealnonPolishedphoE2[2]        = {1.0*eV,8.0*eV };
  double surface_IdealnonPolishedreflectivity[2] = {1-_claddirt, 1-_claddirt};
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "REFLECTIVITY", surface_IdealnonPolishedphoE2, surface_IdealnonPolishedreflectivity, 2 );
  //table->AddProperty( "TRANSMITTANCE", surface_IdealnonPolishedphoE2, surface_IdealnonPolishedreflectivity, 2 );
  surface_IdealnonPolished->SetMaterialPropertiesTable( table );
  new G4LogicalSkinSurface( "fibercore", logicWLSfiber, opSurface );
  //new G4LogicalSkinSurface( "fiberclad", logicWLSfiber_clad, surface_IdealnonPolished );

        new G4LogicalBorderSurface("surfaceClad1Out", physWLSfiber_clad,
                                   physHole, surface_IdealnonPolished);
        new G4LogicalBorderSurface("surfaceClad1In",  physHole, physWLSfiber_clad,
                                   surface_IdealnonPolished);
  */

  //end here
  //G4OpticalSurface* surface_p =MakeS_TiO2Surface();
  //new G4LogicalBorderSurface("surfaceCladin", physWLSfiber_clad, physHole, surface_p); 
  /*new G4LogicalBorderSurface("surfacecladout", physWLSfiber_clad, physHole, opSurface); 
  new G4LogicalBorderSurface("surfacecladin", physHole, physWLSfiber_clad, opSurface); 
  new G4LogicalBorderSurface("surfacecoreout", physWLSfiber_clad, physWLSfiber, opSurface);
  new G4LogicalBorderSurface("surfacecorein", physWLSfiber, physWLSfiber_clad, opSurface);
  */

  ///////////////////////////////////////////////////////////////////////////////
  // Simple version of SiPM
  ///////////////////////////////////////////////////////////////////////////////
  const G4ThreeVector SiPMOffset_chan3( _hole_x1+_WLS_xoff, 0, _WLSfiberZ*0.5 - _WLS_zoff + 0.5*_sipm_z + 0.01*mm);  
  const G4ThreeVector SiPMOffset_chan4( _hole_x1+_WLS_xoff, 0, -_WLSfiberZ*0.5 - _WLS_zoff - 0.5*_sipm_z - 0.01*mm);
  G4Tubs* solidSiPMInnerBox = new G4Tubs( "solidSiPMInnerBox", 0., _WLSfiberR, _sipm_z*0.5, 0., 2*pi);
  G4LogicalVolume* logicSiPM = new G4LogicalVolume( solidSiPMInnerBox
                                                  , fBialkali,  "SiPM" );
  G4VPhysicalVolume* physSiPM_chan3 = new G4PVPlacement( 0, SiPMOffset_chan3
                                                 , logicSiPM
                                                 , "physSiPM_chan3"
                                                 , logicWorld
                                                 , false
                                                 , 0
                                                 , checkOverlaps );
  G4VPhysicalVolume* physSiPM_chan4 = new G4PVPlacement( 0, SiPMOffset_chan4
                                                 , logicSiPM
                                                 , "physSiPM_chan4"
                                                 , logicWorld
                                                 , false
                                                 , 0
                                                 , checkOverlaps );
  /*
  G4OpticalSurface* FiberEndSurface = new G4OpticalSurface("FiberEndSurface",
                                                         unified,
                                                         ground,
                                                         dielectric_dielectric,
                                                         0.02);
  new G4LogicalBorderSurface("FiberEndSurface1",physWLSfiber,physWorld,FiberEndSurface);
  new G4LogicalBorderSurface("FiberEndSurface2",physWorld,physWLSfiber,FiberEndSurface);
  new G4LogicalBorderSurface("FiberEndSurface3",physWLSfiber_clad,physWorld,FiberEndSurface);
  new G4LogicalBorderSurface("FiberEndSurface4",physWorld,physWLSfiber_clad,FiberEndSurface);
  */

  ///////////////////////////////////////////////////////////////////////////////
  // Defining surfaces
  ///////////////////////////////////////////////////////////////////////////////

  // Tile surfaces
  G4LogicalBorderSurface* TileBulkSurface = new G4LogicalBorderSurface( "TileBulkSurface", physTileBulk, physWorld, fTileBulkSurface );
  // SiPM surface
  new G4LogicalBorderSurface("SiPMSurface3_out", physWorld, physSiPM_chan3, fSiPMSurface3);
  new G4LogicalBorderSurface("SiPMSurface4_out", physWorld, physSiPM_chan4, fSiPMSurface4);
  new G4LogicalBorderSurface("SiPMSurface3_in", physSiPM_chan3,physWorld, fSiPMSurface3);
  new G4LogicalBorderSurface("SiPMSurface4_in", physSiPM_chan4,physWorld, fSiPMSurface4);
  
  // Setting the sensitive detector
  if( !fPMTSD ){
    fPMTSD = new LYSimPMTSD( "/LYSimPMT" );
    G4SDManager* sdman = G4SDManager::GetSDMpointer();
    sdman->AddNewDetector( fPMTSD );
  }
  logicSiPM->SetSensitiveDetector( fPMTSD );



  G4Colour white  (1.00, 1.00, 1.00);  // white
  G4Colour gray   (0.50, 0.50, 0.50);  // gray
  G4Colour black  (0.00, 0.00, 0.00);  // black
  G4Colour red    (1.00, 0.00, 0.00);  // red
  G4Colour green  (0.00, 1.00, 0.00);  // green
  G4Colour blue   (0.00, 0.00, 1.00);  // blue
  G4Colour cyan   (0.00, 1.00, 1.00);  // cyan
  G4Colour air    (0.90, 0.94, 1.00);  // cyan
  G4Colour magenta(1.00, 0.00, 1.00);  // magenta 
  G4Colour yellow (1.00, 1.00, 0.00);  // yellow
  G4Colour brass  (0.80, 0.60, 0.40);  // brass
  G4Colour brown  (0.70, 0.40, 0.10);  // brown

  // Visual attributes
  logicWorld->SetVisAttributes( G4VisAttributes::Invisible );
  G4VisAttributes* SiPMVisAtt = new G4VisAttributes( red );
  SiPMVisAtt->SetForceSolid( true );
  SiPMVisAtt->SetVisibility( true );
  logicSiPM->SetVisAttributes( G4VisAttributes::Invisible );

  G4VisAttributes* ResinVisAtt = new G4VisAttributes( cyan );
  ResinVisAtt->SetForceWireframe( true );
  ResinVisAtt->SetVisibility( true );
  //logicHole->SetVisAttributes( ResinVisAtt );
  logicHole->SetVisAttributes( G4VisAttributes::Invisible );

  G4VisAttributes* TileVisAtt = new G4VisAttributes( cyan );
  TileVisAtt->SetForceWireframe( true );
  TileVisAtt->SetVisibility( true );
  logicTileBulk->SetVisAttributes( G4VisAttributes::Invisible );
  //logicTileBulk->SetVisAttributes( TileVisAtt );

  G4VisAttributes* fiberVisAtt = new G4VisAttributes( blue );
  fiberVisAtt->SetForceWireframe( true );
  fiberVisAtt->SetVisibility( true );
  //fiberVisAtt->SetForceAuxEdgeVisible( true );
  logicWLSfiber->SetVisAttributes( fiberVisAtt );
  logicWLSfiber_clad->SetVisAttributes( fiberVisAtt );
  if(_cladlayer==2) logicWLSfiber_clad2->SetVisAttributes( fiberVisAtt );

  G4VisAttributes* WrapVisAtt = new G4VisAttributes( gray );
  WrapVisAtt->SetForceWireframe( true );
  WrapVisAtt->SetVisibility( true );
  logicWrap->SetVisAttributes( WrapVisAtt );
  //logicWrap->SetVisAttributes( G4VisAttributes::Invisible );

  return physWorld;
}

G4VSolid*
LYSimDetectorConstruction::ConstructTrapazoidSolid(
  const G4String& name,
  double          x,
  double          y,
  double          z,
  double          indent_x1,
  double          indent_x2 ) const
{
  const G4ThreeVector corners[8] = {
    G4ThreeVector( -x/2,           -y/2, -z/2 ),
    G4ThreeVector( x/2,            -y/2, -z/2 ),
    G4ThreeVector( -x/2+indent_x2, y/2,  -z/2 ),
    G4ThreeVector( x/2-indent_x1,  y/2,  -z/2 ),
    G4ThreeVector( -x/2,           -y/2, z/2 ),
    G4ThreeVector( x/2,            -y/2, z/2 ),
    G4ThreeVector( -x/2+indent_x2, y/2,  z/2 ),
    G4ThreeVector( x/2-indent_x1,  y/2,  z/2 )
  };

  return new G4Trap( name, corners );
}

G4VSolid*
LYSimDetectorConstruction::ConstructHollowWrapSolid() const
{
  G4VSolid* wrapOuter
    = ConstructTrapazoidSolid( "WrapOuter"
                             , _tilex + 2*wrapgap + 2*wrapthickness
                             , _tiley + 2*wrapgap + 2*wrapthickness
                             , _tilez + 2*wrapgap + 2*wrapthickness
                             , 0, 0 );
  G4VSolid* wrapInner
    = ConstructTrapazoidSolid( "WrapInner"
                             , _tilex + 2*wrapgap
                             , _tiley + 2*wrapgap
                             , _tilez*4 + 2*wrapgap
                             , 0, 0 );
  G4VSolid* wrapbox = new G4SubtractionSolid( "WrapBox"
                                            , wrapOuter, wrapInner );

  return wrapbox;
}

G4VSolid*
LYSimDetectorConstruction::ConstructHollowWrapCladSolid() const
{
  G4VSolid* wrapOuter
    = ConstructTrapazoidSolid( "WrapOuter"
                             , _tilex  + 2*wrapthickness
                             , _tiley  + 2*wrapthickness
                             , _tilez
                             , 0, 0 );
  G4VSolid* wrapInner
    = ConstructTrapazoidSolid( "WrapInner"
                             , _tilex 
                             , _tiley
                             , _tilez*4
                             , 0, 0 );
  G4VSolid* wrapbox = new G4SubtractionSolid( "WrapBox"
                                            , wrapOuter, wrapInner );
  return wrapbox;
}

G4VSolid*
LYSimDetectorConstruction::ConstructDimpleSubtract() const
{
  switch( _dimple_type ){
  case SPHERICAL:
    return ConstructSphereDimpleSolid();
  case FLAT_DOME:
    return ConstructFlatDomeDimpleSolid();
  case CYLINDRICAL:
    return ConstructCylindricalDimpleSolid();
  case ELLIPSOID:
    return ConstructEllipsoidDimpleSolid();
  default:
    return ConstructSphereDimpleSolid();
  }
}

G4VSolid*
LYSimDetectorConstruction::ConstructSphereDimpleSolid() const
{
  return new G4Orb( "DimpleSolid"
                  , GetDimpleSphereRadius() );
}

G4VSolid*
LYSimDetectorConstruction::ConstructFlatDomeDimpleSolid() const
{
  G4VSolid* torus = new G4Torus( "DimpleTorus"
                               , 0, _dimple_indent
                               , _dimple_radius - _dimple_indent
                               , 0, 2*pi );
  G4VSolid* tub = new G4Tubs( "DimpleTub"
                            , 0, _dimple_radius-_dimple_indent
                            , _dimple_indent
                            , 0, 2*pi );

  return new G4UnionSolid( "DimpleSolid", torus, tub );
}

G4VSolid*
LYSimDetectorConstruction::ConstructCylindricalDimpleSolid() const
{
  return new G4Tubs( "DimpleSolid"
                   , 0.0, _dimple_radius
                   , _dimple_indent
                   , 0, 2*pi  );
}

G4VSolid*
LYSimDetectorConstruction::ConstructEllipsoidDimpleSolid() const
{
  return new G4Ellipsoid( "DimpleSolid"
                        , _dimple_radius
                        , _dimple_radius
                        , _dimple_indent );
}

G4ThreeVector
LYSimDetectorConstruction::CalcDimpleOffset() const
{
  switch( _dimple_type ){
  case FLAT_DOME:
    return G4ThreeVector( 0, 0, 0.5* _tilez );
  case CYLINDRICAL:
    return G4ThreeVector( 0, 0, 0.5*_tilez );
  case ELLIPSOID:
    return G4ThreeVector( 0, 0, 0.5*_tilez );
  default:
    return G4ThreeVector( 0, 0,
      0.5*_tilez + GetDimpleSphereRadius() - _dimple_indent );
  }
}

// Additional geometry factors
double
LYSimDetectorConstruction::WorldHalfX() const
{
  return 1*m;
}

double
LYSimDetectorConstruction::WorldHalfY() const
{
  return 1*m;
}

double
LYSimDetectorConstruction::WorldHalfZ() const
{
  return 40*m;
}

double
LYSimDetectorConstruction::LocalTileZ( const double x, const double y ) const
{
  if(x<_tile_x1+_hole_radius && x>_tile_x1-_hole_radius){
     return _tilez - 2*sqrt(pow(_hole_radius,2)-pow(x-_tile_x1,2));
  } else if(x<_tile_x2+_hole_radius && x>_tile_x2-_hole_radius){
    return _tilez - 2*sqrt(pow(_hole_radius,2)-pow(x-_tile_x2,2));
  }else{
    return _tilez;
  }
}


double
LYSimDetectorConstruction::LocalTileZFlatDome( const double r ) const
{
  if( r < _dimple_radius - _dimple_indent ){
    return _tilez - _dimple_indent;
  } else {
    const double dif = r- _dimple_radius + _dimple_indent;
    return _tilez - sqrt( _dimple_indent * _dimple_indent - dif * dif );
  }
}

double
LYSimDetectorConstruction::LocalTileZCylindrical( const double r ) const
{
  return _tilez - _dimple_indent;
}

double
LYSimDetectorConstruction::LocalTileZEllipsoid( const double r ) const
{
  return _tilez - _dimple_indent * sqrt(
    1 - r * r / _dimple_radius / _dimple_radius
    );
}

double
LYSimDetectorConstruction::LocalTileZSpherical( const double r ) const
{
  const double big_r = GetDimpleSphereRadius();
  return _tilez  - ( sqrt( big_r*big_r - r*r ) - ( big_r - _dimple_indent ) );
}


double
LYSimDetectorConstruction::GetDimpleSphereRadius() const
{
  return 0.5 * (
    ( _dimple_radius*_dimple_radius )/_dimple_indent
    + _dimple_indent );
}




void
LYSimDetectorConstruction::SetTileAbsMult( const double mult )
{
  _absmult = mult;
  std::cout<<"========== SetTileAbsMult to "<<mult* mm<<" mm ==================="<<std::endl;
  Set_Flat_AbsLength( fEJ200, _absmult );
}

void
LYSimDetectorConstruction::SetTileScintillation( const double mult )
{
  _ScintiN = mult;
  std::cout<<"========== Update_EJ200_Scinti "<<_ScintiN<<" ==================="<<std::endl;
  Update_EJ200_Scinti( fEJ200, _ScintiN );
}

void
LYSimDetectorConstruction::SetTileDecaytime( const double mult )
{
  _tiledecay = mult;
  std::cout<<"========== Set TileDecaytime "<<_tiledecay<<" ns ==================="<<std::endl;
  Update_EJ200_FASTTIMECONSTANT( fEJ200, _tiledecay );
}

void
LYSimDetectorConstruction::SetTileRisetime( const double mult )
{
  _tilerise = mult;
  std::cout<<"========== Set TileRisetime "<<_tilerise<<" ns ==================="<<std::endl;
  Update_EJ200_RISETIME( fEJ200, _tilerise );
}

void
LYSimDetectorConstruction::SetY11decaytime( const double mult )
{
  _y11_decaytime = mult;
  Update_Y11_timeconstant( mfiber, mult);
  std::cout<<"========== Update_Y11_timeconstant to "<<mult* ns<<" ns ==================="<<std::endl;
}

void
LYSimDetectorConstruction::SetY11attenu( const double mult )
{
  Set_Flat_AbsLength( mfiber, mult );
  std::cout<<"========== SetY11attenu to "<<mult* mm<<" mm ==================="<<std::endl;
  //std::cout<<"========== Fiber abs length is perfect, don't update ==================="<<std::endl;
}

void
LYSimDetectorConstruction::SetGaprefrac_index( const double mult )
{
  std::cout<<"========== SetGaprefrac_index to "<<mult<<" ==================="<<std::endl;
  Update_refrac_index(fResin,mult);
}

void
LYSimDetectorConstruction::SetClad_refrac_index( const double mult1, const double mult2 )
{
  std::cout<<"========== SetClad_refrac_index to "<<mult1<<" "<<mult2<<" ==================="<<std::endl;
  Update_refrac_index(mfiber_clad,mult1);
  Update_refrac_index(mfiber_clad2,mult2);
}

void
LYSimDetectorConstruction::SetWrapReflect( const double r )
{
  // Add entries into properties table
  _wrap_reflect = r;
  static const unsigned nentries = 2;
  static double phoE[nentries]   = {1.0*eV, 6.0*eV};
  double reflectivity[nentries]  = {_wrap_reflect, _wrap_reflect};

  G4MaterialPropertiesTable* table = fESROpSurface->GetMaterialPropertiesTable();
  if( table ){
    table->RemoveProperty( "REFLECTIVITY" );
    table->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
  } else {
    table = new G4MaterialPropertiesTable();
    table->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
    fESROpSurface->SetMaterialPropertiesTable( table );
  }

  //G4MaterialPropertiesTable* table2 = fTiO2Surface->GetMaterialPropertiesTable();
  //if( table2 ){
  //  table2->RemoveProperty( "REFLECTIVITY" );
  //  table2->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
  //} else {
  //  table2 = new G4MaterialPropertiesTable();
  //  table2->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
  //  fTiO2Surface->SetMaterialPropertiesTable( table2 );
  //}

}

void
LYSimDetectorConstruction::SetSiPMReflect( const double r )
{
  // Add entries into properties table
  _sipm_eff = r;
  G4double p_mppc[21] = {2.0*eV, 2.1*eV, 2.2*eV, 2.3*eV, 2.4*eV,
                         2.5*eV, 2.6*eV, 2.7*eV, 2.8*eV, 2.9*eV,
                         3.0*eV, 3.1*eV, 3.2*eV, 3.3*eV, 3.4*eV,
                         3.5*eV, 3.6*eV, 3.7*eV, 3.8*eV, 3.9*eV,
                       15.75*eV};
  G4double effi_mppc[21] = {0.602, 0.697, 0.784, 0.858, 0.933,
                            0.970, 0.990, 1.000, 0.978, 0.958,
                            0.920, 0.871, 0.803, 0.709, 0.622,
                            0.547, 0.398, 0.249, 0.100, 0.000,
                            0.000};
  for(int i=0; i<21; i++) effi_mppc[i]*=_sipm_eff;


  G4MaterialPropertiesTable* table = fSiPMSurface3->GetMaterialPropertiesTable();
  if( table ){
    table->RemoveProperty( "EFFICIENCY" );
    table->AddProperty( "EFFICIENCY",   p_mppc, effi_mppc,21);

  } else {
    table = new G4MaterialPropertiesTable();
    table->AddProperty( "EFFICIENCY",   p_mppc, effi_mppc,21);
    fSiPMSurface3->SetMaterialPropertiesTable( table );
  }

  G4MaterialPropertiesTable* table4 = fSiPMSurface4->GetMaterialPropertiesTable();
  if( table4 ){
    table4->RemoveProperty( "EFFICIENCY" );
    table4->AddProperty( "EFFICIENCY",   p_mppc, effi_mppc,21);

  } else {
    table4 = new G4MaterialPropertiesTable();
    table4->AddProperty( "EFFICIENCY",   p_mppc, effi_mppc,21);
    fSiPMSurface4->SetMaterialPropertiesTable( table4 );
  }

}


void
LYSimDetectorConstruction::SetTileAlpha( const double a )
{
  fTileBulkSurface->SetSigmaAlpha( a );
  _tile_alpha = a;
}

void
LYSimDetectorConstruction::SetDimpleAlpha( const double a )
{
  fTileDimpleSurface->SetSigmaAlpha( a );
  _dimple_alpha = a;
}


