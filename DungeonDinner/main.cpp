#include "Header.h"
#include "Window.h"
#include "Game.h"

int abortGame(std::string failMessage)
{
	fprintf(stderr, failMessage.c_str());
	return -1;
}

int main(int argc, char **argv)
{
	//Allegro initialisation
	if (!al_init())
	{ return abortGame("Failed to initialize allegro"); }
	
	const int FPS = 60;
	ALLEGRO_TIMER *timer = nullptr;
	timer = al_create_timer(1.0/FPS);
	if (!timer)
	{ return abortGame("Failed to create timer"); }

	if (!al_install_keyboard())
	{ return abortGame("Failed to install keyboard"); }

	//Initialise image addon
	if(!al_init_image_addon())
	{ return abortGame("Failed to initialise al_init_image_addon"); }

	//Initialise the font addon
	al_init_font_addon();
	if (!al_init_ttf_addon())
	{ return abortGame("Failed to initalise al_init_ttf_addon"); }

	//Initialise primitives addon
	if (!al_init_primitives_addon())
	{ return abortGame("Failed to initialise al_init_primitives_addon"); }

	//setup display
	ALLEGRO_DISPLAY_MODE disp_data;
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data); //Set resolution to max
	int windowWidth = 1024;
	int windowHeight = 768;
//	int windowWidth = disp_data.width;
//	int windowHeight = disp_data.height;
//	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	al_set_new_display_flags(ALLEGRO_WINDOWED);

	ALLEGRO_DISPLAY *display = nullptr;
	display = al_create_display(windowWidth, windowHeight);
	if(!display)
	{ return abortGame("Failed to create display"); }

	//Initisatise the event queue
	ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
	event_queue = al_create_event_queue();
	if (!event_queue)
	{ return abortGame("Failed to create event queue"); }

	//Populate event queue
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	//set working directory
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(path, "Resources");
	al_change_directory(al_path_cstr(path, '/'));  // change the working directory
	al_destroy_path(path);

	//begin game
	srand(static_cast<unsigned int>(time(0))); //start with different seed for randomness simulation
	rand(); //throw away first value
	al_start_timer(timer); //start timer
	bool redraw = true;
	Window window(windowWidth, windowHeight);
	Game game;
	
	//finally start loop
	while (!game.done())
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

        switch(event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			redraw = true;
            game.update();
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			game.pressKey(event);
			break;
		case ALLEGRO_EVENT_KEY_UP:
			game.releaseKey(event);
			break;
		}

        if (redraw && al_is_event_queue_empty(event_queue))
		{
            redraw = false;
            window.redraw(game);
			al_flip_display();
        }
    }

	window.shutdown();
	game.shutdown();

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_timer(timer);
 
   return 0;
}