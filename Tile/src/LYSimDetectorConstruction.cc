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
  _tilez        = 60.0*mm;  //200*mm
  _tile_x1      = 0.0*mm;
  _tile_x2      = 0.0*mm;
  wrapgap       = 0.1*mm;
  wrapthickness = 0.1*mm;

  _absmult      = 80; // mm
  _ScintiN      = 100; //Set scintillation to 10 /keV
  _tiledecay    = 2.5; //ns
  _tilerise     = 0.5; //ns
  _wrap_reflect = 0.985;
  _tile_alpha   = 0.01;
  _dimple_alpha = 0.1;

  _sipm_deadwidth  = 0.2*mm;
  _sipm_x          = 1.4*mm;
  _sipm_y          = 1.4*mm;
  _sipm_z          = 0.4*mm;
  _sipm_rimwidth   = 0.3*mm;
  _sipm_glasswidth = 0.1*mm;
  _sipm_standz     = 0.3*mm;
  _sipm_x          = 10*mm;
  _sipm_y          = 10*mm;


  // Default Dimple settings
  _dimple_type   = SPHERICAL;
  _dimple_indent = 1.0*mm;
  _dimple_radius = 6.0*mm;// 3.4409*mm
  
  // Default Hole settings
  _pcb_radius       = 2.5;
  _sipm_eff = 1;

  // Defining material list.
  fBialkali = Make_Bialkali();
  fEpoxy    = Make_Epoxy();
  fAir      = Make_Custom_Air();
  fEJ200    = Make_EJ200();
  fResin    = Make_Resin();
  SetTileAbsMult( _absmult );
  SetTileScintillation( _ScintiN ); // N/keV
  SetTileDecaytime(_tiledecay);
  SetTileRisetime(_tilerise);

  // Defining surface list.
  fESROpSurface           = MakeS_RoughMirror();
  //fIdealPolishedOpSurface = MakeS_IdealPolished();
  fTileBulkSurface        = MakeS_RoughInterface( _tile_alpha );
  //fTileDimpleSurface      = MakeS_RoughInterface( _dimple_alpha );
  //fIdealWhiteOpSurface    = MakeS_IdealWhiteSurface();
  fSiPMSurface3            = MakeS_SiPM();
  fSiPMSurface4            = MakeS_SiPM();
  //fPCBSurface             = MakeS_PCBSurface();


//wls
_handwrap   = true;
_cladlayer  = 1;
_holeshape = 2; //0 circle; 1 square; 2 el
_WLSfiberR = 0.7*mm;
_WLSfiber_clad_thick = 0.05*mm;
_WLSfiber_clad2_thick = 0.05*mm;

_hole_radius = (_WLSfiberR+_WLSfiber_clad_thick+_WLSfiber_clad2_thick)*1.01; //1.0*mm;
_hole_radius = 1*mm;//
_hole_x1 = -13*mm;
_hole_x2 = 13*mm;

_WLSfiberZ = 5.2*m;
_WLS_zoff = 1.7*m;

_WLSfiberZ = _tilez*1.5;
_WLSfiberZ = 100;
_WLS_zoff = 0;
_WLS_xoff = 0;//3.25*mm;
_claddirt = 0.01;
//---------------
//material
//---------------
//mfiber  = Make_Y11();
mfiber  = Make_sgc();
//mfiber_clad = Make_Pethylene(); //cald 1
mfiber_clad = Make_acrylic(); //cald 1
mfiber_clad2 = Make_Fluor_acrylic(); //clad 2

fcoating = Make_Coating();
//fholemat = Make_Resin();

