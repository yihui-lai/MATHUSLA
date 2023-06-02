#ifndef MATERIALS_HH
#define MATERIAL_HH

#include "G4Material.hh"

// list of functions for constructing non-standard material in Geant4
extern G4Material* Make_Bialkali();
extern G4Material* Make_Epoxy();
extern G4Material* Make_EJ200();
extern G4Material* Make_Resin();
extern void        Set_Flat_AbsLength( G4Material*, const double x );
extern void        Update_EJ200_Scinti( G4Material*, const double x );
extern void        Update_EJ200_FASTTIMECONSTANT( G4Material*, const double x );
extern void        Update_EJ200_RISETIME( G4Material*, const double x );
extern void        Update_Fiber_timeconstant( G4Material*, const double x );
extern void        Update_refrac_index( G4Material*, const double x );
extern G4Material* Make_Custom_Air();

//wls
extern G4Material* Make_sgc();
extern G4Material* Make_acrylic();
extern G4Material* Make_Fluor_acrylic();

//extern G4Material* Make_PMMA();
extern G4Material* Make_Y11();
extern G4Material* Make_Pethylene();//clad 1
extern G4Material* Make_FPethylene();
extern G4Material* Make_Polystyrene();
extern G4Material* Make_Silicone();
extern G4Material* Make_TiO2();
extern G4Material* Make_Coating();


#endif
