function [BusVehicleRuntime, BusActorRuntime] = paramRoadRunnerBusObject()
% For the simulation one of the bus type from RoadRunner is needed. Load
% all the bus definition types and select as output the one which is needed

%% Implementation
if isMATLABReleaseOlderThan("R2025a")
    %load(fullfile(matlabroot,'toolbox','driving','drivingdata','rrScenarioSimTypes.mat'));
    eval(sprintf('load(''%s'')', fullfile(matlabroot,'toolbox','driving','drivingdata','rrScenarioSimTypes.mat')));
else % path is changed from R2025a
    % load(fullfile(matlabroot,'toolbox','driving', 'core', 'drivingdata','rrScenarioSimTypes.mat'));
    eval(sprintf('load(''%s'')', fullfile(matlabroot,'toolbox','driving', 'core', 'drivingdata','rrScenarioSimTypes.mat')));
end

end