#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <filesystem>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct FileEntry{
    std::string fileName;
    std::string full_path;

    SDL_Rect namePos;
    SDL_Texture *nameTexture;

    std::string fileSize;
    SDL_Rect sizePos;
    SDL_Texture *sizeTexture;

    std::string filePic;
    SDL_Texture *picTexture;
    SDL_Rect picPos;

    bool selected;
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
    SDL_Texture *size;
    SDL_Texture *perm;
    SDL_Rect perm_rect;
    SDL_Rect size_rect;
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
    SDL_Point offset;
    std::vector<FileEntry*> file_list;
} AppData;


void initialize(SDL_Renderer *renderer, AppData *data_ptr);
void render(SDL_Renderer *renderer, AppData *data_ptr);
void quit(AppData *data_ptr);
void listDirectory(std::string dirname, AppData *data_ptr, bool recursive, int indent, SDL_Renderer *renderer); // watch 04/22 recording on printing directory
std::string getFilePic(std::string ext);
std::string stringOfSize(int size);
void initializeFileNames(SDL_Renderer *renderer, AppData *data_ptr, std::string dirname);

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
    listDirectory((std::string)home, &data,false,0,renderer);
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
        		event.button.y > 40 && event.button.y < 100 &&
        		data.file_list.at(0)->namePos.y != 45)
        		{
        			data.up_selected = true;
        			//data.folder_rect.y += 40;
        			for(int i = 0; i < data.file_list.size(); i++){
                    	data.file_list.at(i)->namePos.y +=40;
                        data.file_list.at(i)->sizePos.y +=40;
                        data.file_list.at(i)->picPos.y +=40;
                    }
        		}
        		else if(event.button.button == SDL_BUTTON_LEFT &&
        		event.button.x >= data.down_rect.x &&
        		event.button.y > 500 && event.button.y < 600 &&
        		data.file_list.at(data.file_list.size()-1)->namePos.y >= HEIGHT-50)
        		{
        			data.down_selected = true;
        			//data.folder_rect.y -= 45;
        			for(int i = 0; i < data.file_list.size(); i++){
                    	data.file_list.at(i)->namePos.y -=40;
                        data.file_list.at(i)->sizePos.y -=40;
                        data.file_list.at(i)->picPos.y -=40;
                    }
        		}
                else if(event.button.button == SDL_BUTTON_LEFT && 
                event.button.x >= data.recur_rect.x && event.button.x <= WIDTH &&
                event.button.y <= 40 && event.button.y >= 0){
                    if(data.recur_selected){
                        // data.file_list.clear();
                        // listDirectory((std::string)home,&data,false,0);
                        // data.recur_selected = false;
                    }
                    else{
                        // data.file_list.clear();
                        // listDirectory((std::string)home,&data,true,0);
                        // data.recur_selected = true;
                    }
                    std::cout << " YOU PRESSED THE RECURSIVE BUTTON" << std::endl;
                }
                else if(event.button.y > 40){//If a file is selected
                    int y = event.button.y;
                    int x = event.button.x;
                    FileEntry *hold;
                    for(int i = 0; i < data.file_list.size(); i++){
                        //check if the same x as the text, and if the same y as the text
                        //check if the same x and y as the pic
                        hold = data.file_list.at(i);
                        if(x >= hold->namePos.x && x <= hold->namePos.x + hold->namePos.w &&
                           y >= hold->namePos.y && y <= hold->namePos.y + hold->namePos.h ||
                           x >= hold->picPos.x && x <= hold->picPos.x + hold->picPos.w &&
                           y >= hold->picPos.y && y <= hold->picPos.y + hold->picPos.h){
                               if(hold->filePic == "resrc/illustration-data-folder-icon.jpg"){
                                   data.file_list.clear();
                                   //render the new directory header
                                   listDirectory(hold->full_path,&data,false,0,renderer);
                                   initializeFileNames(renderer,&data,hold->fileName);
                               }
                               else{
                                   //use xdg-open and fork() and exec() to open with the preferred application
                               }
                           }

                    }


                    
                    //char *env = getenv()
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
    SDL_Surface *phrase_surf = TTF_RenderText_Solid(data_ptr->font, "HOME", color);
    data_ptr->phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
    SDL_FreeSurface(phrase_surf);
    data_ptr->phrase_rect.x = 10;
    data_ptr->phrase_rect.y = 0;
    SDL_QueryTexture(data_ptr->phrase, NULL, NULL, &(data_ptr->phrase_rect.w), &(data_ptr->phrase_rect.h));
    data_ptr->phrase_selected = false;

    SDL_Surface *size_surf = TTF_RenderText_Solid(data_ptr->font, "Size", color);
    data_ptr->size = SDL_CreateTextureFromSurface(renderer, size_surf);
    SDL_FreeSurface(size_surf);
    data_ptr->size_rect.x = 450;
    data_ptr->size_rect.y = 0;
    SDL_QueryTexture(data_ptr->size,NULL,NULL, &(data_ptr->size_rect.w), &(data_ptr->size_rect.h));

    SDL_Surface *perm_surf = TTF_RenderText_Solid(data_ptr->font, "Permissions", color);
    data_ptr->perm = SDL_CreateTextureFromSurface(renderer, perm_surf);
    SDL_FreeSurface(size_surf);
    data_ptr->perm_rect.x = 600;
    data_ptr->perm_rect.y = 0;
    SDL_QueryTexture(data_ptr->perm,NULL,NULL, &(data_ptr->perm_rect.w), &(data_ptr->perm_rect.h));
    
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
    data_ptr->recur_rect.x = 760;
    data_ptr->recur_rect.y = 1;
    data_ptr->recur_rect.w = 40;
    data_ptr->recur_rect.h = 40;
    data_ptr->recur_selected = false;
    
    
    
    
    SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &(data_ptr->phrase_rect));

    initializeFileNames(renderer,data_ptr, "HOME");
    
}

