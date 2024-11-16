#include "G4NistManager.hh"
#ifdef CMSSW_GIT_HASH
#include "MATHUSLA/Tile/interface/Materials.hh"
#else
#include "Materials.hh"
#endif

using namespace CLHEP;

// static list of common materials (cannot be const)
static G4Element* C  = G4NistManager::Instance()->FindOrBuildElement( "C" );
static G4Element* H  = G4NistManager::Instance()->FindOrBuildElement( "H" );
static G4Element* Si = G4NistManager::Instance()->FindOrBuildElement( "Si" );
static G4Element* O  = G4NistManager::Instance()->FindOrBuildElement( "O" );
static G4Element* K  = G4NistManager::Instance()->FindOrBuildElement( "K" );
static G4Element* Sb = G4NistManager::Instance()->FindOrBuildElement( "Sb" );
static G4Element* Rb = G4NistManager::Instance()->FindOrBuildElement( "Rb" );
static G4Element* Cs = G4NistManager::Instance()->FindOrBuildElement( "Cs" );
static G4Element* Lu = G4NistManager::Instance()->FindOrBuildElement( "Lu" );
static G4Element* Y  = G4NistManager::Instance()->FindOrBuildElement( "Y" );
static G4Element* Ce = G4NistManager::Instance()->FindOrBuildElement( "Ce" );
static G4Element* La = G4NistManager::Instance()->FindOrBuildElement( "La" );
static G4Element* Br = G4NistManager::Instance()->FindOrBuildElement( "Br" );
static G4Element* Na = G4NistManager::Instance()->FindOrBuildElement( "Na" );
static G4Element* I  = G4NistManager::Instance()->FindOrBuildElement( "I" );
static G4Element* Tl = G4NistManager::Instance()->FindOrBuildElement( "Tl" );
static G4Element* Al = G4NistManager::Instance()->FindOrBuildElement( "Al" );

static const unsigned flatentries = 2;
static const double minenergy     = 1.0*eV;
static const double maxenergy     = 8.0*eV;

// Bialkali definition:
// Ref: http://hypernews.slac.stanford.edu/HyperNews/geant4/get/AUX/2013/03/11/12.39-85121-chDetectorConstruction.cc
G4Material*
Make_Bialkali()
{
  G4Material* material
    = new G4Material( "Bialkali", 4.28*g/cm3, 3, kStateSolid );
  material->AddElement( K,  13.3*perCent );
  material->AddElement( Cs, 45.2*perCent );
  material->AddElement( Sb, 41.5*perCent );
  return material;
}

G4Material*
Make_Epoxy()
{
  G4Material* epoxy = new G4Material( "Epoxy", 3.75*g/cm3, 2, kStateSolid );
  epoxy->AddElement( Al, 40*perCent );
  epoxy->AddElement( O,  60*perCent );
  double photonE[flatentries]      = {minenergy, maxenergy};
  //double refrac_idx[flatentries]   = {1.50, 1.50};
  double refrac_idx[flatentries]   = {1.40, 1.40};  //Use 1.4 for gap between fiber and hole
  //double Reflectivity[flatentries] = {0.0, 0.0};
  // Will obe overwritten in surface property defintiion
  double abs_length[flatentries] = {20*m, 20*m};

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX",  photonE, refrac_idx,   flatentries );
  //table->AddProperty( "REFLECTIVITY", photonE, Reflectivity, flatentries );
  table->AddProperty( "ABSLENGTH",    photonE, abs_length,   flatentries );

  epoxy->SetMaterialPropertiesTable( table );
  return epoxy;
}

G4Material*
Make_Custom_Air()
{
  static double photonE[flatentries]    = {minenergy, maxenergy};
  static double refrac_idx[flatentries] = {1.0, 1.0};

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX", photonE, refrac_idx, flatentries );

  G4Material* material
    = G4NistManager::Instance()->FindOrBuildMaterial( "G4_AIR" );
  material->SetMaterialPropertiesTable( table );
  return material;
}

G4Material*
Make_Resin()
{
  static double photonE[flatentries]    = {minenergy, maxenergy};
  static double refrac_idx[flatentries] = {1., 1.};
  //static double refrac_idx[flatentries] = {0.8, 0.8};
  static double abs_length[flatentries] = {20*m, 20*m};

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX",    photonE, refrac_idx, 2 );
  table->AddProperty( "ABSLENGTH", photonE, abs_length, 2 );

  G4Material* material
    = G4NistManager::Instance()->FindOrBuildMaterial( "G4_Pyrex_Glass" );
  material->SetMaterialPropertiesTable( table );

  return material;
}