//---------------
//surface
//---------------
fTiO2Surface = MakeS_TiO2Surface();
//opSurface =  MakeS_Polished();
opSurface =  MakeS_IdealPolished();
//fholemat = Make_Custom_Air();
SetWrapReflect( _wrap_reflect );
_y11_decaytime = 11.5;//ns
SetY11decaytime( _y11_decaytime );

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

  ///////////////////////////////////////////////////////////////////////////////
  // World volume
  ///////////////////////////////////////////////////////////////////////////////
  G4Box* solidWorld = new G4Box( "World", WorldHalfX()
                               , WorldHalfY(), WorldHalfZ() );

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
  // wrapping or clad
  ///////////////////////////////////////////////////////////////////////////////
  G4VSolid* solidHoleBound;
  if(_holeshape==0) solidHoleBound = new G4Tubs( "TileHole", 0, _hole_radius, _tilez, 0, 2*pi  );
  else if(_holeshape==1) solidHoleBound = new G4Box( "TileHole", _hole_radius, _hole_radius, _tilez  );

  G4LogicalVolume* logicWrap;

  if(_handwrap){
  G4VSolid* solidWrap0 = ConstructHollowWrapSolid();
  G4VSolid* solidWrap1 = new G4SubtractionSolid( "solidWrap"
                            , solidWrap0, solidHoleBound
                            , 0, G4ThreeVector( _hole_x1, 0, 0 ) );


//second hole

  G4VSolid* solidWrap = new G4SubtractionSolid( "solidWrap"
                            , solidWrap1, solidHoleBound
                            , 0, G4ThreeVector( _hole_x2, 0, 0 ) );

//---------------------------
  logicWrap = new G4LogicalVolume( solidWrap, fEpoxy,  "Wrap" );


 // logicWrap = new G4LogicalVolume( solidWrap1, fEpoxy,  "Wrap" ); // comment this line when second hole enabled

  G4VPhysicalVolume* physWrap = new G4PVPlacement( 0
                                                 , G4ThreeVector( 0, 0, 0 )
                                                 , logicWrap
                                                 , "Wrap"
                                                 , logicWorld
                                                 , false
                                                 , 0
                                                 , checkOverlaps );
  G4LogicalSkinSurface* WrapSurface =
    new G4LogicalSkinSurface( "WrapSurface"
                              , logicWrap, fESROpSurface );

  }else{
  G4VSolid* solidExtrusion = ConstructHollowWrapCladSolid();

  logicWrap =
    new G4LogicalVolume(solidExtrusion, fcoating, "Extrusion");

  G4VPhysicalVolume* physWrap = new G4PVPlacement( 0
                                                 , G4ThreeVector( 0, 0, 0 )
                                                 , logicWrap
                                                 , "Wrap"
                                                 , logicWorld
                                                 , false
                                                 , 0
                                                 , checkOverlaps );
                                                 
  G4LogicalSkinSurface* WrapSurface =
    new G4LogicalSkinSurface( "WrapSurface"
                              , logicWrap, fTiO2Surface );
  }
  ///////////////////////////////////////////////////////////////////////////////
  // extruded scintillator
  ///////////////////////////////////////////////////////////////////////////////
  G4VSolid* solidTile = ConstructTrapazoidSolid( "TileTrap"
                                               , _tilex
                                               , _tiley
                                               , _tilez
                                               , _tile_x1
                                               , _tile_x2 );
  G4VSolid* tileBulk1
    = new G4SubtractionSolid( "TileSolid_Bulk"
                            , solidTile, solidHoleBound
                            , 0, G4ThreeVector( _hole_x1, 0, 0 ) );

//second hole ----------

  G4VSolid* tileBulk
    = new G4SubtractionSolid( "TileSolid_Bulk"
                            , tileBulk1, solidHoleBound
                            , 0, G4ThreeVector( _hole_x2, 0, 0 ) );

  G4LogicalVolume* logicTileBulk = new G4LogicalVolume( tileBulk
                                                      , fEJ200
                                                      , "TileBulkLogic" );