void render(SDL_Renderer *renderer, AppData *data_ptr)
{
    SDL_Color color = { 0, 0, 0 };
    // erase renderer content
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    SDL_RenderClear(renderer);
     
    // TODO: draw!

    SDL_Rect seperator; 
    
    for(int i = 0; i < data_ptr->file_list.size(); i++){
        SDL_QueryTexture(data_ptr->file_list.at(i)->nameTexture, NULL, NULL, &(data_ptr->file_list.at(i)->namePos.w), &(data_ptr->file_list.at(i)->namePos.h));
        SDL_RenderCopy(renderer, data_ptr->file_list.at(i)->nameTexture, NULL, &(data_ptr->file_list.at(i)->namePos));

        SDL_QueryTexture(data_ptr->file_list.at(i)->sizeTexture,NULL,NULL, &(data_ptr->file_list.at(i)->sizePos.w), &(data_ptr->file_list.at(i)->sizePos.h));
        SDL_RenderCopy(renderer, data_ptr->file_list.at(i)->sizeTexture, NULL, &(data_ptr->file_list.at(i)->sizePos));

        SDL_RenderCopy(renderer, data_ptr->file_list.at(i)->picTexture, NULL, &(data_ptr->file_list.at(i)->picPos));

        if(i < 13){
            seperator = {0,80 + (i*40),WIDTH,1};
            SDL_SetRenderDrawColor(renderer,120,120,120,255);
            SDL_RenderFillRect(renderer,&seperator);
        }
    }
    
    SDL_Rect titleBar = {0, 0, WIDTH, 40};
    SDL_SetRenderDrawColor(renderer, 76, 52, 235,255);
    SDL_RenderFillRect(renderer, &titleBar);
    
    SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &(data_ptr->phrase_rect));

    SDL_RenderCopy(renderer, data_ptr->size, NULL, &(data_ptr->size_rect));

    SDL_RenderCopy(renderer, data_ptr->perm, NULL, &(data_ptr->perm_rect));
    
    SDL_RenderCopy(renderer, data_ptr->up, NULL, &(data_ptr->up_rect));
    
    SDL_RenderCopy(renderer, data_ptr->down, NULL, &(data_ptr->down_rect));
    
    SDL_RenderCopy(renderer, data_ptr->recur, NULL, &(data_ptr->recur_rect));
 
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