G4Material*
Make_EJ200()
{
  G4Material* material = new G4Material( "EJ200", 1.023*g/cm3, 2, kStateSolid );
  material->AddElement( C, 91.53*perCent );
  material->AddElement( H, 8.47*perCent );

  static double photonE[flatentries]    = {minenergy, maxenergy};
  static double refrac_idx[flatentries] = { 1.58, 1.58 };
  const G4int NUMENTRIES_1 = 139;
  static double FAST_Energy[NUMENTRIES_1] = {3.13 * eV, 3.12 * eV, 3.11 * eV, 3.10 * eV, 3.09 * eV, 3.07 * eV, 3.07 * eV, 3.06 * eV, 3.05 * eV, 3.05 * eV, 3.05 * eV, 3.04 * eV, 3.04 * eV,    3.03 * eV, 3.03 * eV, 3.02 * eV, 3.02 * eV, 3.02 * eV, 3.01 * eV, 3.01 * eV, 3.00 * eV, 3.00 * eV, 3.00 * eV,    3.00 * eV, 2.99 * eV, 2.99 * eV, 2.99 * eV, 2.98 * eV, 2.98 * eV, 2.97 * eV, 2.97 * eV, 2.98 * eV, 2.98 * eV,    2.97 * eV, 2.97 * eV, 2.96 * eV, 2.96 * eV, 2.96 * eV, 2.95 * eV, 2.95 * eV, 2.95 * eV, 2.94 * eV, 2.94 * eV,    2.93 * eV, 2.93 * eV, 2.92 * eV, 2.91 * eV, 2.90 * eV, 2.89 * eV, 2.89 * eV, 2.88 * eV, 2.88 * eV, 2.87 * eV,    2.86 * eV, 2.85 * eV, 2.85 * eV, 2.84 * eV, 2.84 * eV, 2.83 * eV, 2.83 * eV, 2.82 * eV, 2.82 * eV, 2.81 * eV,    2.81 * eV, 2.80 * eV, 2.80 * eV, 2.79 * eV, 2.78 * eV, 2.78 * eV, 2.77 * eV, 2.77 * eV, 2.76 * eV, 2.76 * eV,    2.75 * eV, 2.74 * eV, 2.74 * eV, 2.73 * eV, 2.72 * eV, 2.72 * eV, 2.71 * eV, 2.70 * eV, 2.70 * eV, 2.69 * eV,    2.68 * eV, 2.67 * eV, 2.67 * eV, 2.67 * eV, 2.66 * eV, 2.66 * eV, 2.65 * eV, 2.65 * eV, 2.64 * eV, 2.64 * eV,    2.63 * eV, 2.63 * eV, 2.62 * eV, 2.62 * eV, 2.61 * eV, 2.60 * eV, 2.60 * eV, 2.59 * eV, 2.59 * eV, 2.58 * eV,    2.58 * eV, 2.57 * eV, 2.56 * eV, 2.55 * eV, 2.55 * eV, 2.54 * eV, 2.53 * eV, 2.52 * eV, 2.52 * eV, 2.51 * eV,    2.51 * eV, 2.50 * eV, 2.49 * eV, 2.49 * eV, 2.48 * eV, 2.48 * eV, 2.46 * eV, 2.43 * eV, 2.41 * eV, 2.38 * eV,    2.36 * eV, 2.34 * eV, 2.32 * eV, 2.30 * eV, 2.28 * eV, 2.25 * eV, 2.23 * eV, 2.21 * eV, 2.19 * eV, 2.18 * eV,    2.16 * eV, 2.14 * eV, 2.12 * eV, 2.10 * eV, 2.07 * eV, 1.91 * eV};
  static double FAST_COMPONENT[NUMENTRIES_1] = {0, 0.0008, 0.0065, 0.0089, 0.0185, 0.0355, 0.0573, 0.0839, 0.1081, 0.1274, 0.1444, 0.171, 0.1927, 0.2218,       0.2508, 0.2677, 0.2968, 0.3185, 0.35, 0.379, 0.4056, 0.4323, 0.4565, 0.4903,       0.5218, 0.5532, 0.5798, 0.6065, 0.6282, 0.7177, 0.6984, 0.679, 0.6548, 0.7419,       0.7637, 0.7927, 0.8218, 0.8483, 0.8774, 0.9065, 0.9355, 0.9597, 0.979, 0.9887,       0.9983, 1, 0.996, 0.9887, 0.979, 0.9645, 0.95, 0.9355, 0.9161, 0.8968, 0.8774,       0.8605, 0.8387, 0.8145, 0.7903, 0.7734, 0.7565, 0.7371, 0.7177, 0.6935, 0.6766,       0.6597, 0.6452, 0.6258, 0.6089, 0.5919, 0.5774, 0.5629, 0.5508, 0.5315, 0.5145,       0.5, 0.4855, 0.471, 0.4565, 0.4468, 0.4371, 0.4274, 0.4177, 0.4081, 0.396, 0.379,       0.3887, 0.3669, 0.3548, 0.3403, 0.3258, 0.3016, 0.3113, 0.2823, 0.2629, 0.2484,       0.2339, 0.2242, 0.2097, 0.2, 0.1855, 0.1758, 0.1661, 0.154, 0.1468, 0.1323,       0.1226, 0.1129, 0.1032, 0.096, 0.0887, 0.0839, 0.0766, 0.0742, 0.0669, 0.0597,       0.0573, 0.0548, 0.0536, 0.0402, 0.0302, 0.0226, 0.017, 0.0127, 0.0096, 0.0072,       0.0054, 0.004, 0.003, 0.0023, 0.0017, 0.0013, 0.001, 0.0007, 0.0005, 0.0004, 0.0003, 0.0002, 0};
  double popop_emission_x[]={200, 349.9,350     ,351    ,352    ,353    ,354    ,355    ,356    ,357    ,358    ,359    ,360    ,361    ,362    ,363    ,364    ,365    ,366    ,367    ,368    ,369    ,370    ,371    ,372    ,373    ,374    ,375    ,376    ,377    ,378    ,379    ,380    ,381    ,382    ,383    ,384    ,385    ,386    ,387    ,388    ,389    ,390    ,391    ,392    ,393    ,394    ,395    ,396    ,397    ,398    ,399    ,400    ,401    ,402    ,403    ,404    ,405    ,406    ,407    ,408    ,409    ,410    ,411    ,412    ,413    ,414    ,415    ,416    ,417    ,418    ,419    ,420    ,421    ,422    ,423    ,424    ,425    ,426    ,427    ,428    ,429    ,430    ,431    ,432    ,433    ,434    ,435    ,436    ,437    ,438    ,439    ,440    ,441    ,442    ,443    ,444    ,445    ,446    ,447    ,448    ,449    ,450    ,451    ,452    ,453    ,454    ,455    ,456    ,457    ,458    ,459    ,460    ,461    ,462    ,463    ,464    ,465    ,466    ,467    ,468    ,469    ,470    ,471    ,472    ,473    ,474    ,475    ,476    ,477    ,478    ,479    ,480    ,481    ,482    ,483    ,484    ,485    ,486    ,487    ,488    ,489    ,490    ,491    ,492    ,493    ,494    ,495    ,496    ,497    ,498    ,499    ,500    ,501    ,502    ,503    ,504    ,505    ,506    ,507    ,508    ,509    ,510    ,511    ,512    ,513    ,514    ,515    ,516    ,517    ,518    ,519    ,520    ,521    ,522    ,523    ,524    ,525    ,526    ,527    ,528    ,529    ,530    ,531    ,532    ,533    ,534    ,535    ,536    ,537    ,538    ,539    ,540    ,541    ,542    ,543    ,544    ,545    ,546    ,547    ,548    ,549    ,550    ,551    ,552    ,553    ,554    ,555    ,556    ,557    ,558    ,559    ,560    ,561    ,562    ,563    ,564    ,565    ,566    ,567    ,568    ,569    ,570    ,571    ,572    ,573    ,574    ,575    ,576    ,577    ,578    ,579    ,580    ,581    ,582    ,583    ,584    ,585    ,586    ,587    ,588    ,589    ,590    ,591    ,592    ,593    ,594    ,595    ,596    ,597    ,598    ,599    ,600    ,600.1,1000};
  double popop_emission_y[]={0,0,0.150010303    ,0.180218422    ,0.138058933    ,0.135648053    ,0.147558211    ,0.184772306    ,0.174119102    ,0.167690089    ,0.18209355     ,0.166618586    ,0.203173295    ,0.233051721    ,0.211621677    ,0.22007006     ,0.23634865     ,0.261899856    ,0.268699773    ,0.286420771    ,0.388625592    ,0.529569339    ,0.593241294    ,0.666804039    ,0.844632186    ,1.130640841    ,1.579435401    ,1.927879662    ,2.557181125    ,3.410261694    ,4.640428601    ,6.126107562    ,8.520502782    ,11.32701422    ,16.08077478    ,20.18751288    ,25.8602926     ,31.71234288    ,37.42015248    ,42.83948073    ,46.07459304    ,49.39212858    ,50.77271791    ,49.94848547    ,47.82608696    ,47.74366371    ,46.28065114    ,46.69276736    ,47.68184628    ,50.66968885    ,55.45023697    ,58.06717494    ,60.457449      ,68.39068617    ,73.97486091    ,77.45724294    ,83.37111065    ,90.1092108     ,95.0546054     ,98.24850608    ,100    ,99.15516176    ,95.21945189    ,93.17947661    ,88.81104471    ,80.77477849    ,73.56274469    ,67.58705955    ,62.39439522    ,57.59324129    ,55.09993818    ,53.76056048    ,48.36183804    ,48.81516588    ,47.18730682    ,47.5376056     ,47.5376056     ,47.70245209    ,48.69153101    ,49.74242736    ,50.46363074    ,50.97877601    ,50.62847723    ,50.93756439    ,50.54605399    ,51.37028642    ,49.14485885    ,48.03214506    ,47.74366371    ,45.66247682    ,44.36431074    ,43.37523182    ,40.32557181    ,38.017721      ,37.04924789    ,35.13290748    ,33.36080775    ,30.66144653    ,28.49783639    ,27.22027612    ,25.98392747    ,25.22151247    ,24.12940449    ,22.76942098    ,22.56336287    ,22.48093962    ,21.677313      ,21.96579435    ,20.95610962    ,21.78034206    ,20.57078096    ,20.14424068    ,19.74860911    ,19.61055018    ,19.10364723    ,18.34741397    ,18.09190192    ,16.47228518    ,15.84792912    ,15.20914898    ,13.73377292    ,13.98928498    ,12.77148156    ,12.85802596    ,11.62373789    ,10.45126726    ,10.3379353     ,9.515763445    ,8.553472079    ,8.194930971    ,7.714815578    ,7.041005564    ,6.775190604    ,6.513496806    ,6.1219864      ,5.942715846    ,5.948897589    ,5.487327426    ,5.559447764    ,5.192664331    ,5.077271791    ,5.079332372    ,5.054605399    ,5.56975067     ,5.973624562    ,6.443437049    ,7.189367402    ,8.131052957    ,8.825468782    ,9.975273027    ,11.05913868    ,12.05027818    ,12.87863177    ,13.47413971    ,12.83948073    ,12.92808572    ,11.95343087    ,9.742427365    ,8.378322687    ,6.97712755     ,5.903564805    ,4.945394601    ,4.071708222    ,3.418504018    ,2.862147125    ,2.344941273    ,1.990521327    ,1.768184628    ,1.633216567    ,1.530393571    ,1.325777869    ,1.292396456    ,1.287451061    ,1.148773954    ,1.146095199    ,1.08819287     ,1.056253864    ,1.073768803    ,1.049453946    ,0.948279415    ,0.861941067    ,0.897177004    ,0.862147125    ,0.812899238    ,0.772099732    ,0.709045951    ,0.706573254    ,0.7407789      ,0.652998145    ,0.657531424    ,0.617350093    ,0.577580878    ,0.557387183    ,0.489594066    ,0.513290748    ,0.49783639     ,0.488357717    ,0.464661034    ,0.449824851    ,0.453946013    ,0.380383268    ,0.37584999     ,0.372140944    ,0.370080363    ,0.349062436    ,0.353389656    ,0.300844838    ,0.303111477    ,0.274675458    ,0.290541933    ,0.256748403    ,0.28436019     ,0.256748403    ,0.211003503    ,0.263342263    ,0.233463837    ,0.233051721    ,0.187904389    ,0.177910571    ,0.185060787    ,0.192808572    ,0.182299608    ,0.189841335    ,0.173315475    ,0.156150835    ,0.182711725    ,0.149536369    ,0.146610344    ,0.157655059    ,0.153739955    ,0.139851638    ,0.179950546    ,0.14854729     ,0.1255306      ,0.133072326    ,0.13113538     ,0.14687822     ,0.144508551    ,0.127714816    ,0.120296724    ,0.138944982    ,0.140552236    ,0.132536575    ,0.111188955    ,0.111333196    ,0.104553884    ,0.09655883     ,0.122769421    ,0.120482176    ,0.108242324    ,0.119987637    ,0.114815578    ,0.090892232    , 0, 0 };
  const G4int n_popop_emission = sizeof(popop_emission_x)/sizeof(G4double);
  for(int i=0;i<n_popop_emission;i++){
          popop_emission_x[i] = 1240*eV/popop_emission_x[i];
          popop_emission_y[i] =  popop_emission_y[i]*mm;
  }

  // Add entries into properties table
  G4MaterialPropertiesTable *table = new G4MaterialPropertiesTable();
  table->AddProperty("RINDEX", photonE, refrac_idx, flatentries);
  //table->AddProperty("FASTCOMPONENT", FAST_Energy, FAST_COMPONENT, NUMENTRIES_1);
  table->AddProperty("FASTCOMPONENT", popop_emission_x, popop_emission_y, n_popop_emission);
  table->AddConstProperty("SCINTILLATIONYIELD", 10. / keV);
  table->AddConstProperty("RESOLUTIONSCALE", 1.0);
  table->AddConstProperty("FASTTIMECONSTANT", 2.5 * ns); //2.5
  table->AddConstProperty("YIELDRATIO", 1.0);
  table->AddConstProperty("FASTSCINTILLATIONRISETIME", 0.5 * ns); //0.5

  material->SetMaterialPropertiesTable( table );

  // FIXME: Set the Birks Constant for the EJ200 scintillator
  material->GetIonisation()->SetBirksConstant( 0.126*mm/MeV );

  Update_EJ200_Scinti( material, 10000 );
  Update_EJ200_FASTTIMECONSTANT( material, 3.945 ); // from Jim, 260nm_extrusion_excite_SIPM
  Update_EJ200_RISETIME( material, 0.5 );

  return material;
}
void Update_fiber_timeconstant(G4Material* material, const double mult)
{
  G4MaterialPropertiesTable *tabler = material->GetMaterialPropertiesTable();
  tabler->RemoveConstProperty("WLSTIMECONSTANT");
  tabler->AddConstProperty("WLSTIMECONSTANT", mult * ns); //accounts for any time delay which may occur between absorption and re-emission of the photon, defalt delta
}
void Update_refrac_index(G4Material* material, const double mult)
{

  static const unsigned nentries = 2;
  static double photonE[nentries] = {1* eV, 8*eV};
  static double Normabs_length[nentries] = {mult, mult};
  G4MaterialPropertiesTable *tabler = material->GetMaterialPropertiesTable();
  tabler->RemoveProperty("RINDEX");
  tabler->AddProperty("RINDEX", photonE, Normabs_length, nentries);
  std::cout<<"Update_refrac_index to "<<mult<<std::endl;
}

