
lastState = 0;

	textTuto = Mesh.createText{ str =" " , scale=Vector(1,1,1), font="agency",	lineWidth = 1.5, pos=Vector ( 0,0.6,0), alignx="center", color= "white" }
	textTuto:visible(false)
function DrawTuto (string )
	if textTuto ~= nil then
	textTuto:visible(true)
	textTuto:text(string)
	textTuto:position (Vector ( 0,-0.75,0	));
	end
end

function ChangeShaderStick ( state )
	if state ~= lastState then
		if state == 0 then
			tStickPlane:shader( Shader_Arc_Of_Bolt_Left );
		elseif state == 1 then
			tStickPlane:shader( Shader_Activate_Orb );
			print ( " orb activate")
		elseif state == 2 then
			tStickPlane:shader( Shader_Simple_Bolt);
		elseif state == 3 then
			tStickPlane:shader( Shader_Curve_Right );
		elseif state == 4 then
			tStickPlane:shader( Shader_Curve_Left );
		end
		lastState = state;
	end
end

function HideTutoText ()
	if textTuto ~= nil then
		textTuto:release()
		textTuto = nil
	end
end

function StartTuto ()
	tStickPlane:shader( Shader_Arc_Of_Bolt_Left );
end 

function StopTuto()
	tSpellButton:visible (false)
	tStickPlane:visible (false )
	mHudWatch:visible ( false )
end 