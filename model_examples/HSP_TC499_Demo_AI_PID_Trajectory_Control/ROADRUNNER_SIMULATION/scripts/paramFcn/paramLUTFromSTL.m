% Provide the name of the STL you want to use
stlName = 'simpleLoop2_doubleLane.stl'; 

% Read the file:
STL_data = stlread(stlName);

% Find the smallest integer exponent k such that 2^k is not less than n
k = ceil(log2(sqrt(length(STL_data.Points(:,1))))); 
Number_d = (2^(k+2)); % Calculate the next power of 2

% Create the meshgrid vectors
xi = linspace(min(STL_data.Points(:,1)), max(STL_data.Points(:,1)), Number_d);
yi = linspace(min(STL_data.Points(:,2)), max(STL_data.Points(:,2)), Number_d);

% Create meshgrid 
[XI, YI] = meshgrid(xi, yi);
ZI = griddata(STL_data.Points(:,1), STL_data.Points(:,2), STL_data.Points(:,3), XI, YI, 'linear');

% Clean and transpose data
ZI(isnan(ZI)) = 0; ZI = ZI';

% Plot the results 
hold on
s = surf(xi,yi,ZI,EdgeColor='none',LineStyle='none');
axis equal
hold off

% Store terrain data
Terrain.breakPointsX = xi;
Terrain.breakPointsY = yi;
Terrain.breakPointsZ = ZI;
Terrain.stlFileName = stlName; 

% Save the results
save([strrep(stlName,'.stl',''),'TerrainData.mat'],'Terrain')

