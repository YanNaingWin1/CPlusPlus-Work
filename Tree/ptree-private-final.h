/*
*  File:        ptree-private.h
*  Description: Private functions for the PTree class
*  Date:        2022-03-06 03:30
*
*               DECLARE YOUR PTREE PRIVATE MEMBER FUNCTIONS HERE
*/

#ifndef _PTREE_PRIVATE_H_
#define _PTREE_PRIVATE_H_

/////////////////////////////////////////////////
// DEFINE YOUR PRIVATE MEMBER FUNCTIONS HERE
//
// Just write the function signatures.
//
// Example:
//
// Node* MyHelperFunction(int arg_a, bool arg_b);
//
/////////////////////////////////////////////////

void Clear(Node*& subTree);

void Copy(Node*& thisSubTree, const Node* otherSubTree);

HSLAPixel AverageColor(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h);

int Size(const Node* subTree) const;

int NumLeaves(const Node* subTree) const;

void Render(const Node* subTree, PNG& im) const;

void Prune(Node* subTree, double tolerance);

bool CanBePruned(Node* subTree, HSLAPixel rootColour, double tolerance);

void FlipHorizontal(Node* subTree);

void FlipVertical(Node* subTree);

#endif