void listDirectory(std::string dirname , AppData *data_ptr, bool recursive, int indent, SDL_Renderer *renderer)
{
    // 4/22 afternoon lecture start at 1:00:09
    // for the recursion part 1:15:04
    SDL_Surface *pic_surf;

    std::string buffer = "";
    for(int i = 0; i < indent; i++){
        buffer += "  ";
    }

    struct stat info;
    int err = stat(dirname.c_str(), &info);
    if (err == 0 && S_ISDIR(info.st_mode))
    {
        std::vector<std::string> file_list;
        DIR* dir = opendir(dirname.c_str());
      
        struct dirent *entry;
        while((entry = readdir(dir)) != NULL){
      	    if((std::string)entry->d_name != "."){
                file_list.push_back(entry->d_name);
            }
        }
        closedir(dir);
      
        std::sort(file_list.begin(), file_list.end());
      
        int i, file_err;
        struct stat file_info;
        for(i = 0; i < file_list.size(); i++)
        {
      	    std::string full_path = dirname + "/" + file_list[i];
      	    file_err = stat(full_path.c_str(), &file_info);
      	    
            FileEntry *temp = new FileEntry();
            std::string fileName = buffer + file_list.at(i);
            if(fileName.size() > 27){
                fileName = fileName.substr(0,27);
                fileName += "...";
            }
            temp->fileName = fileName;
            temp->full_path = full_path;
            temp->namePos.x = 60;
            temp->namePos.y = 45 + (i*40);

            temp->sizePos.x = 450;
            temp->sizePos.y = 45 + (i*40);

            temp->picPos.x = 10;
            temp->picPos.y = 45 + (i*40);
            temp->picPos.h = 25;
            temp->picPos.w = 25;

            if(S_ISDIR(file_info.st_mode)){
      	  	    temp->filePic = "resrc/illustration-data-folder-icon.jpg";
                temp->fileSize = "";

                pic_surf = IMG_Load(temp->filePic.c_str());
                temp->picTexture = SDL_CreateTextureFromSurface(renderer, pic_surf);
                SDL_FreeSurface(pic_surf);

                data_ptr->file_list.push_back(temp);

                // if(recursive && file_list[i] != ".."){
                //     listDirectory(full_path,data_ptr,true,indent+1);
                // }
      	    }
      	    else{
                int fileSize = file_info.st_size;
                temp->fileSize = stringOfSize(fileSize);

                int index = -1;
                for(int j = 0; j < file_list[i].size(); j++){
                    if(file_list[i][j] == '.'){
                        index = j;
                    }
                }
                if(index != -1){
                    std::string extension = file_list[i].substr(index);
                    temp->filePic = getFilePic(extension);
                }
                else{
                    temp->filePic = getFilePic("");
                }

                pic_surf = IMG_Load(temp->filePic.c_str());
                temp->picTexture = SDL_CreateTextureFromSurface(renderer, pic_surf);
                SDL_FreeSurface(pic_surf);

                data_ptr->file_list.push_back(temp);
      	    }
        }
    }
    else{
        fprintf(stderr, "Error: directory '%s' is not found\n", dirname.c_str());       
    }
   
}

std::string getFilePic(std::string ext){
    if(ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".tif" || ext == ".tiff" || ext == ".gif"){ //if the extension is a picture
        return "resrc/image.webp";
    }
    else if(ext == ".mp4" || ext == ".mov" || ext == ".mkv" || ext == ".avi" || ext == "webm"){ //video
        return "resrc/video.webp";
    }
    else if(ext == ".h" || ext == ".c" || ext == ".cpp" || ext == ".py" || ext == ".java" || ext == ".js"){ //code file
        return "resrc/code.webp";
    }
    return "resrc/other.webp";//other file pic
}

std::string stringOfSize(int size){
    if(size >= 1024 && size < 1048576){
        double size2 = ((double)size / 1024.0)*10;
        size = (int)size2;
        return std::to_string(size/10) + "." + std::to_string(size%10) + " MiB";
    }
    else if(size >= 1048576 && size < 1073741824){
        double size2 = ((double)size / 1048576.0)*10;
        size = (int)size2;
        return std::to_string(size/10) + "." + std::to_string(size%10) + " KiB";
    }
    else if(size >= 1073741824){
        double size2 = (double)size / 1073741824.0;
        size2 = static_cast<double>(static_cast<int>(size2*10.0))/10.0;
        return std::to_string(size/10) + "." + std::to_string(size%10) + " GiB";
    }
    
    return std::to_string(size) + " B";
}

void initializeFileNames(SDL_Renderer *renderer, AppData *data_ptr, std::string dirname){
    SDL_Surface *phrase_surf;
    SDL_Color color = { 0, 0, 0 };

    phrase_surf = TTF_RenderText_Solid(data_ptr->font, dirname.c_str(), color);
    data_ptr->phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
    SDL_FreeSurface(phrase_surf);
    data_ptr->phrase_rect.x = 10;
    data_ptr->phrase_rect.y = 0;
    SDL_QueryTexture(data_ptr->phrase, NULL, NULL, &(data_ptr->phrase_rect.w), &(data_ptr->phrase_rect.h));
    data_ptr->phrase_selected = false;

    for(int i = 0; i < data_ptr->file_list.size(); i++){
        //Names of the files
        phrase_surf = TTF_RenderText_Solid(data_ptr->font, data_ptr->file_list.at(i)->fileName.c_str(), color);
        data_ptr->file_list.at(i)->nameTexture = SDL_CreateTextureFromSurface(renderer, phrase_surf);
        SDL_FreeSurface(phrase_surf);

        //size of the files
        phrase_surf = TTF_RenderText_Solid(data_ptr->font, data_ptr->file_list.at(i)->fileSize.c_str(),color);
        data_ptr->file_list.at(i)->sizeTexture = SDL_CreateTextureFromSurface(renderer, phrase_surf);
        SDL_FreeSurface(phrase_surf);
    }
}



