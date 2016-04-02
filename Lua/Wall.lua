

tSolid = {}
nWallHeigth = 5
nWallY = nWallHeigth 

function CreateWall(PositionX, PositionZ, SizeX, SizeZ, Angle)
	

	tSolid[#tSolid+1] = Mesh.createBox( SizeX, nWallHeigth, SizeZ, Vector(1,1,1,0.5))
	tSolid[#tSolid]:shader(): matEmissive(Vector(0.1,0.1,0.1,1))
	tSolid[#tSolid]:position( PositionX , nWallY, -PositionZ)
	tSolid[#tSolid]:rotation( 0,  math.deg( Angle ), 0 )
	
end

function DestroyWall( Index )
	tSolid[Index]:release()
end

