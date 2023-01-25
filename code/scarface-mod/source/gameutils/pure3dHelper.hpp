#ifndef _PURE3DHELPER_HPP
#define _PURE3DHELPER_HPP

#include "../common.hpp"

class pure3dH
{
public:
    pure3d::VectorCamera    *thisCam = nullptr;

    inline math::Vector WorldToScreen(pure3d::VectorCamera *thisCam, D3DVIEWPORT9 d3dvp, math::Vector* worldPoint)
    {
        math::Vector cpoint;
        thisCam->WorldToViewport(worldPoint, &cpoint);
        cpoint.x = (float(d3dvp.X) + (float(d3dvp.Width) / 2.0f)) + float(d3dvp.Width) * cpoint.x;
        cpoint.y = (float(d3dvp.Y) + (float(d3dvp.Height) / 2.0f)) + float(d3dvp.Height) * -cpoint.y;
        
        if(cpoint.z>=1.0)
            return cpoint;

        
        return math::Vector(NULL,NULL,NULL);
    }
};

#endif // !_PURE3DHELPER_HPP
