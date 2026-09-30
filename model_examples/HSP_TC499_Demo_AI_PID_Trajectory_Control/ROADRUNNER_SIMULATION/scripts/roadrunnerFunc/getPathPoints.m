classdef getPathPoints < matlab.System
    % This System Object processes raw 500x3 trajectory data from RoadRunner.
    % Since the input buffer may be partially empty (zero-padded), this block 
    % dynamically truncates the vector to return only valid path points. A MATLAB
    % System block is utilized here instead of a MATLAB Function block to natively 
    % support variable-size signal propagation and to ensure robust memory allocation 
    % during the initialization phase

    properties (Access = public)
        timeStep        = -1;
        interpNumPoints = 500; 
    end
    
    methods(Access = protected)
        function [RefPoints] = stepImpl(obj, trajectory, numTrajPoints)
            % Remove repetitive waypoints
            [~,uniqueId] = unique(trajectory(1:numTrajPoints,1),'stable');
            trajectory = trajectory(uniqueId,:);
    
            % Interpolate based on the given number of breakpoints (assigned from the user)
            cumDistance         = [0, cumsum(vecnorm(diff(trajectory),2,2))'];
            cumDistanceResample = linspace(0, cumDistance(end), obj.interpNumPoints);
    
            % Interpolate the position
            RefPoints = zeros(obj.interpNumPoints,3);
            RefPoints(:,1) = interp1(cumDistance,trajectory(:,1),cumDistanceResample);
            RefPoints(:,2) = interp1(cumDistance,trajectory(:,2),cumDistanceResample);
    
            % add z value and pitch information
            RefPoints(:,3) = interp1(cumDistance,trajectory(:,3),cumDistanceResample);
        end



        function [out] = getOutputSizeImpl(obj)
            % Return size for each output port
            out = [obj.interpNumPoints 3];
        end

        function [out] = getOutputDataTypeImpl(~)
            % Return data type for each output port
            out = "double";
        end

        function [out] = isOutputComplexImpl(~)
            % Return true for each output port with complex data
            out = false;
        end

        function [out] = isOutputFixedSizeImpl(~)
            % Return true for each output port with fixed size
            out = true;
        end
        
    end

    methods(Access = protected, Static)
        function simMode = getSimulateUsingImpl
            % Return only allowed simulation mode in System block dialog
            simMode = "Interpreted execution";
        end
        function sts = getSampleTimeImpl(obj)
            if obj.timeStep == -1
                sts = createSampleTime(obj,'ErrorOnPropagation','Controllable');
            else
                sts = createSampleTime(obj,'Type','Discrete','SampleTime', obj.timeStep);
            end
        end
    end
end