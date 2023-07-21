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

using std::atan;
using std::cos;
using std::exp;
using std::sin;
using std::tan;
using namespace CLHEP;

LYSimPMTSD *LYSimDetectorConstruction::fPMTSD = NULL;

LYSimDetectorConstruction::LYSimDetectorConstruction()
    : G4VUserDetectorConstruction()
{
  fdetectorMessenger = new LYSimDetectorMessenger(this);

  _tilex = 10 * mm;
  _tiley = 10 * mm;
  _tilez = 50 * mm; // 200*mm
  _tile_x1 = 0.0 * mm;
  _tile_x2 = 0.0 * mm;
  wrapgap = 0.1 * mm;
  wrapthickness = 0.1 * mm;

  _absmult = 800;  // mm
  _ScintiN = 100;  // Set scintillation to 10 /keV
  _tiledecay = 5;  // ns // tuning to 11.53 ns overall
  _tilerise = 0.5; // ns
  _wrap_reflect = 0.985;
  _tile_alpha = 0.01;
  _dimple_alpha = 0.1;

  _sipm_deadwidth = 0.2 * mm;
  _sipm_x = 1.4 * mm;
  _sipm_y = 1.4 * mm;
  _sipm_z = 0.4 * mm;
  _sipm_rimwidth = 0.3 * mm;
  _sipm_glasswidth = 0.1 * mm;
  _sipm_standz = 0.3 * mm;
  _sipm_x = 10 * mm;
  _sipm_y = 10 * mm;

  // Default Dimple settings
  _dimple_type = SPHERICAL;
  _dimple_indent = 1.0 * mm;
  _dimple_radius = 6.0 * mm; // 3.4409*mm

  // Default Hole settings
  _pcb_radius = 2.5;
  _sipm_eff = 1;

  // Defining material list.
  fBialkali = Make_Bialkali();
  fEpoxy = Make_Epoxy();
  fAir = Make_Custom_Air();
  fEJ200 = Make_EJ200();
  fResin = Make_Resin();
  SetTileAbsMult(_absmult);
  // SetY11attenu(5000);
  SetTileScintillation(_ScintiN); // N/keV
  SetTileDecaytime(_tiledecay);
  SetTileRisetime(_tilerise);

  // Defining surface list.
  fESROpSurface = MakeS_RoughMirror();
  // fIdealPolishedOpSurface = MakeS_IdealPolished();
  fTileBulkSurface = MakeS_RoughInterface(_tile_alpha);
  // fTileDimpleSurface      = MakeS_RoughInterface( _dimple_alpha );
  // fIdealWhiteOpSurface    = MakeS_IdealWhiteSurface();
  fSiPMSurface3 = MakeS_SiPM();
  fSiPMSurface4 = MakeS_SiPM();
  // fPCBSurface             = MakeS_PCBSurface();

  // wls
  _handwrap = false;
  _cladlayer = 1;
  _holeshape = 2; // 0 circle; 1 square; 2 el
  _WLSfiberR = 0.7 * mm;
  _WLSfiber_clad_thick = 0.05 * mm;
  _WLSfiber_clad2_thick = 0.05 * mm;

  _hole_radius = (_WLSfiberR + _WLSfiber_clad_thick + _WLSfiber_clad2_thick) * 1.01; // 1.0*mm;
  _hole_radius = 3 * mm;                                                             //
  _hole_x1 = 0 * mm;
  _hole_x2 = 13 * mm;

  _WLSfiberZ = 5 * m;
  _WLS_zoff = 1.7 * m;

  _WLSfiberZ = _tilez * 1.5;
  //_WLSfiberZ = 3000;
  _WLS_zoff = 0;
  _WLS_xoff = 0; // 3.25*mm;
  //_claddirt = 0.01;
  _claddirt = 0.0;
  //---------------
  // material
  //---------------
  mfiber  = Make_Y11();
  // mfiber = Make_sgc();
  // mfiber_clad = Make_Pethylene(); //cald 1
  mfiber_clad = Make_acrylic();        // cald 1
  mfiber_clad2 = Make_Fluor_acrylic(); // clad 2

  fcoating = Make_Coating();
  // fholemat = Make_Resin();

  //---------------
  // surface
  //---------------
  fTiO2Surface = MakeS_TiO2Surface(); // Extrusion (TiO2 Coating) Surface
  // opSurface =  MakeS_Polished();
  opSurface = MakeS_IdealPolished();
  // fholemat = Make_Custom_Air();
  SetWrapReflect(_wrap_reflect);
  _y11_decaytime = 11.5; // ns
  SetY11decaytime(_y11_decaytime);
}

