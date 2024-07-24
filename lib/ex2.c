#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ex2.h"
#include "common.h"

// djb2
unsigned hash(const char *str)
{
    int hash = 5381, c = 0;
    while (*str != '\0')
    {
        c = *str;
        str++;
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

hash_index_t *h_init_index(int size)
{
    if (size <= 0)
        return NULL;
    hash_index_t *index = calloc(1, sizeof(hash_index_t));
    index->size = size;
    index->buckets = calloc(size, sizeof(hash_node_t *));
    return index;
}

void h_destroy_index(hash_index_t *index)
{
    for (int i = 0; i < index->size; i++)
    {
        hash_node_t *current_bucket = index->buckets[i];
        while (current_bucket)
        {
            Product_t *current_product = current_bucket->product;
            while (current_product)
            {
                Product_t *to_free_product = current_product;
                current_product = current_product->next;
                free(to_free_product);
            }
            hash_node_t *to_free_bucket = current_bucket;
            current_bucket = current_bucket->next;
            free(to_free_bucket);
        }
    }
    free(index->buckets);
    free(index);
}

Product_t *h_get_product(hash_index_t *index, const char *name)
{
    int bucket_index = hash(name) % index->size;
    hash_node_t *current_bucket = index->buckets[bucket_index];
    while (current_bucket)
    {
        Product_t *current = current_bucket->product;
        while (current)
        {
            if (!strcmp(current->name, name))
            {
                return current;
            }
            current = current->next;
        }
        current_bucket = current_bucket->next;
    }
    return NULL;
}

void h_insert_product(hash_index_t *index, Product_t *product)
{
    int bucket_index = hash(product->name) % index->size;
    if (!index->buckets[bucket_index])
    {
        index->buckets[bucket_index] = calloc(1, sizeof(hash_node_t));
        index->buckets[bucket_index]->product = product;
        index->buckets[bucket_index]->next = NULL;
    }
    else
    {
        hash_node_t *current_bucket = index->buckets[bucket_index];
        while (current_bucket->next)
        {
            current_bucket = current_bucket->next;
        }
        current_bucket->next = calloc(1, sizeof(hash_node_t));
        current_bucket->next->product = product;
        current_bucket->next->next = NULL;
    }
}

int h_delete_product(hash_index_t *index, const char *name)
{
    int bucket_index = hash(name) % index->size;
    hash_node_t *current_bucket = index->buckets[bucket_index], *prev_bucket = NULL;
    while (current_bucket)
    {
        Product_t *current_product = current_bucket->product;
        if (current_product && !strcmp(current_product->name, name))
        {
            if (prev_bucket)
            {
                prev_bucket->next = current_bucket->next;
            }
            else
            {
                index->buckets[bucket_index] = current_bucket->next;
            }
            free(current_product);
            free(current_bucket);
            return 0;
        }
        prev_bucket = current_bucket;
        current_bucket = current_bucket->next;
    }
    return 1;
}
