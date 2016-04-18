#include "Window.h"

Window::Window(int width, int height)
	:m_width(width),
	m_height(height),
	m_colours(static_cast<int>(Colour::TOTAL))
{
	m_colours.at(static_cast<int>(Colour::RED)) = al_map_rgb(255, 0, 0);
	m_colours.at(static_cast<int>(Colour::GREEN)) = al_map_rgb(0, 255, 0);
	m_colours.at(static_cast<int>(Colour::BLUE)) = al_map_rgb(0, 0, 255);
	m_colours.at(static_cast<int>(Colour::YELLOW)) = al_map_rgb(255, 255, 0);
	m_colours.at(static_cast<int>(Colour::CYAN)) = al_map_rgb(0, 255, 255);
	m_colours.at(static_cast<int>(Colour::MAGENTA)) = al_map_rgb(255, 0, 255);
	m_colours.at(static_cast<int>(Colour::BROWN)) = al_map_rgb(120, 50, 0);
	m_colours.at(static_cast<int>(Colour::BLACK)) = al_map_rgb(0, 0, 0);
	m_colours.at(static_cast<int>(Colour::DIM)) = al_map_rgb(120, 120, 120);
	m_colours.at(static_cast<int>(Colour::LIT)) = al_map_rgb(255, 255, 255);
}

void Window::redraw(Game& game)
{
	switch(game.getMode())
	{
	case ScreenMode::MENU:
		redrawMenu(game.getMenu());
		break;
	case ScreenMode::BOARD:
		redrawBoard(game.getBoard());
		break;
	}
}

void Window::drawMenuSelect(Menu& menu)
{
	for(int i=0; i<menu.getText().size(); ++i)
	{
		ALLEGRO_COLOR colour;
		if(menu.getOption()==static_cast<MenuOp>(i)) { colour = m_colours.at(static_cast<int>(Colour::LIT)); }
		else { colour = m_colours.at(static_cast<int>(Colour::DIM)); }
		al_draw_text(menu.getFont(), colour, m_width*0.5,
			(m_height+((2*i)-static_cast<int>(MenuOp::TOTAL))*static_cast<int>(TextSize::BIG))*0.5,
			ALLEGRO_ALIGN_CENTRE, menu.getText().at(i).c_str());
	}
}

void Window::drawMenuText(Menu& menu)
{
	for(int i=0; i<menu.getText().size(); ++i)
	{
		al_draw_text(menu.getFont(), m_colours.at(static_cast<int>(Colour::LIT)), m_width*0.5,
			(m_height+((2*i)-static_cast<int>(MenuOp::TOTAL))*static_cast<int>(TextSize::BIG))*0.5,
			ALLEGRO_ALIGN_CENTRE, menu.getText().at(i).c_str());
	}
}

void Window::drawMenuColourText(Menu& menu)
{
	ALLEGRO_COLOR colour;
	for(int i=0; i<menu.getText().size(); ++i)
	{
		if(i==0) { colour = m_colours.at(static_cast<int>(Colour::LIT)); }
		else { colour = m_colours.at(static_cast<int>(menu.getCurrentColour())); }
		al_draw_text(menu.getFont(), colour, m_width*0.5,
			(m_height+((2*i)-static_cast<int>(MenuOp::TOTAL))*static_cast<int>(TextSize::BIG))*0.5,
			ALLEGRO_ALIGN_CENTRE, menu.getText().at(i).c_str());
	}
}

void Window::redrawMenu(Menu& menu)
{
	al_clear_to_color(m_colours.at(static_cast<int>(Colour::BLACK)));
	switch(menu.getState())
	{
	case MenuState::SELECT:
		drawMenuSelect(menu);
		break;
	case MenuState::NUMBER:
		drawMenuText(menu);
		break;
	case MenuState::NAME:
		drawMenuText(menu);
		break;
	case MenuState::COLOUR:
		drawMenuColourText(menu);
		break;
	}
}

