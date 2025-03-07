#include "detcon.hh"
#include "actioninit.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "Shielding.hh"
#include "QGSP_BERT_HP.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4ScoringManager.hh"

#include "G4GDMLParser.hh"

int main(int argc, char **argv) {
    // Instantiate G4UIExecutive if there are no arguments (interactive mode)
    G4UIExecutive *ui = nullptr;
    if (argc == 1) ui = new G4UIExecutive(argc, argv);

    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

    // Construct the run manager
#ifdef G4MULTITHREADED
    G4MTRunManager *runManager = new G4MTRunManager();
    runManager->SetNumberOfThreads(48);
#else
    G4RunManager* runManager = new G4RunManager();
#endif

    // Activate UI-command base scorer
    G4ScoringManager * scManager = G4ScoringManager::GetScoringManager();
    scManager->SetVerboseLevel(1);

    // Parse GDML file
    G4GDMLParser parser;
    parser.SetStripFlag(false);
    parser.SetOverlapCheck(true);
    parser.Read("AstranisGDML-worldVOL.gdml"); // Replace with your GDML file path
    auto detector = new detcon(parser);
    runManager->SetUserInitialization(detector);

    // Set mandatory initialization classes
    auto physics = new QGSP_BERT_HP();
    runManager->SetUserInitialization(physics);

    // Set user action classes through Worker Initialization
    runManager->SetUserInitialization(new actioninit());

    // Visualization manager
    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    // Initialize G4 kernel
    runManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (!ui) {
        // Define UI session for interactive mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {
        // Batch mode
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    //                 owned and deleted by the run manager, so they should not
    //                 be deleted in the main() program !
    delete visManager;
    delete runManager;
}
