

tDestructible = {}
nWallHeigth = 5
nWallY = nWallHeigth 

function CreateDestructible(Index, PositionX, PositionZ)
	

	tDestructible[Index] = Mesh.createBox( 0.5, 3, 0.5, Vector(1,1,1,0.5))
	tDestructible[Index]:shader(): matEmissive(Vector(0.5,0.5,0,1))
	tDestructible[Index]:position( PositionX , 3, -PositionZ)
	tDestructible[Index]:rotation( 0,  math.deg( Angle ), 0 )
	print ( "createBox")
	
end

function DestroyDestructible( Index )
	tDestructible[Index]:release()
end

