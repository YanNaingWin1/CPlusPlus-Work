// File:        imglist.cpp
// Date:        2022-01-27 10:21
// Description: Contains partial implementation of ImgList class
//              for CPSC 221 2021W2 PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "imglist.h"
#include <math.h> // provides fmin and fabs functions

/**************************
* MISCELLANEOUS FUNCTIONS *
**************************/

/*
* This function is NOT part of the ImgList class,
* but will be useful for one of the ImgList functions.
* Returns the "difference" between two hue values.
* PRE: hue1 is a double between [0,360).
* PRE: hue2 is a double between [0,360).
* 
* The hue difference is the absolute difference between two hues,
* but takes into account differences spanning the 360 value.
* e.g. Two pixels with hues 90 and 110 differ by 20, but
*      two pixels with hues 5 and 355 differ by 10.
*/
double HueDiff(double hue1, double hue2) {
  return fmin(fabs(hue1 - hue2), fabs(hue2 + 360 - hue1));
}

/*********************
* CONSTRUCTORS, ETC. *
*********************/

/*
* Default constructor. Makes an empty list
*/
ImgList::ImgList() {
  // set appropriate values for all member attributes here
  northwest = NULL;
  southeast = NULL;
}

/*
* Creates a list from image data
* PRE: img has dimensions of at least 1x1
*/
ImgList::ImgList(PNG& img) {
  // build the linked node structure and set the member attributes appropriately
  ImgNode* prev;
  ImgNode* horRef;
  ImgNode* verRef;

  for (unsigned y = 0; y < img.height(); y++) {
    for (unsigned x = 0; x < img.width(); x++) {
        if (x == 0 && y == 0) {  // Start up the NW node
          ImgNode* temp = new ImgNode();
          northwest = temp;
          horRef = northwest;
          northwest->colour.a = img.getPixel(x, y)->a;
          northwest->colour.s = img.getPixel(x, y)->s;
          northwest->colour.h = img.getPixel(x, y)->h;
          northwest->colour.l = img.getPixel(x, y)->l;

        } else if (x == img.width() - 1 && y == img.height() - 1) {
          ImgNode* temp = new ImgNode();
          southeast = temp;
          southeast->colour.a = img.getPixel(x, y)->a;
          southeast->colour.s = img.getPixel(x, y)->s;
          southeast->colour.h = img.getPixel(x, y)->h;
          southeast->colour.l = img.getPixel(x, y)->l;
        }
    }
    verRef = northwest;
  }
}

/*
* Copy constructor.
* Creates this this to become a separate copy of the data in otherlist
*/
ImgList::ImgList(const ImgList& otherlist) {
  // build the linked node structure using otherlist as a template
  Copy(otherlist);
}

/*
* Assignment operator. Enables statements such as list1 = list2;
*   where list1 and list2 are both variables of ImgList type.
* POST: the contents of this list will be a physically separate copy of rhs
*/
ImgList& ImgList::operator=(const ImgList& rhs) {
  // Re-build any existing structure using rhs as a template
  
  if (this != &rhs) { // if this list and rhs are different lists in memory
    // release all existing heap memory of this list
    Clear();    
    
    // and then rebuild this list using rhs as a template
    Copy(rhs);
  }
  
  return *this;
}

/*
* Destructor.
* Releases any heap memory associated with this list.
*/
ImgList::~ImgList() {
  // Ensure that any existing heap memory is deallocated
  Clear();
}

/************
* ACCESSORS *
************/

/*
* Returns the horizontal dimension of this list (counted in nodes)
* Note that every row will contain the same number of nodes, whether or not
*   the list has been carved.
* We expect your solution to take linear time in the number of nodes in the
*   x dimension.
*/
unsigned int ImgList::GetDimensionX() const {
  unsigned int counter = 0;
  ImgNode* temp = northwest;
  while (temp != NULL) {
    counter++;
    temp = temp->east;
  }
  return counter;
}

