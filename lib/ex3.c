#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ex3.h"
#include "common.h"

void free_node(bp_tree_node_t *node)
{
    if (node->is_leaf_node)
    {
        for (int i = 0; i < node->n; i++)
        {
            free(node->products[i]);
        }
    }
    else
    {
        for (int i = 0; i <= node->n; i++)
        {
            free_node(node->children[i]);
        }
    }
    free(node);
}

void bp_destroy_tree(bp_tree_t *tree)
{
    free_node(tree->root);
    free(tree);
}

bp_tree_t *bp_init_index()
{
    bp_tree_t *tree = calloc(1, sizeof(bp_tree_t));
    tree->root = calloc(1, sizeof(bp_tree_node_t));
    tree->m = 4;
    tree->root->is_leaf_node = true;
    tree->root->n = 0;
    return tree;
}

Product_t *bp_get_product(bp_tree_t *tree, int key)
{
    bp_tree_node_t *node = tree->root;
    while (node && !node->is_leaf_node)
    {
        int i = 0;
        while (i < node->n && key >= node->keys[i])
            i++;
        node = node->children[i];
    }

    if (node)
    {
        for (int i = 0; i < node->n; i++)
        {
            if (node->keys[i] == key)
                return node->products[i];
        }
    }
    return NULL;
}

bp_tree_node_t *create_new_node(bool is_leaf)
{
    bp_tree_node_t *node = calloc(1, sizeof(bp_tree_node_t));
    node->is_leaf_node = is_leaf;
    return node;
}

int bp_insert_product(bp_tree_t *tree, int key, Product_t *product)
{
    // Check empty case
    // Check if product is already indexed
    // Get the corresponding leaf node
    // Insert
    // If there is split needed handle that (you may implement some helper functions then use it here.)
    bp_tree_node_t *node = tree->root, *parent;
    int i;
    while (!node->is_leaf_node)
    {
        i = 0;
        while (i < node->n && key >= node->keys[i])
            i++;
        node = node->children[i];
    }
    for (i = 0; i < node->n; i++)
    {
        if (node->keys[i] == key)
            return 1;
    }
    if (node->n < tree->m)
    {
        i = 0;
        while (i < node->n && node->keys[i] < key)
            i++;
        for (int j = node->n; j > i; j--)
        {
            node->keys[j] = node->keys[j - 1];
            node->products[j] = node->products[j - 1];
        }
        node->keys[i] = key;
        node->products[i] = product;
        node->n++;
    }
    else
    {
        bp_tree_node_t *new_leaf = create_new_node(true);
        new_leaf->parent = node->parent;
        unsigned temp_keys[tree->m + 1];
        Product_t *temp_products[tree->m + 1];
        for (i = 0; i < tree->m; i++)
        {
            temp_keys[i] = node->keys[i];
            temp_products[i] = node->products[i];
        }
        i = 0;
        while (i < tree->m && temp_keys[i] < key)
            i++;
        for (int j = tree->m; j > i; j--)
        {
            temp_keys[j] = temp_keys[j - 1];
            temp_products[j] = temp_products[j - 1];
        }
        temp_keys[i] = key;
        temp_products[i] = product;
        int split_index = 2;
        node->n = split_index;
        new_leaf->n = (tree->m + 1) - split_index;
        for (i = 0; i < new_leaf->n; i++)
        {
            new_leaf->keys[i] = temp_keys[i + split_index];
            new_leaf->products[i] = temp_products[i + split_index];
        }
        new_leaf->next = node->next;
        node->next = new_leaf;
        unsigned new_key = new_leaf->keys[0];
        while (true)
        {
            if (node == tree->root)
            {
                bp_tree_node_t *new_root = create_new_node(false);
                new_root->keys[0] = new_key;
                new_root->children[0] = node;
                new_root->children[1] = new_leaf;
                new_root->n = 1;
                node->parent = new_root;
                new_leaf->parent = new_root;
                tree->root = new_root;
                break;
            }
            parent = node->parent;
            if (parent->n < tree->m)
            {
                i = 0;
                while (i < parent->n && parent->keys[i] < new_key)
                    i++;
                for (int j = parent->n; j > i; j--)
                {
                    parent->keys[j] = parent->keys[j - 1];
                    parent->children[j + 1] = parent->children[j];
                }
                parent->keys[i] = new_key;
                parent->children[i + 1] = new_leaf;
                parent->n++;
                break;
            }
            else
            {
                bp_tree_node_t *new_internal = create_new_node(false);
                unsigned temp_keys_internal[tree->m + 1];
                bp_tree_node_t *temp_children[tree->m + 2];
                for (i = 0; i < tree->m; i++)
                {
                    temp_keys_internal[i] = parent->keys[i];
                    temp_children[i] = parent->children[i];
                }
                temp_children[tree->m] = parent->children[tree->m];
                i = 0;
                while (i < tree->m && temp_keys_internal[i] < new_key)
                    i++;
                for (int j = tree->m; j > i; j--)
                {
                    temp_keys_internal[j] = temp_keys_internal[j - 1];
                    temp_children[j + 1] = temp_children[j];
                }
                temp_keys_internal[i] = new_key;
                temp_children[i + 1] = new_leaf;
                split_index = (tree->m + 1) / 2;
                parent->n = split_index;
                new_internal->n = tree->m - split_index;
                for (i = 0; i < new_internal->n; i++)
                {
                    new_internal->keys[i] = temp_keys_internal[i + split_index + 1];
                    new_internal->children[i] = temp_children[i + split_index + 1];
                }
                new_internal->children[new_internal->n] = temp_children[tree->m + 1];
                new_key = temp_keys_internal[split_index];
                new_leaf = new_internal;
                node = parent;
            }
        }
    }
    return 0;
}

// This is a helper function to printout the B+tree

void printMWayTree(bp_tree_node_t *root, int level)
{
    if (root == NULL)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");
    for (int i = 0; i < root->n; i++)
        printf("%d ", root->keys[i]);

    printf("\n");
    for (int i = 0; i < root->n + 1; i++)
    {
        printMWayTree(root->children[i], level + 1);
    }
}