void LYSimDetectorConstruction::UpdateGeometry()
{
  // clean-up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();

  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4LogicalSkinSurface::CleanSurfaceTable();
  G4LogicalBorderSurface::CleanSurfaceTable();

  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

LYSimDetectorConstruction::~LYSimDetectorConstruction()
{
  if (fdetectorMessenger)
  {
    delete fdetectorMessenger;
  }
}

G4VPhysicalVolume *
LYSimDetectorConstruction::Construct()
{
  static const bool checkOverlaps = true;

  ///////////////////////////////////////////////////////////////////////////////
  // World volume
  ///////////////////////////////////////////////////////////////////////////////
  G4Box *solidWorld = new G4Box("World", WorldHalfX(), WorldHalfY(), WorldHalfZ());

  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, fAir, "World");

  G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicWorld, "World", 0, false, 0, checkOverlaps);

  
  ///////////////////////////////////////////////////////////////////////////////
  // fiber. single layer clad
  ///////////////////////////////////////////////////////////////////////////////
  assert(_hole_radius >= _WLSfiberR + _WLSfiber_clad_thick + _WLSfiber_clad2_thick);
  G4VSolid *solidWLSfiber = new G4Tubs("WLSFiber", 0., _WLSfiberR, _WLSfiberZ * 0.5, 0., 2 * pi);
  G4VSolid *solidWLSfiber_clad = new G4Tubs("WLSFiber_clad", _WLSfiberR, _WLSfiberR + _WLSfiber_clad_thick, _WLSfiberZ * 0.5, 0., 2 * pi);
  G4LogicalVolume *logicWLSfiber = new G4LogicalVolume(solidWLSfiber, mfiber, "logicWLSfiber");
  G4LogicalVolume *logicWLSfiber_clad = new G4LogicalVolume(solidWLSfiber_clad, mfiber_clad, "logicWLSfiber_clad");

  G4VPhysicalVolume *physWLSfiber = new G4PVPlacement(0, G4ThreeVector(_hole_x1 + _WLS_xoff, 0, -_WLS_zoff), logicWLSfiber, "PhyhWLSfiber_core", logicWorld, false, 0, checkOverlaps);

  G4VPhysicalVolume *physWLSfiber_clad = new G4PVPlacement(0, G4ThreeVector(_hole_x1 + _WLS_xoff, 0, -_WLS_zoff), logicWLSfiber_clad, "PhyhWLSfiber_cald", logicWorld, false, 0, checkOverlaps);

  ///////////////////////////////////////////////////////////////////////////////
  // fiber. second layer clad
  ///////////////////////////////////////////////////////////////////////////////
  G4LogicalVolume *logicWLSfiber_clad2;
  G4VSolid *solidWLSfiber_clad2;
  std::cout << "============  add a second cladding! =================" << std::endl;
  solidWLSfiber_clad2 = new G4Tubs("WLSFiber_clad2", _WLSfiberR + _WLSfiber_clad_thick, _WLSfiberR + _WLSfiber_clad_thick + _WLSfiber_clad2_thick, _WLSfiberZ * 0.5, 0., 2 * pi);
  logicWLSfiber_clad2 = new G4LogicalVolume(solidWLSfiber_clad2, mfiber_clad2, "logicWLSfiber_clad2");

  if (_cladlayer == 2)
  {
    G4VPhysicalVolume *physWLSfiber_clad2 = new G4PVPlacement(0, G4ThreeVector(_hole_x1 + _WLS_xoff, 0, -_WLS_zoff), logicWLSfiber_clad2, "PhyhWLSfiber_cald2", logicWorld, false, 0, checkOverlaps);
  }

  
  ///////////////////////////////////////////////////////////////////////////////
  // Simple version of SiPM
  ///////////////////////////////////////////////////////////////////////////////

  const G4ThreeVector SiPMOffset_chan3(_hole_x1 + _WLS_xoff, 0, _WLSfiberZ * 0.5 - _WLS_zoff + 0.5 * _sipm_z + 0.01 * mm);
  const G4ThreeVector SiPMOffset_chan4(_hole_x1 + _WLS_xoff, 0, -_WLSfiberZ * 0.5 - _WLS_zoff - 0.5 * _sipm_z - 0.01 * mm); // air gap

  // G4Tubs* solidSiPMInnerBox = new G4Tubs( "solidSiPMInnerBox", 0., _WLSfiberR, _sipm_z*0.5, 0., 2*pi);
  G4VSolid *solidSiPMInnerBox = new G4Box("solidSiPMInnerBox", _WLSfiberR, _WLSfiberR, _sipm_z * 0.5);

  G4LogicalVolume *logicSiPM = new G4LogicalVolume(solidSiPMInnerBox, fBialkali, "SiPM");

  G4VPhysicalVolume *physSiPM_chan3 = new G4PVPlacement(0, SiPMOffset_chan3, logicSiPM, "physSiPM_chan3", logicWorld, false, 0, checkOverlaps);
  G4VPhysicalVolume *physSiPM_chan4 = new G4PVPlacement(0, SiPMOffset_chan4, logicSiPM, "physSiPM_chan4", logicWorld, false, 0, checkOverlaps);

  new G4LogicalBorderSurface("SiPMSurface3_out", physWorld, physSiPM_chan3, fSiPMSurface3);
  new G4LogicalBorderSurface("SiPMSurface4_out", physWorld, physSiPM_chan4, fSiPMSurface4);
  new G4LogicalBorderSurface("SiPMSurface3_in", physSiPM_chan3, physWorld, fSiPMSurface3);
  new G4LogicalBorderSurface("SiPMSurface4_in", physSiPM_chan4, physWorld, fSiPMSurface4);

  //    if(_cladlayer==2){
  //        new G4LogicalBorderSurface("SiPMSurface3_out1", physWLSfiber_clad, physSiPM_chan3, fSiPMSurface3);
  //        new G4LogicalBorderSurface("SiPMSurface4_out1", physWLSfiber_clad, physSiPM_chan4, fSiPMSurface4);
  //        new G4LogicalBorderSurface("SiPMSurface3_out2", physWLSfiber_clad2, physSiPM_chan3, fSiPMSurface3);
  //        new G4LogicalBorderSurface("SiPMSurface4_out2", physWLSfiber_clad2, physSiPM_chan4, fSiPMSurface4);
  //    }else{
  //        new G4LogicalBorderSurface("SiPMSurface3_out1", physWLSfiber_clad, physSiPM_chan3, fSiPMSurface3);
  //        new G4LogicalBorderSurface("SiPMSurface4_out1", physWLSfiber_clad, physSiPM_chan4, fSiPMSurface4);
  //    }

  G4OpticalSurface *FiberEndSurface = new G4OpticalSurface("FiberEndSurface",
                                                           unified,
                                                           ground,
                                                           dielectric_dielectric,
                                                           0.02);
  new G4LogicalBorderSurface("FiberEndSurface1", physWLSfiber, physWorld, FiberEndSurface);
  new G4LogicalBorderSurface("FiberEndSurface2", physWorld, physWLSfiber, FiberEndSurface);
  new G4LogicalBorderSurface("FiberEndSurface3", physWLSfiber_clad, physWorld, FiberEndSurface);
  new G4LogicalBorderSurface("FiberEndSurface4", physWorld, physWLSfiber_clad, FiberEndSurface);

  ///////////////////////////////////////////////////////////////////////////////
  // Defining surfaces
  ///////////////////////////////////////////////////////////////////////////////

  
  // Other optical surfaces

  // Setting the sensitive detector
  if (!fPMTSD)
  {
    fPMTSD = new LYSimPMTSD("/LYSimPMT");
    G4SDManager *sdman = G4SDManager::GetSDMpointer();
    sdman->AddNewDetector(fPMTSD);
  }
  logicSiPM->SetSensitiveDetector(fPMTSD);

  G4Colour white(1.00, 1.00, 1.00);   // white
  G4Colour gray(0.50, 0.50, 0.50);    // gray
  G4Colour black(0.00, 0.00, 0.00);   // black
  G4Colour red(1.00, 0.00, 0.00);     // red
  G4Colour green(0.00, 1.00, 0.00);   // green
  G4Colour blue(0.00, 0.00, 1.00);    // blue
  G4Colour cyan(0.00, 1.00, 1.00);    // cyan
  G4Colour air(0.90, 0.94, 1.00);     // cyan
  G4Colour magenta(1.00, 0.00, 1.00); // magenta
  G4Colour yellow(1.00, 1.00, 0.00);  // yellow
  G4Colour brass(0.80, 0.60, 0.40);   // brass
  G4Colour brown(0.70, 0.40, 0.10);   // brown

  // Visual attributes
  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  G4VisAttributes *SiPMVisAtt = new G4VisAttributes(red);
  SiPMVisAtt->SetForceSolid(true);
  SiPMVisAtt->SetVisibility(true);
  // logicSiPM->SetVisAttributes( SiPMVisAtt );
  logicSiPM->SetVisAttributes(G4VisAttributes::Invisible);

  
  
  G4VisAttributes *fiberVisAtt = new G4VisAttributes(G4Colour(1, 0, 0));
  fiberVisAtt->SetForceWireframe(true);
  fiberVisAtt->SetVisibility(true);
  fiberVisAtt->SetForceAuxEdgeVisible(true);
  logicWLSfiber->SetVisAttributes(fiberVisAtt);

  G4VisAttributes *fiberVisAtt2 = new G4VisAttributes(G4Colour(0.5, 0.5, 0));
  fiberVisAtt2->SetForceWireframe(true);
  fiberVisAtt2->SetVisibility(true);
  fiberVisAtt2->SetForceAuxEdgeVisible(true);
  logicWLSfiber_clad->SetVisAttributes(fiberVisAtt2);
  if (_cladlayer == 2)
    logicWLSfiber_clad2->SetVisAttributes(fiberVisAtt2);

  
  G4VisAttributes *PCBVisAtt = new G4VisAttributes(G4Colour(0.0, 0.4, 0.1));
  PCBVisAtt->SetForceSolid(true);
  PCBVisAtt->SetVisibility(true);
  // logicPCB->SetVisAttributes( PCBVisAtt );

  return physWorld;
}

