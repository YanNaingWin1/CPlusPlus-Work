/*
*  File:        filler.cpp
*  Description: Implementation of functions in the filler namespace.
*
*/

/*
*  Performs a flood fill using breadth first search.
*
*  PARAM:  config - FillerConfig struct to setup the fill
*  RETURN: animation object illustrating progression of flood fill algorithm
*/

animation filler::FillBFS(FillerConfig& config) {
  return Fill<Queue>(config); 
}

/*
*  Performs a flood fill using depth first search.
*
*  PARAM:  config - FillerConfig struct to setup the fill
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
animation filler::FillDFS(FillerConfig& config) {
  return Fill<Stack>(config); 
}

// void filler::AddToFrame(int n, int frameFre, animation an, PNG& img) {
//   if ((n % frameFre) == 0) {
//     an.addFrame(img);
//   }
// }

bool filler::Tolerance(HSLAPixel current, double tol, HSLAPixel* neighbour) {
  if (current.dist(*neighbour) <= tol) {
    return true;
  } else {
    return false;
  }
}

/*
*  Run a flood fill on an image starting at the seed point
*
*  PARAM:  config - FillerConfig struct with data for flood fill of image
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
template <template <class T> class OrderingStructure> animation filler::Fill(FillerConfig& config)
{
  /*
  * You need to implement this function!
  *
  * This is the basic description of a flood-fill algorithm: Every fill
  * algorithm requires an ordering structure, which is passed to this
  * function via its template parameter. For a breadth-first-search
  * fill, that structure is a Queue, for a depth-first-search, that
  * structure is a Stack. To begin the algorithm, you simply place the
  * given point in the ordering structure, marking it processed
  * (the way you mark it is a design decision you'll make yourself).
  * We have a choice to either change the color, if appropriate, when we
  * add the point to the OS, or when we take it off. In our test cases,
  * we have assumed that you will change the color when a point is added
  * to the structure.
  * Until the structure is empty, you do the following:
  *
  * 1.     Remove a point from the ordering structure, and then...
  *
  *        1.    add its unprocessed neighbors (up/down/left/right) whose color values are
  *              within (or equal to) tolerance distance from the seed point,
  *              to the ordering structure, and
  *              mark them as processed.
  *        2.    if it is an appropriate frame, send the current PNG to the
  *              animation (as described below).
  *
  * 2.     When implementing your breadth-first-search and
  *        depth-first-search fills, you will need to explore neighboring
  *        pixels (up/down/left/right) in some order.
  *
  *        While the order in which you examine neighbors does not matter
  *        for a proper fill, you must use the same order as we do for
  *        your animations to come out like ours! The order you should put
  *        neighboring pixels **ONTO** the queue or stack is based on the
  *        following priority condition:
  *        ** MINIMUM COLOR DISTANCE FROM THE CURRENT PIXEL **
  *        Ties are broken first by minimum y-coordinate, then by minimum x-coordinate.
  *        The HSLAPixel.dist() function will be useful, and you should
  *        take advantage of the functionality in your PriorityNeighbours class.
  * 
  *        If you process the neighbours in a different order, your fill may
  *        still work correctly, but your animations will be different
  *        from the grading scripts!
  *
  * 3.     For every k pixels filled, **starting at the kth pixel**, you
  *        must add a frame to the animation, where k = frameFreq.
  *
  *        For example, if frameFreq is 4, then after the 4th pixel has
  *        been filled you should add a frame to the animation, then again
  *        after the 8th pixel, etc.  You must only add frames for the
  *        number of pixels that have been filled, not the number that
  *        have been checked. So if frameFreq is set to 1, a pixel should
  *        be filled every frame.
  *
  * 4.     Finally, as you leave the function, send one last frame to the
  *        animation. This frame will be the final result of the fill, and
  *        it will be the one we test against.
  *
  */

  int framecount = 0; // increment after processing one pixel; used for producing animation frames (step 3 above)
  animation anim;
  OrderingStructure<PixelPoint> os;

  // complete your implementation below
  // HINT: you will likely want to declare some kind of structure to track
  //       which pixels have already been visited

  // Initialises a structure to store whether the points added have been processed or not
  unsigned int structure_width = config.img.width();
  unsigned int structure_height = config.img.height();
  vector<vector<bool>> pixel_marker( structure_width , vector<bool> (structure_height, 0));
  for (unsigned int y = 0; y < structure_height; y++) {
    for (unsigned int x = 0; x < structure_width; x++) {
      pixel_marker[x][y] = false;
    }
  }

  //Operation for the given seed
  unsigned int seed_x = config.seedpoint.x;
  unsigned int seed_y = config.seedpoint.y;
  HSLAPixel* first = config.img.getPixel(seed_x, seed_y);
  HSLAPixel original = *first;
  os.Add(config.seedpoint);
  pixel_marker[seed_x][seed_y] = true;
  *first = (*(config.picker)).operator()(config.seedpoint);
  framecount++;
  if ((framecount % config.frameFreq) == 0) {
    anim.addFrame(config.img);
  }

  while (!os.IsEmpty()) {
    PixelPoint to_be_removed;
    to_be_removed = os.Remove();

    /******  determining neighboring pixels up/down/left/right *******/

    //Determine whether the point is within the tolerance distance first and check if its been marked processed
    //If so, add it to the structure within the priority neighbour

    // check if up neighbour is valid and if so, add it to priority neighbor
    PixelPoint up_neighbour;
    HSLAPixel* up_pixel;
    if (to_be_removed.x < structure_width && to_be_removed.y - 1 < structure_height) {
      up_neighbour.x = to_be_removed.x;
      up_neighbour.y = to_be_removed.y - 1;
      up_pixel = config.img.getPixel(up_neighbour.x, up_neighbour.y);
      if (Tolerance(original, config.tolerance, up_pixel) && !(pixel_marker[up_neighbour.x][up_neighbour.y])) {
        config.neighbourorder.Insert(up_neighbour);
      }
    }

    // check if up neighbour is valid and if so, add it to priority neighbor
    PixelPoint down_neighbour;
    HSLAPixel* down_pixel;
    if (to_be_removed.x < structure_width && to_be_removed.y + 1 < structure_height) {
      down_neighbour.x = to_be_removed.x;
      down_neighbour.y = to_be_removed.y + 1;
      down_pixel = config.img.getPixel(down_neighbour.x, down_neighbour.y);
      if (Tolerance(original, config.tolerance, down_pixel) && !(pixel_marker[down_neighbour.x][down_neighbour.y])) {
        config.neighbourorder.Insert(down_neighbour);
      }
    }

    // check if up neighbour is valid and if so, add it to priority neighbor
    PixelPoint left_neighbour;
    HSLAPixel* left_pixel;
    if (to_be_removed.x - 1 < structure_width && to_be_removed.y < structure_height) {
      left_neighbour.x = to_be_removed.x - 1;
      left_neighbour.y = to_be_removed.y;
      left_pixel = config.img.getPixel(left_neighbour.x, left_neighbour.y);
      if (Tolerance(original, config.tolerance, left_pixel) && !(pixel_marker[left_neighbour.x][left_neighbour.y])) {
        config.neighbourorder.Insert(left_neighbour);
      }
    }

    // check if up neighbour is valid and if so, add it to priority neighbor
    PixelPoint right_neighbour;
    HSLAPixel* right_pixel;
    if (to_be_removed.x + 1 < structure_width && to_be_removed.y < structure_height) {
      right_neighbour.x = to_be_removed.x + 1;
      right_neighbour.y = to_be_removed.y;
      right_pixel = config.img.getPixel(right_neighbour.x, right_neighbour.y);
      if (Tolerance(original, config.tolerance, right_pixel) && !(pixel_marker[right_neighbour.x][right_neighbour.y])) {
        config.neighbourorder.Insert(right_neighbour);
      }
    }

    // adds the neighbouring pixel points to the ordering structure according to the restriction and fill color in them.
    // mark the pixel to be processed and add frame if necessary
    while (!config.neighbourorder.IsEmpty()) {
      PixelPoint priority_pp = config.neighbourorder.Remove();
      unsigned int pos_x = priority_pp.x;
      unsigned int pos_y = priority_pp.y;

      if (pos_x == up_neighbour.x && pos_y == up_neighbour.y) {
        os.Add(up_neighbour);
        *up_pixel = (*(config.picker)).operator()(up_neighbour);
      } else if (pos_x == down_neighbour.x && pos_y == down_neighbour.y) {
        os.Add(down_neighbour);
        *down_pixel = (*(config.picker)).operator()(down_neighbour);
      } else if (pos_x == left_neighbour.x && pos_y == left_neighbour.y) {
        os.Add(left_neighbour);
        *left_pixel = (*(config.picker)).operator()(left_neighbour);
      } else {
        os.Add(right_neighbour);
        *right_pixel = (*(config.picker)).operator()(right_neighbour);
      }
      pixel_marker[pos_x][pos_y] = true;
      framecount++;
      if ((framecount % config.frameFreq) == 0) {
        anim.addFrame(config.img);
      }
    }
  }
  anim.addFrame(config.img);
  return anim;
}
