#ifndef CLIPPER_H
#define CLIPPER_H

#include <iostream>

#include "drawable.h"

class Clipper
{
public:
    // enum Mode { COHEN_SUTHERLAND,
    //             LIANG_BARSKY,
    //             NICHOLL_LEE_NICHOLL };

    Drawable* clip(Drawable *pDrawable){
        Point *point = dynamic_cast<Point *>(pDrawable);
        if(point){
            return clip(point);
        }

        Line *line = dynamic_cast<Line *>(pDrawable);
        if(line){
            return clip(line);
        }

        return clip((Wireframe *) pDrawable);
    }

protected:
    enum class CohenSutherlandClass { TOP_LEFT    = 9, TOP    = 8, TOP_RIGHT    = 10,
                                      LEFT        = 1, MIDDLE = 0, RIGHT        = 2 ,
                                      BOTTOM_LEFT = 5, BOTTOM = 4, BOTTOM_RIGHT = 6 };

    Drawable *clip(Point *pPoint){
        if(cohenSutherlandCoordClassifier(pPoint->window_coordinate) == CohenSutherlandClass::MIDDLE){
            return pPoint;
        }

        return nullptr;
    }

    Drawable *clip(Line *pLine){
        auto coord_a = pLine->window_coordinate_a;
        auto coord_b = pLine->window_coordinate_b;
        auto cs_class_a = cohenSutherlandCoordClassifier(coord_a);
        auto cs_class_b = cohenSutherlandCoordClassifier(coord_b);
        if(cs_class_a == CohenSutherlandClass::MIDDLE
           && cs_class_b == CohenSutherlandClass::MIDDLE){
            // completely inside
            return pLine;
        }

        if((((int) cs_class_a) & ((int) cs_class_b)) != 0) {
            // completely outside
            return nullptr;
        }

        double m = cohenSutherlandAngularCoeficient(coord_a, coord_b);

        if(cs_class_a == CohenSutherlandClass::LEFT){
            coord_a = cohenSutherlandIntersectVertical(m, -1, coord_a);
        } else 
        if(cs_class_a == CohenSutherlandClass::RIGHT){
            coord_a = cohenSutherlandIntersectVertical(m, 1, coord_a);
        } else
        if(cs_class_a == CohenSutherlandClass::TOP){
            coord_a = cohenSutherlandIntersectHorizontal(m, 1, coord_a);
        } else
        if(cs_class_a == CohenSutherlandClass::BOTTOM){
            coord_a = cohenSutherlandIntersectHorizontal(m, -1, coord_a);
        } else
        if(cs_class_a == CohenSutherlandClass::TOP_LEFT){
            Coordinate temp = cohenSutherlandIntersectVertical(m, -1, coord_a);
            if(cohenSutherlandCoordClassifier(temp) == CohenSutherlandClass::MIDDLE){
                coord_a = temp;
            } else {
                coord_a = cohenSutherlandIntersectHorizontal(m, 1, coord_a);
            }
        } else
        if(cs_class_a == CohenSutherlandClass::TOP_RIGHT){
            Coordinate temp = cohenSutherlandIntersectVertical(m, 1, coord_a);
            if(cohenSutherlandCoordClassifier(temp) == CohenSutherlandClass::MIDDLE){
                coord_a = temp;
            } else {
                coord_a = cohenSutherlandIntersectHorizontal(m, 1, coord_a);
            }
        } else
        if(cs_class_a == CohenSutherlandClass::BOTTOM_LEFT){
            Coordinate temp = cohenSutherlandIntersectVertical(m, -1, coord_a);
            if(cohenSutherlandCoordClassifier(temp) == CohenSutherlandClass::MIDDLE){
                coord_a = temp;
            } else {
                coord_a = cohenSutherlandIntersectHorizontal(m, -1, coord_a);
            }
        } else
        if(cs_class_a == CohenSutherlandClass::BOTTOM_RIGHT){
            Coordinate temp = cohenSutherlandIntersectVertical(m, 1, coord_a);
            if(cohenSutherlandCoordClassifier(temp) == CohenSutherlandClass::MIDDLE){
                coord_a = temp;
            } else {
                coord_a = cohenSutherlandIntersectHorizontal(m, -1, coord_a);
            }
        }

        if(coord_a.x < -1 || coord_a.x > 1
        || coord_a.y < -1 || coord_a.y > 1){
            return nullptr;
        }

        if(cs_class_b == CohenSutherlandClass::LEFT){
            coord_b = cohenSutherlandIntersectVertical(m, -1, coord_b);
        } else 
        if(cs_class_b == CohenSutherlandClass::RIGHT){
            coord_b = cohenSutherlandIntersectVertical(m, 1, coord_b);
        } else
        if(cs_class_b == CohenSutherlandClass::TOP){
            coord_b = cohenSutherlandIntersectHorizontal(m, 1, coord_b);
        } else
        if(cs_class_b == CohenSutherlandClass::BOTTOM){
            coord_b = cohenSutherlandIntersectHorizontal(m, -1, coord_b);
        } else
        if(cs_class_b == CohenSutherlandClass::TOP_LEFT){
            Coordinate temp = cohenSutherlandIntersectVertical(m, -1, coord_b);
            if(cohenSutherlandCoordClassifier(temp) == CohenSutherlandClass::MIDDLE){
                coord_b = temp;
            } else {
                coord_b = cohenSutherlandIntersectHorizontal(m, 1, coord_b);
            }
        } else
        if(cs_class_b == CohenSutherlandClass::TOP_RIGHT){
            Coordinate temp = cohenSutherlandIntersectVertical(m, 1, coord_b);
            if(cohenSutherlandCoordClassifier(temp) == CohenSutherlandClass::MIDDLE){
                coord_b = temp;
            } else {
                coord_b = cohenSutherlandIntersectHorizontal(m, 1, coord_b);
            }
        } else
        if(cs_class_b == CohenSutherlandClass::BOTTOM_LEFT){
            Coordinate temp = cohenSutherlandIntersectVertical(m, -1, coord_b);
            if(cohenSutherlandCoordClassifier(temp) == CohenSutherlandClass::MIDDLE){
                coord_b = temp;
            } else {
                coord_b = cohenSutherlandIntersectHorizontal(m, -1, coord_b);
            }
        } else
        if(cs_class_b == CohenSutherlandClass::BOTTOM_RIGHT){
            Coordinate temp = cohenSutherlandIntersectVertical(m, 1, coord_b);
            if(cohenSutherlandCoordClassifier(temp) == CohenSutherlandClass::MIDDLE){
                coord_b = temp;
            } else {
                coord_b = cohenSutherlandIntersectHorizontal(m, -1, coord_b);
            }
        }

        if(coord_b.x < -1 || coord_b.x > 1
        || coord_b.y < -1 || coord_b.y > 1){
            return nullptr;
        }

        pLine->window_coordinate_a = coord_a;
        pLine->window_coordinate_b = coord_b;

        return pLine;
    }
 