/*
* Returns the vertical dimension of the list (counted in nodes)
* It is useful to know/assume that the grid will never have nodes removed
*   from the first or last columns. The returned value will thus correspond
*   to the height of the PNG image from which this list was constructed.
* We expect your solution to take linear time in the number of nodes in the
*   y dimension.
*/
unsigned int ImgList::GetDimensionY() const {
  unsigned int counter = 0;
  ImgNode* temp = northwest;
  while (temp != NULL) {
    counter++;
    temp = temp->south;
  }
  return counter;
}

/*
* Returns the horizontal dimension of the list (counted in original pixels, pre-carving)
* The returned value will thus correspond to the width of the PNG image from
*   which this list was constructed.
* We expect your solution to take linear time in the number of nodes in the
*   x dimension.
*/
unsigned int ImgList::GetDimensionFullX() const {
  unsigned int counter = 0;
  ImgNode* temp = northwest;
  while (temp != NULL) {
    counter += (temp->skipright + 1);
    temp = temp->east;
  }
  return counter;
  return 0;
}

/*
* Returns a pointer to the node which best satisfies the specified selection criteria.
* The first and last nodes in the row cannot be returned.
* PRE: rowstart points to a row with at least 3 physical nodes
* PRE: selectionmode is an integer in the range [0,1]
* PARAM: rowstart - pointer to the first node in a row
* PARAM: selectionmode - criterion used for choosing the node to return
*          0: minimum luminance across row, not including extreme left or right nodes
*          1: node with minimum total of "hue difference" with its left neighbour and with its right neighbour.
*        In the (likely) case of multiple candidates that best match the criterion,
*        the left-most node satisfying the criterion (excluding the row's starting node)
*        will be returned.
* A note about "hue difference": For PA1, consider the hue value to be a double in the range [0, 360).
* That is, a hue value of exactly 360 should be converted to 0.
* The hue difference is the absolute difference between two hues,
* but be careful about differences spanning the 360 value.
* e.g. Two pixels with hues 90 and 110 differ by 20, but
*      two pixels with hues 5 and 355 differ by 10.
*/
ImgNode *ImgList::SelectNode(ImgNode *rowstart, int selectionmode)
{
  // add your implementation below
  if (selectionmode == 0)
  {
    ImgNode *curr = rowstart->east;
    ImgNode *next = curr->east;
    ImgNode *to_return = curr;
    while (next->east != NULL)
    {
      if (next->colour.l < curr->colour.l) {
        to_return = next;
      }
      curr = curr->east;
      next = next->east;
    }
    return to_return;
  } else
  {
    ImgNode *curr = rowstart->east;
    ImgNode *next = curr->east;
    ImgNode *prev = curr->west;
    double min = HueDiff(prev->colour.h, next->colour.h);
    ImgNode *to_return = curr;

    curr = rowstart->east;
    next = curr->east;
    prev = curr->west;
    while (next->east != NULL)
    {
      double val = HueDiff(prev->colour.h, next->colour.h);
      if (val < min) {
        to_return = curr;
      }
      prev = prev->east;
      curr = curr->east;
      next = next->east;
    }
    return to_return;
  }
}

