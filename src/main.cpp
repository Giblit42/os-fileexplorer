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

typedef struct FileEntry{
    std::string fileName;
    SDL_Rect namePos;
    //Some way of knowing what type it is
    //file size
    SDL_Texture *nameTexture;
    bool text_selected;
    bool pic_selected;
} FileEntry;

typedef struct AppData {
    TTF_Font *font;
    SDL_Texture *folder;
    SDL_Texture *phrase;
    SDL_Texture *exe;
    SDL_Texture *image;
    SDL_Texture *video;
    SDL_Texture *code;
    SDL_Texture *other;
    SDL_Texture *up;
    SDL_Texture *down;
    SDL_Texture *recur;
    SDL_Texture *back;
    SDL_Rect folder_rect;
    SDL_Rect phrase_rect;
    SDL_Rect up_rect;
    SDL_Rect down_rect;
    SDL_Rect exe_rect;
    SDL_Rect image_rect;
    SDL_Rect video_rect;
    SDL_Rect code_rect;
    SDL_Rect other_rect;
    SDL_Rect recur_rect;
    SDL_Rect back_rect;
    bool folder_selected;
    bool phrase_selected;
    bool exe_selected;
    bool image_selected;
    bool video_selected;
    bool code_selected;
    bool other_selected;
    bool up_selected;
    bool down_selected;
    bool recur_selected;
    bool back_selected;
    SDL_Point offset;
    std::vector<FileEntry*> file_list;
} AppData;


