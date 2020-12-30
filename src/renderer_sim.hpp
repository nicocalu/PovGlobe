#pragma once	
#include <CImg.h>

#include "renderer_base.hpp"

class RendererSim: public RendererBase
{
public:
    RendererSim(
        RpmMeasureBase& rpm_measure_base);
    ~RendererSim();


    virtual void initialize(Globe& globe);
    
protected:

    cimg_library::CImgDisplay m_draw_display;
    cimg_library::CImgDisplay m_draw_display_3d;
    cimg_library::CImg<float> m_sphere_pts;
    cimg_library::CImgList<unsigned int> m_sphere_primitives;
    cimg_library::CImgList<unsigned char> m_sphere_colors;
    cimg_library::CImg<float> m_sphere_opacs;
    cimg_library::CImg<unsigned char> m_visu_buffer;
    cimg_library::CImg<unsigned char> m_visu_buffer3d;

    virtual void render(const Framebuffer&);

};