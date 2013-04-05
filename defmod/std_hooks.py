import game

mouse=game.input.mouse

def On_A():
	game.player.left()

def On_D():
	game.player.right()

def On_W():
	game.player.forward()

def On_S():
	game.player.backward()

def On_MouseMove():
	if mouse.x<game.width/4:
		game.player.rotate(-40.0/game.width*mouse.x)
	elif mouse.x>game.width*3.0/4:
		game.player.rotate(40.0/game.width*mouse.x)

game.hook("A",On_A)
game.hook("D",On_D)
game.hook("W",On_W)
game.hook("S",On_S)
game.hook("Mouse Move",On_MouseMove)