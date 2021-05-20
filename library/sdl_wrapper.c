#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "sdl_wrapper.h"

const char WINDOW_TITLE[] = "CS 3";
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 512;
const double MS_PER_S = 1e3;
const double map_scale = 10;

/**
 * The coordinate at the center of the screen.
 */
vector_t center;

vector_t camera;

/**
 * The coordinate difference from the center to the top right corner.
 */
vector_t max_diff;
/**
 * The SDL window where the scene is rendered.
 */
SDL_Window *window;
/**
 * The renderer used to draw the scene.
 */
SDL_Renderer *renderer;
/**
 * The keypress handler, or NULL if none has been configured.
 */
key_handler_t key_handler = NULL;
/**
 * SDL's timestamp when a key was last pressed or released.
 * Used to mesasure how long a key has been held.
 */
uint32_t key_start_timestamp;
/**
 * The value of clock() when time_since_last_tick() was last called.
 * Initially 0.
 */
clock_t last_clock = 0;

/** Computes the center of the window in pixel coordinates */
vector_t get_window_center(void) {
    int *width = malloc(sizeof(*width)),
        *height = malloc(sizeof(*height));
    assert(width != NULL);
    assert(height != NULL);
    SDL_GetWindowSize(window, width, height);
    vector_t dimensions = {.x = *width, .y = *height};
    free(width);
    free(height);
    return vec_multiply(0.5, dimensions);
}

/**
 * Computes the scaling factor between scene coordinates and pixel coordinates.
 * The scene is scaled by the same factor in the x and y dimensions,
 * chosen to maximize the size of the scene while keeping it in the window.
 */
double get_scene_scale(vector_t window_center) {
    // Scale scene so it fits entirely in the window
    double x_scale = window_center.x / max_diff.x,
           y_scale = window_center.y / max_diff.y;
    return x_scale < y_scale ? x_scale : y_scale;
}

/** Maps a scene coordinate to a window coordinate */
vector_t get_window_position(vector_t scene_pos, vector_t window_center) {
    // Scale scene coordinates by the scaling factor
    // and map the center of the scene to the center of the window
    vector_t scene_center_offset = vec_subtract(scene_pos, center);
    double scale = get_scene_scale(window_center);
    vector_t pixel_center_offset = vec_multiply(scale, scene_center_offset);
    vector_t pixel = {
        .x = round(window_center.x + pixel_center_offset.x),
        // Flip y axis since positive y is down on the screen
        .y = round(window_center.y - pixel_center_offset.y)
    };
    return pixel;
}

/**
 * Converts an SDL key code to a char.
 * 7-bit ASCII characters are just returned
 * and arrow keys are given special character codes.
 */
char get_keycode(SDL_Keycode key) {
    switch (key) {
        case SDLK_LEFT:  return LEFT_ARROW;
        case SDLK_UP:    return UP_ARROW;
        case SDLK_RIGHT: return RIGHT_ARROW;
        case SDLK_DOWN:  return DOWN_ARROW;
        default:
            // Only process 7-bit ASCII characters
            return key == (SDL_Keycode) (char) key ? key : '\0';
    }
}


void sdl_init(vector_t min, vector_t max) {
    // Check parameters
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int Width = DM.w;
    int Height = DM.h;

    printf("%d %d\n", Width, Height);
    assert(min.x < max.x);
    assert(min.y < max.y);

    center = vec_multiply(0.5, vec_add(min, max));
    max_diff = vec_subtract(max, center);
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    camera = (vector_t) {0, 0};
    renderer = SDL_CreateRenderer(window, -1, 0);
}

int nearest_window_width_multiple(double width) {
    printf("New w/h %f %d\n", width, (int) (floor(width) - ((int) floor(width) % 1024)));
    return (int) (floor(width) - ((int) floor(width) % 1024));
} 

bool sdl_is_done(scene_t *scene) {
    SDL_Event *event = malloc(sizeof(*event));
    assert(event != NULL);
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT:
                free(event);
                return true;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                // Skip the keypress if no handler is configured
                // or an unrecognized key was pressed
                if (key_handler == NULL) break;
                char key = get_keycode(event->key.keysym.sym);
                if (key == '\0') break;

                uint32_t timestamp = event->key.timestamp;
                if (!event->key.repeat) {
                    key_start_timestamp = timestamp;
                }
                key_event_type_t type =
                    event->type == SDL_KEYDOWN ? KEY_PRESSED : KEY_RELEASED;
                double held_time = (timestamp - key_start_timestamp) / MS_PER_S;
                key_handler(key, type, held_time, scene);
                break;
        }
    }
    free(event);
    return false;
}

