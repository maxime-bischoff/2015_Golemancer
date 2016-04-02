

tCollectible = {}


function CreateCollectible(Index, PositionX, PositionZ, Height )
	tCollectible[Index] = Mesh.createBox( 0.05, 0.05 , 0.05, Vector(0,1,1,0.5))
	tCollectible[Index]:shader(): matEmissive(Vector(0.1,0.1,0.1,1))
	tCollectible[Index]:position( PositionX , Height, -PositionZ)
end

function UpdateCollectible(Index, PositionX, PositionZ, Height)
	tCollectible[Index]:position( PositionX , Height + 0.05, -PositionZ)
end

function DestroyCollectible( Index ,Count )
	tCollectible[Index]:release()
	tCollectible[Index] = tCollectible[Count]
	tCollectible[Count] = nil
end
