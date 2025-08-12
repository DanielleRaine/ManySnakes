-- I used https://www.reddit.com/r/gamedev/comments/1oascs/hey_rgamedev_heres_a_tutorial_for_creating_a/ to make the main menu title texture.

textures =
{
	title =
	{
		filepath = "./assets/textures/title.png",
		position = {x = 1920 / 2 - 250, y = 1080 / 8},
		dimensions = {width = 500, height = 300},
	}
}

function RenderMainMenu()
	ManySnakesTextures.RenderTexture(textures.title)
end

