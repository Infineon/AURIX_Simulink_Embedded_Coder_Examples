%% Description: 
% Run this function after running simulateRoundAboutScene to make sure that
% you have already loaded all the data required in the workspace
close all

%% Main
% Load the data
load('refPathRoadRunner.mat');

% Path given as X, Y, Z
roadRunnerPath = squeeze(refPath.Data(:,:,1));

% Calculate the yaw of the Path
stepDistances = sqrt(diff(roadRunnerPath(:,1)).^2 + diff(roadRunnerPath(:,2)).^2 + diff(roadRunnerPath(:,3)).^2);

% Compute the cumulative distance: We start with 0 for the first point
dist = [0; cumsum(stepDistances)];

% Calculate yaw angle in rad
yaw = atan2(gradient(roadRunnerPath(:,2)), gradient(roadRunnerPath(:,1)));

% Calculate pitch in rad -> THIS IS A VERY BASIC ESTIMATION
horizontalDist = sqrt(roadRunnerPath(:,1).^2 + roadRunnerPath(:,2).^2);
pitch          = atan2(gradient(roadRunnerPath(:,3)), horizontalDist);

% Select ID of the desired position
roadRunnerPathID = 450;

% Position that you want to provide Default is [-84.4652, -7.0969, 0]
position = [roadRunnerPath(roadRunnerPathID,:), yaw(roadRunnerPathID), pitch(roadRunnerPathID)];

% Plot position: 
figure('Color','w'); scatter(roadRunnerPath(:,1),roadRunnerPath(:,2),'o'); hold on; grid on; axis equal
scatter(roadRunnerPath(roadRunnerPathID,1),roadRunnerPath(roadRunnerPathID,2),'red','filled','SizeData',100);
xlabel('X direction'); ylabel('Y direction'); title('Complete Path and Selected Position');

% Set position
[InitVehicle,VehicleData] = setInitialPosition("rrAppObj", rrApp,"scenarioSimulationObj", rrSim, "positionXYZ", position);

% Start simulation and wait until RoadRunner Scenario simulation is finished.
set(rrSim,"SimulationCommand","Start");
while strcmp(get(rrSim,"SimulationStatus"),"Running"); pause(1); end



%% Subfunctions:
function [InitVehicle,VehicleData] = setInitialPosition(nvp)
% This setup function enables the user to: 
% 1) Get Handle to the RoadRunner "Sedan" Actor 
% 2) Update the initial coordinate (X, Y, Z) of the Roadrunner model 
% 3) Update the initial coordinate (X, Y, Z) of the corresponding Simscape Multibody model

%% Implementation
arguments
    nvp.scenarioSimulationObj = [];
    nvp.rrAppObj = [];   
    nvp.positionXYZ = [];
end

%% 1) Get Handle to the RoadRunner "Sedan" Actor 
% Read actor profiles from RoadRunner Scenario
worldActor = nvp.scenarioSimulationObj.getScenario();
world      = worldActor.actor_spec.world_spec;

% Loop through all the actors name and create a list
actorNames = arrayfun(@(i) world.actors(i).actor_spec.name, 1:numel(world.actors), 'UniformOutput', false);

% Find the handle of the desired actor -> IF THE ACTORNAME CHANGES YOU HAVE TO UPDATE THE STRING 'Sedan'
actorRunTime = world(find(strcmp(actorNames, 'Sedan'))).actors.actor_runtime; 

%% 2) Modify position of the vehicle in RoadRunner
% Pose matrix containing orientation AND position
actorRunTime.pose.matrix.col3.x = nvp.positionXYZ(1);
actorRunTime.pose.matrix.col3.y = nvp.positionXYZ(2);
actorRunTime.pose.matrix.col3.z = nvp.positionXYZ(3);

% RoadRunner refers the rotation with respect to Y so we have to correct
% the calculated angle to be represented correctly in Roadrunner 
initialOrientation = [nvp.positionXYZ(4)-pi/2, 0,nvp.positionXYZ(5)];
poseForRoadRunner  = eul2rotm(initialOrientation,"ZYX");

% ASSUMING ONLY ROTATION ON Z: We need to update only some of the models
actorRunTime.pose.matrix.col0.x = poseForRoadRunner(1,1);
actorRunTime.pose.matrix.col0.y = poseForRoadRunner(2,1);
actorRunTime.pose.matrix.col0.z = poseForRoadRunner(3,1);
actorRunTime.pose.matrix.col1.x = poseForRoadRunner(1,2);
actorRunTime.pose.matrix.col1.y = poseForRoadRunner(2,2);
actorRunTime.pose.matrix.col1.z = poseForRoadRunner(3,2);
actorRunTime.pose.matrix.col2.x = poseForRoadRunner(1,3);
actorRunTime.pose.matrix.col2.y = poseForRoadRunner(2,3);
actorRunTime.pose.matrix.col2.z = poseForRoadRunner(3,3);

% Now Transform the Position from RoadRunner back in Multibody
% This step has redundant portion of code but is good to check if going
% from Roadrunner-> Simscape Multibody delivers the same position as the
% one that was given to Roadrunner

% Pose matrix containing orientation AND position
m = actorRunTime.pose.matrix;

% Restructure matrix (see https://ch.mathworks.com/help/driving/ug/what-is-roadrunner-pose-matrix.html)
c1 = m.col0; c2 = m.col1; c3 = m.col2; c4 = m.col3;

% Build pose matrix 
pose = [c1.x c2.x c3.x c4.x; ...
        c1.y c2.y c3.y c4.y; ...
        c1.z c2.z c3.z c4.z; ...
        c1.w c2.w c3.w c4.w];

% Extract position as [X, Y, Z] 
initialPos = pose(1:3,4)';

% Extract orientation using Euler Angles in rad -> orientation = [Yaw,Pitch,Roll]
initialOrientation = rotm2eul(pose(1:3, 1:3), "ZYX"); % The default order for Euler angle rotations is "ZYX"

% RoadRunner uses a convention where it refers the yaw with respect to the
% Y axle (see https://ch.mathworks.com/help/driving/ug/what-is-roadrunner-pose-matrix.html)
% to refer the yaw rotation with an X-axle based system, we need to correct by 90°
initialOrientation(1) = initialOrientation(1) + pi/2;

%% 3) Read the NEW position in RoadRunner and use it to Update the Simscape Multibody Initial Position
% Please note that we need to perform a series of coordinate transform between Roadrunner and Simscape Multibody 
% Read Speed from RoadRunner and apply to Simscape Multibody    
egoSetSpeed = str2double(nvp.rrAppObj.getScenarioVariable('egoInitialSpeed')); % get speed, m/s

% Calculate Outputs
[VehicleData, ~] = paramMultibodyVehicle();  

% To assign the speed of the wheels we need the VEHICLE-FIXED coordinate system
initialSpeedVehFixed = [egoSetSpeed;0;0];

[InitVehicle] = paramMultibodyVehicleInitialPose(initialPos, initialOrientation, initialSpeedVehFixed, VehicleData);
end
