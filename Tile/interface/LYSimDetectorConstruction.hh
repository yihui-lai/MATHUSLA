#ifndef LYSimDetectorConstruction_h
#define LYSimDetectorConstruction_h 1

#ifdef CMSSW_GIT_HASH
#include "MATHUSLA/Tile/interface/LYSimPMTSD.hh"
#else
#include "LYSimPMTSD.hh"
#endif

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class LYSimDetectorMessenger;
class G4Box;
class G4LogicalVolume;
class G4Material;
class G4OpticalSurface;
class LYSimPMTSD;

class LYSimDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  LYSimDetectorConstruction();
  virtual ~LYSimDetectorConstruction();
  virtual G4VPhysicalVolume* Construct();
  void                       UpdateGeometry();

  // Get/Set functions of tile geometry parameters
  inline void
  SetTileZ ( const G4double x ){ _tilez = x*CLHEP::mm;}
  inline G4double
  GetTileZ() const {return _tilez;}
  inline void
  SetTileX ( const G4double x ){_tilex = x*CLHEP::mm;}
  inline G4double
  GetTileX() const {return _tilex;}
  inline void
  SetTileY ( const G4double y ){_tiley = y*CLHEP::mm;}
  inline G4double
  GetTileY() const {return _tiley;}

  // Get/Set functions of dimple geometry parameters
  
//  inline double
//  GetDimpleRadius() const { return _dimple_radius; }
//  inline void
//  SetDimpleRadius( const double x ){ _dimple_radius = x; }
//  inline double
//  GetDimpleIndent() const { return _dimple_indent; }
//  inline void
//  SetDimpleIndent( const double x ){ _dimple_indent = x; }
//  int
//  GetDimpleType() const {return _dimple_type; }
//  inline void
//  SetDimpleType( const int x ){ _dimple_type = x; }
//  enum  DIMPLE_TYPE
//  {
//    SPHERICAL,
//    ELLIPSOID,
//    FLAT_DOME,
//    CYLINDRICAL
//  };

  // Additional geometric factors.
  double WorldHalfX() const;
  double WorldHalfY() const;
  double WorldHalfZ() const;

  double LocalTileZ( const double x, const double y ) const;

  // Material updating functions
  void SetTileAbsMult( const double x );
  void SetTileScintillation( const double x );
  void SetTileRisetime(const double x);
  void SetTileDecaytime(const double x);
  inline double GetTileAbsMult() const { return _tile_abs_multi_factor; }
  double GetTileScintillation() const { return _ScintiN; }
  void SetFiberDecayTime( const double x );
  double GetFiberDecayTime() const { return _fiber_decaytime; }
  void SetFiberAttenu( const double x );

  void SetGaprefrac_index( const double x );
  void SetClad_refrac_index( const double x1, const double x2 );
  void SetWrapReflect( const double x );
  inline double GetWrapReflect() const { return _wrap_reflect; }

  void SetTileAlpha( const double x );
  inline double GetTileAlpha(){ return _tile_alpha; }

  void SetSiPMReflect( const double x );
  inline double GetSiPMReflect() const { return _sipm_eff; }

  // Accesing Custom Materials
  inline G4Material* GetEJ200(){ return fEJ200; }
  inline G4OpticalSurface* GetSiPMSurface(){ return fSiPMSurface3; }

  //wls
  void Set_wrapper_type( const int x ){ _wrapper_type = x; }
  inline int
  Get_wrapper_type() const { return _wrapper_type; }
  void
  SetHoleRadius( const double x ){ _hole_radius = x*CLHEP::mm; }
  inline double
  GetHoleRadius() const { return _hole_radius; }
  void
  SetHoleX1( const double x ){ _hole_x1 = x; }
  inline double
  GetHoleX1() const { return _hole_x1; }
  void
  SetHoleX2( const double x ){ _hole_x2 = x; }
  inline double
  GetHoleX2() const { return _hole_x2; }
  void
  SetFiberR( const double x ){ 
      _WLSfiberR = x*CLHEP::mm; 
      _WLSfiber_clad_thick = 0.04*_WLSfiberR;
      _WLSfiber_clad2_thick = 0.04*_WLSfiberR ;
  }
  inline double
  GetFiberR() const { return _WLSfiberR; }
  void SetFiberClad(const unsigned x){ _cladlayer = x; } 
  inline unsigned GetFibeClad() const { return _cladlayer; }
  void SetHoleShape(const unsigned x){ _holeshape = x; }

  void
  SetFiber_clad_thick( const double x ){ _WLSfiber_clad_thick = x; }
  inline double
  GetFiber_clad_thick() const { return _WLSfiber_clad_thick; }

  void Setcladdirt(const double x){_claddirt=x;}

  void
  SetFiberZ( const double x ){ _WLSfiberZ = x*CLHEP::m; }
  inline double
  GetFiberZ() const { return _WLSfiberZ; }
  void
  SetFiberZoff( const double x ){ _WLS_zoff = x*CLHEP::m; }
  void
  SetFiberXoff( const double x ){ _WLS_xoff = x*CLHEP::mm; }
  inline double
  GetFiberZoff() const { return _WLS_zoff; }


