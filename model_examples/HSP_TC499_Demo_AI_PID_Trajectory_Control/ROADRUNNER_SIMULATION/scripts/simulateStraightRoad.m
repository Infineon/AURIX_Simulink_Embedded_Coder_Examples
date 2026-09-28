%% Description: 
% This is the Main to simulate the scenario straight road. The script will:
% 1) Open the Roadrunner scenario.
% 2) Open the in Simulink model controlling the actor behavior.
% 3) Simulate the scenario.

%% Implementation
%% 0) Installation Folder
% Provide below the installation folder of RoadRunner:
installationFolder = 'C:\Program Files\RoadRunner R2024b';

% Name of the scenario to be simulated
scenario = 'straightRoad';

%% 1) Close open RoadRunner instances
% Close the current RoadRunner Window if already open
if exist("rrApp", "var"); if class(rrApp) == "roadrunner"; rrApp.close();end; end

% Delete previous results if in workspace
if exist("rrSim", "var"); delete(rrSim); end

% Change to the rootFolder 
cd(currentProject().RootFolder);

%% 2) Load the App
% Open and Load the App 
rrProj = strcat(currentProject().RootFolder,"\roadRunner"); 
rrApp = roadrunner(rrProj,InstallationFolder=installationFolder); 

% Start the scenario. The scene is linked to the scenario, so it will open automatically.
rrApp.openScenario(scenario);

rrSim = rrApp.createSimulation();
paramRoadRunnerCamera(rrApp, "follow", actorID=1);
rrSim.set("PacerStatus", "Off");
rrSim.set("MaxSimulationTime", 14);
set(rrSim,"Logging","on");

% Define a Bus object to obtain RoadRunner information in Simulink. To support Reader and Writer blocks, some bus objects must be loaded.
[BusVehicleRuntime, BusActorRuntime] = paramRoadRunnerBusObject();

% Open Simulink model
open_system('straightRoad.slx');

% Set timestep for RoadRunner
RRtimeStep = 0.1; rrSim.set("StepSize", RRtimeStep); 

%% 3) Simulate without dynamics
% Start simulation
set(rrSim,"SimulationCommand","Start");

% Wait until RoadRunner Scenario simulation is finished.
while strcmp(get(rrSim,"SimulationStatus"),"Running"); pause(1); end

% Get log
rrLog = get(rrSim,"SimulationLog");