

tTrigger = {}


function CreateTrigger(PositionX, PositionZ, SizeX, SizeZ, Angle)
	print(#tTrigger)
	tTrigger[#tTrigger+1] = Mesh.createBox( 0.5, 0.5, 0.5, Vector(1,1,0,1))
	tTrigger[#tTrigger]:shader(): matEmissive(Vector(0.1,0.1,0.1,1))
	tTrigger[#tTrigger]:position( PositionX , 0, -PositionZ)
	tTrigger[#tTrigger]:rotation( 0,  math.deg( Angle ), 0 )
	
end

function DestroyTrigger( Index )
	tTrigger[Index]:release()
end