void initialize(SDL_Renderer *renderer, AppData *data_ptr);
void render(SDL_Renderer *renderer, AppData *data_ptr);
void quit(AppData *data_ptr);
void listDirectory(std::string dirname, AppData *data_ptr); // watch 04/22 recording on printing directory

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
    listDirectory((std::string)home, &data);
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
        		
        		
        		break;
        	case SDL_MOUSEBUTTONDOWN:
        		
        		if(event.button.button == SDL_BUTTON_LEFT && 
        		event.button.x >= data.up_rect.x &&
        		event.button.y > 5 && event.button.y < 105 &&
        		data.file_list.at(0)->namePos.y != 45)
        		{
        			data.up_selected = true;
        			data.folder_rect.y += 50;
        			for(int i = 0; i < data.file_list.size(); i++){
                        		data.file_list.at(i)->namePos.y +=50;
                    		}
        		}
        		else if(event.button.button == SDL_BUTTON_LEFT &&
        		event.button.x >= data.down_rect.x &&
        		event.button.y > 500 && event.button.y < 600 &&
        		data.file_list.at(data.file_list.size()-1)->namePos.y >= HEIGHT-50)
        		{
        			data.down_selected = true;
        			data.folder_rect.y -= 50;
        			for(int i = 0; i < data.file_list.size(); i++){
                        		data.file_list.at(i)->namePos.y -=50;
                    		}
        		}
        		else if(event.button.button == SDL_BUTTON_LEFT && 
        		event.button.x >= data.folder_rect.x &&
        		event.button.y >= data.folder_rect.y)
        		{
        			//change to current directory
        			// how do we change our current dirrectory?
        			// populate home directory with the .. as our home directory
        			// replacing string dirname 
        			
        			data.folder_selected = true;
        		}
        		else if(event.button.button == SDL_BUTTON_LEFT && 
        		event.button.x >= data.back_rect.x &&
        		event.button.y >= data.back_rect.y)
        		{
        			//move back one directory or level
        			// delete everything after most recent /
        			data.back_selected = true;
        		}
        		break;
        	case SDL_MOUSEBUTTONUP:
        		data.phrase_selected = false;
                	data.folder_selected = false;
                	data.up_selected = false;
                	data.down_selected = false;
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
    data_ptr->folder_rect.w = 60;
    data_ptr->folder_rect.h = 60;
    data_ptr->folder_selected = false;

    SDL_Color color = { 0, 0, 0 };
    SDL_Surface *phrase_surf = TTF_RenderText_Solid(data_ptr->font, "Home Directory", color);
    data_ptr->phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
    SDL_FreeSurface(phrase_surf);
    data_ptr->phrase_rect.x = 10;
    data_ptr->phrase_rect.y = 0;
    SDL_QueryTexture(data_ptr->phrase, NULL, NULL, &(data_ptr->phrase_rect.w), &(data_ptr->phrase_rect.h));
    data_ptr->phrase_selected = false;
    
    SDL_Surface *exe_surf = IMG_Load("resrc/exe.webp");
    data_ptr->exe = SDL_CreateTextureFromSurface(renderer, exe_surf);
    SDL_FreeSurface(exe_surf);
    data_ptr->exe_rect.x = 200;
    data_ptr->exe_rect.y = 10;
    data_ptr->exe_rect.w = 60;
    data_ptr->exe_rect.h = 60;
    data_ptr->exe_selected = false;
    
    SDL_Surface *image_surf = IMG_Load("resrc/image.webp");
    data_ptr->image = SDL_CreateTextureFromSurface(renderer, image_surf);
    SDL_FreeSurface(image_surf);
    data_ptr->image_rect.x = 400;
    data_ptr->image_rect.y = 10;
    data_ptr->image_rect.w = 60;
    data_ptr->image_rect.h = 60;
    data_ptr->image_selected = false;
    
    SDL_Surface *video_surf = IMG_Load("resrc/video.webp");
    data_ptr->video = SDL_CreateTextureFromSurface(renderer, video_surf);
    SDL_FreeSurface(video_surf);
    data_ptr->video_rect.x = 10;
    data_ptr->video_rect.y = 200;
    data_ptr->video_rect.w = 60;
    data_ptr->video_rect.h = 60;
    data_ptr->video_selected = false;
    
    SDL_Surface *code_surf = IMG_Load("resrc/code.webp");
    data_ptr->code = SDL_CreateTextureFromSurface(renderer, code_surf);
    SDL_FreeSurface(code_surf);
    data_ptr->code_rect.x = 200;
    data_ptr->code_rect.y = 200;
    data_ptr->code_rect.w = 60;
    data_ptr->code_rect.h = 60;
    data_ptr->code_selected = false;
    
    SDL_Surface *other_surf = IMG_Load("resrc/other.webp");
    data_ptr->other = SDL_CreateTextureFromSurface(renderer, other_surf);
    SDL_FreeSurface(other_surf);
    data_ptr->other_rect.x = 400;
    data_ptr->other_rect.y = 200;
    data_ptr->other_rect.w = 60;
    data_ptr->other_rect.h = 60;
    data_ptr->other_selected = false;
    
    SDL_Surface *up_surf = IMG_Load("resrc/up.svg");
    data_ptr->up = SDL_CreateTextureFromSurface(renderer, up_surf);
    SDL_FreeSurface(up_surf);
    data_ptr->up_rect.x = 740;
    data_ptr->up_rect.y = 40;
    data_ptr->up_rect.w = 60;
    data_ptr->up_rect.h = 60;
    data_ptr->up_selected = false;
    
    SDL_Surface *down_surf = IMG_Load("resrc/down2.png");
    data_ptr->down = SDL_CreateTextureFromSurface(renderer, down_surf);
    SDL_FreeSurface(down_surf);
    data_ptr->down_rect.x = 740;
    data_ptr->down_rect.y = 540;
    data_ptr->down_rect.w = 60;
    data_ptr->down_rect.h = 60;
    data_ptr->down_selected = false;
    
    SDL_Surface *recur_surf = IMG_Load("resrc/Recursion.jpeg");
    data_ptr->recur = SDL_CreateTextureFromSurface(renderer, recur_surf);
    SDL_FreeSurface(recur_surf);
    data_ptr->recur_rect.x = 450;
    data_ptr->recur_rect.y = 1;
    data_ptr->recur_rect.w = 30;
    data_ptr->recur_rect.h = 30;
    data_ptr->recur_selected = false;
    
    SDL_Surface *back_surf = IMG_Load("resrc/back.jwebp");
    data_ptr->back = SDL_CreateTextureFromSurface(renderer, back_surf);
    SDL_FreeSurface(back_surf);
    data_ptr->back_rect.x = 450;
    data_ptr->back_rect.y = 1;
    data_ptr->back_rect.w = 30;
    data_ptr->back_rect.h = 30;
    data_ptr->back_selected = false;
    
    
    
    
    SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &(data_ptr->phrase_rect));
    for(int i = 0; i < data_ptr->file_list.size(); i++){
        phrase_surf = TTF_RenderText_Solid(data_ptr->font, data_ptr->file_list.at(i)->fileName.c_str(), color);
        data_ptr->file_list.at(i)->nameTexture = SDL_CreateTextureFromSurface(renderer, phrase_surf);
        SDL_FreeSurface(phrase_surf);
    }
    
}

