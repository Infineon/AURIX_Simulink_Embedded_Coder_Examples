function [position, orientation] = getActorInitialPoseByName(nvp)
%% Add description


%% Output:
% position    [3x1] in m as [x,y,z]
% orientation [3x1] in rad as [Yaw, Pitch, Roll] 

%% Example: getActorInitialPoseByName("rrAppObj", rrApp,"scenarioSimulationObj", rrSim, "ActorName", "Sedan");

%%
arguments
    nvp.scenarioSimulationObj = [];
    nvp.rrAppObj = [];
    nvp.ActorName = "";
end

% Read actor profiles from RoadRunner Scenario
worldActor = nvp.scenarioSimulationObj.getScenario();
world      = worldActor.actor_spec.world_spec;

% Loop through all the actors name and create a list
actorNames = arrayfun(@(i) world.actors(i).actor_spec.name, 1:numel(world.actors), 'UniformOutput', false);

% Find the id of the desired actor
actorID = find(strcmp(actorNames, nvp.ActorName));

% Get actor_runtime field
actorRunTime = world(actorID).actors.actor_runtime; 

% Pose matrix containing orientation AND position
m = actorRunTime.pose.matrix;

% Restructure matrix (see https://ch.mathworks.com/help/driving/ug/what-is-roadrunner-pose-matrix.html)
c1 = m.col0;
c2 = m.col1;
c3 = m.col2;
c4 = m.col3;

% Build pose matrix 
pose = [c1.x c2.x c3.x c4.x; ...
        c1.y c2.y c3.y c4.y; ...
        c1.z c2.z c3.z c4.z; ...
        c1.w c2.w c3.w c4.w];

% Extract position as [X, Y, Z] 
position = pose(1:3,4)';

% Extract orientation using Euler Angles in rad -> orientation = [Yaw,Pitch,Roll]
orientation = rotm2eul(pose(1:3, 1:3), "ZYX"); % The default order for Euler angle rotations is "ZYX"

% RoadRunner uses a convention where it refers the yaw with respect to the
% Y axle (see https://ch.mathworks.com/help/driving/ug/what-is-roadrunner-pose-matrix.html)
% to refer the yaw rotation with an X-axle based system, we need to correct by 90°
orientation(1) = orientation(1) + pi/2;
end