//----------------
  /*G4LogicalVolume* logicTileBulk = new G4LogicalVolume( tileBulk1
                                                      , fEJ200
                                                      , "TileBulkLogic" ); // comment this line when second hole enabled
*/
  G4VPhysicalVolume* physTileBulk = new G4PVPlacement( 0
                                                     , G4ThreeVector( 0, 0, 0 )
                                                     , logicTileBulk
                                                     , "TileBulkPhysic"
                                                     , logicWorld
                                                     , false
                                                     , 0
                                                     , checkOverlaps );

  ///////////////////////////////////////////////////////////////////////////////
  // fiber. single layer clad
  ///////////////////////////////////////////////////////////////////////////////
  assert(_hole_radius>=_WLSfiberR+_WLSfiber_clad_thick+_WLSfiber_clad2_thick); 
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
  // circle
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


  //G4OpticalSurface* surface_p =MakeS_TiO2Surface();
  //new G4LogicalBorderSurface("surfaceCladin", physWLSfiber_clad, physHole, surface_p); 
  /*new G4LogicalBorderSurface("surfacecladout", physWLSfiber_clad, physHole, opSurface); 
  new G4LogicalBorderSurface("surfacecladin", physHole, physWLSfiber_clad, opSurface); 
  new G4LogicalBorderSurface("surfacecoreout", physWLSfiber_clad, physWLSfiber, opSurface);
  new G4LogicalBorderSurface("surfacecorein", physWLSfiber, physWLSfiber_clad, opSurface);
  */
///////////////////////////////////////////////////////////////////////////////
  // realistic SiPM
  ///////////////////////////////////////////////////////////////////////////////
  
  ///////////////////////////////////////////////////////////////////////////////
  // Simple version of SiPM
  ///////////////////////////////////////////////////////////////////////////////
  const G4ThreeVector SiPMOffset_chan3( _hole_x1+_WLS_xoff, 0, _WLSfiberZ*0.5 - _WLS_zoff + 0.5*_sipm_z);  
  const G4ThreeVector SiPMOffset_chan4( _hole_x1+_WLS_xoff, 0, -_WLSfiberZ*0.5 - _WLS_zoff - 0.5*_sipm_z);
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

        new G4LogicalBorderSurface("SiPMSurface3_out", physWLSfiber, physSiPM_chan3, fSiPMSurface3);
        new G4LogicalBorderSurface("SiPMSurface4_out", physWLSfiber, physSiPM_chan4, fSiPMSurface4);

