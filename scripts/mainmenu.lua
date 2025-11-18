-- I used https://www.reddit.com/r/gamedev/comments/1oascs/hey_rgamedev_heres_a_tutorial_for_creating_a/ to make the main menu title texture.

textures =
{
	title =
	{
		filepath = "./assets/textures/title.png",
		position = {x = 1920 / 2 - 250, y = 1080 / 8},
		dimensions = {width = 500, height = 300}
	}
}

buttons =
{
	classic =
	{
		filepaths = {button = "./assets/textures/title.png", hover = "./assets/textures/apple.png", click = "./assets/textures/title.png"},
		position = {x = 1000, y = 500},
		dimensions = {width = 200, height = 200},
	}
}

function RenderMainMenu()
	ManySnakesRender.RenderTexture(textures.title)
	ManySnakesRender.RenderButton(buttons.classic)
end

function PointInBounds(point, bounds)
	return bounds.x <= point.x && point.x <= bounds.x + bounds.w && bounds.y <= point.y && point.y <= bounds.y + bounds.h
end

function MouseButtonUpMainMenu(x, y)
	PointInBounds(buttons.classic.