void
Update_EJ200_Scinti(G4Material* material, const double mult)
{
  G4MaterialPropertiesTable *tabler = material->GetMaterialPropertiesTable();
  tabler->RemoveConstProperty("SCINTILLATIONYIELD");
  tabler->AddConstProperty("SCINTILLATIONYIELD", mult / MeV);
}

void
Update_EJ200_FASTTIMECONSTANT(G4Material* material, const double mult)
{
  G4MaterialPropertiesTable *tabler = material->GetMaterialPropertiesTable();
  tabler->RemoveConstProperty("FASTTIMECONSTANT"); //2.5
  tabler->AddConstProperty("FASTTIMECONSTANT",  mult * ns);
}

void
Update_EJ200_RISETIME(G4Material* material, const double mult)
{
  G4MaterialPropertiesTable *tabler = material->GetMaterialPropertiesTable();
  tabler->RemoveConstProperty("FASTSCINTILLATIONRISETIME");
  tabler->AddConstProperty("FASTSCINTILLATIONRISETIME", mult * ns);
}

void
Set_Flat_AbsLength( G4Material* material, const double mult )
{
  static const unsigned nentries = 2;
  static double photonE[nentries] = {1* eV, 6*eV};
  static double Normabs_length[nentries] = {1* mm, 1* mm};
  //flat abs
  double abs_length[nentries];
  for (unsigned i = 0; i < nentries; ++i)
  {
    abs_length[i] = Normabs_length[i] * mult;
  }

  G4MaterialPropertiesTable *tabler = material->GetMaterialPropertiesTable();
  tabler->RemoveProperty("ABSLENGTH");
  tabler->AddProperty("ABSLENGTH", photonE, abs_length, nentries);
}

