#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"

using namespace CLHEP;

static const unsigned flatentries = 2;
static const double minenergy     = 1.0*eV;
static const double maxenergy     = 8.0*eV;


G4OpticalSurface*
MakeS_TyvekCrystal()
{
  const unsigned num       = 2;
  double Ephoton[num]      = {1.5*eV, 8.0*eV};
  double Reflectivity[num] = {0.979, 0.979};

  //////////////////////////////////
  // Realistic Crystal-Tyvek surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "TyvekOpSurface" );
  surface->SetType( dielectric_LUT );
  surface->SetModel( LUT );
  surface->SetFinish( polishedtyvekair );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty( "REFLECTIVITY", Ephoton, Reflectivity, num );

  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_ESR()
{
  const unsigned num       = 2;
  double Ephoton[num]      = {1.5*eV, 8.0*eV};
  double Reflectivity[num] = {0.985, 0.985};
  // source: https://www.osti.gov/servlets/purl/1184400
  //////////////////////////////////
  // ESR surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "ESROpSurface" );
  surface->SetType( dielectric_LUT );
  surface->SetModel( LUT );
  surface->SetFinish( polishedvm2000air );


  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty( "REFLECTIVITY", Ephoton, Reflectivity, num );

  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_IdealTyvekCrystal()
{
  //////////////////////////////////
  // Ideal Crystal-Tyvek surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "IdealTyvekOpSurface" );
  surface->SetType( dielectric_LUT );
  surface->SetModel( LUT );
  surface->SetFinish( polishedtyvekair );

  return surface;
}

G4OpticalSurface*
MakeS_UnifiedTyvek()
{
  const unsigned nentries        = 2;
  double phoE[nentries]          = {2.0*eV, 6.0*eV};
  double specularlobe[nentries]  = {1.0, 1.0};
  double specularspike[nentries] = {0., 0.};
  double backscatter[nentries]   = {0., 0.};
  double rindex[nentries]        = {1.0, 1.0};
  double reflectivity[nentries]  = {0.979, 0.979};
  double efficiency[nentries]    = {0.0, 0.0};

  //////////////////////////////////
  // Unified Tyvek surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "UnifiedTyvekOpSurface" );
  surface->SetType( dielectric_dielectric );
  surface->SetModel( unified );
  surface->SetFinish( groundbackpainted );
  surface->SetSigmaAlpha( 1.3*degree );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty( "RINDEX",                phoE, rindex,        nentries );
  table->AddProperty( "SPECULARLOBECONSTANT",  phoE, specularlobe,  nentries );
  table->AddProperty( "SPECULARSPIKECONSTANT", phoE, specularspike, nentries );
  table->AddProperty( "BACKSCATTERCONSTANT",   phoE, backscatter,   nentries );
  table->AddProperty( "REFLECTIVITY",          phoE, reflectivity,  nentries );
  table->AddProperty( "EFFICIENCY",            phoE, efficiency,    nentries );

  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_IdealTyvek()
{
  const unsigned nentries        = 2;
  double phoE[nentries]          = {2.0*eV, 6.0*eV};
  double specularlobe[nentries]  = {1.0, 1.0};
  double specularspike[nentries] = {0., 0.};
  double backscatter[nentries]   = {0., 0.};
  double rindex[nentries]        = {1.0, 1.0};
  double reflectivity[nentries]  = {1.0, 1.0};
  double efficiency[nentries]    = {0.0, 0.0};

  //////////////////////////////////
  // Unified Ideal Tyvek surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface(
    "UnifiedIdealTyvekOpSurface" );
  surface->SetType( dielectric_dielectric );
  surface->SetModel( unified );
  surface->SetFinish( groundbackpainted );
  surface->SetSigmaAlpha( 1.3*degree );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty( "RINDEX",                phoE, rindex,        nentries );
  table->AddProperty( "SPECULARLOBECONSTANT",  phoE, specularlobe,  nentries );
  table->AddProperty( "SPECULARSPIKECONSTANT", phoE, specularspike, nentries );
  table->AddProperty( "BACKSCATTERCONSTANT",   phoE, backscatter,   nentries );
  table->AddProperty( "REFLECTIVITY",          phoE, reflectivity,  nentries );
  table->AddProperty( "EFFICIENCY",            phoE, efficiency,    nentries );

  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_Polished()
{
  static const unsigned nentries       = 2;
  static double phoE[nentries]         = {1.0*eV, 6.0*eV};
  static double specularlobe[nentries] = {1.0, 1.0};

  //////////////////////////////////
  // Realistic polished surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "PolishedOpSurface" );
  surface->SetType( dielectric_dielectric );
  surface->SetModel( unified );
  surface->SetFinish( ground );
  // necessary even for polished surfaces to enable UNIFIED code
  surface->SetSigmaAlpha( 1.3 * degree );// Janecek2010 (1.3 * degree)

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty(
    "SPECULARLOBECONSTANT", phoE, specularlobe, nentries );
  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_IdealPolished()
{
  //Set the Surface Roughness between Cladding 1 and WLS fiber
  // Pre: 0 < roughness <= 1
  G4OpticalSurface* surface = new G4OpticalSurface("IdealPolished", glisur, ground,
                                     dielectric_dielectric, 1);

  return surface;
}

G4OpticalSurface*
MakeS_Mirror()
{
  const unsigned nentries       = 2;
  double phoE[nentries]         = {1.0*eV, 6.0*eV};
  double reflectivity[nentries] = {1.0, 1.0};
  //////////////////////////////////
  // Mirror surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "MirrorOpSurface" );
  surface->SetType( dielectric_metal );
  surface->SetFinish( polished );
  surface->SetModel( unified );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_IdealMirror()
{
  const unsigned nentries       = 2;
  double phoE[nentries]         = {1.0*eV, 6.0*eV};
  double reflectivity[nentries] = {1.0, 1.0};
  //////////////////////////////////
  // Ideal mirror surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "MirrorOpSurface" );
  surface->SetType( dielectric_metal );
  surface->SetFinish( polished );
  surface->SetModel( unified );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_IdealWhiteSurface()
{
  //////////////////////////////////
  // Ideal mirror surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "WhiteOpSurface" );
  surface->SetType( dielectric_metal );
  surface->SetFinish( ground );
  surface->SetModel( unified );

  double phoE[flatentries]         = {minenergy, maxenergy};
  double reflectivity[flatentries] = {0.80, 0.80};

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "REFLECTIVITY", phoE, reflectivity, flatentries );
  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_Absorbing()
{
  const unsigned nentries       = 2;
  double phoE[nentries]         = {1.0*eV, 6.0*eV};
  double reflectivity[nentries] = {0.0, 0.0};
  //////////////////////////////////
  // Absorbing surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "AbsorbingOpSurface" );
  surface->SetType( dielectric_dielectric );
  surface->SetFinish( groundfrontpainted );
  surface->SetModel( unified );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_RoughInterface( const double alpha )
{
  char name[1024];
  sprintf( name, "RoughInterfaceOpSurface%.lf", alpha );

  //////////////////////////////////
  // Rough surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( name );
  surface->SetType( dielectric_dielectric );
  surface->SetModel( unified );
  surface->SetFinish( ground );
  surface->SetSigmaAlpha( alpha );

  return surface;
}

G4OpticalSurface*
MakeS_RoughMirror()
{
  //////////////////////////////////
  // Rough surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "RoughMirrorOpSurface" );
  surface->SetType( dielectric_metal );
  surface->SetModel( unified );
  surface->SetFinish( ground );

  return surface;
}

G4OpticalSurface*
MakeS_SiPM()
{
  // From Meeting on HB SiPM selection (James Hirschauer)
  G4double p_mppc[21] = {2.0*eV, 2.1*eV, 2.2*eV, 2.3*eV, 2.4*eV, 
                         2.5*eV, 2.6*eV, 2.7*eV, 2.8*eV, 2.9*eV, 
                         3.0*eV, 3.1*eV, 3.2*eV, 3.3*eV, 3.4*eV, 
                         3.5*eV, 3.6*eV, 3.7*eV, 3.8*eV, 3.9*eV,
                       15.75*eV};
  G4double refl_mppc[21];
  for(int i=0; i<21; i++) refl_mppc[i]=0.2;

  G4OpticalSurface* surface = new G4OpticalSurface( "SiPM_Surface",glisur,ground,dielectric_metal,1.0 );
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  G4double p_perfectSIPM[] = { 1.0 * eV, 6.0 * eV };
  G4double effi_perfectSIPM[] = { 0.5, 0.5 };

  table -> AddProperty("REFLECTIVITY",p_mppc,refl_mppc,21);
  table->AddProperty("EFFICIENCY", p_perfectSIPM, effi_perfectSIPM,2);

  surface->SetMaterialPropertiesTable( table );
  return surface;
}

G4OpticalSurface*
MakeS_PCBSurface()
{
  // PCB is a flat gray surface for now
  double phoE2[flatentries]        = {minenergy, maxenergy };
  double reflectivity[flatentries] = {1, 1};

  G4OpticalSurface* surface = new G4OpticalSurface( "PCB_Surface" );
  surface->SetType( dielectric_metal );
  surface->SetFinish( ground );
  surface->SetModel( unified );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty( "REFLECTIVITY", phoE2, reflectivity, flatentries );
  surface->SetMaterialPropertiesTable( table );
  return surface;
}



//wls
G4OpticalSurface*
MakeS_TiO2Surface()
{
  //Coating clad

  G4OpticalSurface* TiO2Surface = new G4OpticalSurface("TiO2Surface",unified,
                                                       ground,dielectric_metal,1.5);
  G4MaterialPropertiesTable* TiO2SurfaceProperty = new G4MaterialPropertiesTable();

  G4double p_TiO2[11] =    {2.00*eV, 2.75*eV, 2.88*eV, 2.95*eV, 3.02*eV, 3.10*eV, 3.18*eV, 3.26*eV, 3.35*eV, 3.44*eV, 15.75*eV};
  G4double refl_TiO2[11] = {0.91,    0.91,    0.90,    0.85,    0.69,    0.44,    0.27,    0.13,    0.08,    0.07,    0.07}; //assume a constant value for energies > 3.44eV (most photons with these energies get absorbed and wave length shifted)
  G4double effi_TiO2[11] = {0,       0 ,      0,       0,       0,       0,       0,       0,       0,       0,       0};
  for(int i=0; i<11; i++) refl_TiO2[i]=1.0-0.75*(1.0-refl_TiO2[i]);  //a higher reflectivities comparared to the numbers given by Anna 
  TiO2SurfaceProperty->AddProperty("REFLECTIVITY",p_TiO2,refl_TiO2,11);
  TiO2SurfaceProperty->AddProperty("EFFICIENCY",p_TiO2,effi_TiO2,11);

  G4double pp[2] = {2.00*eV, 15.75*eV};
  G4double specularlobe[2] = {1.0, 1.0};
  G4double specularspike[2] = {0.0, 0.0};
  G4double backscatter[2] = {0.0, 0.0};
  TiO2SurfaceProperty->AddProperty("SPECULARLOBECONSTANT",pp,specularlobe,2);
  TiO2SurfaceProperty->AddProperty("SPECULARSPIKECONSTANT",pp,specularspike,2);
  TiO2SurfaceProperty->AddProperty("BACKSCATTERCONSTANT",pp,backscatter,2);
  TiO2Surface -> SetMaterialPropertiesTable(TiO2SurfaceProperty);
  return TiO2Surface;

}


