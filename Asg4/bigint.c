//Garrett Stoll gstoll
// $Id$

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"
#include "debug.h"

#define MIN_CAPACITY 16

struct bigint {
   size_t capacity;
   size_t size;
   bool negative;
   char *digits;
};

bool do_compare (bigint *this, bigint *that) {
   bool biggest;
   size_t index;
   if(this->size > that->size) {
      index = this->size;
   }else{
      index = that->size;
   }
   for(size_t i = 0; i < index; ++i) {
      if(this->digits[i] > that->digits[i]) {
      biggest = true;
   } else { biggest = false;
   }
  }
 return biggest;
}

void trim_zeros (bigint *this) {
   while (this->size > 0) {
      size_t digitpos = this->size - 1;
      if (this->digits[digitpos] != 0) break;
      --this->size;
   }
}

bigint *new_bigint (size_t capacity) {

   bigint *this = malloc (sizeof (bigint));
   assert (this != NULL);
   this->capacity = capacity;
   this->size = 0;
   this->negative = false;
   this->digits = calloc (this->capacity, sizeof (char));
   assert (this->digits != NULL);
   DEBUGS ('b', show_bigint (this));
   return this;
}


bigint *new_string_bigint (const char *string) {
   assert (string != NULL);
   size_t length = strlen (string);
   bigint *this = new_bigint (length > MIN_CAPACITY
                            ? length : MIN_CAPACITY);
   const char *strdigit = &string[length - 1];
   if (*string == '_') {
      this->negative = true;
      ++string;
   }
   char *thisdigit = this->digits;
   while (strdigit >= string) {
      assert (isdigit (*strdigit));
      *thisdigit++ = *strdigit-- - '0';
   }
   this->size = thisdigit - this->digits;
   trim_zeros (this);
   DEBUGS ('b', show_bigint (this));
   return this;
}

static bigint *do_add (bigint *this, bigint *that) {
  bigint *result = new_bigint(this->capacity + 1);
  result->size = result->capacity;
  int carry = 0;
  int digit;
  
  for(size_t i = 0; i < this->size; ++i){ 
     digit = this->digits[i] + that->digits[i] + carry;
     result->digits[i] = digit % 10;
     carry = digit / 10;
     }
     trim_zeros(result);
     return result;  

}

static bigint *do_sub (bigint *this, bigint *that) {
   
   bigint *result = new_bigint(this->capacity);
   result->size = this->size;
   
   int borrow = 0;
   int digit = 0;
  
  for(size_t i = 0; i < this->size; ++i){ 
     digit = this->digits[i] - that->digits[i] - borrow + 10;
     result->digits[i] = digit % 10;
     borrow = 1 - digit / 10;

     
     }
     
    trim_zeros(result);
    return result;
}

void free_bigint (bigint *this) {
   free (this->digits);
   free (this);
}

void print_bigint (bigint *this) {
   
   if(this->negative) printf("-");
   for(char *out = &this->digits[this->size-1]; out >= this->digits; 
   --out){
   printf("%d", *out);
  }
  printf("\n");
}

bigint *add_bigint (bigint *this, bigint *that) {
    
   bigint *result;
   bool this_bigger = do_compare (this,that);
   
   if((this->negative) == that->negative){
    
      result = do_add(this, that);
      result->negative = this->negative;
   }
   else{
   
   if(this_bigger){
   
     result = do_sub(this, that);
     result->negative = this->negative;
   
   }
   else{
    
     result = do_sub(that, this);
     result->negative = that->negative;
   }
    
   }
   
   return result;
}

bigint *sub_bigint (bigint *this, bigint *that) {

   bigint *result;
   bool this_bigger = do_compare (this,that);
    
   if(this->negative != that->negative){
     
      result = do_add(this, that);
      result->negative = this->negative;
   }
   else{
      if(this_bigger){
      
        if(this->negative){
           result = do_sub(this, that);
           result->negative = true;
          
        }
        else{
       
           result = do_sub(this, that);
           result->negative = false;           
        }
      }
        else{
            if(this->negative){
           
               result = do_sub(that, this);
               result->negative = false;
            }
            else{
            
              result = do_sub(that, this);
              result->negative = true;
        }
     
       }
   }
    
    
   return result;
}


bigint *mul_bigint (bigint *this, bigint *that) {
   bigint *result = new_bigint(this->capacity + that->capacity);
   result->size = this->size + that->size;
   result->negative = this->negative != that->negative ? true: false;
   for(size_t m = 0; m < this->capacity; ++m){
   int carry = 0;
      for(size_t n = 0; n < that->capacity; ++n){
         int digit = result->digits[m + n] + 
         (this->digits[m] * that->digits[n]) + carry;
         result->digits[m + n] = digit % 10;
         carry = digit / 10;      
      }
      result->digits[m + that->capacity] += carry;
   }
   trim_zeros(result);
   return result;
}

void show_bigint (bigint *this) {
   fprintf (stderr, "bigint@%p->{%lu,%lu,%c,%p->", this,
            this->capacity, this->size, this->negative ? '-' : '+',
            this->digits);
   for (char *byte = &this->digits[this->size - 1];
        byte >= this->digits; --byte) {
      fprintf (stderr, "%d", *byte);
   }
   fprintf (stderr, "}\n");
}