/*
* Renders this list's pixel data to a PNG, with or without filling gaps caused by carving.
* PRE: fillmode is an integer in the range of [0,2]
* PARAM: fillgaps - whether or not to fill gaps caused by carving
*          false: render one pixel per node, ignores fillmode
*          true: render the full width of the original image,
*                filling in missing nodes using fillmode
* PARAM: fillmode - specifies how to fill gaps
*          0: solid, uses the same colour as the node at the left of the gap
*          1: solid, using the averaged values (all channels) of the nodes at the left and right of the gap
*             Note that "average" for hue will use the closer of the angular distances,
*             e.g. average of 10 and 350 will be 0, instead of 180.
*             Average of diametric hue values will use the smaller of the two averages
*             e.g. average of 30 and 210 will be 120, and not 300
*                  average of 170 and 350 will be 80, and not 260 //not this
*          2: *** OPTIONAL - FOR BONUS ***
*             linear gradient between the colour (all channels) of the nodes at the left and right of the gap
*             e.g. a gap of width 1 will be coloured with 1/2 of the difference between the left and right nodes
*             a gap of width 2 will be coloured with 1/3 and 2/3 of the difference
*             a gap of width 3 will be coloured with 1/4, 2/4, 3/4 of the difference, etc.
*             Like fillmode 1, use the smaller difference interval for hue,
*             and the smaller-valued average for diametric hues
*/
PNG ImgList::Render(bool fillgaps, int fillmode) const {
  // Add/complete your implementation below
  PNG outpng; //this will be returned later. Might be a good idea to resize it at some point.

  if (fillgaps) {
    int dimx = GetDimensionFullX();
    int dimy = GetDimensionY();
    outpng.resize(dimx, dimy);
    ImgNode* curr;
    ImgNode* verRef = northwest;
    int num_to_fill;

    if (fillmode == 0) {
      for (unsigned int y = 0; y < outpng.height(); y++) {
        curr = verRef;
        num_to_fill = curr->skipright;
        for (unsigned int x = 0; x < outpng.width(); x++) {
          *outpng.getPixel(x, y) = curr->colour;
          num_to_fill = curr->skipright;
          if (num_to_fill > 0) {
            num_to_fill--;
          } else {
            curr = curr->east;
          }
        }
        verRef = verRef->south;
      }
    } else if (fillmode == 1) {
        ImgNode* temp;
        double pi = (2 * acos(0.0));
        for (unsigned int y = 0; y < outpng.height(); y++) {
        curr = verRef;
        num_to_fill = curr->skipright;
        for (unsigned int x = 0; x < outpng.width(); x++) {
          if (num_to_fill > 0) {
              double left = curr->east->colour.h;
              double right = curr->colour.h;

              // angle1 represents avg for hue 
              //double diff = (((int) left - (int) right + 180 + 360 ) % 360) - 180;
              double diff1 = fmod((left - right + 180 + 360), 360) - 180;
              //double angle = (360 + (int) right + ( (int) diff / 2 ) ) % 360;
              double angle1 = fmod((360 + right + (diff1 / 2 )), 360);

              // angle2 represents another avgm might be smaller or bigger than angle1 - main use is to compare
              //double diff = (((int) left - (int) right + 180 + 360 ) % 360) - 180;
              double diff2 = fmod((right - left + 180 + 360), 360) - 180;
              //double angle = (360 + (int) right + ( (int) diff / 2 ) ) % 360;
              double angle2 = fmod((360 + left + (diff2 / 2 )), 360);

              *outpng.getPixel(x,y) = curr->colour;
              outpng.getPixel(x,y)->h = angle1;
          } else {
            *outpng.getPixel(x, y) = curr->colour;
            curr = curr->east;
            num_to_fill = curr->skipright;
          }
        }
        verRef = verRef->south;
      }
    } else {
      //BONUS
    }
  } else {
    int dimx = GetDimensionX();
    int dimy = GetDimensionY();
    outpng.resize(dimx, dimy);
    ImgNode* curr;
    ImgNode* verRef = northwest;

    for (unsigned int y = 0; y < outpng.height(); y++) {
      curr = verRef;
      for (unsigned int x = 0; x < outpng.width(); x++) {
        *outpng.getPixel(x,y) = curr->colour;
        curr = curr->east;
      }
      verRef = verRef->south;
    }
  }
  return outpng;
}

/************
* MODIFIERS *
************/

/*
* Removes exactly one node from each row in this list, according to specified criteria.
* The first and last nodes in any row cannot be carved.
* PRE: this list has at least 3 nodes in each row
* PRE: selectionmode is an integer in the range [0,1]
* PARAM: selectionmode - see the documentation for the SelectNode function.
* POST: this list has had one node removed from each row. Neighbours of the created
*       gaps are linked appropriately, and their skip values are updated to reflect
*       the size of the gap.
*/
void ImgList::Carve(int selectionmode) {
  //add your implementation here
  ImgNode* curr = northwest;
  while (curr != NULL) {
    ImgNode* to_be_deleted = SelectNode(curr, selectionmode);
    ImgNode* east_node = to_be_deleted->east;
    ImgNode* west_node = to_be_deleted->west;
    ImgNode* north_node = to_be_deleted->north;
    ImgNode* south_node = to_be_deleted->south;
    if (east_node != NULL) {
      east_node->west = west_node;
      east_node->skipleft++;
    }
    if (west_node != NULL) {
      west_node->east = east_node;
      west_node->skipright++;
    }
    if (north_node != NULL) {
      north_node->south = south_node;
      north_node->skipdown++;
    }
    if (south_node != NULL) {
      south_node->north = north_node;
      south_node->skipup++;
    }
    delete to_be_deleted;
    to_be_deleted = NULL;
    curr = curr->south;
  }
}

