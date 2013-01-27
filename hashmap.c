#include <stdlib.h>
#include <stdio.h>

#include "hashmap.h"

#define INITIAL_SIZE 100

struct hashmap_element{
  const void * key;
  void * data;
};

typedef struct hashmap_element * e_p;

int get_index(hashmap_pointer hm, const void * key){
  return (*hm->h_f)(key) % hm->map_size;
}

e_p getElementByKey(hashmap_pointer hm, const void * key){
  // The key is in this list if it's in the map
  linked_list_pointer l = hm->map[get_index(hm, key)];
  if (linked_list_is_empty(l))
    return NULL;
  linked_list_restart(l);
  while (!linked_list_end(l)){
    e_p element = linked_list_get(l);
    if ((*hm->e_f)(element->key, key))
      return element;
    linked_list_next(l);
  }
  e_p element = linked_list_get(l);
  if ((*hm->e_f)(element->key, key))
    return element;
  return NULL;
}

void fill_map(linked_list_pointer * map, int size){
  for (int i = 0; i < size; i++)
    map[i] = new_linked_list();
}

hashmap_pointer new_hashmap_sized(hash_function h_f,
                                  equals_function e_f,
                                  int size){
  struct hashmap * hm = malloc(sizeof(struct hashmap));
  hm->h_f = h_f;
  hm->e_f = e_f;
  hm->map_size = size;
  hm->nb_elements = 0;
  hm->map = malloc(size * sizeof(linked_list_pointer));
  hm->actual_indice = 0;
  fill_map(hm->map, size);
  return hm;
}

hashmap_pointer new_hashmap(hash_function h_f, equals_function e_f){
  hashmap_pointer hm = new_hashmap_sized(h_f, e_f, INITIAL_SIZE);
  return hm;
}

void hashmap_resize(hashmap_pointer hm, int new_size){
  hashmap_pointer new_hm = new_hashmap_sized(hm->h_f, hm->e_f, new_size);
  hashmap_start_iteration(hm);
  while(!hashmap_is_ended_iteration(hm)){
    void * value = hashmap_get_current_value(hm);
    const void * key = hashmap_get_current_key(hm);
    hashmap_add(new_hm, key, value);
    hashmap_next_element(hm);
  }
  int old_map_size = hm->map_size;
  linked_list_pointer * old_map = hm->map;
  hm->map = new_hm->map;
  hm->map_size = new_hm->map_size;
  new_hm->map = old_map;
  new_hm->map_size = old_map_size;
  hashmap_destroy(new_hm, false, false);
}

void * hashmap_get(hashmap_pointer hm, const void * key){
  e_p element = getElementByKey(hm, key);
  if (element == NULL)
    return NULL;  
  return element->data;
}

bool hashmap_exists(hashmap_pointer hm, const void * key){
  return getElementByKey(hm, key) != NULL;
}

void * hashmap_add(hashmap_pointer hm, const void * key, void * value){
  if (hm->map_size <= hm->nb_elements)
    hashmap_resize(hm, hm->map_size * 2);
  if (hashmap_exists(hm, key))
    return NULL;
  int index = get_index(hm, key);
  e_p element = malloc(sizeof(struct hashmap_element));
  element->key = key;
  element->data = value;
  linked_list_insert_in_head(hm->map[index], element);
  hm->nb_elements++;
  return value;
}

void hashmap_remove(hashmap_pointer hm,
                    const void * key,
                    bool free_key,
                    bool free_data){
  if (hm->map_size > 4 &&
      hm->map_size / 4 >= hm->nb_elements)
    hashmap_resize(hm, hm->map_size / 2);
  linked_list_pointer l = hm->map[get_index(hm, key)];
  if (linked_list_is_empty(l))
    return;
  linked_list_restart(l);
  e_p element;
  while (!linked_list_end(l)){
    element = linked_list_get(l);
    if ((*hm->e_f)(element->key, key)){
      if (free_key)
        free((void *)element->key);
      if (free_data)
        free(element->data);
      linked_list_remove(l);
      return;
    }
    linked_list_next(l);
  }
  element = linked_list_get(l);
  if ((*hm->e_f)(element->key, key)){
    if (free_key)
      free((void*)element->key);
    if (free_data)
      free(element->data);
    linked_list_remove(l);
  }
}

int hashmap_size(hashmap_pointer hm){
  return hm->nb_elements;
}

/* destroy an hashmap and free all of it's content */
void hashmap_destroy(hashmap_pointer hm,
                      bool free_keys,
                      bool free_data){
  for(int i = 0; i < hm->map_size; i++){
    linked_list_pointer l = hm->map[i];
    linked_list_restart(l);
    while(!linked_list_end(l)){
      e_p element = linked_list_get(l);
      if (free_keys)
        free((void*)element->key);
      if (free_data)
        free(element->data);
      linked_list_next(l);
    }
    if (!linked_list_is_empty(l)){
      e_p element = linked_list_get(l);
      if (free_keys)
        free((void *)element->key);
      if (free_data)
        free(element->data);
    }
    linked_list_destroy(l);
  }
  free(hm->map);
  free(hm);
}


void hashmap_start_iteration(hashmap_pointer hm){
  if (hm->nb_elements == 0){
    hm->actual_indice = hm->map_size;
    return;
  }
  hm->actual_indice = 0;
  while(linked_list_size(hm->map[hm->actual_indice]) == 0)
    hm->actual_indice++;
  linked_list_restart(hm->map[hm->actual_indice]);
}
e_p hashmap_get_current_element(hashmap_pointer hm){
  return linked_list_get(hm->map[hm->actual_indice]);
}

const void * hashmap_get_current_key(hashmap_pointer hm){
  return hashmap_get_current_element(hm)->key;
}

void * hashmap_get_current_value(hashmap_pointer hm){
  return hashmap_get_current_element(hm)->data;
}

void hashmap_next_element(hashmap_pointer hm){
  if (!linked_list_end(hm->map[hm->actual_indice])){
    linked_list_next(hm->map[hm->actual_indice]);
    return;
  }
  while(hm->actual_indice < hm->map_size - 1){
    hm->actual_indice++;
    if (linked_list_size(hm->map[hm->actual_indice]) > 0){
      linked_list_restart(hm->map[hm->actual_indice]);
      return;
    }
  }
  hm->actual_indice = hm->map_size;
  return;

}

bool hashmap_is_ended_iteration(hashmap_pointer hm){
  return hm->actual_indice == hm->map_size;
}
