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

Node* CopyTree(const Node* base);

void ClearTree(Node* &base);

int CalculateSize(Node* base) const;

int CountLeaves(Node* base) const;

void ColorRenderedImage(PNG* img, pair<unsigned int, unsigned int> ul,Node* curr) const;

void Prune_Tree(Node* &base, double tolerance);

bool All_Within_Tolerance(Node* &base, HSLAPixel root_avg, double tolerance);

Node* FlipHor(Node* base);

Node* FlipVer(Node* base);

#endif