G4VSolid *
LYSimDetectorConstruction::ConstructTrapazoidSolid(
    const G4String &name,
    double x,
    double y,
    double z,
    double indent_x1,
    double indent_x2) const
{
  const G4ThreeVector corners[8] = {
      G4ThreeVector(-x / 2, -y / 2, -z / 2),
      G4ThreeVector(x / 2, -y / 2, -z / 2),
      G4ThreeVector(-x / 2 + indent_x2, y / 2, -z / 2),
      G4ThreeVector(x / 2 - indent_x1, y / 2, -z / 2),
      G4ThreeVector(-x / 2, -y / 2, z / 2),
      G4ThreeVector(x / 2, -y / 2, z / 2),
      G4ThreeVector(-x / 2 + indent_x2, y / 2, z / 2),
      G4ThreeVector(x / 2 - indent_x1, y / 2, z / 2)};

  return new G4Trap(name, corners);
}

G4VSolid *
LYSimDetectorConstruction::ConstructHollowWrapSolid() const
{
  G4VSolid *wrapOuter = ConstructTrapazoidSolid("WrapOuter", _tilex + 2 * wrapgap + 2 * wrapthickness, _tiley + 2 * wrapgap + 2 * wrapthickness, _tilez + 2 * wrapgap + 2 * wrapthickness, 0, 0);
  G4VSolid *wrapInner = ConstructTrapazoidSolid("WrapInner", _tilex + 2 * wrapgap, _tiley + 2 * wrapgap, _tilez + 2 * wrapgap, 0, 0);
  G4VSolid *wrapbox = new G4SubtractionSolid("WrapBox", wrapOuter, wrapInner);

  return wrapbox;
}