//    if(_cladlayer==2){
//        new G4LogicalBorderSurface("SiPMSurface3_out1", physWLSfiber_clad, physSiPM_chan3, fSiPMSurface3);
//        new G4LogicalBorderSurface("SiPMSurface4_out1", physWLSfiber_clad, physSiPM_chan4, fSiPMSurface4);
//        new G4LogicalBorderSurface("SiPMSurface3_out2", physWLSfiber_clad2, physSiPM_chan3, fSiPMSurface3);
//        new G4LogicalBorderSurface("SiPMSurface4_out2", physWLSfiber_clad2, physSiPM_chan4, fSiPMSurface4);
//    }else{
//        new G4LogicalBorderSurface("SiPMSurface3_out1", physWLSfiber_clad, physSiPM_chan3, fSiPMSurface3);
//        new G4LogicalBorderSurface("SiPMSurface4_out1", physWLSfiber_clad, physSiPM_chan4, fSiPMSurface4);
//    }

  ///////////////////////////////////////////////////////////////////////////////
  // Defining surfaces
  ///////////////////////////////////////////////////////////////////////////////
  
  // Tile surfaces
  G4LogicalBorderSurface* TileBulkSurface =
    new G4LogicalBorderSurface( "TileBulkSurface"
                              , physTileBulk
                              , physWorld
                              , fTileBulkSurface );

  // Other optical surfaces
  
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
  //logicSiPM->SetVisAttributes( SiPMVisAtt );
  logicSiPM->SetVisAttributes( G4VisAttributes::Invisible );

  G4VisAttributes* ResinVisAtt = new G4VisAttributes( cyan );
  ResinVisAtt->SetForceWireframe( true );
  ResinVisAtt->SetVisibility( true );
  logicHole->SetVisAttributes( ResinVisAtt );

  G4VisAttributes* TileVisAtt = new G4VisAttributes( G4Colour( 0, 0, 1. ) );
  TileVisAtt->SetForceWireframe( true );
  TileVisAtt->SetVisibility( true );
  logicTileBulk->SetVisAttributes( G4VisAttributes::Invisible );

  G4VisAttributes* fiberVisAtt = new G4VisAttributes( G4Colour( 1, 0, 0) );
  fiberVisAtt->SetForceWireframe( true );
  fiberVisAtt->SetVisibility( true );
  fiberVisAtt->SetForceAuxEdgeVisible( true );
  logicWLSfiber->SetVisAttributes( fiberVisAtt );

  G4VisAttributes* fiberVisAtt2 = new G4VisAttributes( G4Colour( 0.5, 0.5, 0  ) );
  fiberVisAtt2->SetForceWireframe( true );
  fiberVisAtt2->SetVisibility( true );
  fiberVisAtt2->SetForceAuxEdgeVisible( true );
  logicWLSfiber_clad->SetVisAttributes( fiberVisAtt2 );
  if(_cladlayer==2) logicWLSfiber_clad2->SetVisAttributes( fiberVisAtt2 );

  G4VisAttributes* WrapVisAtt = new G4VisAttributes( gray );
  WrapVisAtt->SetForceWireframe( true );
  WrapVisAtt->SetVisibility( true );
  logicWrap->SetVisAttributes( WrapVisAtt );

  G4VisAttributes* PCBVisAtt = new G4VisAttributes( G4Colour( 0.0, 0.4, 0.1 ) );
  PCBVisAtt->SetForceSolid( true );
  PCBVisAtt->SetVisibility( true );
  //logicPCB->SetVisAttributes( PCBVisAtt );

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
                             , _tilez + 2*wrapgap
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
                             , _tilez*1.5
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
  Update_EJ200_AbsLength( fEJ200, _absmult );
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
  Update_EJ200_AbsLength( mfiber, mult );
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

  G4MaterialPropertiesTable* table2 = fTiO2Surface->GetMaterialPropertiesTable();
  if( table2 ){
    table2->RemoveProperty( "REFLECTIVITY" );
    table2->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
  } else {
    table2 = new G4MaterialPropertiesTable();
    table2->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
    fTiO2Surface->SetMaterialPropertiesTable( table2 );
  }

}

