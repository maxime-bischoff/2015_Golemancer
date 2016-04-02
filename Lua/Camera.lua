cam = Camera.getActive();
cam:backColor(Vector(0,0,0,1))
cam:position( 0,0,-3)
cam:lookat( 0,0,1)
-- camTransform = Transform.new()
 cam:position( tPlayer:position().x-2, 2, tPlayer:position().z )
-- cam:parent(camTransform)
print ( "cameraposition" .. cam:position().x ..  cam:position().y ..  cam:position().z)
CameraPositionStart = Vector ( 0,2,0)

merge("Data/Fonts/FontArial42.pak")
font = Font.find("arial")

textCameraPosition = 1.6
textFov = Mesh.createText{ str ="FOV [+/-]" , scale=Vector(0.7,0.7,0.7), font="arial",	            pos=Vector ( textCameraPosition,0.9,0), alignx="right", color= "white" }
textHeight = Mesh.createText{ str ="HEIGHT [1/7]" , scale=Vector(0.7,0.7,0.7), font="arial",          pos=Vector ( textCameraPosition,0.8,0), alignx="right", color= "white" }
textDistance = Mesh.createText { str = "DISTANCE [3/9]", scale=Vector(0.7,0.7,0.7), font="arial",     pos=Vector ( textCameraPosition,0.7,0), alignx="right", color= "white" }
textOffset = Mesh.createText { str = "OFFSET [4/6]", scale=Vector(0.7,0.7,0.7), font="arial",         pos=Vector ( textCameraPosition,0.6,0), alignx="right", color= "white" }
textLookAtHeight = Mesh.createText { str = "HEIGHT2 [2/8]", scale=Vector(0.7,0.7,0.7), font="arial",  pos=Vector ( textCameraPosition,0.5,0), alignx="right", color= "white" }

textFov:visible ( false ) 
textHeight:visible ( false ) 
textDistance:visible ( false ) 
textOffset:visible ( false ) 
textLookAtHeight:visible ( false ) 

cam:range(0.05,1000)


CAM_FOV_UP = 0
CAM_FOV_DOWN = 1
CAM_DISTANCE_UP = 2
CAM_DISTANCE_DOWN = 3
CAM_HEIGHT_UP = 4
CAM_HEIGHT_DOWN = 5
CAM_LOOKAT_UP = 6
CAM_LOOKAT_DOWN = 7

CAM_LATERAL_OFFSET_UP = 11
CAM_LATERAL_OFFSET_DOWN = 12

CAM_PARAM_PRINT = 8

fFovChangeSpeed = 10
fCamMoveSpeed = 5

Input.setCtrlMapping(CAM_FOV_UP ,	"KB_ADD" )
Input.setCtrlMapping(CAM_FOV_DOWN ,	"KB_SUBTRACT" )
Input.setCtrlMapping(CAM_DISTANCE_UP ,	"KB_NUMPAD8" )
Input.setCtrlMapping(CAM_DISTANCE_DOWN ,	"KB_NUMPAD2" )
Input.setCtrlMapping(CAM_HEIGHT_UP ,	"KB_NUMPAD9" )
Input.setCtrlMapping(CAM_HEIGHT_DOWN ,	"KB_NUMPAD3" )

Input.setCtrlMapping(CAM_LOOKAT_UP ,	"KB_NUMPAD7" )
Input.setCtrlMapping(CAM_LOOKAT_DOWN ,	"KB_NUMPAD1" )

Input.setCtrlMapping(CAM_LATERAL_OFFSET_UP ,	"KB_NUMPAD6" )
Input.setCtrlMapping(CAM_LATERAL_OFFSET_DOWN ,	"KB_NUMPAD4" )

Input.setCtrlMapping(CAM_PARAM_PRINT ,	"KB_DELETE" )




tCamLookAtOffset =  Vector(0,0,0)

fCamHeight = 1.5
fCamOffset = 0

fCameraShakeStrength = 0
fCameraShakeAttenuation = 0.01

function round2(num, idp)
  local mult = 10^(idp or 0)
  return math.floor(num * mult + 0.5) / mult
end

function round3(num, idp)
  return tonumber(string.format("%." .. (idp or 0) .. "f", num))
end