void sdl_clear(void) {
    SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
    SDL_RenderClear(renderer);
}

/* loads textures */
SDL_Texture *sdl_load_texture(const char *path) {
    return IMG_LoadTexture(renderer, path);
}

void sdl_set_camera(vector_t cam) {
    camera = cam;
}

bool sdl_is_onscreen(double x, double y) {
    //printf("Camera: %f %f\n", camera.x, camera.y);
    return x >= camera.x && x <= WINDOW_WIDTH+camera.x && y >= camera.y && y <= WINDOW_HEIGHT+camera.y;
}

void sdl_draw_texture(SDL_Texture *texture, SDL_Rect source, rect_t destination, bool flipped) {
    // Check all four corners are on screen
    if(!sdl_is_onscreen(destination.x, destination.y) &&
       !sdl_is_onscreen(destination.x+destination.w, destination.y+destination.h) &&
       !sdl_is_onscreen(destination.x+destination.w, destination.y) &&
       !sdl_is_onscreen(destination.x, destination.y+destination.h)) return;
    SDL_Rect dest_window;
    vector_t window_center = get_window_center();
    double scale = get_scene_scale(window_center);
    dest_window.w = (int) round(destination.w) * scale;
    dest_window.h = (int) round(destination.h) * scale;

    //change x and y of destination to window coordinates
    vector_t destPos = get_window_position(vec_subtract((vector_t) {destination.x, destination.y}, camera), window_center);
    dest_window.x = (int) round(destPos.x);
    dest_window.y = (int) round(destPos.y) - dest_window.h; // Scene standard: x,y is bottom left of sprite

    SDL_RendererFlip flip;
    if(flipped == true) {
        flip = SDL_FLIP_HORIZONTAL;
    } else {
        flip = SDL_FLIP_NONE;
    }

    SDL_RenderCopyEx(renderer, texture, &source, &dest_window, 0, NULL, flip);
}


void sdl_show(void) {
    // Draw boundary lines
    vector_t window_center = get_window_center();
    vector_t max = vec_add(center, max_diff),
             min = vec_subtract(center, max_diff);
    vector_t max_pixel = get_window_position(max, window_center),
             min_pixel = get_window_position(min, window_center);
    SDL_Rect *boundary = malloc(sizeof(*boundary));
    boundary->x = min_pixel.x;
    boundary->y = max_pixel.y;
    boundary->w = max_pixel.x - min_pixel.x;
    boundary->h = min_pixel.y - max_pixel.y;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, boundary);
    free(boundary);

    SDL_RenderPresent(renderer);
}


void sdl_render_tilemap(list_t *tiles) {
    for(size_t i = 0; i<list_size(tiles); i++) {
        tile_t *tile = list_get(tiles, i);
        tile_draw(tile);
    }
}
void sdl_render_scene(scene_t *scene) {
    sdl_clear();
    // Tilemap rendering: floor
    sdl_render_tilemap(scene_get_floor_tiles(scene));

    // Sprite rendering
    size_t body_count = scene_bodies(scene);
    for (size_t i = 0; i < body_count; i++) {
        body_t *body = scene_get_body(scene, i);
        SDL_Rect shape = body_get_shape(body);
        rect_t hitbox = body_get_hitbox(body);
        SDL_Texture *texture = body_get_texture(body);
        sdl_draw_texture(texture, shape, hitbox, body_get_flipped(body));
    }

    // Tilemap rendering: walls
    sdl_render_tilemap(scene_get_wall_tiles(scene));
    //sdl_render_tilemap(scene_get_collider_tiles(scene));

    sdl_show();
}

void sdl_on_key(key_handler_t handler) {
    key_handler = handler;
}


double time_since_last_tick(void) {
    clock_t now = clock();
    double difference = last_clock
        ? (double) (now - last_clock) / CLOCKS_PER_SEC
        : 0.0; // return 0 the first time this is called
    last_clock = now;
    return difference;
}