    Drawable *clip(Wireframe *pWireframe){
        std::vector<Line> lines;

        int size = pWireframe->window_coordinates.size();

        for(int i = 0; i < size; i++){
            Line workaround = Line("workaround",Coordinate(), Coordinate());
            workaround.window_coordinate_a = pWireframe->window_coordinates[i];
            workaround.window_coordinate_b = pWireframe->window_coordinates[(i+1)%size];

            Line *clipped = (Line *) clip(&workaround);
            if(clipped){
                // workaround will have its window_coordinates already clipped
                // no need to dereference the clipped pointer
                lines.push_back(workaround);
            }
        }

        std::vector<Coordinate> coords;

        size = lines.size();

        for(int i = 0; i < size; i++){
            auto current = lines[i];
            auto next = lines[(i+1)%size];
            coords.push_back(current.window_coordinate_b);

            if(current.window_coordinate_b.y == 1){
                if(next.window_coordinate_a.x == 1){
                    coords.push_back(Coordinate(1,1));
                } else
                if(next.window_coordinate_a.y == -1){
                    coords.push_back(Coordinate(1,1));
                    coords.push_back(Coordinate(1,-1));
                } else
                if(next.window_coordinate_a.x == -1){
                    coords.push_back(Coordinate(-1,1));
                }
                coords.push_back(next.window_coordinate_a);
            } else
            if(current.window_coordinate_b.x == 1){
                if(next.window_coordinate_a.y == 1){
                    coords.push_back(Coordinate(1,1));
                } else
                if(next.window_coordinate_a.y == -1){
                    coords.push_back(Coordinate(1,-1));
                } else
                if(next.window_coordinate_a.x == -1){
                    coords.push_back(Coordinate(1,-1));
                    coords.push_back(Coordinate(-1,-1));
                }
                coords.push_back(next.window_coordinate_a);
            } else
            if(current.window_coordinate_b.y == -1){
                if(next.window_coordinate_a.y == 1){
                    coords.push_back(Coordinate(-1,-1));
                    coords.push_back(Coordinate(-1,1));
                } else
                if(next.window_coordinate_a.x == 1){
                    coords.push_back(Coordinate(1,-1));
                } else
                if(next.window_coordinate_a.x == -1){
                    coords.push_back(Coordinate(-1,-1));
                }
                coords.push_back(next.window_coordinate_a);
            } else
            if(current.window_coordinate_b.x == -1){
                if(next.window_coordinate_a.y == 1){
                    coords.push_back(Coordinate(-1,1));
                } else
                if(next.window_coordinate_a.x == 1){
                    coords.push_back(Coordinate(-1,1));
                    coords.push_back(Coordinate(1,1));
                } else
                if(next.window_coordinate_a.y == -1){
                    coords.push_back(Coordinate(-1,-1));
                }
                coords.push_back(next.window_coordinate_a);
            }
        }

        pWireframe->window_coordinates = coords;

        return pWireframe;
    }

    Coordinate cohenSutherlandIntersectHorizontal(double m, double y, const Coordinate& old_coord){
        return Coordinate((y - old_coord.y) / m + old_coord.x, y);
    }

    Coordinate cohenSutherlandIntersectVertical(double m, double x, const Coordinate& old_coord){
        return Coordinate(x, m * (x - old_coord.x) + old_coord.y);
    }

    double cohenSutherlandAngularCoeficient(const Coordinate& coord_a, const Coordinate& coord_b){
        return (coord_b.y - coord_a.y) / (coord_b.x - coord_a.x);
    }

    CohenSutherlandClass cohenSutherlandCoordClassifier(const Coordinate& coord){
        if(coord.y >  1 && coord.x < -1) return CohenSutherlandClass::TOP_LEFT;
        if(coord.y >  1 && coord.x >  1) return CohenSutherlandClass::TOP_RIGHT;
        if(coord.y >  1                ) return CohenSutherlandClass::TOP;
        if(coord.y < -1 && coord.x < -1) return CohenSutherlandClass::BOTTOM_LEFT;
        if(coord.y < -1 && coord.x >  1) return CohenSutherlandClass::BOTTOM_RIGHT;
        if(coord.y < -1                ) return CohenSutherlandClass::BOTTOM;
        if(                coord.x < -1) return CohenSutherlandClass::LEFT;
        if(                coord.x >  1) return CohenSutherlandClass::RIGHT;
                                         return CohenSutherlandClass::MIDDLE;
    }
};

#endif // CLIPPER_H
