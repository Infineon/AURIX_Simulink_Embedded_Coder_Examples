function plotSimulationResults(rrLog)

%% Plot results
% Get vehicle speed
velocityAgent1 = get(rrLog,'Velocity','ActorID',1); % Get speed of Actor1
simTime = [velocityAgent1.Time];% Get time of Actor1

% Get vehicle movement
poseActor1 = rrLog.get('Pose','ActorID',1);
positionX = arrayfun(@(x) x.Pose(1,4),poseActor1);
positionY = arrayfun(@(x) x.Pose(2,4),poseActor1); 
positionZ = arrayfun(@(x) x.Pose(3,4),poseActor1); 

velocityX = arrayfun(@(x) x.Velocity(1,1), velocityAgent1);
velocityY = arrayfun(@(x) x.Velocity(1,2), velocityAgent1);
velocityZ = arrayfun(@(x) x.Velocity(1,3), velocityAgent1);

subplot(2,3,1);
plot(simTime,positionX,'LineWidth',1.5); grid on; 
xlabel('Time in sec'); ylabel('PosX in m');

subplot(2,3,2);
plot(simTime,positionY,'LineWidth',1.5); grid on; 
xlabel('Time in sec'); ylabel('PosY in m');

subplot(2,3,3);
plot(simTime,positionZ,'LineWidth',1.5); grid on; 
xlabel('Time in sec'); ylabel('PosZ in m');

subplot(2,3,4);
plot(simTime,velocityX,'LineWidth',1.5); grid on; 
xlabel('Time in sec'); ylabel('SpeedX in km/h');

subplot(2,3,5);
plot(simTime,velocityY,'LineWidth',1.5); grid on; 
xlabel('Time in sec'); ylabel('SpeedY in km/h');

subplot(2,3,6);
plot(simTime,velocityZ,'LineWidth',1.5); grid on; 
xlabel('Time in sec'); ylabel('SpeedZ in km/h');







end