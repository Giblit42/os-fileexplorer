#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WIDTH 800
#define HEIGHT 600


typedef struct AppData {
    TTF_Font *font;
    SDL_Texture *folder;
    SDL_Texture *phrase;
    SDL_Texture *scroll;
    SDL_Texture *exe;
    SDL_Texture *image;
    SDL_Texture *video;
    SDL_Texture *code;
    SDL_Texture *other;
    SDL_Texture *up;
    SDL_Rect folder_rect;
    SDL_Rect phrase_rect;
    SDL_Rect scroll_rect;
    SDL_Rect exe_rect;
    SDL_Rect image_rect;
    SDL_Rect video_rect;
    SDL_Rect code_rect;
    SDL_Rect other_rect;
    SDL_Rect up_rect;
    bool folder_selected;
    bool phrase_selected;
    bool exe_selected;
    bool image_selected;
    bool video_selected;
    bool code_selected;
    bool other_selected;
    SDL_Point offset;
} AppData;


void initialize(SDL_Renderer *renderer, AppData *data_ptr);
void render(SDL_Renderer *renderer, AppData *data_ptr);
void quit(AppData *data_ptr);
void listDirectory(std::string dirname); // watch 04/22 recording on printing directory

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
        			// call list directories and pass in the getenv() pointer
        			
        			// for fork and exec should i check my list of directories and see if some are executable?
        			// how about going back up a level in the directory?
        				//delete everything after the most recent slash
        			// scroll bars?
        				//same thing we did in class draw it and then add a on button event to interact with it.
        			
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
    
    SDL_Surface *scroll_surf = IMG_Load("resrc/scroll.png");
    data_ptr->scroll = SDL_CreateTextureFromSurface(renderer, scroll_surf);
    SDL_FreeSurface(scroll_surf);
    data_ptr->scroll_rect.x = 730;
    data_ptr->scroll_rect.y = 10;
    data_ptr->scroll_rect.w = 120;
    data_ptr->scroll_rect.h = 600;
    
    SDL_Surface *exe_surf = IMG_Load("resrc/exe.webp");
    data_ptr->exe = SDL_CreateTextureFromSurface(renderer, exe_surf);
    SDL_FreeSurface(exe_surf);
    data_ptr->exe_rect.x = 200;
    data_ptr->exe_rect.y = 10;
    data_ptr->exe_rect.w = 180;
    data_ptr->exe_rect.h = 180;
    
    SDL_Surface *image_surf = IMG_Load("resrc/image.webp");
    data_ptr->image = SDL_CreateTextureFromSurface(renderer, image_surf);
    SDL_FreeSurface(image_surf);
    data_ptr->image_rect.x = 400;
    data_ptr->image_rect.y = 10;
    data_ptr->image_rect.w = 180;
    data_ptr->image_rect.h = 180;
    
    SDL_Surface *video_surf = IMG_Load("resrc/video.webp");
    data_ptr->video = SDL_CreateTextureFromSurface(renderer, video_surf);
    SDL_FreeSurface(video_surf);
    data_ptr->video_rect.x = 10;
    data_ptr->video_rect.y = 200;
    data_ptr->video_rect.w = 180;
    data_ptr->video_rect.h = 180;
    
    SDL_Surface *code_surf = IMG_Load("resrc/code.webp");
    data_ptr->code = SDL_CreateTextureFromSurface(renderer, code_surf);
    SDL_FreeSurface(code_surf);
    data_ptr->code_rect.x = 200;
    data_ptr->code_rect.y = 200;
    data_ptr->code_rect.w = 180;
    data_ptr->code_rect.h = 180;
    
    SDL_Surface *other_surf = IMG_Load("resrc/other.webp");
    data_ptr->other = SDL_CreateTextureFromSurface(renderer, other_surf);
    SDL_FreeSurface(other_surf);
    data_ptr->other_rect.x = 400;
    data_ptr->other_rect.y = 200;
    data_ptr->other_rect.w = 180;
    data_ptr->other_rect.h = 180;
}

void render(SDL_Renderer *renderer, AppData *data_ptr)
{
    // erase renderer content
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    SDL_RenderClear(renderer);
     
    // TODO: draw!
    SDL_RenderCopy(renderer, data_ptr->folder, NULL, &(data_ptr->folder_rect));
 
    SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &(data_ptr->phrase_rect));
    
    //SDL_RenderCopy(renderer, data_ptr->scroll, NULL, &(data_ptr->scroll_rect));
    
    //SDL_RenderCopy(renderer, data_ptr->exe, NULL, &(data_ptr->exe_rect));
    
    //SDL_RenderCopy(renderer, data_ptr->image, NULL, &(data_ptr->image_rect));
    
    //SDL_RenderCopy(renderer, data_ptr->video, NULL, &(data_ptr->video_rect));
    
    //SDL_RenderCopy(renderer, data_ptr->code, NULL, &(data_ptr->code_rect));
    
    //SDL_RenderCopy(renderer, data_ptr->other, NULL, &(data_ptr->other_rect));
 
    // show rendered frame
    SDL_RenderPresent(renderer);
}

void quit(AppData *data_ptr)
{
    SDL_DestroyTexture(data_ptr->folder);
    SDL_DestroyTexture(data_ptr->phrase);
    SDL_DestroyTexture(data_ptr->scroll);
    SDL_DestroyTexture(data_ptr->exe);
    SDL_DestroyTexture(data_ptr->image);
    SDL_DestroyTexture(data_ptr->video);
    SDL_DestroyTexture(data_ptr->code);
    SDL_DestroyTexture(data_ptr->other);
    TTF_CloseFont(data_ptr->font);
}

void listDirectory(std::string dirname)
{
   // 4/22 afternoon lecture start at 1:00:09
   // for the recursion part 1:15:04
   struct stat info;
   int err = stat(dirname.c_str(), &info);
   if (err == 0 && S_ISDIR(info.st_mode))
   {
      std::vector<std::string> file_list;
      DIR* dir = opendir(dirname.c_str());
      
      struct dirent *entry;
      while((entry = readdir(dir)) != NULL){
           file_list.push_back(entry->d_name);
      }
      closedir(dir);
      
      std::sort(file_list.begin(), file_list.end());
      
      int i, file_err;
      struct stat file_info;
      for(i = 0; i < file_list.size(); i++)
      {
      	  std::string full_path = dirname + "/" + file_list[i];
      	  file_err = stat(file_list[i].c_str(), &file_info);
      	  if(file_err)
      	  {
      	  	fprintf(stderr, "Uh Oh! Should not get here\n");
      	  }
      	  else if(S_ISDIR(file_info.st_mode))
      	  {
      	  	printf("%s (directory)\n", file_list[i].c_str());
      	  }
      	  else
      	  {
      	  	printf("%s (%ld bytes)\n", file_list[i].c_str(), file_info.st_size);
      	  }
      }
   }
   else{
       fprintf(stderr, "Error: directory '%s' is not found\n", dirname.c_str());       
   }
}

