function varargout = hybridv2(varargin)
% HYBRIDV2 MATLAB code for hybridv2.fig
%      HYBRIDV2, by itself, creates a new HYBRIDV2 or raises the existing
%      singleton*.
%
%      H = HYBRIDV2 returns the handle to a new HYBRIDV2 or the handle to
%      the existing singleton*.
%
%      HYBRIDV2('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in HYBRIDV2.M with the given input arguments.
%
%      HYBRIDV2('Property','Value',...) creates a new HYBRIDV2 or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before hybridv2_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to hybridv2_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help hybridv2

% Last Modified by GUIDE v2.5 13-Dec-2015 00:46:19

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @hybridv2_OpeningFcn, ...
                   'gui_OutputFcn',  @hybridv2_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before hybridv2 is made visible.
function hybridv2_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to hybridv2 (see VARARGIN)

% Choose default command line output for hybridv2
handles.output = hObject;

%import MASASerial

global controllerSerial;

global matServoAngle;
global matFillState;
global matVentState;
global matOverride;
global matUnlock;

global connected;


global arServoAngle;
global arFillState;
global arVentState;
global arUnlockState;
global arOverrideState;
global arFuseState;
global arIgnitionState;
global arEmergencyState;
global arPressure;
global arAveragePressure;
global arTime;

global arFillTimeOn;
global arVentTimeOn;
global arFillTimeCool;
global arVentTimeCool;

global receiveTimer;
global sendTimer;

global guiTimer;



matServoAngle=str2double(handles.closedAngle.String);
matFillState=0;
matVentState=0;
matOverride=0;
matUnlock=0;


%serial = MASASerial('COM12');
%connected = 1;


receiveTimer = timer();
receiveTimer.TimerFcn = {@receiveSerial,hObject};
receiveTimer.ExecutionMode = 'fixedSpacing';
receiveTimer.Period = .05;
receiveTimer.BusyMode = 'queue';
receiveTimer.Name = 'receiveTimer';

sendTimer = timer();
sendTimer.TimerFcn = {@sendSerial,hObject};
sendTimer.ExecutionMode = 'fixedSpacing';
sendTimer.Period = .1;
sendTimer.BusyMode = 'queue';
sendTimer.Name = 'sendTimer';


guiTimer = timer();
guiTimer.TimerFcn = {@updateGUI,hObject};
guiTimer.ExecutionMode = 'fixedSpacing';
guiTimer.Period = .05;
guiTimer.BusyMode = 'queue';
guiTimer.Name = 'receiveTimer';

%start(receiveTimer);
start(guiTimer);

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes hybridv2 wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = hybridv2_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


function receiveSerial(obj,event,hObject)
global controllerSerial;
global connected;

global arServoAngle;
global arFillState;
global arVentState;
global arUnlockState;
global arOverrideState;
global arFuseState;
global arIgnitionState;
global arEmergencyState;

global arPressure;
global arAveragePressure;
global arTime;

global arFillTimeOn;
global arVentTimeOn;
global arFillTimeCool;
global arVentTimeCool;

if (connected)
     [header data] = getData(controllerSerial);
     if(header ~= 0 & length(data) >= 29)
         arFillState = data(3);
         arVentState = data(4);
         arServoAngle = data(5);
         
         arPressure = bitshift(data(6),8);
         arPressure = arPressure + data(7);
         
         arAveragePressure = bitshift(data(8),8);
         arAveragePressure = arAveragePressure + data(9);
         
         arTime = [data(12) data(11) data(10)];
         
         arOverrideState = data(13);
         
         arEmergencyState = data(14);
         
         arIgnitionState = data(15);
         
         arFuseState = data(16);
         
         arUnlockState = data(17);
         
         arFillState = data(18);
         arVentState = data(19);
         
         arFillTimeOn = bitshift(data(22),8) + data(23);
         arVentTimeOn = bitshift(data(24),8) + data(25);
         
         arFillTimeCool = bitshift(data(26),8) + data(27);
         arVentTimeCool = bitshift(data(28),8) + data(29);
         
     end




end

function sendSerial(obj,event,hObject)
global matServoAngle;
global matFillState;
global matVentState;
global matOverride;
global matUnlock;
global connected;
global controllerSerial;

if (connected)
data = ['M' matServoAngle matFillState matVentState matOverride matUnlock];
sendData(controllerSerial,controllerSerial.SHOUT,data);
end

function updateGUI(obj,event,hObject)
global arServoAngle;
global arFillState;
global arVentState;
global arUnlockState;
global arOverrideState;
global arFuseState;
global arIgnitionState;
global arEmergencyState;
global arPressure;
global arAveragePressure;
global arTime;

global matServoAngle;
global matFillState;
global matVentState;
global matOverride;
global matUnlock;

global arFillTimeOn;
global arVentTimeOn;
global arFillTimeCool;
global arVentTimeCool;


handle = guidata(hObject);

handle.arServoAngle.String = ['Angle: ' num2str(arServoAngle)];
handle.arPressure.String = ['Pressure: ' num2str(arPressure)];
handle.arAveragePressure.String = ['AvPressure: ' num2str(arAveragePressure)];

handle.arTime.String = [num2str(arTime(1)) ':' num2str(arTime(2)) ':' num2str(arTime(3))];

handle.arFillState.String = ['Fill: ' num2str(arFillState)];

handle.arVentState.String = ['Vent: ' num2str(arVentState)];

handle.arFuseState.String = ['Fuse: ' num2str(arFuseState)];

handle.arIgnitionState.String = ['Ignition: ' num2str(arIgnitionState)];

handle.arEmergencyState.String = ['Emergency: ' num2str(arEmergencyState)];

handle.arUnlockState.String = ['Unlocked: ' num2str(arUnlockState)];

handle.arOverrideState.String = ['Override: ' num2str(arOverrideState)];

handle.arFillOn.String = ['Fill On: ' num2str(arFillTimeOn/1000)];
handle.arVentOn.String = ['Vent On: ' num2str(arVentTimeOn/1000)];
handle.arFillCool.String = ['Fill Cool: ' num2str(arFillTimeCool/1000)];
handle.arVentCool.String = ['Vent Cool: ' num2str(arVentTimeCool/1000)];


handle.matServoAngle.String = ['Angle: ' num2str(matServoAngle)];
handle.matFillState.String = ['Fill: ' num2str(matFillState)];
handle.matVentState.String = ['Vent: ' num2str(matVentState)];

handle.matUnlockState.String = ['Unlock: ' num2str(matUnlock)];
handle.matOverrideState.String = ['Override: ' num2str(matOverride)];







function openConnection(handles)
global controllerSerial;
global connected;
global sendTimer;
global receiveTimer;

stop(sendTimer);
stop(receiveTimer);


port = handles.arCOM.String;
%port =  get(handles.arCOM,'String');

try
   
    controllerSerial = MASASerial(port);
    
    if (controllerSerial.connected)
      
       pause(.1);
       data = ['M' 1 1 second(now) minute(now) hour(now)];
       sendData(controllerSerial,controllerSerial.SHOUT,data);
       
       
       
       
       pause(.1);
       
       [header data] = getData(controllerSerial);
       
       pause(.1);
       
      
       
       if (length(data)>0)
          
       if (data(1)=='H')
           connected = 1;
           
           
           
           start(sendTimer);
           start(receiveTimer);
           
           handles.arConnectionStatus.String = ['Arduino Connected on ' port];
           
           
       end
       
       end
    
    end
catch
    disp('failed');
    
end



%{
handles.arConnectionStatus.String = 'Searching';

wrongPort = 1;
nextPort = 1;


while wrongPort
    
    temp = MASASerial(['COM' num2str(nextPort)]);
    
    if (temp.connected)
       pause(.1);
       data = ['M' 1 1 second(now) minute(now) hour(now)];
       sendData(temp,temp.SHOUT,data);
       
       pause(.1);
       
       [header data] = getData(temp);
       if (length(data)>0)
       if (data(1)=='H')
           connected = 1;
           controllerSerial = temp;
           wrongPort = 0;
           
           start(sendTimer);
           start(receiveTimer);
           
           handles.arConnectionStatus.String = ['Arduino Connected on COM' num2str(nextPort)];
           
           
       end
       end
        
    end

    nextPort=nextPort+1;
	
	%if checked all ports, stop trying
	if nextPort==16
		handles.arConnectionStatus.String = 'No Arduino Connected';
		wrongPort = 0;
	end
    delete(temp);

end

%}


% --- Executes on button press in connectController.
function connectController_Callback(hObject, eventdata, handles)
% hObject    handle to connectController (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
openConnection(handles);

% --- Executes on button press in toggleServo.
function toggleServo_Callback(hObject, eventdata, handles)
% hObject    handle to toggleServo (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global matServoAngle;
value = get(hObject,'Value');
if (value==1)
    matServoAngle = str2double(handles.openAngle.String);
    
else
    matServoAngle = str2double(handles.closedAngle.String);
end



% --- Executes on button press in toggleFill.
function toggleFill_Callback(hObject, eventdata, handles)
% hObject    handle to toggleFill (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global matFillState;
matFillState = get(hObject,'Value');


% --- Executes on button press in toggleVent.
function toggleVent_Callback(hObject, eventdata, handles)
% hObject    handle to toggleVent (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global matVentState;
matVentState = get(hObject,'Value');

% --- Executes on button press in unlockIgnition.
function unlockIgnition_Callback(hObject, eventdata, handles)
% hObject    handle to unlockIgnition (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global matUnlock;
matUnlock = get(hObject,'Value');

% --- Executes on button press in overrideEmergency.
function overrideEmergency_Callback(hObject, eventdata, handles)
% hObject    handle to overrideEmergency (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

global matOverride;
matOverride = get(hObject,'Value');


% --- Executes on button press in resetFuse.
function resetFuse_Callback(hObject, eventdata, handles)
% hObject    handle to resetFuse (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function closedAngle_Callback(hObject, eventdata, handles)
% hObject    handle to closedAngle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of closedAngle as text
%        str2double(get(hObject,'String')) returns contents of closedAngle as a double


% --- Executes during object creation, after setting all properties.
function closedAngle_CreateFcn(hObject, eventdata, handles)
% hObject    handle to closedAngle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function openAngle_Callback(hObject, eventdata, handles)
% hObject    handle to openAngle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of openAngle as text
%        str2double(get(hObject,'String')) returns contents of openAngle as a double


% --- Executes during object creation, after setting all properties.
function openAngle_CreateFcn(hObject, eventdata, handles)
% hObject    handle to openAngle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object deletion, before destroying properties.
function figure1_DeleteFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global receiveTimer;
global sendTimer;
global guiTimer;

global controllerSerial;


stop(timerfindall);
delete(timerfindall);


fclose(instrfind);
delete(instrfind);

%{
stop(receiveTimer);
delete(receiveTimer);
stop(sendTimer);
delete(sendTimer);
stop(guiTimer);
delete(guiTimer);
delete(serial);
%}



function arCOM_Callback(hObject, eventdata, handles)
% hObject    handle to arCOM (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of arCOM as text
%        str2double(get(hObject,'String')) returns contents of arCOM as a double


% --- Executes during object creation, after setting all properties.
function arCOM_CreateFcn(hObject, eventdata, handles)
% hObject    handle to arCOM (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in configMenu.
function configMenu_Callback(hObject, eventdata, handles)
% hObject    handle to configMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns configMenu contents as cell array
%        contents{get(hObject,'Value')} returns selected item from configMenu


% --- Executes during object creation, after setting all properties.
function configMenu_CreateFcn(hObject, eventdata, handles)
% hObject    handle to configMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in startConfig.
function startConfig_Callback(hObject, eventdata, handles)
% hObject    handle to startConfig (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

global controllerSerial;

port = handles.arCOM.String;
disp('going');
contents = cellstr(handles.configMenu.String);
selected = contents{handles.configMenu.Value};
disp('got contents')
if (strcmp(selected, 'Write General'))
    destination = 0;
elseif (strcmp(selected, 'Write Servo'))
    destination = 1;
end

if (destination == 0)
    dataLength = 11;
elseif (destination == 1)
    dataLength = 19;
end

%port =  get(handles.arCOM,'String');
disp('starting serial');
%try
   
    controllerSerial = MASASerial(port);
    disp('opened port')
    if (controllerSerial.connected)
      
       pause(.1);
       data = ['M' destination uint8(bitshift(dataLength,-8)) uint8(dataLength)];
       sendData(controllerSerial,controllerSerial.CONFIG,data);
       
       disp('sent data');
       
       
       pause(.1);
       
       [header data] = getData(controllerSerial);
       
       pause(.1);
       
       disp('got data');
       
       
       if (length(data)>0)
          
       disp('data > 0');
           
       if (data(1)=='H')
           
           disp('data1 = H');
           
           handles.arConnectionStatus.String = ['Config Started on ' port];
           
           disp('updated string');
           
           
           data = [45 155 uint8(bitshift(1000,-8)) uint8(1000) ...
               uint8(bitshift(100,-8)) uint8(100) uint8(bitshift(50,-8)) uint8(50) ...
               1 uint8(bitshift(9600,-8)) uint8(9600)];
           
           disp('created data');
           
           
           lastSent = 1;
           
           responseReceived = 1;
           
           while (messageAvailable(controllerSerial)==true)
               getData(controllerSerial);
           end
           
           while (lastSent < length(data))
              pause(.25);
              % disp('started loop');
               
              if (responseReceived == 1)
              send = ['M' data(lastSent) (lastSent-1)];
              
              disp('message received');
              disp(send);
              
              pause(.1);
              
              responseReceived = 0;
              lastSent = lastSent + 1;
              end
              
              sendData(controllerSerial,controllerSerial.CONFIG,send);
              
              disp('sent');
              if (messageAvailable(controllerSerial)==true)
                  disp('message available');

                 [header dataIn] = getData(controllerSerial);
                 disp(header);
                 disp(dataIn);
                 
                 if (header == controllerSerial.CONFIG_RESPONSE)
                     disp('got message');
                     responseReceived = 1;
                 end
                 
                 
                 pause(.1);
              end
               
               
           end
           
           
           disp('finished');
           
           
       end
       
       end
    
    end
%catch
   % disp('failed');
    
%end



function byte = hex(str)
			byte = uint8(hex2dec(str));


