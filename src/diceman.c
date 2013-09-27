#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x82, 0x08, 0xC3, 0xBA, 0x5D, 0x44, 0x46, 0x79, 0x99, 0xBA, 0x1D, 0x94, 0x00, 0x35, 0xE9, 0x33 }
PBL_APP_INFO(MY_UUID,
             "Diceman", "@yonbergman",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;
#define NUMBER_OF_FACES 6
const int IMAGE_RESOURCE_IDS[NUMBER_OF_FACES] = {
  RESOURCE_ID_DIE_FACE_1, RESOURCE_ID_DIE_FACE_2,
  RESOURCE_ID_DIE_FACE_3, RESOURCE_ID_DIE_FACE_4,
  RESOURCE_ID_DIE_FACE_5, RESOURCE_ID_DIE_FACE_6
};

BmpContainer image_container;


void remove_image(){
  layer_remove_from_parent(&image_container.layer.layer);
  bmp_deinit_container(&image_container);
}

void add_image(){
  bmp_init_container(IMAGE_RESOURCE_IDS[(rand() % 6)], &image_container);
  layer_add_child(&window.layer, &image_container.layer.layer);
}

void randomize(){
  remove_image();
  add_image();
}

void single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  randomize();
}


void click_config_provider(ClickConfig **config, Window *window) {
  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) single_click_handler;
  config[BUTTON_ID_UP]->click.handler = (ClickHandler) single_click_handler;
  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) single_click_handler;
}


void handle_init(AppContextRef ctx) {
  resource_init_current_app(&DICE_IMAGES_RESOURCES);
  srand(time(NULL));


  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
  add_image();
  

  WindowHandlers window_handlers = {
    .load = randomize, 
    .appear = randomize
  };
  window_set_window_handlers(&window, window_handlers);
  window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
}

void handle_deinit(AppContextRef ctx) {
  remove_image();
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit
  };
  app_event_loop(params, &handlers);
}