void
LYSimDetectorConstruction::SetSiPMReflect( const double r )
{
  // Add entries into properties table
  _sipm_eff = r;
  static const unsigned nentries = 2;
  static double phoE[nentries]   = {1.0*eV, 6.0*eV};
  double efficiency[nentries]  = {r, r};
  /*
  const int nentries = 51;
  double phoE[nentries]   = {
    4.456977245*eV, 4.405663883*eV, 4.371335649*eV,  4.337993527*eV,
    4.305156186*eV, 4.272812248*eV, 4.120034327*eV,  4.047934735*eV,
    3.885068561*eV, 3.795976762*eV, 3.710990512*eV,  3.641025285*eV,
    3.562661791*eV, 3.416389546*eV, 3.254178294*eV,  3.200252775*eV,
    3.130913965*eV, 3.072870849*eV, 3.016867241*eV,  2.948003733*eV,
    2.868012792*eV, 2.772393126*eV, 2.622893865*eV,  2.488842802*eV,
    2.467789116*eV, 2.382432947*eV, 2.325721122*eV,  2.276066914*eV,
    2.224250888*eV, 2.174741594*eV, 2.115879533*eV,  2.067333517*eV,
    2.024496146*eV, 1.98339801*eV,  1.943935293*eV,  1.906041585*eV,
    1.869568783*eV, 1.834465614*eV, 1.80065635*eV,   1.76540215*eV,
    1.731477711*eV, 1.696391875*eV, 1.66504429*eV,   1.628029216*eV,
    1.594775069*eV, 1.562852229*eV, 1.546342471*eV,  1.512389673*eV,
    1.477990547*eV, 1.443354983*eV, 1.40859115*eV
  };
  double efficiency[nentries] = {
    0.011045, 0.024287, 0.044945,  0.058187,  0.071429,  0.084672,   0.119633,
    0.132347, 0.155127, 0.167841,  0.180556,  0.19327,   0.205984,   0.216053,
    0.225064, 0.237778, 0.257379,  0.270093,  0.282808,  0.294993,   0.307179,
    0.317778, 0.320435, 0.317796,  0.316738,  0.307211,  0.295563,   0.283914,
    0.271736, 0.260618, 0.246852,  0.235734,  0.224086,  0.212438,   0.20079,
    0.189142, 0.177494, 0.165846,  0.154198,  0.14255,   0.131432,   0.120315,
    0.109197, 0.09755,  0.086962,  0.076375,  0.071611,  0.061554,   0.053086,
    0.044618, 0.036681};
  for(int i=0;i<nentries;i++){
      efficiency[i]*=_sipm_eff;
  }
  const unsigned ref_ent = 32;
  double phoE2[ref_ent]  = {
    4.928613174*eV, 4.760932071*eV, 4.57675131 *eV, 4.416020551*eV,
    4.27782469 *eV, 4.132944198*eV, 3.988297134*eV, 3.870877084*eV,
    3.751866883*eV, 3.647774074*eV, 3.538260695*eV, 3.441997529*eV,
    3.347666946*eV, 3.264460058*eV, 3.176475533*eV, 3.098752668*eV,
    3.019365195*eV, 2.751902006*eV, 2.432636667*eV, 2.254544997*eV,
    2.136148464*eV, 2.032627719*eV, 1.907273067*eV, 1.796976535*eV,
    1.722050515*eV, 1.570692624*eV, 1.511965476*eV, 1.459049532*eV,
    1.377862407*eV, 1.305495288*eV, 1.265726027*eV, 1.240350474*eV
  };
  double reflectivity[ref_ent] = {
    0.67091, 0.70309, 0.73346, 0.72132, 0.68334, 0.62105, 0.59036, 0.57232,
    0.56450, 0.56008, 0.56441, 0.58236, 0.58280, 0.54482, 0.51025, 0.48443,
    0.46541, 0.41855, 0.38169, 0.36570, 0.35705, 0.35096, 0.34397, 0.33772,
    0.33321, 0.32645, 0.32498, 0.32241, 0.31988, 0.31625, 0.31417, 0.31323
  };
  */

  G4MaterialPropertiesTable* table = fSiPMSurface3->GetMaterialPropertiesTable();
  if( table ){
    table->RemoveProperty( "EFFICIENCY" );
    //table->RemoveProperty( "REFLECTIVITY" );
    //table->AddProperty( "EFFICIENCY", phoE, efficiency, nentries );
    table->AddProperty( "EFFICIENCY",   phoE,  efficiency,   nentries );
    //table->AddProperty( "REFLECTIVITY", phoE2, reflectivity, ref_ent  );

  } else {
    table = new G4MaterialPropertiesTable();
    table->AddProperty( "EFFICIENCY", phoE, efficiency, nentries );
    //table->AddProperty( "REFLECTIVITY", phoE2, reflectivity, ref_ent  );
    fSiPMSurface3->SetMaterialPropertiesTable( table );
  }

  G4MaterialPropertiesTable* table4 = fSiPMSurface4->GetMaterialPropertiesTable();
  if( table4 ){
    table4->RemoveProperty( "EFFICIENCY" );
    //table4->RemoveProperty( "REFLECTIVITY" );
    table4->AddProperty( "EFFICIENCY",   phoE,  efficiency,   nentries );
    //table4->AddProperty( "REFLECTIVITY", phoE2, reflectivity, ref_ent  );

  } else {
    table4 = new G4MaterialPropertiesTable();
    table4->AddProperty( "EFFICIENCY", phoE, efficiency, nentries );
    //table4->AddProperty( "REFLECTIVITY", phoE2, reflectivity, ref_ent  );
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


