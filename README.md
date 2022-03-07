

# Iterative Tree Traversal Algorithm (Tricky Traversal)

This repo acts to document the Iterative Tree Traversal Algorithm.
I like to call it the Tricky Traversal.
In short, it is an algorithm that can traverse trees without recursive
function calls.
Realistically,
this algorithm is just for fun and doesn't have much practical use.
The most practical use is for traversing unbalanced trees that lack parent
pointers.


## Conflict Notes

Note that I have no idea if this method has already been discovered
or documented.
If it is, then it must go by a name I had trouble finding.
I'm publishing this because I find it to be a fun algorithmic curiosity.


## Algorithm

See `traverse` function in `tricky.c`.


## Tricky Traversal Notes

Trees have a naturally recursive structure.
We often want to traverse the tree or subtree performing an operation.
However, to minimize the memory footprint of the datastructure
the parent node pointers may be omitted.
Storing the pointers on the stack might be an option,
unless the tree is possibly unbalanced, then you take a gamble.
Unbalanced trees could cause a stack overflow.
Here I present an alternative method.

The algorithm mutates the tree and requires one bit of information per level
of the tree.
The bit of information can be stored in the lowest bit of a pointer that
has a minumum of a two-byte alignment (can be forced with `posix_memalign`).
Alternatively, if the tree is a balanced binary tree a `uint32_t` or `uint64_t`
could be used to store the bits needed at each level.

## Limitations

* Not reentrant.
* Not thread-safe unless surrounded by mutex.
* Two-byte minimum aligned pointers for unbalanced tree implementations
  (lowest bit in pointer is available for use).
* Mutates datastructure during traversal.


