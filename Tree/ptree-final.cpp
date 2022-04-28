/*
*  File:        ptree.cpp
*  Description: Implementation of a partitioning tree class for CPSC 221 PA3
*  Date:        2022-03-03 01:53
*
*               ADD YOUR PRIVATE FUNCTION IMPLEMENTATIONS TO THE BOTTOM OF THIS FILE
*/

#include "ptree.h"
#include "hue_utils.h" // useful functions for calculating hue averages

using namespace cs221util;
using namespace std;

// The following definition may be convenient, but is not necessary to use
typedef pair<unsigned int, unsigned int> pairUI;

/////////////////////////////////
// PTree private member functions
/////////////////////////////////

/*
*  Destroys all dynamically allocated memory associated with the current PTree object.
*  You may want to add a recursive helper function for this!
*  POST: all nodes allocated into the heap have been released.
*/
void PTree::Clear() {
  // add your implementation below
  Clear(root);
}

void PTree::Clear(Node*& subTree) {
  if (subTree) {
    Clear(subTree->A);
    Clear(subTree->B);
    delete subTree;
    subTree = nullptr;
  }
}

/*
*  Copies the parameter other PTree into the current PTree.
*  Does not free any memory. Should be called by copy constructor and operator=.
*  You may want a recursive helper function for this!
*  PARAM: other - the PTree which will be copied
*  PRE:   There is no dynamic memory associated with this PTree.
*  POST:  This PTree is a physically separate copy of the other PTree.
*/
void PTree::Copy(const PTree& other) {
  // add your implementation below
  Copy(root, other.root);
}

void PTree::Copy(Node*& thisSubTree, const Node* otherSubTree) {
  if (otherSubTree) {
    thisSubTree = new Node(otherSubTree->upperleft, otherSubTree->width, otherSubTree->height, otherSubTree->avg, nullptr, nullptr);
    Copy(thisSubTree->A, otherSubTree->A);
    Copy(thisSubTree->B, otherSubTree->B);
  }
}

/*
*  Private helper function for the constructor. Recursively builds the tree
*  according to the specification of the constructor.
*  You *may* change this if you like, but we have provided here what we
*  believe will be sufficient to use as-is.
*  PARAM:  im - full reference image used for construction
*  PARAM:  ul - upper-left image coordinate of the currently building Node's image region
*  PARAM:  w - width of the currently building Node's image region
*  PARAM:  h - height of the currently building Node's image region
*  RETURN: pointer to the fully constructed Node
*/
Node* PTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
  // replace the line below with your implementation
  Node* newNode = new Node(ul, w, h, AverageColor(im, ul, w, h), nullptr, nullptr);
  if (h == 1 && w == 1) {
    // do nothing
  } else if (h > w) {
    newNode->A = BuildNode(im, ul, w, h/2);
    newNode->B = BuildNode(im, make_pair((unsigned int)ul.first, (unsigned int)ul.second + h/2), w, h-h/2);
  } else {
    newNode->A = BuildNode(im, ul, w/2, h);
    newNode->B = BuildNode(im, make_pair((unsigned int)ul.first + w/2, (unsigned int)ul.second), w-w/2, h);
  }
  return newNode;
}

HSLAPixel PTree::AverageColor(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
  double totalHueX = 0;
  double totalHueY = 0;
  double stotal = 0;
  double ltotal = 0;
  double atotal = 0;

  for (unsigned x = ul.first; x < ul.first + w; x++) {
    for (unsigned y = ul.second; y < ul.second + h; y++) {
      totalHueX += Deg2X(im.getPixel(x, y)->h);
      totalHueY += Deg2Y(im.getPixel(x, y)->h);
      stotal += im.getPixel(x, y)->s;
      ltotal += im.getPixel(x, y)->l;
      atotal += im.getPixel(x, y)->a;
    }
  }

  unsigned int numPixels = w * h;

  return HSLAPixel(XY2Deg(totalHueX, totalHueY), stotal / numPixels, ltotal / numPixels, atotal / numPixels);
}

////////////////////////////////
// PTree public member functions
////////////////////////////////

