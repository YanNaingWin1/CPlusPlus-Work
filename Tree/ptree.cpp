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
  ClearTree(root);
}

// deletes all the children beyond base node
void PTree::ClearTree(Node* &base) {
  if (base == NULL) {
    return;
  }
  ClearTree(base->A);
  ClearTree(base->B);
  delete base;
  base = NULL;
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
  root = CopyTree(other.root);
}

Node* PTree::CopyTree(const Node* base) {
  if (base == NULL) {
    return NULL;
  }
  Node* to_return = new Node(base->upperleft, base->width, base->height, base->avg, base->A, base->B);
  to_return->A = CopyTree(base->A);
  to_return->B = CopyTree(base->B);
  return to_return;
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
  Node* curr = new Node();
  curr->width = w;
  curr->height = h;
  curr->upperleft = ul;

  // read documentation on hue_utils.h to assign avg color
  // can we use memoize to save repeated computation?
  double total_cartesian_X = 0;
  double total_cartesian_Y = 0;
  double total_s = 0;
  double total_l = 0;
  double total_a = 0;
  for (unsigned int y = ul.second; y < h + ul.second; y++) {
    for (unsigned int x = ul.first; x < w + ul.first; x++) {
      HSLAPixel* pixel = im.getPixel(x,y);
      total_cartesian_X += Deg2X(pixel->h);
      total_cartesian_Y += Deg2Y(pixel->h);
      total_s += pixel->s;
      total_l += pixel->l;
      total_a += pixel->a;
    }
  }

  double dimension = (double) w * (double) h;
  double final_h = XY2Deg(total_cartesian_X / dimension, total_cartesian_Y / dimension);
  double final_s = total_s / dimension;
  double final_l = total_l / dimension;
  double final_a = total_a / dimension;

  curr->avg.h = final_h;
  curr->avg.s = final_s;
  curr->avg.l = final_l;
  curr->avg.a = final_a;
  
  // base case - when the width and height are both 1
  if (w == 1 && h == 1) {
    return curr;
  } else if (h > w) {
    unsigned int height_A = (unsigned int) floor(h/2);
    curr->A = BuildNode(im, ul, w, height_A);
    pair<unsigned int, unsigned int> point_B = make_pair(ul.first, ul.second + height_A);
    curr->B = BuildNode(im, point_B, w, h - height_A);
  } else {
    unsigned int width_A = (unsigned int) floor(w/2);
    curr->A = BuildNode(im, ul, width_A, h);
    pair<unsigned int, unsigned int> point_B = make_pair(ul.first + width_A, ul.second);
    curr->B = BuildNode(im, point_B, w - width_A, h);
  }
  return curr;
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
  pair<unsigned int, unsigned int> start_point = make_pair((unsigned int) 0, (unsigned int) 0);
  unsigned int width = im.width();
  unsigned int height = im.height();
  root = BuildNode(im, start_point, width, height);
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
  if (this != &other) {
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
  PNG* rendered_img = new PNG();
  unsigned int img_width = root->width;
  unsigned int img_height = root->height;
  rendered_img->resize(img_width, img_height);
  ColorRenderedImage(rendered_img, root->upperleft, root);
  return *rendered_img;
}

void PTree::ColorRenderedImage(PNG* img, pair<unsigned int, unsigned int> ul, Node* curr) const {
  if (curr->A == NULL && curr->B == NULL) {
    for (unsigned int y = ul.second; y < curr->height + ul.second; y++) {
      for (unsigned int x = ul.first; x < curr->width + ul.first; x++) {
        HSLAPixel* pixel = img->getPixel(x, y);
        *pixel = curr->avg;
      }
    }
  } else {
    Node* left_child = curr->A;
    Node* right_child = curr->B;
    ColorRenderedImage(img, left_child->upperleft, left_child);
    ColorRenderedImage(img, right_child->upperleft, right_child);
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
  // need a function to check if all leaves are within tolerance
  Prune_Tree(root, tolerance); 
}

void PTree::Prune_Tree(Node* &subTree, double t) {
  //check whether the current subtree's leaves are within tolerance; if so, delete its children, else go to its left and right subtrees
  if (All_Within_Tolerance(subTree, subTree->avg, t)) {
    ClearTree(subTree->A);
    ClearTree(subTree->B);
  } else {
    Prune_Tree(subTree->A, t);
    Prune_Tree(subTree->B, t);
  }
}

bool PTree::All_Within_Tolerance(Node* &subTree, HSLAPixel root_avg, double t) {
  //base case - we have reached the leaf
  if (subTree->width == 1 && subTree->height == 1) {
    if (root_avg.dist(subTree->avg) < t) {
      return true;
    } else {
      return false;
    }
  }
  return All_Within_Tolerance(subTree->A, root_avg, t) && All_Within_Tolerance(subTree->B, root_avg, t);
}

/*
*  Returns the total number of nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::Size() const {
  // replace the line below with your implementation
  return CalculateSize(root);
}

int PTree::CalculateSize(Node* base) const {
  if (base == NULL) {
    return 0;
  } else {
    return 1 + CalculateSize(base->A) + CalculateSize(base->B);
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
  return CountLeaves(root);
}

int PTree::CountLeaves(Node* base) const {
  if (base == NULL) {
    return 0;
  } else if (base->A == NULL && base->B == NULL) {
    return 1;
  } else {
    return CountLeaves(base->A) + CountLeaves(base->B); 
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
  FlipHor(root);
}

Node* PTree::FlipHor(Node* base) {
  if (base == NULL) {
    return NULL;
  }
  Node* left = FlipHor(base->A);
  Node* right = FlipHor(base->B);
  base->A = left;
  base->B = right;
  return base;
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
  FlipVer(root);
}

Node* PTree::FlipVer(Node* base) {
  if (base == NULL) {
    return NULL;
  }
  //********* TODO ***************
  return NULL;
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

