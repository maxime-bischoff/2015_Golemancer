merge("Data/Menus/Splashscreens.pak")


local splashscreens =  Transform.find("Splashscreens")


function PlaySplashcreens()

	splashscreens:animPlay("PlaySplashscreens")
	  splashscreens:animSpeed(3)  

end


function IsSplashScreenFinished()


	if splashscreens:animPlaying() then return 0;
									 else  return 1;
									 end

end

function ShowSplashscreens(bShow)
	splashscreens:visible(bShow)
end

