function [VehicleData, Camera] = paramMultibodyVehicle()
%% Description: 
% Returns a struct `VehicleData` containing body, suspension (front/rear),
% tire, rim, and also returns `Camera`, a struct with camera frame settings 
% used by the Simscape Multibody Mechanics Explorer

% NOTES
%- Indexing convention: X points forward (+), Y to the left (+), Z up (+).
%- Front/rear longitudinal distances (lf, lr) are measured from the CG.
%- Hub heights are synchronized to the UNLOADED_RADIUS from the .tir file.

    
% Vehicle body parameters
VehicleData.Body.FrontAxleOffset = [ 1.5 0 -0.6147];               % m
VehicleData.Body.RearAxleOffset  = [-1.5 0 -0.6147];   % m
VehicleData.Body.Wheelbase       = abs(VehicleData.Body.RearAxleOffset(1) - VehicleData.Body.FrontAxleOffset(1));
VehicleData.Body.GeometryOffset  = [2.75 0.985 -0.85]; % m
VehicleData.Body.Mass            = 1575;            % kg
VehicleData.Body.Inertia         = [600 3000 2100]; % kg*m^2
VehicleData.Body.Color           = [1,0,0]; % RGB
VehicleData.Body.Opacity         = 1.0;
    
% Front suspension parameters: 2 DOF for axle (heave, roll), spin DOF for wheels, and Ackermann steering
VehicleData.SuspF.Heave.Stiffness = 40000;  % N/m
VehicleData.SuspF.Heave.Damping   = 3500;   % N/m
VehicleData.SuspF.Heave.EqPos     = -0.2;   % m
VehicleData.SuspF.Heave.Height    = 0.1647; % m
VehicleData.SuspF.Roll.Stiffness  = 66000;  % N*m/rad
VehicleData.SuspF.Roll.Damping    = 2050;   % N*m/(rad/s)
VehicleData.SuspF.Roll.Height     = 0.0647; % m
VehicleData.SuspF.Roll.EqPos      = 0;      % rad

% Track distance at the front axle
VehicleData.SuspF.Track           = 1.6;    % m
    
% Unsprung mass - radius and length for visualization only
VehicleData.SuspF.UnsprungMass.Mass    = 95;         % kg
VehicleData.SuspF.UnsprungMass.Inertia = [1 1 1]; % kg*m^2
VehicleData.SuspF.UnsprungMass.Height  = 0.355;    % m
VehicleData.SuspF.UnsprungMass.Radius  = 0.1;      % m
VehicleData.SuspF.UnsprungMass.Length  = 1.6;      % m
    
% Synchronize hub height should be synchronized with tire parameters
VehicleData.TireDataF.filename = 'KT_MF_Tool_245_60_R16.tir';
VehicleData.TireDataF.param    = simscape.multibody.tirread(which(VehicleData.TireDataF.filename));
VehicleData.SuspF.Hub.Height   = VehicleData.TireDataF.param.DIMENSION.UNLOADED_RADIUS; % m

% Rim mass and inertia typically not part of .tir file
VehicleData.RimF.Mass          = 10;    % kg
VehicleData.RimF.Inertia       = [1 2]; % kg*m^2

% Steering ratio to be applied at the driver steering command 
VehicleData.SuspF.SteerRatio = 18; % m
    
% Rear suspension parameters: 2 DOF for axle (heave, roll), spin DOF for wheels, and Ackermann steering
VehicleData.SuspR.Heave.Stiffness  = 50000;  % N/m
VehicleData.SuspR.Heave.Damping    = 3500;   % N/m
VehicleData.SuspR.Heave.EqPos      = -0.16;  % m
VehicleData.SuspR.Heave.Height     = 0.1647; % m
VehicleData.SuspR.Roll.Stiffness   = 27500;  % N*m/rad
VehicleData.SuspR.Roll.Damping     = 2050;   % N*m/(rad/s)
VehicleData.SuspR.Roll.Height      = 0.1147; % m
VehicleData.SuspR.Roll.EqPos       = 0;      % rad
    
% Track distance at the rear axle
VehicleData.SuspR.Track            = 1.6;    % m
    
% Unsprung mass - radius and length for visualization only
VehicleData.SuspR.UnsprungMass.Mass    = 90;      % kg
VehicleData.SuspR.UnsprungMass.Inertia = [1 1 1]; % kg*m^2
VehicleData.SuspR.UnsprungMass.Height  = 0.355;   % m
VehicleData.SuspR.UnsprungMass.Length  = 1.6;     % m
VehicleData.SuspR.UnsprungMass.Radius  = 0.1;     % m
    
% Hub height should be synchronized with tire parameters
VehicleData.TireDataR.filename = 'KT_MF_Tool_245_60_R16.tir';
VehicleData.TireDataR.param    = simscape.multibody.tirread(which(VehicleData.TireDataR.filename));
VehicleData.SuspR.Hub.Height   = VehicleData.TireDataF.param.DIMENSION.UNLOADED_RADIUS; % m
    
% Rim mass and inertia typically not part of .tir file
VehicleData.RimR.Mass          = 10;    % kg
VehicleData.RimR.Inertia       = [1 2]; % kg
    
%% Camera data
% These camera frames are used in the Simscape Multibody visualization
Camera =  paramCameraFrame;
end