/*
*  Constructor that builds the PTree using the provided PNG.
*
*  The PTree represents the sub-image (actually the entire image) from (0,0) to (w-1, h-1) where
*  w-1 and h-1 are the largest valid image coordinates of the original PNG.
*  Each node corresponds to a rectangle of pixels in the original PNG, represented by
*  an (x,y) pair for the upper-left corner of the rectangle, and two unsigned integers for the
*  number of pixels on the width and height dimensions of the rectangular sub-image region the
*  node defines.
*
*  A node's two children correspond to a partition of the node's rectangular region into two
*  equal (or approximately equal) size regions which are either tiled horizontally or vertically.
*
*  If the rectangular region of a node is taller than it is wide, then its two children will divide
*  the region into vertically-tiled sub-regions of equal height:
*  +-------+
*  |   A   |
*  |       |
*  +-------+
*  |   B   |
*  |       |
*  +-------+
*
*  If the rectangular region of a node is wider than it is tall, OR if the region is exactly square,
*  then its two children will divide the region into horizontally-tiled sub-regions of equal width:
*  +-------+-------+
*  |   A   |   B   |
*  |       |       |
*  +-------+-------+
*
*  If any region cannot be divided exactly evenly (e.g. a horizontal division of odd width), then
*  child B will receive the larger half of the two subregions.
*
*  When the tree is fully constructed, each leaf corresponds to a single pixel in the PNG image.
* 
*  For the average colour, this MUST be computed separately over the node's rectangular region.
*  Do NOT simply compute this as a weighted average of the children's averages.
*  The functions defined in hue_utils.h and implemented in hue_utils.cpp will be very useful.
*  Computing the average over many overlapping rectangular regions sounds like it will be
*  inefficient, but as an exercise in theory, think about the asymptotic upper bound on the
*  number of times any given pixel is included in an average calculation.
* 
*  PARAM: im - reference image which will provide pixel data for the constructed tree's leaves
*  POST:  The newly constructed tree contains the PNG's pixel data in each leaf node.
*/
PTree::PTree(PNG& im) {
  // add your implementation below
  root = BuildNode(im, make_pair((unsigned int)0, (unsigned int)0), im.width(), im.height());
}

/*
*  Copy constructor
*  Builds a new tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  This tree is constructed as a physically separate copy of other tree.
*/
PTree::PTree(const PTree& other) {
  // add your implementation below
  Copy(other);
}

/*
*  Assignment operator
*  Rebuilds this tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  If other is a physically different tree in memory, all pre-existing dynamic
*           memory in this tree is deallocated and this tree is reconstructed as a
*           physically separate copy of other tree.
*         Otherwise, there is no change to this tree.
*/
PTree& PTree::operator=(const PTree& other) {
  // add your implementation below
  if (&other != this) {
    Clear();
    Copy(other);
  }
  return *this;
}

/*
*  Destructor
*  Deallocates all dynamic memory associated with the tree and destroys this PTree object.
*/
PTree::~PTree() {
  // add your implementation below
  Clear();
}

/*
*  Traverses the tree and puts the leaf nodes' color data into the nodes'
*  defined image regions on the output PNG.
*  For non-pruned trees, each leaf node corresponds to a single pixel that will be coloured.
*  For pruned trees, each leaf node may cover a larger rectangular region that will be
*  entirely coloured using the node's average colour attribute.
*
*  You may want to add a recursive helper function for this!
*
*  RETURN: A PNG image of appropriate dimensions and coloured using the tree's leaf node colour data
*/
PNG PTree::Render() const {
  // replace the line below with your implementation
  PNG im(root->width, root->height);
  Render(root, im);
  return im;
}

void PTree::Render(const Node* subTree, PNG& im) const {
  if (subTree) {
    if (!(subTree->A || subTree->B)) {
      for (unsigned x = subTree->upperleft.first; x < subTree->upperleft.first + subTree->width; x++) {
        for (unsigned y = subTree->upperleft.second; y < subTree->upperleft.second + subTree->height; y++) {
          *(im.getPixel(x, y)) = subTree->avg;
        }
      }
    } else {
      Render(subTree->A, im);
      Render(subTree->B, im);
    }
  }
}

/*
*  Trims subtrees as high as possible in the tree. A subtree is pruned
*  (its children are cleared/deallocated) if ALL of its leaves have colour
*  within tolerance of the subtree root's average colour.
*  Pruning criteria should be evaluated on the original tree, and never on a pruned
*  tree (i.e. we expect that Prune would be called on any tree at most once).
*  When processing a subtree, you should determine if the subtree should be pruned,
*  and prune it if possible before determining if it has subtrees that can be pruned.
* 
*  You may want to add (a) recursive helper function(s) for this!
*
*  PRE:  This tree has not been previously pruned (and is not copied/assigned from a tree that has been pruned)
*  POST: Any subtrees (as close to the root as possible) whose leaves all have colour
*        within tolerance from the subtree's root colour will have their children deallocated;
*        Each pruned subtree's root becomes a leaf node.
*/
void PTree::Prune(double tolerance) {
  // add your implementation below
  Prune(root, tolerance);
}

