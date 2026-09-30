%% Description: 
% This is the Main to simulate the scenario straight road. The script will:
% 1) Open the Roadrunner scenario.
% 2) Open the corresponding physics model (in Simulink and Simscape).
% 3) Instantiate the required parameters for the physics model.
% 4) Simulate the model and collect the results. 

%% 0) Installation Folder
% Provide below the installation folder of RoadRunner:
installationFolder = 'C:\Program Files\RoadRunner R2024b';

% Name of the scenario to be simulated
scenario = 'roundAbout';

% Change to the rootFolder 
cd(currentProject().RootFolder);

%% 1) Set up RoadRunner
% Close the current RoadRunner Window if already open
if exist("rrApp", "var"); if class(rrApp) == "roadrunner"; rrApp.close();end; end

% Delete previous results if in workspace
if exist("rrSim", "var"); delete(rrSim); end; setenv("NO_PROXY", "localhost");

% Open and Load the App 
rrProj = strcat(currentProject().RootFolder,"\roadRunner"); 
rrApp = roadrunner(rrProj,InstallationFolder=installationFolder); 

% Load the scenario. The scene is linked to the scenario, so it will open automatically.
rrApp.openScenario(scenario);

% Simulation settings
rrSim = rrApp.createSimulation(); set(rrSim,"Logging","on");
paramRoadRunnerCamera(rrApp, "follow", actorID=1);
rrSim.set("PacerStatus", "Off");
rrSim.set("MaxSimulationTime", 60);

% Define a Bus object to obtain RoadRunner information in Simulink. To support Reader and Writer blocks, some bus objects must be loaded.
[BusVehicleRuntime, BusActorRuntime] = paramRoadRunnerBusObject();

% Open Simulink model
open_system('simscapeMultibodyVehicle.slx');

% Align the execution step size of RoadRunner and Simulink.
timeStep   = 0.01;
RRtimeStep = 0.1;
rrSim.set("StepSize", RRtimeStep); 

%% 2) Simulate with Simscape Multibody
% Load terrain data
load('roundAboutTerrainData.mat')

% This function is needed to create the bus object used by the path reader 
helperCreatePathTargetBus(1000)

% Assign Parameters to vehicle and driver + provide initial conditions for vehicle model 
paramVehDynSimulation("rrAppObj", rrApp,"scenarioSimulationObj", rrSim, "behaviorName", 'SimscapeMultibody.rrbehavior');

% Start simulation and wait until RoadRunner Scenario simulation is finished.
set(rrSim,"SimulationCommand","Start");
while strcmp(get(rrSim,"SimulationStatus"),"Running"); pause(1); end

% Get log
rrLog = get(rrSim,"SimulationLog");