//https://www.crystals.saint-gobain.com/sites/imdf.crystals.com/files/documents/sgc-scintillation-fiber_0.pdf
G4Material* Make_sgc(){

  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4String> elements;
  elements.push_back("C");
  natoms.push_back(8);
  elements.push_back("H");
  natoms.push_back(8);
  density = 1.05 * g / cm3;
  G4Material* fsgc = G4NistManager::Instance()->ConstructNewMaterial("sgc", elements, natoms, density);

  G4double photonEnergy[] ={2.614246802 *eV,2.601505936 *eV,2.591177042 *eV,2.582063819 *eV,2.569633972 *eV,2.559556123 *eV,2.549551772 *eV,2.540728492 *eV,2.535242944 *eV,2.528692503 *eV,2.522175824 *eV,
   2.517853408 *eV,2.516775118 *eV,2.510319641 *eV,2.508176859 *eV,2.504964476 *eV,2.503897195 *eV,
   2.500700801 *eV,2.497507528 *eV,2.495386553 *eV,2.494327417 *eV,2.492206829 *eV,2.487986436 *eV,
   2.486933571 *eV,2.485876612 *eV,2.484825531 *eV,2.48063008  *eV,2.47958343  *eV,2.478532709 *eV,
   2.476443827 *eV,2.475400705 *eV,2.473317097 *eV,2.469155469 *eV,2.466047105 *eV,2.461909886 *eV,
   2.459848918 *eV,2.457786525 *eV,2.456759059 *eV,2.453681809 *eV,2.451634588 *eV,2.446526623 *eV,
   2.443470122 *eV,2.441439898 *eV,2.440426051 *eV,2.436374282 *eV,2.43133441  *eV,2.425309276 *eV,
   2.420310307 *eV,2.41433962  *eV,2.410373938 *eV,2.405440952 *eV,2.397580764 *eV,2.388804981 *eV,
   2.377200348 *eV,2.362854404 *eV,2.35244569  *eV,2.344927487 *eV,2.339322502 *eV,2.33374864  *eV,
   2.330043087 *eV,2.324508947 *eV,2.319001034 *eV,2.317172302 *eV,2.313519161 *eV,2.311699062 *eV,
   2.308063144 *eV,2.305346918 *eV,2.301730939 *eV,2.297232206 *eV,2.295433383 *eV,2.290062238 *eV,
   2.286498228 *eV,2.282054585 *eV,2.277628181 *eV,2.271461649 *eV,2.266201973 *eV,2.2609666   *eV,
   2.252297252 *eV,2.245408682 *eV,2.236002892 *eV,2.224147425 *eV,2.210752458 *eV,2.199162545 *eV,
   2.190139427 *eV,2.182814211 *eV,2.169907236 *eV,2.157947861 *eV,2.146119591 *eV,2.135195775 *eV,
   2.122077651 *eV,2.1060963   *eV,2.08886785  *eV,2.076311132 *eV,2.066084719 *eV,2.053803014 *eV,
   2.040245488 *eV,2.031072128 *eV,2.017119488 *eV,2.005408133 *eV,1.997226432 *eV,};
  G4double emissionFib[] = {0   ,0.002538406    ,0.005076812    ,0.007615218    ,0.017764443    ,0.022841255    ,0.027918067    ,0.035533285    ,0.048225315    ,0.07106657     ,0.096446232    ,0.121825893    ,0.134517923    ,0.164974396    ,0.187815651    ,0.215733718    ,0.241117779    ,0.291877101    ,0.317256762    ,0.33756401     ,0.362943671    ,0.375635701    ,0.401015362    ,0.434010242    ,0.459389903    ,0.477158745    ,0.515226037    ,0.543148503    ,0.560912946    ,0.60406145     ,0.626902705    ,0.637056329    ,0.680200433    ,0.741117779    ,0.786800289    ,0.814718356    ,0.829948792    ,0.847713235    ,0.865482077    ,0.888323332    ,0.916241399    ,0.939087054    ,0.959389903    ,0.972081933    ,0.984769564    ,1      ,0.984769564    ,0.959389903    ,0.94162106     ,0.926395023    ,0.908626181    ,0.888323332    ,0.875631302    ,0.860405265    ,0.840102416    ,0.832487198    ,0.81217995     ,0.796953913    ,0.774112658    ,0.753805409    ,0.72081053     ,0.695430869    ,0.672589614    ,0.64974396     ,0.626902705    ,0.596446232    ,0.573604977    ,0.550759322    ,0.525379661    ,0.497461594    ,0.467005121    ,0.444163866    ,0.423856617    ,0.39593855     ,0.368020483    ,0.345179228    ,0.317256762    ,0.294415507    ,0.271574252    ,0.248728597    ,0.228425748    ,0.203046087    ,0.185277245    ,0.17512802     ,0.16243599     ,0.14974396     ,0.137056329    ,0.121825893    ,0.114215074    ,0.098984638    ,0.081220195    ,0.068528164    ,0.060912946    ,0.048225315    ,0.045686909    ,0.027918067    ,0.025379661    ,0.017764443    ,0.002538406    ,0      ,};
  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);
  G4double energySmall[]= { 1.0 * eV, 6.0 * eV };
  G4double refractiveIndexWLSfiber[] = { 1.60, 1.60 };

  //A naive boundary
  G4double photonEnergy2[] = {1*eV, 2.62*eV, 2.621*eV, 6.0 * eV };
  G4double absWLSfiber[] = {500.0*m, 500.0*m, 0.1*mm, 0.1*mm};
  //BCF-92
  G4double wls_abs_bcf92_x[] = {359.666, 363.246, 366.826, 372.196, 375.776, 380.549, 387.112, 391.885, 397.852, 404.415, 411.575, 419.928, 427.088, 431.862, 435.442, 439.021, 441.408, 445.585, 446.778, 449.761, 450.955, 452.745, 455.728, 456.921, 459.905, 461.098, 462.291, 464.678, 467.064, 469.451, 472.434, 474.224, 475.418, 478.401, 479.594, 479.6, 1000, };
