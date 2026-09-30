% Get config
cfg = Simulink.fileGenControl('getConfig')

% Set config

% set CodeGenFolderStructure to 'TargetEnvironmentSubfolder'
cfg.CodeGenFolderStructure = 'TargetEnvironmentSubfolder';


Simulink.fileGenControl('setConfig', 'config', cfg);
Simulink.fileGenControl('getConfig')