void render(SDL_Renderer *renderer, AppData *data_ptr)
{
    SDL_Color color = { 0, 0, 0 };
    // erase renderer content
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    SDL_RenderClear(renderer);
     
    // TODO: draw!
    //SDL_RenderCopy(renderer, data_ptr->folder, NULL, &(data_ptr->folder_rect));
 
    //SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &(data_ptr->phrase_rect));
    
    
    for(int i = 0; i < data_ptr->file_list.size(); i++){
        SDL_QueryTexture(data_ptr->file_list.at(i)->nameTexture, NULL, NULL, &(data_ptr->file_list.at(i)->namePos.w), &(data_ptr->file_list.at(i)->namePos.h));
        SDL_RenderCopy(renderer, data_ptr->file_list.at(i)->nameTexture, NULL, &(data_ptr->file_list.at(i)->namePos));
    }
    
    SDL_Rect titalBar = {0, 0, WIDTH, 40};
    SDL_SetRenderDrawColor(renderer, 76, 52, 235,255);
    SDL_RenderFillRect(renderer, &titalBar);
    
    SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &(data_ptr->phrase_rect));
    
    SDL_RenderCopy(renderer, data_ptr->up, NULL, &(data_ptr->up_rect));
    
    SDL_RenderCopy(renderer, data_ptr->down, NULL, &(data_ptr->down_rect));
    
    SDL_RenderCopy(renderer, data_ptr->recur, NULL, &(data_ptr->recur_rect));
      
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
    SDL_DestroyTexture(data_ptr->exe);
    SDL_DestroyTexture(data_ptr->image);
    SDL_DestroyTexture(data_ptr->video);
    SDL_DestroyTexture(data_ptr->code);
    SDL_DestroyTexture(data_ptr->other);
    SDL_DestroyTexture(data_ptr->up);
    SDL_DestroyTexture(data_ptr->down);
    SDL_DestroyTexture(data_ptr->recur);
    TTF_CloseFont(data_ptr->font);
}

void listDirectory(std::string dirname , AppData *data_ptr)
{
   // 4/22 afternoon lecture start at 1:00:09
   // for the recursion part 1:15:04
   
   
   // the way everyting is printing out he said is just fine we don't need to remove anymore dots
   
   // need to make .. our home directory rather than just printing out "Home Directory"
   
   // I shared a link in discord for the file permissions
   
   // for getting the file extensions he said we can use sup string and just look at the half after the . and see if it is a .txt or something
   //then we can store the icons in an array or we can have a conditonal block of if/else for each extension type
   
   //for the .. button wich we still need when it is clicked on we will move up one level in the directory, this can be done by deleting everything after the most recent /
   //then set it as are current dirrectory
   
   //we do something similar for changing directories  
   
   struct stat info;
   int err = stat(dirname.c_str(), &info);
   if (err == 0 && S_ISDIR(info.st_mode))
   {
      std::vector<std::string> file_list;
      DIR* dir = opendir(dirname.c_str());
      
      struct dirent *entry;
      while((entry = readdir(dir)) != NULL){
      	    if((std::string)entry->d_name != ".")
      	    {
              file_list.push_back(entry->d_name);
           }
      }
      closedir(dir);
      
      std::sort(file_list.begin(), file_list.end());
      
      for(int k = 0; k < file_list.size(); k++){
            FileEntry *temp = new FileEntry();
            temp->fileName = file_list.at(k);
            temp->namePos.x = 60;
            temp->namePos.y = 45 + (k*40);
            data_ptr->file_list.push_back(temp);
      }
      
      int i, file_err;
      struct stat file_info;
      for(i = 0; i < file_list.size(); i++)
      {
      	  std::string full_path = dirname + "/" + file_list[i];
      	  file_err = stat(full_path.c_str(), &file_info);
      	  if(file_err)
      	  {
      	  	fprintf(stderr, "Uh Oh! Should not get here\n");
      	  }
      	  else if(S_ISDIR(file_info.st_mode))
      	  {
      	  	printf("%s (directory)\n",file_list[i].c_str());
      	  	if(file_list[i] != "." && file_list[i] != "..")
      	  	{
      	  	   //listDirectory(full_path , indent + 1, data_ptr);
      	  	}
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