G4double wls_abs_bcf92_y[] = {1225.3, 1052.1, 934.5, 802.4, 700.8, 622.5, 552.9, 525.52, 491.11, 458.95, 466.79, 516.7, 581.71, 689, 762.7, 888.2, 1088.3, 1356.3, 1661.8, 2106.3, 2669.7, 4008.1, 4994.9, 6120, 7890, 10701, 14270, 18395, 24949, 37457, 49111, 7.01E+04, 9.03E+04, 1.23E+05, 2.07E+05, 2.07E+15, 2.07E+15}; //mm  
const G4int n_wls_bcf92_abs = sizeof(wls_abs_bcf92_x)/sizeof(G4double);
  for(int i=0;i<n_wls_bcf92_abs;i++){
          wls_abs_bcf92_x[i] = 1240*eV/wls_abs_bcf92_x[i];
          wls_abs_bcf92_y[i] =  wls_abs_bcf92_y[i]*mm/1000.0;
  }

  //G4double realabs_e[]={2.469788905     *eV,2.46085797  *eV,2.450328967 *eV,2.441473547 *eV,2.431120857 *eV,2.422141814 *eV,2.4119624   *eV,2.403099844 *eV,2.393371904 *eV,2.384929095 *eV,2.375382911 *eV,2.365587418 *eV,2.356763874 *eV,2.348034364 *eV,2.339321619 *eV,2.321568287 *eV,2.295443157 *eV,2.278841635 *eV,2.253935662 *eV,2.2134774   *eV,2.174693096 *eV,};
  //G4double realabsWLSfiber[] = { 2.64495        *m,2.40574      *m,2.80068      *m,3.07617      *m,3.37919      *m,3.26865      *m,3.48896      *m,3.58062      *m,3.87446      *m,4.09481      *m,4.09454      *m,4.3516       *m,4.48921      *m,4.37866      *m,4.68171      *m,4.8742       *m,5.11237      *m,5.78281      *m,5.98422      *m,5.97371      *m,5.98159      *m,};
  //const G4int nEntries_realabs = sizeof(realabs_e)/sizeof(G4double);

  G4double wls_abs_y11_x[] = {200, 349.9, 350	,351	,352	,353	,354	,355	,356	,357	,358	,359	,360	,361	,362	,363	,364	,365	,366	,367	,368	,369	,370	,371	,372	,373	,374	,375	,376	,377	,378	,379	,380	,381	,382	,383	,384	,385	,386	,387	,388	,389	,390	,391	,392	,393	,394	,395	,396	,397	,398	,399	,400	,401	,402	,403	,404	,405	,406	,407	,408	,409	,410	,411	,412	,413	,414	,415	,416	,417	,418	,419	,420	,421	,422	,423	,424	,425	,426	,427	,428	,429	,430	,431	,432	,433	,434	,435	,436	,437	,438	,439	,440	,441	,442	,443	,444	,445	,446	,447	,448	,449	,450	,451	,452	,453	,454	,455	,456	,457	,458	,459	,460	,461	,462	,463	,464	,465	,466	,467	,468	,469	,470	,471	,472	,473	,474	,475	,476	,477	,478	,479	,480	,481	,482	,483	,484	,485	,486	,487	,488	,489	,490	,490.1,1000};
  G4double wls_abs_y11_y[] = { 2.07E+15, 2.07E+15, 3.330381712	,3.481762699	,3.330381712	,3.259522527	,3.126480791	,3.126480791	,3.126480791	,3.126480791	,3.126480791	,3.126480791	,3.003873701	,2.946106899	,2.890519976	,2.735670692	,2.687676469	,2.553292646	,2.431707282	,2.321175133	,2.252905276	,2.127743871	,2.042634117	,1.964071266	,1.84575372	,1.740881349	,1.647285578	,1.531975587	,1.459024369	,1.367835346	,1.298284396	,1.235464183	,1.17844276	,1.126452638	,1.086507509	,1.042160264	,1.007878676	,0.969604802	,0.934131456	,0.90649443	,0.870440675	,0.846395352	,0.810569094	,0.781620198	,0.747305165	,0.715876443	,0.683917673	,0.651904505	,0.617732092	,0.586963827	,0.559115178	,0.531935968	,0.507276685	,0.484802401	,0.467065728	,0.449259703	,0.436460281	,0.424369969	,0.415169536	,0.408526823	,0.40315147	,0.398951976	,0.395859325	,0.391809613	,0.388826291	,0.384918489	,0.379201878	,0.370938399	,0.36302739	,0.352178296	,0.340439019	,0.328750126	,0.316523882	,0.30517442	,0.294045218	,0.282652322	,0.273079427	,0.265047679	,0.257908348	,0.252384776	,0.248294261	,0.245902984	,0.245508908	,0.247092837	,0.250732502	,0.255329265	,0.262324587	,0.270189698	,0.279048377	,0.289051998	,0.298630719	,0.307009136	,0.313287441	,0.317837259	,0.319827889	,0.320496985	,0.317837259	,0.314574043	,0.308866046	,0.303361502	,0.296894494	,0.291250112	,0.285284094	,0.280068663	,0.276529889	,0.274056456	,0.273567069	,0.2750405	,0.279048377	,0.285816341	,0.296894494	,0.312011321	,0.332315746	,0.358776484	,0.392814253	,0.436460281	,0.491017816	,0.559115178	,0.646403201	,0.754667777	,0.890683481	,1.0713116	,1.287374443	,1.563240395	,1.939209604	,2.431707282	,3.003873701	,3.736525823	,4.787423711	,6.12790235	,7.659877937	,9.574847422	,11.7844276	,15.31975587	,19.14969484	,21.88536554	,25.53292646	,30.63951175	,38.29938969	,51.06585292	,51.06585292	,76.59877937	,76.59877937	, 2.07E+15, 2.07E+15};
  const G4int n_wls_y11_abs = sizeof(wls_abs_y11_x)/sizeof(G4double);
  for(int i=0;i<n_wls_y11_abs;i++){
          wls_abs_y11_x[i] = 1240*eV/wls_abs_y11_x[i];
          //wls_abs_y11_y[i] = wls_abs_y11_y[i]*mm;
          wls_abs_y11_y[i] = wls_abs_y11_y[i]*mm/2;
  }

  G4MaterialPropertiesTable* mptWLSfiber = new G4MaterialPropertiesTable();
  mptWLSfiber->AddProperty("RINDEX", energySmall, refractiveIndexWLSfiber, 2);
  mptWLSfiber->AddProperty("WLSCOMPONENT", photonEnergy, emissionFib, nEntries); //WLSCOMPONENT is the relative emission spectrum of the material as a function of the photon's momentum
  //mptWLSfiber->AddProperty("WLSABSLENGTH", photonEnergy2, absWLSfiber, 4); //WLSABSLENGTH is the absorption length of the material as a function of the photon's momentum. -> re-emit
  //mptWLSfiber->AddProperty("WLSABSLENGTH", wls_abs_bcf92_x, wls_abs_bcf92_y, n_wls_bcf92_abs); //WLSABSLENGTH is the absorption length of the material as a function of the photon's momentum. -> re-emit
  mptWLSfiber->AddProperty("WLSABSLENGTH", wls_abs_y11_x, wls_abs_y11_y, n_wls_y11_abs);
  mptWLSfiber->AddConstProperty("WLSTIMECONSTANT", 2.7 * ns); //accounts for any time delay which may occur between absorption and re-emission of the photon, defalt delta
  fsgc->SetMaterialPropertiesTable(mptWLSfiber);
  Set_Flat_AbsLength( fsgc, 1000 );
  elements.clear();
  natoms.clear();
  return fsgc;


}