private:
  // Subfunctions for cleaner code
  G4VSolid* ConstructTrapazoidSolid(
    const G4String& name,
    double          x,// Maximum Length along z direction.
    double          y,// Length along y direction
    double          z,// length along z direction
    double          indent_x1,// Indent in x direction
    double          indent_x2// indent of corner
    ) const;

  G4VSolid* ConstructHollowWrapSolid() const;
  G4VSolid* ConstructHollowWrapCladSolid() const;

//  G4VSolid* ConstructDimpleSubtract() const;
//
//  G4VSolid* ConstructSphereDimpleSolid() const;
//  G4VSolid* ConstructFlatDomeDimpleSolid() const;
//  G4VSolid* ConstructCylindricalDimpleSolid() const;
//  G4VSolid* ConstructEllipsoidDimpleSolid() const;

//  G4ThreeVector CalcDimpleOffset() const;
//  double        LocalTileZSpherical( const double r ) const;
//  double        LocalTileZFlatDome( const double r ) const;
//  double        LocalTileZCylindrical( const double r ) const;
//  double        LocalTileZEllipsoid( const double r ) const;

//  double GetDimpleSphereRadius() const;

  // Pointer to detector messenger class
  LYSimDetectorMessenger* fdetectorMessenger;

  // Pointers to materials
  G4Material* fAir;
  G4Material* fEJ200;
  G4Material* fBialkali;
  G4Material* fEpoxy;
  G4Material* fResin;

  // Pointers to surfaces
  G4OpticalSurface* fESROpSurface;
  G4OpticalSurface* fIdealPolishedOpSurface;
  G4OpticalSurface* fTileBulkSurface;
//  G4OpticalSurface* fTileDimpleSurface;
  G4OpticalSurface* fIdealWhiteOpSurface;
  G4OpticalSurface* fSiPMSurface3;
  G4OpticalSurface* fSiPMSurface4;
  G4OpticalSurface* fPCBSurface;
  G4OpticalSurface* fwrapperendSurface;

  // Pointers for access to Sensitive Detector
  static LYSimPMTSD* fPMTSD;

  // Tile Geometric definitions
  double _tilex;
  double _tiley;
  double _tilez;
  double _tile_x1;
  double _tile_x2;
  double _tile_abs_multi_factor;
  double _tilerise;
  double _tiledecay;

  int _holeshape;
  double _hole_radius;
  double _hole_x1;
  double _hole_x2;

  // Wrapping Geometry definitions
  int _wrapper_type;
  double _wrapgap;// distance between tile and wrapping
  double _wrapthickness;

  // Absorption length multiplier
  double _wrap_reflect;
  double _tile_alpha;

  // Dimple variables;
//  double _dimple_indent;// Depth of dimple
//  double _dimple_radius;// Dimple radius
//  int _dimple_type;

  // Parameters for plate behind SiPM
  double _sipm_eff;
  double _sipm_z;

  //wls
  double _WLSfiberR;
  double _WLSfiberZ;
  double _WLSfiber_clad_thick;
  double _WLSfiber_clad2_thick;
  double _WLS_zoff;
  double _WLS_xoff;
  double _ScintiN;
  double _fiber_decaytime;
  int _cladlayer;
  double _claddirt;
  G4Material* mfiber;
  G4Material* mfiber_clad;
  G4Material* mfiber_clad2;
  G4Material* fcoating;
  G4Material* fholemat;
  G4OpticalSurface* fTiO2Surface;
  G4OpticalSurface* opSurface;
};

#endif
