#include "bed.h"

bool running = true;

void cleanup();
int delete_char(char *text, size_t pos);

void scc(int code) {
    if (code < 0) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        cleanup();
        exit(1);
    }
}
void *scp(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        cleanup();
        exit(2);
    }
    return ptr;
}

#define BUFFER_SIZE 1024
TTF_Font *font = NULL;
SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
char text_buffer[BUFFER_SIZE] = {0};
size_t cursor_pos;
SDL_Surface *text_surface = NULL;
SDL_Rect text_rect = {.x = 0, .y = 0, .w = 100, .h = 100};
SDL_Rect cursor_rect = {.x = 0, .y = 0, .w = 5, .h = 100};
SDL_Texture *text_texture = NULL;

SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;

int main(void) {

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    font = scp(TTF_OpenFont("FiraMono.ttf", 50));

    cursor_pos = strlen(text_buffer);
    text_surface = NULL;
    text_texture = NULL;

    win = scp(SDL_CreateWindow("bed", 0, 0, WIN_WIDTH, WIN_HEIGHT,
                               SDL_WINDOW_RESIZABLE));
    renderer = scp(SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED));

    SDL_Event ev;
    while (running) {
        SDL_WaitEvent(&ev);
        switch (ev.type) {
        case SDL_QUIT: {
            running = false;
            printf("quitting\n");
        } break;
        case SDL_KEYDOWN: {
            switch (ev.key.keysym.sym) {
            case SDLK_BACKSPACE: {
                if (delete_char(text_buffer, cursor_pos - 1))
                    --cursor_pos;
                // if (cursor_pos > 0) {
                // memmove(&text_buffer[cursor_pos - 1],
                //&text_buffer[cursor_pos],
                // strlen(&text_buffer[cursor_pos]));
                // text_buffer[strlen(text_buffer) - 1] = 0;
                //--cursor_pos;
                //}
            } break;
            case SDLK_DELETE: {
                // if (cursor_pos < strlen(text_buffer)) {
                // memmove(&text_buffer[cursor_pos],
                //&text_buffer[cursor_pos + 1],
                // strlen(&text_buffer[cursor_pos + 1]));
                // text_buffer[strlen(text_buffer) - 1] = 0;
                //}
                delete_char(text_buffer, cursor_pos);
            } break;
            case SDLK_RIGHT: {
                if (strlen(text_buffer) > cursor_pos)
                    ++cursor_pos;
            } break;
            case SDLK_LEFT: {
                if (cursor_pos > 0)
                    --cursor_pos;
            } break;
            default:
                break;
            }
        } break;
        case SDL_TEXTINPUT: {
            size_t text_size = strlen(ev.text.text);
            size_t buffer_size = strlen(ev.text.text);
            if (BUFFER_SIZE - buffer_size > text_size) {
                memmove(&text_buffer[cursor_pos + text_size],
                        &text_buffer[cursor_pos],
                        strlen(&text_buffer[cursor_pos]));

                strncpy(&text_buffer[cursor_pos], ev.text.text, text_size);
                cursor_pos += strlen(ev.text.text);
            }
        } break;
        default:
            break;
        }
        scc(SDL_RenderClear(renderer));

        if (strlen(text_buffer) > 0) {
            text_surface =
                scp(TTF_RenderText_Solid(font, text_buffer, white));
            size_t char_width = text_surface->w / strlen(text_buffer);
            text_rect.w = text_surface->w;
            text_rect.h = text_surface->h;

            cursor_rect.x = cursor_pos * char_width;
            cursor_rect.h = text_surface->h;
            cursor_rect.w = char_width;
            text_texture =
                scp(SDL_CreateTextureFromSurface(renderer, text_surface));

            scc(SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0));
            scc(SDL_RenderFillRect(renderer, &cursor_rect));
            SDL_SetTextureColorMod(text_texture, 255, 255, 255);
            scc(SDL_RenderCopy(renderer, text_texture, NULL, &text_rect));

            if (text_texture)
                SDL_DestroyTexture(text_texture);
            if (text_surface)
                SDL_FreeSurface(text_surface);
        }

        scc(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0));
        SDL_RenderPresent(renderer);
    }

    return 0;
}

int delete_char(char *text, size_t pos) {
    if (pos < 0 || pos >= strlen(text))
        return 0;
    memmove(&text[pos], &text[pos + 1], strlen(&text[pos + 1]));
    text[strlen(text) - 1] = 0;
    return 1;
}

void cleanup() {
    if (font)
        TTF_CloseFont(font);
    TTF_Quit();
    if (win)
        SDL_DestroyWindow(win);
    if (renderer)
        SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
