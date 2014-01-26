local game=require "game"

game.player.jump=function()
	

game.on("A",function(e)
	game.player.camera.translate(0,-0.1,0)
end)
game.on("D",function(e)
	game.player.camera.translate(0,0.1,0)
end)
game.on("W",function(e)
	game.player.camera.translate(0.1,0,0)
end)
game.on("S",function(e)
	game.player.camera.translate(-0.1,0,0)
end)
game.on("mouse move",function(e)
	game.player.camera.rotate(
		(1-2*e.mx/game.scene.width())*0.01,
		(1-2*e.my/game.scene.height())*0.01
	)
end)
game.on("space",function(e)
	game.player.vz=10
end)