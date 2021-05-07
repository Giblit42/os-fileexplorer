#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WIDTH 800
#define HEIGHT 600


typedef struct AppData {
    TTF_Font *font;
    SDL_Texture *folder;
    SDL_Texture *phrase;
    SDL_Rect folder_rect;
    SDL_Rect phrase_rect;
    bool folder_selected;
    bool phrase_selected;
    SDL_Point offset;
} AppData;


void initialize(SDL_Renderer *renderer, AppData *data_ptr);
void render(SDL_Renderer *renderer, AppData *data_ptr);
void quit(AppData *data_ptr);

int main(int argc, char **argv)
{
    char *home = getenv("HOME");
    printf("HOME: %s\n", home);

    // initializing SDL as Video
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    // create window and renderer
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    // initialize and perform rendering loop
    AppData data;
    initialize(renderer, &data);
    render(renderer, &data);
    SDL_Event event;
    SDL_WaitEvent(&event);
    while (event.type != SDL_QUIT)
    {
        //render(renderer);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        	case SDL_MOUSEMOTION:
        		if(data.folder_selected)
        		{
        			data.folder_rect.x = event.motion.x;
        			data.folder_rect.y = event.motion.y;
        		}
        		else if(data.phrase_selected)
        		{
        			data.phrase_rect.x = event.motion.x;
        			data.phrase_rect.y = event.motion.y;
        		}
        		
        		break;
        	case SDL_MOUSEBUTTONDOWN:
        		if(event.button.button == SDL_BUTTON_LEFT && 
        		event.button.x >= data.phrase_rect.x &&
        		event.button.x <= data.phrase_rect.x + data.phrase_rect.w &&
        		event.button.y >= data.phrase_rect.y &&
        		event.button.y <= data.phrase_rect.y + data.phrase_rect.h)
        		{
        			data.phrase_selected = true;
                    		data.offset.x = event.button.x - data.phrase_rect.x;
                    		data.offset.y = event.button.y - data.phrase_rect.y;
        		}
        		else if(event.button.button == SDL_BUTTON_LEFT && 
        		event.button.x >= data.folder_rect.x &&
        		event.button.x <= data.folder_rect.x + data.folder_rect.w &&
        		event.button.y >= data.folder_rect.y &&
        		event.button.y <= data.folder_rect.y + data.folder_rect.h)
        		{
        			data.folder_selected = true;
                    		data.offset.x = event.button.x - data.folder_rect.x;
                    		data.offset.y = event.button.y - data.folder_rect.y;
        		}
        		
        		break;
        	case SDL_MOUSEBUTTONUP:
        		data.phrase_selected = false;
                	data.folder_selected = false;
        		break;
        }
        
        render(renderer, &data);

    }

    // clean up
    quit(&data);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

void initialize(SDL_Renderer *renderer, AppData *data_ptr)
{
    data_ptr->font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 24);

	// I think we want images of folders
    SDL_Surface *img_surf = IMG_Load("resrc/illustration-data-folder-icon.jpg");
    data_ptr->folder = SDL_CreateTextureFromSurface(renderer, img_surf);
    SDL_FreeSurface(img_surf);
    data_ptr->folder_rect.x = 10;
    data_ptr->folder_rect.y = 10;
    data_ptr->folder_rect.w = 180;
    data_ptr->folder_rect.h = 180;
    data_ptr->folder_selected = false;

    SDL_Color color = { 0, 0, 0 };
    SDL_Surface *phrase_surf = TTF_RenderText_Solid(data_ptr->font, "Home Directory", color);
    data_ptr->phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
    SDL_FreeSurface(phrase_surf);
    data_ptr->phrase_rect.x = 10;
    data_ptr->phrase_rect.y = 10;
    SDL_QueryTexture(data_ptr->phrase, NULL, NULL, &(data_ptr->phrase_rect.w), &(data_ptr->phrase_rect.h));
    data_ptr->phrase_selected = false;
}

void render(SDL_Renderer *renderer, AppData *data_ptr)
{
    // erase renderer content
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    SDL_RenderClear(renderer);
     
    // TODO: draw!
    SDL_RenderCopy(renderer, data_ptr->folder, NULL, &(data_ptr->folder_rect));
 
    SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &(data_ptr->phrase_rect));
 
    // show rendered frame
    SDL_RenderPresent(renderer);
}

void quit(AppData *data_ptr)
{
    SDL_DestroyTexture(data_ptr->folder);
    SDL_DestroyTexture(data_ptr->phrase);
    TTF_CloseFont(data_ptr->font);
}