//clad 1
G4Material* Make_acrylic(){
  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4String> elements;
  elements.push_back("C");
  natoms.push_back(5);
  elements.push_back("H");
  natoms.push_back(8);
  elements.push_back("O");
  natoms.push_back(2);
  density = 1.190 * g / cm3;
  G4Material* acrylic = G4NistManager::Instance()->ConstructNewMaterial("acrylic", elements, natoms, density);
  G4double refractiveIndexClad1[] = { 1.49, 1.49 };
  G4double energySmall[] = { 1.0 * eV, 6.0 * eV };
  G4double absClad[] = { 20.0 * m, 20.0 * m };
  G4MaterialPropertiesTable* mptClad1 = new G4MaterialPropertiesTable();
  mptClad1->AddProperty("RINDEX", energySmall, refractiveIndexClad1, 2);
  mptClad1->AddProperty("ABSLENGTH", energySmall, absClad, 2);
  acrylic->SetMaterialPropertiesTable(mptClad1);
  elements.clear();
  natoms.clear();
  return acrylic;
}
//clad 2
G4Material* Make_Fluor_acrylic(){
  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4String> elements;
  elements.push_back("C");
  natoms.push_back(5);
  elements.push_back("H");
  natoms.push_back(8);
  elements.push_back("O");
  natoms.push_back(2);
  density = 1.190 * g / cm3;
  G4Material* acrylic2 = G4NistManager::Instance()->ConstructNewMaterial("acrylic2", elements, natoms, density);
  G4double refractiveIndexClad2[] = { 1.42, 1.42 };
  G4double energySmall[] = { 1.0 * eV, 6.0 * eV };
  G4double absClad[] = { 20.0 * m, 20.0 * m };
  G4MaterialPropertiesTable* mptClad2 = new G4MaterialPropertiesTable();
  mptClad2->AddProperty("RINDEX", energySmall, refractiveIndexClad2, 2);
  mptClad2->AddProperty("ABSLENGTH", energySmall, absClad, 2);
  acrylic2->SetMaterialPropertiesTable(mptClad2);
  elements.clear();
  natoms.clear();
  return acrylic2;
}



