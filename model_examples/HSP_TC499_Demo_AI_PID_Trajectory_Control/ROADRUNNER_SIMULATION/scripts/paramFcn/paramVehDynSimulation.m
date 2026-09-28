function paramVehDynSimulation(nvp)
%setUpFunction initializes parameters for
%the model.
% 
% MaxPathPoints name-value argument defines the upper limit on the number
% of path points to read from RoadRunner Scenario.
%
%

%%
arguments
    nvp.scenarioSimulationObj = [];
    nvp.rrAppObj = [];
    nvp.behaviorName = "";
    nvp.MultibodySetting = false;
end

% Get initial position from RoadRunner and provide them in a Simscape-Multibody-Compatible Frame
[initialPos, initialOrientation] = getActorInitialPoseByName("rrAppObj", nvp.rrAppObj,"scenarioSimulationObj", nvp.scenarioSimulationObj, "ActorName", "Sedan");

% Get ego speed from Roadrunner, in this case we have an ego speed along X    
egoSetSpeed = str2double(nvp.rrAppObj.getScenarioVariable('egoInitialSpeed')); % get speed, m/s

% To assign the speed of the wheels we need the VEHICLE-FIXED coordinate system
initialSpeedVehFixed = [egoSetSpeed;0;0];

% Create and Update VehicleData and Camera values
[VehicleData, Camera] = paramMultibodyVehicle();    
[InitVehicle] = paramMultibodyVehicleInitialPose(initialPos,initialOrientation, initialSpeedVehFixed, VehicleData);
[Driver, Maneuver] = paramDriver(VehicleData); 

assignin('base',"InitVehicle",InitVehicle);
assignin('base',"VehicleData",VehicleData);
assignin('base',"Camera",Camera);
assignin('base',"Driver",Driver);
assignin('base',"Maneuver",Maneuver);

end