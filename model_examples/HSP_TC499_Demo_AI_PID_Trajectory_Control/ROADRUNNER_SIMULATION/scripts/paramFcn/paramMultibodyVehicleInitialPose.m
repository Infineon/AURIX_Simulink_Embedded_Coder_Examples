function [InitVehicle] = paramMultibodyVehicleInitialPose(initialPose, initialOrientation, initialSpeedVehFixed, VehicleData)

    % The velocity vector is assigned currently in the vehicle frame of
    % reference. However, in Simscape Multibody we assign the speed in the
    % world frame reference, so we need to transform it back
  
    % Defaults for vehicle initial position in World coordinate frame
    InitVehicle.Vehicle.px = initialPose(1);  % m
    InitVehicle.Vehicle.py = initialPose(2);  % m
    InitVehicle.Vehicle.pz = initialPose(3);  % m
    
    % Defaults for vehicle initial translational velocity. Represented in vehicle coordinates: 
    % +vx is forward, +vy is left, +vz is up in initial vehicle frame
    InitVehicle.Vehicle.vx  =  initialSpeedVehFixed(1);      % m/s
    InitVehicle.Vehicle.vy  =  initialSpeedVehFixed(2);      % m/s
    InitVehicle.Vehicle.vz  =  initialSpeedVehFixed(3);      % m/s
    
    % Initial vehicle coordinates
    InitVehicle.Vehicle.yaw   = initialOrientation(1);   % rad
    InitVehicle.Vehicle.pitch = initialOrientation(2);   % rad
    InitVehicle.Vehicle.roll  = initialOrientation(3);   % rad
   
    % Set initial position and speed of vehicle and wheels. 
    % ASSUMPTION: THE WHEELS CONSIDER THE VEHICLE-LOCAL SPEED
    InitVehicle.Wheel.wFL = InitVehicle.Vehicle.vx/VehicleData.TireDataF.param.DIMENSION.UNLOADED_RADIUS; %rad/s
    InitVehicle.Wheel.wFR = InitVehicle.Vehicle.vx/VehicleData.TireDataF.param.DIMENSION.UNLOADED_RADIUS; %rad/s
    InitVehicle.Wheel.wRL = InitVehicle.Vehicle.vx/VehicleData.TireDataR.param.DIMENSION.UNLOADED_RADIUS; %rad/s
    InitVehicle.Wheel.wRR = InitVehicle.Vehicle.vx/VehicleData.TireDataR.param.DIMENSION.UNLOADED_RADIUS; %rad/s
end
