#ifndef CLIPPER_H
#define CLIPPER_H

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
        return pLine;   
    }

    Drawable *clip(Wireframe *pWireframe){
        return pWireframe;
    }

    CohenSutherlandClass cohenSutherlandCoordClassifier(const Coordinate& coord){
        if(coord.y < -1 && coord.x < -1) return CohenSutherlandClass::TOP_LEFT;
        if(coord.y < -1 && coord.x >  1) return CohenSutherlandClass::TOP_RIGHT;
        if(coord.y < -1                ) return CohenSutherlandClass::TOP;
        if(coord.y >  1 && coord.x < -1) return CohenSutherlandClass::BOTTOM_LEFT;
        if(coord.y >  1 && coord.x >  1) return CohenSutherlandClass::BOTTOM_RIGHT;
        if(coord.y >  1                ) return CohenSutherlandClass::BOTTOM;
        if(                coord.x < -1) return CohenSutherlandClass::LEFT;
        if(                coord.x >  1) return CohenSutherlandClass::RIGHT;
                                         return CohenSutherlandClass::MIDDLE;
    }
};

#endif // CLIPPER_H
