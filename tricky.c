
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum branch
{
    LEFT  = 0,
    RIGHT = 1,
};

#define SET_LEFT(p) (p)
#define SET_RIGHT(p) ((void *)(((intptr_t)(p)) | RIGHT))
#define RECOVER(p) (((intptr_t)(p)) & (intptr_t)1)
#define CLEAR(p) ((void *)(((intptr_t)(p)) & ~((intptr_t)1)))

enum direction
{
    DOWN,
    UP,
};

enum order
{
    PREORDER,
    INORDER,
    POSTORDER,
    LEAFORDER,
};

const char *names[] = {
    "PREORDER",
    "INORDER",
    "POSTORDER",
    "LEAFORDER",
};

const char *
order_name(enum order order)
{
    return names[order];
}

typedef struct node * Node;

typedef struct node
{
    int value;
    Node left;
    Node right;
} node_t;

void
visit(Node n)
{
    printf("VISIT: %d\n", n->value);
}

void
traverse(enum order order, Node root)
{
    printf("Order: %s\n", order_name(order));
    if (NULL == root)
    {
        printf("EMPTY\n");
        return;
    }


    Node current = root;
    Node parent = NULL;
    Node next = NULL;
    bool done = false;
    enum direction direction = DOWN;


    do
    {
        switch (direction)
        {
            case DOWN:
                // printf("DOWN\n");
                if (PREORDER == order)
                {
                    visit(current);
                }
                if (NULL != current->left)
                {
                    next = current->left;
                    current->left = parent;
                    parent = SET_LEFT(current);
                    current = next;
                    break;
                }
                else if (NULL != current->right)
                {
                    if (INORDER == order)
                    {
                        visit(current);
                    }
                    next = current->right;
                    current->right = parent;
                    parent = SET_RIGHT(current);
                    current = next;
                    break;
                }
                else
                {
                    if (INORDER == order)
                    {
                        visit(current);
                    }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough="
                    direction = UP;
                }
            case UP:
#pragma GCC diagnostic pop
                //printf("UP\n");
                if (POSTORDER == order)
                {
                    visit(current);
                }
                if (NULL != parent)
                {
                    enum branch branch = RECOVER(parent);
                    parent = CLEAR(parent);
                    switch (branch)
                    {
                        case LEFT:
                            if (NULL != parent->right)
                            {
                                if (INORDER == order)
                                {
                                    visit(parent);
                                }
                                next = current;
                                current = parent->right;
                                parent->right = parent->left;
                                parent->left = next;
                                parent = SET_RIGHT(parent);
                                direction = DOWN;
                            }
                            else
                            {
                                if (INORDER == order)
                                {
                                    visit(parent);
                                }
                                next = parent->left;
                                parent->left = current;
                                current = parent;
                                parent = next;
                            }
                            break;
                        case RIGHT:
                            next = parent->right;
                            parent->right = current;
                            current = parent;
                            parent = next;
                            break;
                    }
                }
                else
                {
                    done = true;
                }
                break;
        }
    } while (!done);
}

Node
mk_tree_null(void)
{
    return NULL;
}

static int g_node_i = 0;

void
reset_node(void)
{
    g_node_i = 0;
}

Node
get_node()
{
    void *_n;
    int error;
    if ((error = posix_memalign(&_n, sizeof(void *), sizeof(node_t))))
    {
        printf("Error (%d): %s\n", error, strerror(error));
        exit(1);
    }
    Node n = (Node)_n;
    n->value = g_node_i;
    ++g_node_i;
    n->left = NULL;
    n->right = NULL;
    return n;
}

Node
mk_tree_one(void)
{
    Node n = get_node();
    return n;
}

Node
mk_tree_left(void)
{
    Node n = get_node();
    n->left = get_node();
    return n;
}

Node
mk_tree_right(void)
{
    Node n = get_node();
    n->right = get_node();
    return n;
}

Node
mk_tree_three(void)
{
    Node n = get_node();
    n->left = get_node();
    n->right = get_node();
    return n;
}

Node
mk_tree_small(void)
{
    Node n = get_node();
    n->left = mk_tree_three();
    n->right = mk_tree_three();
    return n;
}

void
free_tree(Node root)
{
    if (NULL == root)
    {
        return;
    }

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}


typedef Node mk_tree_factory(void);

int
main(/*int argc, char *argv[]*/)
{
    mk_tree_factory* factories[] =
    {
        mk_tree_null,
        mk_tree_one,
        mk_tree_left,
        mk_tree_right,
        mk_tree_three,
        mk_tree_small,
    };
    const char *test_names[] =
    {
        "EMPTY",
        "ONE NODE",
        "LEFT NODE",
        "RIGHT NODE",
        "THREE TREE",
        "SMALL TREE",
    };
    int factories_len = sizeof(factories)/sizeof(mk_tree_factory*);

    int i;
    for (i = 0; i < factories_len; ++i)
    {
        reset_node();
        Node root = factories[i]();
        printf("--------------------------------------------\n");
        printf("TEST: %s\n", test_names[i]);
        printf("--------------------------------------------\n");
        traverse(PREORDER, root);
        traverse(INORDER, root);
        traverse(POSTORDER, root);
        free_tree(root);
    }

    return 0;
}