G4VSolid *
LYSimDetectorConstruction::ConstructHollowWrapCladSolid() const
{
  G4VSolid *wrapOuter = ConstructTrapazoidSolid("WrapOuter", _tilex + 2 * wrapthickness, _tiley + 2 * wrapthickness, _tilez, 0, 0);
  G4VSolid *wrapInner = ConstructTrapazoidSolid("WrapInner", _tilex, _tiley, _tilez * 1.5, 0, 0);
  G4VSolid *wrapbox = new G4SubtractionSolid("WrapBox", wrapOuter, wrapInner);
  return wrapbox;
}

G4VSolid *
LYSimDetectorConstruction::ConstructDimpleSubtract() const
{
  switch (_dimple_type)
  {
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

G4VSolid *
LYSimDetectorConstruction::ConstructSphereDimpleSolid() const
{
  return new G4Orb("DimpleSolid", GetDimpleSphereRadius());
}

G4VSolid *
LYSimDetectorConstruction::ConstructFlatDomeDimpleSolid() const
{
  G4VSolid *torus = new G4Torus("DimpleTorus", 0, _dimple_indent, _dimple_radius - _dimple_indent, 0, 2 * pi);
  G4VSolid *tub = new G4Tubs("DimpleTub", 0, _dimple_radius - _dimple_indent, _dimple_indent, 0, 2 * pi);

  return new G4UnionSolid("DimpleSolid", torus, tub);
}

G4VSolid *
LYSimDetectorConstruction::ConstructCylindricalDimpleSolid() const
{
  return new G4Tubs("DimpleSolid", 0.0, _dimple_radius, _dimple_indent, 0, 2 * pi);
}

G4VSolid *
LYSimDetectorConstruction::ConstructEllipsoidDimpleSolid() const
{
  return new G4Ellipsoid("DimpleSolid", _dimple_radius, _dimple_radius, _dimple_indent);
}

G4ThreeVector
LYSimDetectorConstruction::CalcDimpleOffset() const
{
  switch (_dimple_type)
  {
  case FLAT_DOME:
    return G4ThreeVector(0, 0, 0.5 * _tilez);
  case CYLINDRICAL:
    return G4ThreeVector(0, 0, 0.5 * _tilez);
  case ELLIPSOID:
    return G4ThreeVector(0, 0, 0.5 * _tilez);
  default:
    return G4ThreeVector(0, 0,
                         0.5 * _tilez + GetDimpleSphereRadius() - _dimple_indent);
  }
}

// Additional geometry factors
double
LYSimDetectorConstruction::WorldHalfX() const
{
  return 1 * m;
}

double
LYSimDetectorConstruction::WorldHalfY() const
{
  return 1 * m;
}

double
LYSimDetectorConstruction::WorldHalfZ() const
{
  return 40 * m;
}

double
LYSimDetectorConstruction::LocalTileZ(const double x, const double y) const
{
  if (x < _tile_x1 + _hole_radius && x > _tile_x1 - _hole_radius)
  {
    return _tilez - 2 * sqrt(pow(_hole_radius, 2) - pow(x - _tile_x1, 2));
  }
  else if (x < _tile_x2 + _hole_radius && x > _tile_x2 - _hole_radius)
  {
    return _tilez - 2 * sqrt(pow(_hole_radius, 2) - pow(x - _tile_x2, 2));
  }
  else
  {
    return _tilez;
  }
}

double
LYSimDetectorConstruction::LocalTileZFlatDome(const double r) const
{
  if (r < _dimple_radius - _dimple_indent)
  {
    return _tilez - _dimple_indent;
  }
  else
  {
    const double dif = r - _dimple_radius + _dimple_indent;
    return _tilez - sqrt(_dimple_indent * _dimple_indent - dif * dif);
  }
}

double
LYSimDetectorConstruction::LocalTileZCylindrical(const double r) const
{
  return _tilez - _dimple_indent;
}

double
LYSimDetectorConstruction::LocalTileZEllipsoid(const double r) const
{
  return _tilez - _dimple_indent * sqrt(
                                       1 - r * r / _dimple_radius / _dimple_radius);
}

double
LYSimDetectorConstruction::LocalTileZSpherical(const double r) const
{
  const double big_r = GetDimpleSphereRadius();
  return _tilez - (sqrt(big_r * big_r - r * r) - (big_r - _dimple_indent));
}

double
LYSimDetectorConstruction::GetDimpleSphereRadius() const
{
  return 0.5 * ((_dimple_radius * _dimple_radius) / _dimple_indent + _dimple_indent);
}

void LYSimDetectorConstruction::SetTileAbsMult(const double mult)
{
  _absmult = mult;
  std::cout << "========== SetTileAbsMult to " << mult * mm << " mm ===================" << std::endl;
  Update_EJ200_AbsLength(fEJ200, _absmult);
}

void LYSimDetectorConstruction::SetTileScintillation(const double mult)
{
  _ScintiN = mult;
  std::cout << "========== Update_EJ200_Scinti " << _ScintiN << " ===================" << std::endl;
  Update_EJ200_Scinti(fEJ200, _ScintiN);
}

void LYSimDetectorConstruction::SetTileDecaytime(const double mult)
{
  _tiledecay = mult;
  std::cout << "========== Set TileDecaytime " << _tiledecay << " ns ===================" << std::endl;
  Update_EJ200_FASTTIMECONSTANT(fEJ200, _tiledecay);
}

void LYSimDetectorConstruction::SetTileRisetime(const double mult)
{
  _tilerise = mult;
  std::cout << "========== Set TileRisetime " << _tilerise << " ns ===================" << std::endl;
  Update_EJ200_RISETIME(fEJ200, _tilerise);
}

void LYSimDetectorConstruction::SetY11decaytime(const double mult)
{
  _y11_decaytime = mult;
  Update_Y11_timeconstant(mfiber, mult);
  std::cout << "========== Update_Y11_timeconstant to " << mult * ns << " ns ===================" << std::endl;
}

void LYSimDetectorConstruction::SetY11attenu(const double mult)
{
  Update_EJ200_AbsLength(mfiber, mult);
  std::cout << "========== SetY11attenu to " << mult * mm << " mm ===================" << std::endl;
  // std::cout<<"========== Fiber abs length is perfect, don't update ==================="<<std::endl;
}

void LYSimDetectorConstruction::SetGaprefrac_index(const double mult)
{
  std::cout << "========== SetGaprefrac_index to " << mult << " ===================" << std::endl;
  Update_refrac_index(fResin, mult);
}

void LYSimDetectorConstruction::SetClad_refrac_index(const double mult1, const double mult2)
{
  std::cout << "========== SetClad_refrac_index to " << mult1 << " " << mult2 << " ===================" << std::endl;
  Update_refrac_index(mfiber_clad, mult1);
  Update_refrac_index(mfiber_clad2, mult2);
}

void LYSimDetectorConstruction::SetWrapReflect(const double r)
{
  // Add entries into properties table
  _wrap_reflect = r;
  static const unsigned nentries = 2;
  static double phoE[nentries] = {1.0 * eV, 6.0 * eV};
  double reflectivity[nentries] = {_wrap_reflect, _wrap_reflect};

  G4MaterialPropertiesTable *table = fESROpSurface->GetMaterialPropertiesTable();
  if (table)
  {
    table->RemoveProperty("REFLECTIVITY");
    table->AddProperty("REFLECTIVITY", phoE, reflectivity, nentries);
  }
  else
  {
    table = new G4MaterialPropertiesTable();
    table->AddProperty("REFLECTIVITY", phoE, reflectivity, nentries);
    fESROpSurface->SetMaterialPropertiesTable(table);
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

void LYSimDetectorConstruction::SetSiPMReflect(const double r)
{
  // Add entries into properties table
  _sipm_eff = r;
  G4double p_mppc[62] = {1.38 * eV,
                         1.39 * eV,
                         1.43 * eV,
                         1.43 * eV,
                         1.47 * eV,
                         1.48 * eV,
                         1.51 * eV,
                         1.53 * eV,
                         1.55 * eV,
                         1.59 * eV,
                         1.64 * eV,
                         1.68 * eV,
                         1.69 * eV,
                         1.72 * eV,
                         1.77 * eV,
                         1.78 * eV,
                         1.87 * eV,
                         1.92 * eV,
                         1.95 * eV,
                         1.97 * eV,
                         2.00 * eV,
                         2.12 * eV,
                         2.18 * eV,
                         2.23 * eV,
                         2.23 * eV,
                         2.29 * eV,
                         2.30 * eV,
                         2.37 * eV,
                         2.37 * eV,
                         2.39 * eV,
                         2.47 * eV,
                         2.47 * eV,
                         2.55 * eV,
                         2.62 * eV,
                         2.71 * eV,
                         2.80 * eV,
                         2.80 * eV,
                         2.94 * eV,
                         2.96 * eV,
                         2.99 * eV,
                         3.09 * eV,
                         3.17 * eV,
                         3.24 * eV,
                         3.30 * eV,
                         3.31 * eV,
                         3.36 * eV,
                         3.43 * eV,
                         3.44 * eV,
                         3.50 * eV,
                         3.53 * eV,
                         3.57 * eV,
                         3.58 * eV,
                         3.58 * eV,
                         3.64 * eV,
                         3.64 * eV,
                         3.65 * eV,
                         3.70 * eV,
                         3.72 * eV,
                         3.80 * eV,
                         3.82 * eV,
                         3.87 * eV,
                         3.91 * eV};
  G4double effi_mppc[62] = {0.03,
                            0.04,
                            0.05,
                            0.05,
                            0.06,
                            0.06,
                            0.07,
                            0.08,
                            0.08,
                            0.09,
                            0.11,
                            0.12,
                            0.12,
                            0.14,
                            0.15,
                            0.15,
                            0.19,
                            0.21,
                            0.22,
                            0.23,
                            0.24,
                            0.28,
                            0.30,
                            0.32,
                            0.32,
                            0.34,
                            0.34,
                            0.37,
                            0.36,
                            0.37,
                            0.39,
                            0.39,
                            0.39,
                            0.40,
                            0.40,
                            0.40,
                            0.39,
                            0.38,
                            0.38,
                            0.37,
                            0.35,
                            0.34,
                            0.31,
                            0.29,
                            0.30,
                            0.27,
                            0.25,
                            0.25,
                            0.22,
                            0.20,
                            0.21,
                            0.18,
                            0.18,
                            0.16,
                            0.14,
                            0.12,
                            0.12,
                            0.09,
                            0.05,
                            0.04,
                            0.02,
                            0.02};
  for (int i = 0; i < 62; i++)
    effi_mppc[i] *= _sipm_eff;

  G4MaterialPropertiesTable *table = fSiPMSurface3->GetMaterialPropertiesTable();
  if (table)
  {
    table->RemoveProperty("EFFICIENCY");
    table->AddProperty("EFFICIENCY", p_mppc, effi_mppc, 62);
  }
  else
  {
    table = new G4MaterialPropertiesTable();
    table->AddProperty("EFFICIENCY", p_mppc, effi_mppc, 62);
    fSiPMSurface3->SetMaterialPropertiesTable(table);
  }

  G4MaterialPropertiesTable *table4 = fSiPMSurface4->GetMaterialPropertiesTable();
  if (table4)
  {
    table4->RemoveProperty("EFFICIENCY");
    table4->AddProperty("EFFICIENCY", p_mppc, effi_mppc, 62);
  }
  else
  {
    table4 = new G4MaterialPropertiesTable();
    table4->AddProperty("EFFICIENCY", p_mppc, effi_mppc, 62);
    fSiPMSurface4->SetMaterialPropertiesTable(table4);
  }
}

void LYSimDetectorConstruction::SetTileAlpha(const double a)
{
  fTileBulkSurface->SetSigmaAlpha(a);
  _tile_alpha = a;
}

void LYSimDetectorConstruction::SetDimpleAlpha(const double a)
{
  fTileDimpleSurface->SetSigmaAlpha(a);
  _dimple_alpha = a;
}