// note that a node on the boundary will never be selected for removal
/*
* Removes "rounds" number of nodes (up to a maximum of node width - 2) from each row,
* based on specific selection criteria.
* Note that this should remove one node from every row, repeated "rounds" times,
* and NOT remove "rounds" nodes from one row before processing the next row.
* PRE: selectionmode is an integer in the range [0,1]
* PARAM: rounds - number of nodes to remove from each row
*        If rounds exceeds node width - 2, then remove only node width - 2 nodes from each row.
*        i.e. Ensure that the final list has at least two nodes in each row.
* POST: this list has had "rounds" nodes removed from each row. Neighbours of the created
*       gaps are linked appropriately, and their skip values are updated to reflect
*       the size of the gap.
*/
void ImgList::Carve(unsigned int rounds, int selectionmode)
{
  // add your implementation here
  unsigned int roundsAdj = rounds;
  if (rounds > GetDimensionX() - 2) {
    roundsAdj = GetDimensionX() - 2;
  }
  for (unsigned int i=0; i < roundsAdj; i++) {
    Carve(selectionmode);
  }
}

/*
* Helper function deallocates all heap memory associated with this list,
* puts this list into an "empty" state. Don't forget to set your member attributes!
* POST: this list has no currently allocated nor leaking heap memory,
*       member attributes have values consistent with an empty list.
*/
void ImgList::Clear()
{
  // // add your implementation here
  ImgNode* temp = northwest;
  ImgNode* to_delete;
  while (temp != NULL)
  {
    if (temp->east != NULL)
    {
      to_delete = temp;
      temp = temp->east;
      to_delete->south = NULL;
      to_delete->east = NULL;
      to_delete->west = NULL;
      to_delete->north = NULL;
      delete to_delete;
      to_delete = NULL;
    }
    else if (temp->south != NULL)
    {
      to_delete = temp;
      temp = temp->south;
      to_delete->south = NULL;
      to_delete->east = NULL;
      to_delete->west = NULL;
      to_delete->north = NULL;
      delete to_delete;
      to_delete = NULL;
    }
    else if (temp->west != NULL)
    {
      to_delete = temp;
      temp = temp->west;
      to_delete->south = NULL;
      to_delete->east = NULL;
      to_delete->west = NULL;
      to_delete->north = NULL;
      delete to_delete;
      to_delete = NULL;
    }
    else if (temp->north != NULL)
    {
      to_delete = temp;
      temp = temp->north;
      to_delete->south = NULL;
      to_delete->east = NULL;
      to_delete->west = NULL;
      to_delete->north = NULL;
      delete to_delete;
      to_delete = NULL;
    } else {
      temp = NULL;
    }
  }
}

// if (to_delete->south != NULL) {
//         to_delete->south = NULL;
//       }
//       if (to_delete->east != NULL) {
//         to_delete->east = NULL;
//       }
//       if (to_delete->west != NULL) {
//         to_delete->west = NULL;
//       }
//       if (to_delete->north != NULL) {
//         to_delete->north = NULL;
//       }

/* ************************
*  * OPTIONAL - FOR BONUS *
** ************************
* Helper function copies the contents of otherlist and sets this list's attributes appropriately
* PRE: this list is empty
* PARAM: otherlist - list whose contents will be copied
* POST: this list has contents copied from by physically separate from otherlist
*/
void ImgList::Copy(const ImgList& otherlist) {
  // add your implementation here
  
}

/*************************************************************************************************
* IF YOU DEFINED YOUR OWN PRIVATE FUNCTIONS IN imglist.h, YOU MAY ADD YOUR IMPLEMENTATIONS BELOW *
*************************************************************************************************/