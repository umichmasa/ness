


classdef MASASerial < handle
	
	

	
	properties (GetAccess=private)
		
		
	end
	
	properties (Constant)
		PACKET_SIZE = 64;
		DATA_SIZE = 32;
		OVERHEAD_SIZE = 5;
		
		SOP = hex('FF');
		EOP = 126;
		
		RESET = hex('00');
		INCOMPLETE = hex('01');
		COMPLETE = hex('02');
		SHOUT = hex('DF');
        CONFIG = hex('CF');
        CONFIG_RESPONSE = hex('D0');
	end
	
	properties (Dependent)
		
	end
	
	properties
		%PRIVATE-----
		port;
		updateTimer;
        
		inMessage = false;
		
		connected = false;
		
		bufferIn;
		bufferOut;
		%------------------
	end
	
	methods
		
		
		function this=MASASerial(COM)
			this.updateTimer = timer();
            this.port = serial('COM0');
			try
				%disp('start init');
				this.port = serial(COM);
				set(this.port,'BaudRate', 9600);
				set(this.port,'DataBits',8);
				set(this.port,'StopBits',2);
				set(this.port,'Parity','odd');
                set(this.port,'Terminator',this.EOP);
                
				
				fopen(this.port);
				
				%disp('opened port ');
				
				
				%obj.updateTimer.TimerFcn = {@MASASerial.updateSerial,obj};
				this.updateTimer.TimerFcn = @MASASerial.updateSerial;
				
				this.updateTimer.ErrorFcn = {@MASASerial.timerError};
				this.updateTimer.ExecutionMode = 'fixedSpacing';
				this.updateTimer.Period = .01;
				this.updateTimer.BusyMode = 'queue';
				this.updateTimer.Name = 'updateTimer';
				this.updateTimer.UserData = this;
				
                
                %this = this.updateTimer.UserData;
				
				start(this.updateTimer);
               
				
				
				
				this.connected = true;
				%disp(['connected = ' this.connected]);
            catch
                if (strcmp(class(this.port),'serial'))
                fclose(this.port);
				delete(this.port);
                end
				%if (isvalid(this.updateTimer))
                if (strcmp(class(this.updateTimer),'timer'))
				stop(this.updateTimer);
				delete(this.updateTimer);
                end
                    
				%end
				%error('Initialization failed');
                this.connected = false;
			end
			
			
			%set(obj.port,'TimeOut',5);
		end
		
		function in = messageAvailable(this)
			in = this.inMessage;
			
		end
		
		function fopen(this)
			fopen(this.port);
		end
		
		function fclose(this)
			fclose(this.port);
		end
		
		function buffer = fread(this,size)
			buffer = fread(this.port,size);
			
		end
		
		function delete(this)
			
            if (strcmp(class(this.port),'serial'))
                fclose(this.port);
                delete(this.port);
            end
            
            if (strcmp(class(this.updateTimer),'timer'))
                stop(this.updateTimer);
                delete(this.updateTimer);
            end
			
		end
		
		function conn = isConnected(this)
			conn = this.connected;
		end
		
		
		
		function sent = sendPacket(this, packet)
			try
				if (this.connected && ~isempty(packet))
					%disp('sending');
					for i = 1:length(packet)
						fwrite(this.port,packet(i));
					end
					%disp('finished sending');
					sent = true;
				
				else
					disp('not connected');
				end
			catch
				sent = false;
			end
		end
		
		%Add checksum, idiot
		function sent = sendData(this, header, data)
			try
				if (this.connected)
					buffer = [MASASerial.SOP, header, cast(length(data),'uint8')];
					%disp('filled buffer');
					
					for i = 1:length(data)
						buffer = [buffer data(i)];
					end
					
					buffer = [buffer this.checksum(buffer) MASASerial.EOP];
					
					
					sent = this.sendPacket(buffer);
					%disp('called send');
					
				else
					%disp('not connected');
					sent = false;
				end
			catch
				error('message failed');
				
			end
			
		end
		
		function [header, data] = getData(this)
			if (this.messageAvailable())
				
				if (length(this.bufferIn) > MASASerial.OVERHEAD_SIZE)
				header = this.bufferIn(2);
				data = this.bufferIn(4:(3+this.bufferIn(3)));
				this.inMessage = false;
				this.bufferIn = zeros(0);
                flushinput(this.port)
				else
					error('out of bounds');
				end
			else
				header = zeros(0);
				data = zeros(0);
			end
			
			
		end
		
		
		function sum = checksum(~, message)
			sum = 0;
			
			
			for i = 1:(message(3)+3)
				sum = MASASerial.addBytes(sum,message(i));
				
				
				
				%sum = sum + message(i);
				%if(sum>255)
				%	sum = sum - 256;
				%end
			
			end
			
			
		end
		
		
		function state = inputState(this)
			%disp('looking at input')
			
			if (isempty(this.bufferIn))
				state = MASASerial.INCOMPLETE;
				return;
			end
				
			if (length(this.bufferIn) < MASASerial.OVERHEAD_SIZE)
				state = MASASerial.INCOMPLETE;
				return;
			end
			
			
			if (~isempty(this.bufferIn)&&this.bufferIn(1)==uint8(0))
				state = MASASerial.INCOMPLETE;
				return;
			end
			
			if (~isempty(this.bufferIn)&&this.bufferIn(1)~=MASASerial.SOP)
				state = MASASerial.RESET;
				%disp('wrong sop');
				return;
			end
			
			if (length(this.bufferIn)>1&&this.bufferIn(2)==uint8(0))
				state = MASASerial.INCOMPLETE;
				return;
			end
			
			if (length(this.bufferIn)>2&&this.bufferIn(3)==uint8(0))
				state = MASASerial.INCOMPLETE;
				return;
			end
			
			if (length(this.bufferIn)>3&&length(this.bufferIn) < (MASASerial.OVERHEAD_SIZE + this.bufferIn(3)))
				state = MASASerial.INCOMPLETE;
				return;
			end
			
			if ((this.bufferIn(this.bufferIn(3)+4))~=(this.checksum(this.bufferIn)))
				state = MASASerial.RESET;
				disp('failed checksum');
				disp(this.bufferIn);
				disp((this.bufferIn(this.bufferIn(3)+4)));
				disp(this.checksum(this.bufferIn));
				return;
			end
			
			if (uint8(this.bufferIn(length(this.bufferIn))==MASASerial.EOP))
				state = MASASerial.COMPLETE;
				return;
				
			else
				state = MASASerial.RESET;
				return;
			end
			
			disp('yikes');
			
			state = MASASerial.RESET;
			
		end
		
	end
	
	
	
	methods (Static)
		
		
		%inputState
		function serialOBJ = updateSerial(timer,~)
			serialOBJ = timer.UserData;
			
			%disp('started update')
			if(~serialOBJ.inMessage)
				%disp('not available')
				while (serialOBJ.port.BytesAvailable>0)
					serialOBJ.bufferIn = [serialOBJ.bufferIn fread(serialOBJ.port,1)];
					%disp('found a byte')
					%serialOBJ.lastIndex = serialOBJ.lastIndex+1;
				end
				%disp('checked bytes');
				
			if (serialOBJ.inputState()==MASASerial.RESET)
				serialOBJ.bufferIn = uint8(zeros(0));
                serialOBJ.inMessage = false;
				%disp('reset')
				
			elseif ((~serialOBJ.inMessage)&&serialOBJ.inputState()==MASASerial.COMPLETE)	
				serialOBJ.inMessage = true;
				%disp('complete')
				
				%disp(serialOBJ.inAvailable);
			end
			
			if (length(serialOBJ.bufferIn) > MASASerial.PACKET_SIZE*3)
					serialOBJ.inMessage = false;
					serialOBJ.bufferIn = zeros(0);
			end
			
			else
				if (serialOBJ.port.BytesAvailable > MASASerial.PACKET_SIZE*3)
					serialOBJ.inMessage = false;
					serialOBJ.bufferIn = zeros(0);
				end
				
				if (length(serialOBJ.bufferIn) > MASASerial.PACKET_SIZE*3)
					serialOBJ.inMessage = false;
					serialOBJ.bufferIn = zeros(0);
				end
				
				
				
			end
			
			timer.UserData = serialOBJ;
			%timer.UserData
        end
		
        
        
		function timerError(~,~)
			%start(timerOBJ);
			error('timer failure');
			
		end
		
		function sum = addBytes(a,b)
			a = cast(a,'int32');
			b = cast(b,'int32');
			
			if (a+b) < 256
				sum = cast(a+b,'uint8');
			else
				sum = cast(a+b-256,'uint8');
			end
		end
		
		
		
	end
	
	methods (Access = private)
		
		
		
	end
	
	
end


function byte = hex(str)
			byte = uint8(hex2dec(str));
end