void Window::drawHexes(Board& board)
{
	int hex = 48;
	int hexWidth = 64;
	int hexHeight = 49;
// draw a big mat of hexes
//	for(int y=0; y<16; ++y)
//	{
//		for(int x=0; x<19; ++x)
//		{
//			if(!(y==15 && ((x+1)%2)))
//			{ al_draw_bitmap(board.getHexBMP(0), (x+1)*hex, (y*hex)+(((x+1)%2)*(hex/2)), 0); }
//		}
//	}

	//get max and min values to offset hexes
	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;
	for(int i=0; i<board.getHexes().size(); ++i)
	{
		if(board.getHexes().at(i).x<minX) { minX = board.getHexes().at(i).x; }
		if(board.getHexes().at(i).x>maxX) { maxX = board.getHexes().at(i).x; }
		if(board.getHexes().at(i).y<minY) { minY = board.getHexes().at(i).y; }
		if(board.getHexes().at(i).y>maxY) { maxY = board.getHexes().at(i).y; }
	}
	int extraX = (maxX - abs(minX))*hex/2;
	int extraY = (maxY - abs(minY))*hex/2;

	//finally draw hexes
	for(int i=0; i<board.getHexes().size(); ++i)
	{
		int hexX = (m_width/2)+((board.getHexes().at(i).x)*hex)-(hexWidth/2)-extraX;
		int hexY = (m_height/2)+(board.getHexes().at(i).y*hex)+(((abs(board.getHexes().at(i).x))%2)*(hex/2))-(hexHeight/2)-extraY;
		al_draw_tinted_bitmap(board.getHexBMP(0),
			m_colours.at(static_cast<int>(board.getHexes().at(i).terrain)), hexX, hexY, 0);
		al_draw_text(board.getFont(), m_colours.at(static_cast<int>(Colour::BLACK)),
			hexX+(hexWidth/2), hexY+((hexHeight-static_cast<int>(TextSize::MEDIUM))/2), ALLEGRO_ALIGN_CENTRE,
			std::to_string(board.getHexes().at(i).number).c_str());
	}
	//draw selected hex
	int targX = (m_width/2)+((board.getSelectedX())*hex)-(hexWidth/2)-extraX;
	int targY = (m_height/2)+(board.getSelectedY()*hex)+(((abs(board.getSelectedX()))%2)*(hex/2))-(hexHeight/2)-extraY;
	al_draw_bitmap(board.getHexBMP(1), targX, targY, 0);
}

void Window::drawPlayers(Board& board)
{
	for(int i=0; i<board.getNumberPlayers(); ++i)
	{
		int x; int y; int f;
		switch(i)
		{
		case 0:
			x = static_cast<int>(TextSize::MEDIUM);
			y = static_cast<int>(TextSize::MEDIUM);
			f = ALLEGRO_ALIGN_LEFT;
			break;
		case 1:
			x = m_width-static_cast<int>(TextSize::MEDIUM);
			y = static_cast<int>(TextSize::MEDIUM);
			f = ALLEGRO_ALIGN_RIGHT;
			break;
		case 2:
			x = static_cast<int>(TextSize::MEDIUM);
			y = m_height-(static_cast<int>(TextSize::MEDIUM)*2);
			f = ALLEGRO_ALIGN_LEFT;
			break;
		case 3:
			x = m_width-static_cast<int>(TextSize::MEDIUM);
			y = m_height-(static_cast<int>(TextSize::MEDIUM)*2);
			f = ALLEGRO_ALIGN_RIGHT;
			break;
		}
		al_draw_text(board.getFont(), m_colours.at(static_cast<int>(board.getPlayer(i).getColour())),
			x, y, f, board.getPlayer(i).getName().c_str());
	}
	al_draw_text(board.getFont(), m_colours.at(static_cast<int>(Colour::LIT)),
		m_width/2, static_cast<int>(TextSize::MEDIUM), ALLEGRO_ALIGN_CENTRE,
		std::string("Spawn: "+std::to_string(board.getSpawnDie())).c_str());
	al_draw_text(board.getFont(), m_colours.at(static_cast<int>(Colour::LIT)),
		m_width/2, m_height-(2*static_cast<int>(TextSize::MEDIUM)), ALLEGRO_ALIGN_CENTRE,
		std::string("Move: "+std::to_string(board.getMoveDie())).c_str());
}

void Window::redrawBoard(Board& board)
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	drawHexes(board);
	drawPlayers(board);
}

void Window::shutdown()
{}

Window::~Window(void)
{
}