//wls
G4Material* Make_Y11(){
 
  //--------------------------------------------------
  // WLSfiber Y11
  //--------------------------------------------------

  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4String> elements;
  elements.push_back("C");
  natoms.push_back(5);
  elements.push_back("H");
  natoms.push_back(8);
  elements.push_back("O");
  natoms.push_back(2);

  density = 1.190 * g / cm3;
  G4Material* fPMMA = G4NistManager::Instance()->ConstructNewMaterial("PMMA", elements, natoms, density);

/*
  G4double photonEnergy[] =
  {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
   2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
   2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
   2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
   2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
   2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
   2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
   3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
   3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
   3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};

  G4double absWLSfiber[] =
  {5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,1.10*m,
   1.10*m,1.10*m,1.10*m,1.10*m,1.10*m,1.10*m, 1.*mm, 1.*mm, 1.*mm, 1.*mm,
    1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm};

  G4double emissionFib[] =
  {0.05, 0.10, 0.30, 0.50, 0.75, 1.00, 1.50, 1.85, 2.30, 2.75,
   3.25, 3.80, 4.50, 5.20, 6.00, 7.00, 8.50, 9.50, 11.1, 12.4,
   12.9, 13.0, 12.8, 12.3, 11.1, 11.0, 12.0, 11.0, 17.0, 16.9,
   15.0, 9.00, 2.50, 1.00, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00,
   0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};
*/
  //Y11 from
  //http://kuraraypsf.jp/psf/ws.html
  /*
  G4double photonEnergy[] ={
2.862439029	*eV,2.840778923	*eV,2.815225831	*eV,2.788754124	*eV,2.756033268	*eV,2.741312912	*eV,2.729395813	*eV,2.721518432	*eV,2.707216697	*eV,2.707181235	*eV,2.693076188	*eV,2.693035246	*eV,2.67778668	*eV,2.669025741	*eV,2.668951059	*eV,2.665291033	*eV,2.66035048	*eV,2.660327649	*eV,2.659175227	*eV,2.659141012	*eV,2.651703078	*eV,2.645581665	*eV,2.645536511	*eV,2.640663232	*eV,2.63824174	*eV,2.634587028	*eV,2.632171074	*eV,2.626078227	*eV,2.624888601	*eV,2.616425667	*eV,2.616398064	*eV,2.609196999	*eV,2.599591613	*eV,2.59122036	*eV,2.585261508	*eV,2.584054715	*eV,2.578123376	*eV,2.57219786	*eV,2.560459892	*eV,2.553442102	*eV,2.541874304	*eV,2.53611406	*eV,2.530385027	*eV,2.516790443	*eV,2.507816703	*eV,2.494493004	*eV,2.482417875	*eV,2.469386576	*eV,2.457557584	*eV,2.447961085	*eV,2.438420061	*eV,2.42893409	*eV,2.418492889	*eV,2.417422599	*eV,2.408094304	*eV,2.397807939	*eV,2.394710008	*eV,2.386565943	*eV,2.37846796	*eV,2.376439519	*eV,2.370420214	*eV,2.366412214	*eV,2.360430055	*eV,2.354482536	*eV,2.348564913	*eV,2.343655498	*eV,2.331984293	*eV,2.322358317	*eV,2.312811483	*eV,2.301457528	*eV,2.293013889	*eV,2.27724336	*eV,2.266230965	*eV,2.259854057	*eV,2.249910639	*eV,2.243625119	*eV,2.232938727	*eV,2.221477748	*eV,2.213613724	*eV,2.203206732	*eV,2.188647275	*eV,2.182698942	*eV,2.174263731	*eV,2.161739991	*eV,2.146903611	*eV,2.130661072	*eV,2.114658849	*eV,2.091108237	*eV,2.081064162	*eV,2.059000322	*eV,2.041098506	*eV,2.032993517	*eV,2.019869653	*eV,2.006917391	*eV,1.992012672	*eV,1.962177447	*eV,1.948600936	*eV};

  G4double emissionFib[] = {0.0063, 0.00248, 0.0025, 0.00638, 0.01987, 0.04296, 0.07182, 0.09491, 0.16223, 0.13146, 
  0.2334, 0.20455, 0.28919, 0.38535, 0.32573, 0.42766, 0.50267, 0.48151, 0.56613, 0.53728, 
  0.59691, 0.67768, 0.64499, 0.71423, 0.75654, 0.79308, 0.82962, 0.86424, 0.89309, 0.95079, 
  0.92579, 0.9835, 1.00658, 1.00467, 0.99314, 0.97391, 0.96045, 0.92585, 0.88355, 0.83741, 
  0.79511, 0.76243, 0.73551, 0.70668, 0.69708, 0.6971, 0.71442, 0.73751, 0.75676, 0.76639, 
  0.76448, 0.73949, 0.72604, 0.6972, 0.67029, 0.63184, 0.59338, 0.56263, 0.5261, 0.50687, 
  0.4838, 0.46073, 0.42805, 0.39536, 0.37037, 0.34346, 0.31463, 0.30118, 0.28389, 0.26659, 
  0.25315, 0.23971, 0.21665, 0.2032, 0.18783, 0.17245, 0.15516, 0.13787, 0.12635, 0.10713, 
  0.09562, 0.08024, 0.06872, 0.0572, 0.04761, 0.03995, 0.03228, 0.02848, 0.02658, 0.01892, 
  0.01704, 0.01513, 0.00938, 0.00749, 0.00559, 0.00181, 0.0};
*/


//from paper
  G4double photonEnergy[] ={2.614246802	*eV,2.601505936	*eV,2.591177042	*eV,2.582063819	*eV,2.569633972	*eV,2.559556123	*eV,2.549551772	*eV,2.540728492	*eV,2.535242944	*eV,2.528692503	*eV,2.522175824	*eV,2.517853408	*eV,2.516775118	*eV,2.510319641	*eV,2.508176859	*eV,2.504964476	*eV,2.503897195	*eV,2.500700801	*eV,2.497507528	*eV,2.495386553	*eV,2.494327417	*eV,2.492206829	*eV,2.487986436	*eV,2.486933571	*eV,2.485876612	*eV,2.484825531	*eV,2.48063008	*eV,2.47958343	*eV,2.478532709	*eV,2.476443827	*eV,2.475400705	*eV,2.473317097	*eV,2.469155469	*eV,2.466047105	*eV,2.461909886	*eV,2.459848918	*eV,2.457786525	*eV,2.456759059	*eV,2.453681809	*eV,2.451634588	*eV,2.446526623	*eV,2.443470122	*eV,2.441439898	*eV,2.440426051	*eV,2.436374282	*eV,2.43133441	*eV,2.425309276	*eV,2.420310307	*eV,2.41433962	*eV,2.410373938	*eV,2.405440952	*eV,2.397580764	*eV,2.388804981	*eV,2.377200348	*eV,2.362854404	*eV,2.35244569	*eV,2.344927487	*eV,2.339322502	*eV,2.33374864	*eV,2.330043087	*eV,2.324508947	*eV,2.319001034	*eV,2.317172302	*eV,2.313519161	*eV,2.311699062	*eV,2.308063144	*eV,2.305346918	*eV,2.301730939	*eV,2.297232206	*eV,2.295433383	*eV,2.290062238	*eV,2.286498228	*eV,2.282054585	*eV,2.277628181	*eV,2.271461649	*eV,2.266201973	*eV,2.2609666	*eV,2.252297252	*eV,2.245408682	*eV,2.236002892	*eV,2.224147425	*eV,2.210752458	*eV,2.199162545	*eV,2.190139427	*eV,2.182814211	*eV,2.169907236	*eV,2.157947861	*eV,2.146119591	*eV,2.135195775	*eV,2.122077651	*eV,2.1060963	*eV,2.08886785	*eV,2.076311132	*eV,2.066084719	*eV,2.053803014	*eV,2.040245488	*eV,2.031072128	*eV,2.017119488	*eV,2.005408133	*eV,1.997226432	*eV,};
  G4double emissionFib[] = {0	,0.002538406	,0.005076812	,0.007615218	,0.017764443	,0.022841255	,0.027918067	,0.035533285	,0.048225315	,0.07106657	,0.096446232	,0.121825893	,0.134517923	,0.164974396	,0.187815651	,0.215733718	,0.241117779	,0.291877101	,0.317256762	,0.33756401	,0.362943671	,0.375635701	,0.401015362	,0.434010242	,0.459389903	,0.477158745	,0.515226037	,0.543148503	,0.560912946	,0.60406145	,0.626902705	,0.637056329	,0.680200433	,0.741117779	,0.786800289	,0.814718356	,0.829948792	,0.847713235	,0.865482077	,0.888323332	,0.916241399	,0.939087054	,0.959389903	,0.972081933	,0.984769564	,1	,0.984769564	,0.959389903	,0.94162106	,0.926395023	,0.908626181	,0.888323332	,0.875631302	,0.860405265	,0.840102416	,0.832487198	,0.81217995	,0.796953913	,0.774112658	,0.753805409	,0.72081053	,0.695430869	,0.672589614	,0.64974396	,0.626902705	,0.596446232	,0.573604977	,0.550759322	,0.525379661	,0.497461594	,0.467005121	,0.444163866	,0.423856617	,0.39593855	,0.368020483	,0.345179228	,0.317256762	,0.294415507	,0.271574252	,0.248728597	,0.228425748	,0.203046087	,0.185277245	,0.17512802	,0.16243599	,0.14974396	,0.137056329	,0.121825893	,0.114215074	,0.098984638	,0.081220195	,0.068528164	,0.060912946	,0.048225315	,0.045686909	,0.027918067	,0.025379661	,0.017764443	,0.002538406	,0	,};
  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);
  //std::cout<<"G4int nEntries:"<<nEntries<<std::endl;
  //for(int i=0;i<nEntries;i++){
  //  absWLSfiber[i]/=100;
  //}

  G4double energySmall[]= { 1.0 * eV, 6.0 * eV };
  G4double refractiveIndexWLSfiber[] = { 1.59, 1.59 };
  //G4double refractiveIndexWLSfiber[] = { 1.764, 1.764 }; //to make c/n = 1.7
  //G4double refractiveIndexWLSfiber[] = { 1.875, 1.875 }; //to make c/n = 1.6
  //cut at 500nm
  //G4double photonEnergy2[] = {1*eV, 2.4799*eV, 2.48*eV, 2.4801*eV, 6.0 * eV };
  //cut at emission end
  G4double photonEnergy2[] = {1*eV, 2.62*eV, 2.621*eV, 6.0 * eV };
  G4double absWLSfiber[] = {500.0*m, 500.0*m, 0.1*mm, 0.1*mm};
  G4double realabs_e[]={2.469788905	*eV,2.46085797	*eV,2.450328967	*eV,2.441473547	*eV,2.431120857	*eV,2.422141814	*eV,2.4119624	*eV,2.403099844	*eV,2.393371904	*eV,2.384929095	*eV,2.375382911	*eV,2.365587418	*eV,2.356763874	*eV,2.348034364	*eV,2.339321619	*eV,2.321568287	*eV,2.295443157	*eV,2.278841635	*eV,2.253935662	*eV,2.2134774	*eV,2.174693096	*eV,};
  G4double realabsWLSfiber[] = { 2.64495	*m,2.40574	*m,2.80068	*m,3.07617	*m,3.37919	*m,3.26865	*m,3.48896	*m,3.58062	*m,3.87446	*m,4.09481	*m,4.09454	*m,4.3516	*m,4.48921	*m,4.37866	*m,4.68171	*m,4.8742	*m,5.11237	*m,5.78281	*m,5.98422	*m,5.97371	*m,5.98159	*m,}; 
  const G4int nEntries_realabs = sizeof(realabs_e)/sizeof(G4double);
  //for(int i=0;i<nEntries_realabs;i++) realabs_e[i]=realabs_e[i];

  // Add entries into properties table
  G4MaterialPropertiesTable* mptWLSfiber = new G4MaterialPropertiesTable();
  mptWLSfiber->AddProperty("RINDEX", energySmall, refractiveIndexWLSfiber, 2);
  mptWLSfiber->AddProperty("WLSCOMPONENT", photonEnergy, emissionFib, nEntries); //WLSCOMPONENT is the relative emission spectrum of the material as a function of the photon's momentum
  //mptWLSfiber->AddProperty("WLSABSLENGTH", photonEnergy, absWLSfiber, nEntries); //WLSABSLENGTH is the absorption length of the material as a function of the photon's momentum. -> re-emit
  mptWLSfiber->AddProperty("WLSABSLENGTH", photonEnergy2, absWLSfiber, 4); //WLSABSLENGTH is the absorption length of the material as a function of the photon's momentum. -> re-emit
  //mptWLSfiber->AddProperty("ABSLENGTH", realabs_e, realabsWLSfiber,nEntries_realabs); //ABSLENGTH is the absorption length of the material as a function of the photon's momentum. -> real absorption
  mptWLSfiber->AddConstProperty("WLSTIMECONSTANT", 11.5 * ns); //accounts for any time delay which may occur between absorption and re-emission of the photon, defalt delta

  fPMMA->SetMaterialPropertiesTable(mptWLSfiber);

  elements.clear();
  natoms.clear();
  return fPMMA;


}

