void object_list_init(object_list_t* object_list);
void object_list_deinit(object_list_t* object_list);

void object_list_init_addr(object_list_t* object_list, rom_t* rom, uint32_t addr);
void object_list_deinit_addr(object_list_t* object_list);


void object_list_add(object_list_t* objects, struct object object) {


}

void object_list_del(object_list_t* objects, struct object* object) {


}

void object_list_render(object_list_t* objects, map16_t* map16, layer16_t* layer) {


}