void PTree::Prune(Node* subTree, double tolerance) {
  if (subTree) {
    if (CanBePruned(subTree, subTree->avg, tolerance)) {
      Clear(subTree->A);
      Clear(subTree->B);
    } else {
      Prune(subTree->A, tolerance);
      Prune(subTree->B, tolerance);
    }
  }
}

bool PTree::CanBePruned(Node* subTree, HSLAPixel rootColour, double tolerance) {
  if (!subTree) {
    return false;
  } else {
    if (!(subTree->A || subTree->B)) {
      return subTree->avg.dist(rootColour) <= tolerance;
    } else {
      return CanBePruned(subTree->A, rootColour, tolerance) && CanBePruned(subTree->B, rootColour, tolerance);
    }
  }
}

/*
*  Returns the total number of nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::Size() const {
  // replace the line below with your implementation
  return Size(root);
}

int PTree::Size(const Node* subTree) const {
  if (!subTree) {
    return 0;
  } else {
    return 1 + Size(subTree->A) + Size(subTree->B);
  }
}

/*
*  Returns the total number of leaf nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::NumLeaves() const {
  // replace the line below with your implementation
  return NumLeaves(root);
}

int PTree::NumLeaves(const Node* subTree) const {
  if (!subTree) {
    return 0;
  } else if (!(subTree->A || subTree->B)) {
    return 1;
  } else {
    return NumLeaves(subTree->A) + NumLeaves(subTree->B);
  }
}

/*
*  Rearranges the nodes in the tree, such that a rendered PNG will be flipped horizontally
*  (i.e. mirrored over a vertical axis).
*  This can be achieved by manipulation of the nodes' member attribute(s).
*  Note that this may possibly be executed on a pruned tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*
*  POST: Tree has been modified so that a rendered PNG will be flipped horizontally.
*/
void PTree::FlipHorizontal() {
  // add your implementation below
  FlipHorizontal(root);
}

void PTree::FlipHorizontal(Node* subTree) {
  if (subTree && subTree->A && subTree->B) {
    if (subTree->height <= subTree->width) {
      subTree->B->upperleft = subTree->upperleft;
      subTree->A->upperleft = subTree->upperleft;
      subTree->A->upperleft.first += subTree->B->width;
      swap(subTree->A, subTree->B);
    } else {
      subTree->A->upperleft.first = subTree->upperleft.first;
      subTree->B->upperleft.first = subTree->upperleft.first;
    }
    FlipHorizontal(subTree->A);
    FlipHorizontal(subTree->B);
  }
}

/*
*  Like the function above, rearranges the nodes in the tree, such that a rendered PNG
*  will be flipped vertically (i.e. mirrored over a horizontal axis).
*  This can be achieved by manipulation of the nodes' member attribute(s).
*  Note that this may possibly be executed on a pruned tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*
*  POST: Tree has been modified so that a rendered PNG will be flipped vertically.
*/
void PTree::FlipVertical() {
  // add your implementation below
  FlipVertical(root);
}

void PTree::FlipVertical(Node* subTree) {
  if (subTree && subTree->A && subTree->B) {
    if (subTree->height > subTree->width) {
      subTree->B->upperleft = subTree->upperleft;
      subTree->A->upperleft = subTree->upperleft;
      subTree->A->upperleft.second += subTree->B->height;
      swap(subTree->A, subTree->B);
    } else {
      subTree->A->upperleft.second = subTree->upperleft.second;
      subTree->B->upperleft.second = subTree->upperleft.second;
    }
    FlipVertical(subTree->A);
    FlipVertical(subTree->B);
  }
}

/*
    *  Provides access to the root of the tree.
    *  Dangerous in practice! This is only used for testing.
    */
Node* PTree::GetRoot() {
  return root;
}

//////////////////////////////////////////////
// PERSONALLY DEFINED PRIVATE MEMBER FUNCTIONS
//////////////////////////////////////////////