G4Material* Make_Pethylene(){
  //clad 1
  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;

  //--------------------------------------------------
  // Cladding (polyethylene)
  //--------------------------------------------------

  elements.push_back("C");
  natoms.push_back(2);
  elements.push_back("H");
  natoms.push_back(4);

  density = 1.200 * g / cm3;

  G4Material* fPethylene =
    G4NistManager::Instance()->ConstructNewMaterial("Pethylene", elements, natoms, density);
  G4double refractiveIndexClad1[] = { 1.49, 1.49 };
  G4double energySmall[] = { 1.0 * eV, 6.0 * eV };
  G4double absClad[] = { 20.0 * m, 20.0 * m };
  G4MaterialPropertiesTable* mptClad1 = new G4MaterialPropertiesTable();
  mptClad1->AddProperty("RINDEX", energySmall, refractiveIndexClad1, 2);
  mptClad1->AddProperty("ABSLENGTH", energySmall, absClad, 2);

  fPethylene->SetMaterialPropertiesTable(mptClad1);

  elements.clear();
  natoms.clear();

  return fPethylene;

}

G4Material* Make_FPethylene(){

  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;

  //--------------------------------------------------
  // Double Cladding (fluorinated polyethylene)
  //--------------------------------------------------

  elements.push_back("C");
  natoms.push_back(2);
  elements.push_back("H");
  natoms.push_back(4);

  density = 1.400 * g / cm3;

  G4Material* fFPethylene =
    G4NistManager::Instance()->ConstructNewMaterial("FPethylene", elements, natoms, density);

  G4double refractiveIndexClad2[] = { 1.42, 1.42 };
  G4double absClad[] = { 20.0 * m, 20.0 * m };

  // Add entries into properties table 
   G4double energySmall[] = { 2.0 * eV, 3.47 * eV };
  G4MaterialPropertiesTable* mptClad2 = new G4MaterialPropertiesTable();
  mptClad2->AddProperty("RINDEX", energySmall, refractiveIndexClad2, 2);
  mptClad2->AddProperty("ABSLENGTH", energySmall, absClad, 2);

  fFPethylene->SetMaterialPropertiesTable(mptClad2);
  elements.clear();
  natoms.clear();
return fFPethylene;
}



G4Material* Make_Polystyrene(){

  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;

  //--------------------------------------------------
  // Polystyrene
  //--------------------------------------------------

  elements.push_back("C");
  natoms.push_back(8);
  elements.push_back("H");
  natoms.push_back(8);

  density = 1.050 * g / cm3;

  G4Material* fPolystyrene =
    G4NistManager::Instance()->ConstructNewMaterial("Polystyrene", elements, natoms, density);

  G4double energy[] = {
    2.00 * eV, 2.03 * eV, 2.06 * eV, 2.09 * eV, 2.12 * eV, 2.15 * eV, 2.18 * eV,
    2.21 * eV, 2.24 * eV, 2.27 * eV, 2.30 * eV, 2.33 * eV, 2.36 * eV, 2.39 * eV,
    2.42 * eV, 2.45 * eV, 2.48 * eV, 2.51 * eV, 2.54 * eV, 2.57 * eV, 2.60 * eV,
    2.63 * eV, 2.66 * eV, 2.69 * eV, 2.72 * eV, 2.75 * eV, 2.78 * eV, 2.81 * eV,
    2.84 * eV, 2.87 * eV, 2.90 * eV, 2.93 * eV, 2.96 * eV, 2.99 * eV, 3.02 * eV,
    3.05 * eV, 3.08 * eV, 3.11 * eV, 3.14 * eV, 3.17 * eV, 3.20 * eV, 3.23 * eV,
    3.26 * eV, 3.29 * eV, 3.32 * eV, 3.35 * eV, 3.38 * eV, 3.41 * eV, 3.44 * eV,
    3.47 * eV
  };
  G4double energySmall[] = { 2.0 * eV, 3.47 * eV };
  G4double refractiveIndexPS[] = { 1.50, 1.50 };

  G4double absPS[] = { 2. * cm, 2. * cm };

  G4double scintilFast[] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
  };

  // Add entries into properties table
  G4MaterialPropertiesTable* mptPolystyrene = new G4MaterialPropertiesTable();
  mptPolystyrene->AddProperty("RINDEX", energySmall, refractiveIndexPS, 2);
  mptPolystyrene->AddProperty("ABSLENGTH", energySmall, absPS, 2);
  mptPolystyrene->AddProperty("FASTCOMPONENT", energy, scintilFast, 50);
  mptPolystyrene->AddConstProperty("SCINTILLATIONYIELD", 10. / keV);
  mptPolystyrene->AddConstProperty("RESOLUTIONSCALE", 1.0);
  mptPolystyrene->AddConstProperty("FASTTIMECONSTANT", 10. * ns);

  fPolystyrene->SetMaterialPropertiesTable(mptPolystyrene);

  // Set the Birks Constant for the Polystyrene scintillator
  fPolystyrene->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

  elements.clear();
  natoms.clear();
return fPolystyrene;

}
G4Material* Make_Silicone(){

  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;

  //--------------------------------------------------
  // Silicone (Template for Optical Grease)
  //--------------------------------------------------

  elements.push_back("C");
  natoms.push_back(2);
  elements.push_back("H");
  natoms.push_back(6);

  density = 1.060 * g / cm3;

  G4Material* fSilicone =
    G4NistManager::Instance()->ConstructNewMaterial("Silicone", elements, natoms, density);

  G4double refractiveIndexSilicone[] = { 1.46, 1.46 };

  G4double absClad[] = { 20.0 * m, 20.0 * m };
  G4double energySmall[] = { 2.0 * eV, 3.47 * eV };
  // Add entries into properties table
  G4MaterialPropertiesTable* mptSilicone = new G4MaterialPropertiesTable();
  mptSilicone->AddProperty("RINDEX", energySmall, refractiveIndexSilicone, 2);
  mptSilicone->AddProperty("ABSLENGTH", energySmall, absClad, 2);

  fSilicone->SetMaterialPropertiesTable(mptSilicone);

  elements.clear();
  natoms.clear();
  return fSilicone;

}
G4Material* Make_TiO2(){

  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;

  //--------------------------------------------------
  // TiO2
  //--------------------------------------------------

  elements.push_back("Ti");
  natoms.push_back(1);
  elements.push_back("O");
  natoms.push_back(2);

  density = 4.26 * g / cm3;

  G4Material* TiO2 =
    G4NistManager::Instance()->ConstructNewMaterial("TiO2", elements, natoms, density);

  elements.clear();
  natoms.clear(); 
   return TiO2;

}

G4Material* Make_Coating(){

  G4double density;
  G4int ncomponents;
  G4double fractionmass;
  //--------------------------------------------------
  // Scintillator Coating - 15% TiO2 and 85% polystyrene by weight.
  //--------------------------------------------------

  density = 1.52 * g / cm3;

  G4Material* fCoating = new G4Material("Coating", density, ncomponents = 2);

  fCoating->AddMaterial(Make_TiO2(), fractionmass = 15 * perCent);
  fCoating->AddMaterial(Make_Polystyrene(), fractionmass = 85 * perCent);

   return fCoating;

}