function round5(num, idp)
  if idp and idp>0 then
    local mult = 10^idp
    return math.floor(num * mult + 0.5) / mult
  end
  return math.floor(num + 0.5)
end

function round(n)
	    return n % 1 >= 0.5 and math.ceil(n) or math.floor(n)
end

function round3(t)
    return round(t*10)*0.1
end



function UpdateCamera( PosX, PosY, PosZ, nFOV, nLookatX, nLookatY, nLookatZ, nDistance, nOffset )

	cam:position(PosX, PosY , -PosZ)
	lookatcorrect = Vector( PosX, PosY , -PosZ) + (  Vector( nLookatX, nLookatY, -nLookatZ) - Vector( PosX, PosY , -PosZ) ) * 4000

	--cam:lookat(Vector(nLookatX, nLookatY, -nLookatZ))
	cam:lookat(lookatcorrect)
	cam:fov(nFOV)

	textFov:text("[-/+] Fov : " .. String.float(nFOV,2))
	textHeight:text("[2/8] Height : " .. String.float(PosY,2))
	textDistance:text("[3/9] Distance : " .. String.float(nDistance,2))
	textOffset:text("[4/6] Offset : " .. String.float(nOffset,2))
	textLookAtHeight:text("[1/7] LookAt : " .. String.float(nLookatY,2))

	ParamCamera()

	UpdateScreenShake()
end


function ShakeScreen(strength, attenuation)
	fCameraShakeStrength = strength
	fCameraShakeAttenuation = attenuation
end


function UpdateScreenShake()


	if fCameraShakeStrength >0 then

		local cosA=math.cos(Clock.time() * 0.5 *2 ) * 5   * fCameraShakeStrength
		local cosB=math.cos(Clock.time() * 22 *2   ) * 3   * fCameraShakeStrength
		local cosC=math.cos(Clock.time() * 15 *2   ) * 2   * fCameraShakeStrength

		cam:translate( cosA+cosB,  cosA-cosC , 0)
		fCameraShakeStrength = fCameraShakeStrength - fCameraShakeAttenuation * Clock:deltaTime()

	end

end


function ParamCamera()
	bCamFovUp = Input.getCtrlState(CAM_FOV_UP)
	bCamFovDown = Input.getCtrlState(CAM_FOV_DOWN)

	bCamDistanceUp =  Input.getCtrlState(CAM_DISTANCE_UP)
	bCamDistanceDown =    Input.getCtrlState(CAM_DISTANCE_DOWN)

    bCamHeightUp =     Input.getCtrlState(CAM_HEIGHT_UP)
	bCamHeightDown =   Input.getCtrlState(CAM_HEIGHT_DOWN)

	bCamLookatUp =    Input.getCtrlState(CAM_LOOKAT_UP)
	bCamLookatDown =    Input.getCtrlState(CAM_LOOKAT_DOWN)

	bCamLateralOffsetUp =    Input.getCtrlState(CAM_LATERAL_OFFSET_UP)
	bCamLateralOffsetDown =    Input.getCtrlState(CAM_LATERAL_OFFSET_DOWN)




	if Input.getCtrlPushed(CAM_PARAM_PRINT) == true then

		print ("--------- CAMERA PARAMS -----------")
		print("FOV :"..cam:fov())
		print("Height :"..cam:position().y)
		print("Proximity Offset  :"..fCamOffset)
		print("Lookat Offset :"..tCamLookAtOffset.y)
		print ("----------------------------")
	end


end


function GetFovState()

	if bCamFovUp == 1 then
		return 1
	end

	if bCamFovDown == 1 then
		return -1
	end
	return 0
end

function GetDistanceState()
	if bCamDistanceUp == 1 then
		return 1
	end
	if bCamDistanceDown == 1 then
		return -1
	end
	return 0
end

function GetCameraHeightState()
	if bCamHeightUp == 1  then


		return 1
	end
	if bCamHeightDown == 1 then
		return -1
	end
	return 0

end

function GetTargetHeightState()
	if bCamLookatUp == 1 then
		return 1
	end
	if bCamLookatDown == 1 then
		return -1
	end
	return 0
end

function GetLateralOffsetState()
	if bCamLateralOffsetUp == 1 then
		return 1
	end
	if bCamLateralOffsetDown == 1 then
		return -1
	end
	return 0
end
