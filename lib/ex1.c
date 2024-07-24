#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ex1.h"
#include "common.h"

Product_t *search_product_by_name(Product_t *product_list, const char *name)
{
    while (product_list != NULL) {
        if (strcmp(product_list->name, name) == 0) {
            return product_list;
        }
        product_list = product_list->next;
    }
    return NULL;
}

Product_t *search_product_by_price(Product_t * product_list, int price)
{
    while (product_list != NULL) {
        if (product_list->price == price) {
            return product_list;
        }
        product_list = product_list->next; 
    }
    return NULL;
}

Product_t *search_product_by_id(Product_t * product_list, unsigned id)
{
    while (product_list != NULL) {
        if (product_list->id == id) {
            return product_list;
        }
        product_list = product_list->next;
    }
    return NULL;
}
