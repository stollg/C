//Garrett Stoll gstoll
//Zachary llewelyn zllewely

// $Id$

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "hashset.h"
#include "strhash.h"

#define HASH_NEW_SIZE 15

typedef struct hashnode hashnode;
struct hashnode {
   char *word;
   hashnode *link;
};

struct hashset {
   size_t size;
   size_t load;
   hashnode **chains;
};

hashset *new_hashset (void) {
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = HASH_NEW_SIZE;
   this->load = 0;
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   memset (this->chains, 0, sizeof_chains);
   DEBUGF ('h', "%p -> struct hashset {size = %zd, chains=%p}\n",
                this, this->size, this->chains);
   return this;
}

void free_hashset (hashset *this) {
   DEBUGF ('h', "free (%p)\n", this);
   
    for(size_t i = 0; i < this->size; i++){
       free(this->chains[i]);
    }
}

void array_dub (hashset *this){
   
      size_t double_length = (2 * this->size)+1;
      //update size
      //create new chain
      hashnode **chain_dub = calloc(double_length, sizeof(hashnode *));
      for(size_t j = 0; j < this->size; j++){
         hashnode *temp = this->chains[j];
            
         while (temp != NULL){
            size_t hash_index = strhash (temp->word) % double_length;
            hashnode *next = temp->link;
            temp->link = chain_dub[j];
            chain_dub[hash_index] = temp;
            temp = next;
         }
      }
          
      free(this->chains);
      //free old chain and replace with new (twice the size)
      this->chains = chain_dub;
      this->size = double_length;
         
} 



void put_hashset (hashset *this, const char *item) {
   
      size_t hash_index;
      this->load++;
      
      if (this->load * 2 > this->size) array_dub(this);      
      
      hash_index = strhash(item) % this->size;
      
      if(has_hashset(this, item)) return;
      
      hashnode *temp = malloc (sizeof (struct hashnode));
      assert (temp != NULL);
      temp->word = strdup(item);  
      temp->link = this->chains[hash_index];
      this->chains[hash_index] = temp;
}


bool has_hashset (hashset *this, const char *item) {
   if (item == NULL) return false;
  
   size_t hash_index;
   hash_index = strhash(item) % this->size;
   hashnode *curr = this->chains[hash_index];
   
   while(curr != NULL){     
       int comp = strcmp(curr->word, item);  
       if(comp == 0) return true;
       curr = curr->link;
   } 
   
   
   return false; 
    

}

void debug_output(hashset *this){
     int *count = calloc(100, sizeof(int));
     printf("%5zd words in the hash set\n", this->load);
     printf("%5zd size of the hash array\n", this->size);
     for (size_t i = 0; i < this->size; i++){
         hashnode *curr = this->chains[i];
         int chaindex = 0;
         for (;curr != NULL; curr = curr->link){
             chaindex++;    
         }
         count[chaindex]++;      
     }
     for(int size = 1; size < 100; size++){
            if(count[size] != 0){
            printf("%5d chains of size %d\n", count[size], size);
            }

 }
 getchar();
 }
void dump_hash(hashset *this){
     for(size_t i = 0; i < this->size; i++){
         hashnode *temp = this->chains[i]; 
         if(temp != NULL){
            size_t hash_index = strhash(temp->word);
            printf("array[%10d] = %20lu \"%s\"\n",
            (int)i, (int)hash_index, temp->word);
            temp = temp->link;
         }
    
} 
}


