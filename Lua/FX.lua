
merge("data/sfx/Teleport_FX.pak")
tTeleportFXbase = Transform.find("Teleport_FX")
tTeleportFXbase:visible(false)
tTeleportFXbase:scale(1.4,1.4,1.4)
tTeleportFXs = {}

function UpdateFX()
	UpdateHeavyImpacts()
	UpdateImpactsParticles()
	UpdateTeleportFX()

end

function ClearFXs()

	for i =1 , #tTeleportFXs do
		tTeleportFXs[i]:release()
	end

	tTeleportFXs = {}

end


function CreateTeleportFX(posX, posY, posZ  )
	if(#tTeleportFXs > 0) then return end
	tTeleportFXs[ #tTeleportFXs +1] = tTeleportFXbase:clone()
	tTeleportFXs[ #tTeleportFXs]:position(posX, posY, -posZ )
	tTeleportFXs[ #tTeleportFXs]:animPlay("Play_Teleport")
	tTeleportFXs[ #tTeleportFXs]:visible(true)

	 -- tTeleportFXs[ #tTeleportFXs]:animSpeed(2)  
end

function UpdateTeleportFX()
	i = 1
	while i <= #tTeleportFXs  do
		if ( not tTeleportFXs[i]:animPlaying())	 then
			tTeleportFXs[i]:release()
			tTeleportFXs[i] = tTeleportFXs[#tTeleportFXs]
			tTeleportFXs[#tTeleportFXs] = nil
		end
		i = i + 1
	end
end


