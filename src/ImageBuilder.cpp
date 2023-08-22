#include "ImageBuilder.hpp"

namespace Vizior {

    Color RED = {255,0,0,255};
    Color GRN = {0,255,0,255};
    Color BLU = {0,0,255,255};
    Color WHT = {255,255,255,255};
    Color BLK = {0,0,0,1};

    ImageBuilder::ImageBuilder(){
        // At most 10000 vertices
        // TODO change this or handle it better
        m_Verts = new float[m_nVertexVals*10000];
        m_VertIdx = new unsigned int[3*10000];
        m_NextElemPos = 0;
        m_NextVertPos = 0;

        // Will be set for real once bound to a window, polly not needed
        m_Width = 100;
        m_Height = 100;
    }

    ImageBuilder::~ImageBuilder(){
        delete[] m_Verts;
        delete[] m_VertIdx;
    }

    int ImageBuilder::addVert(int x, int y, Color& color){
        m_Verts[m_NextVertPos] = (2.0f * x/m_Width) -1;
        m_Verts[m_NextVertPos+1] = (2.0f * y/m_Height) -1;
        m_Verts[m_NextVertPos+2] = color.r/255.0;
        m_Verts[m_NextVertPos+3] = color.g/255.0;
        m_Verts[m_NextVertPos+4] = color.b/255.0;
        m_Verts[m_NextVertPos+5] = color.a/255.0;
        m_NextVertPos += m_nVertexVals;
        return (m_NextVertPos - m_nVertexVals)/m_nVertexVals;
    }


    void ImageBuilder::clearAll(){
        m_NextElemPos = 0;
        m_NextVertPos = 0;
    }

    void ImageBuilder::drawTriangle(Point2D* ps, Color& color){
        for(int i = 0; i < 3; i++){
            m_VertIdx[m_NextElemPos] = m_NextVertPos/m_nVertexVals;
            addVert(ps[i].x, ps[i].y, color);
            m_NextElemPos++;
        }
    }

    void ImageBuilder::drawRectangle(ANCHOR a, Point2D& anch, int w, int h, float rot, Color& color){
        Point2D bl, tl, br, tr;

        switch(a){
            case ANCHOR_C:
                bl = Point2D(anch.x - w/2, anch.y - h/2);
                tl = Point2D(anch.x - w/2, anch.y + h/2);
                br = Point2D(anch.x + w/2, anch.y - h/2);
                tr = Point2D(anch.x + w/2, anch.y + h/2);
                break;
            case ANCHOR_BL:
                bl = Point2D(anch.x  , anch.y  );
                tl = Point2D(anch.x  , anch.y+h);
                br = Point2D(anch.x+w, anch.y  );
                tr = Point2D(anch.x+w, anch.y+h);
                break;
            case ANCHOR_TL:
                bl = Point2D(anch.x  , anch.y-h);
                tl = Point2D(anch.x  , anch.y  );
                br = Point2D(anch.x+w, anch.y-h);
                tr = Point2D(anch.x+w, anch.y  );
                break;
            case ANCHOR_BR:
                bl = Point2D(anch.x-w, anch.y  );
                tl = Point2D(anch.x-w, anch.y+h);
                br = Point2D(anch.x  , anch.y  );
                tr = Point2D(anch.x  , anch.y+h);
                break;
            case ANCHOR_TR:
                bl = Point2D(anch.x-w, anch.y-h);
                tl = Point2D(anch.x-w, anch.y  );
                br = Point2D(anch.x  , anch.y-h);
                tr = Point2D(anch.x  , anch.y  );
                break;
        }

        Point2D pbl = bl.rotateAroundBy(anch, rot);
        Point2D ptl = bl.rotateAroundBy(anch, rot);
        Point2D pbr = bl.rotateAroundBy(anch, rot);
        Point2D ptr = bl.rotateAroundBy(anch, rot);

        int blIdx = addVert(pbl.x, pbl.y, color); 
        int tlIdx = addVert(ptl.x, ptl.y, color); 
        int brIdx = addVert(pbr.x, pbr.y, color); 
        int trIdx = addVert(ptr.x, ptr.y, color); 

        m_VertIdx[m_NextElemPos] = blIdx;
        m_VertIdx[m_NextElemPos+1] = tlIdx;
        m_VertIdx[m_NextElemPos+2] = brIdx;
        m_NextElemPos+=3;

        m_VertIdx[m_NextElemPos] = trIdx;
        m_VertIdx[m_NextElemPos+1] = brIdx;
        m_VertIdx[m_NextElemPos+2] = tlIdx;
        m_NextElemPos+=3;
    }

    void ImageBuilder::drawCircle(Point2D& center, int r, Color& color){
        int startVertIdx = m_NextVertPos;
        // Center Point
        addVert(center.x, center.y, color);

        // Circle points
        for(int i = 0; i < 180; i++){
            int px = center.x + cos(M_PI * i*2 / 180) * r;
            int py = center.y + sin(M_PI * i*2 / 180) * r;
            if(i >= 1){
                m_VertIdx[m_NextElemPos] = startVertIdx/6;
                m_VertIdx[m_NextElemPos+1] = (m_NextVertPos-m_nVertexVals)/6;
                m_VertIdx[m_NextElemPos+2] = m_NextVertPos/6;
                m_NextElemPos += 3;
            } 
            addVert(px, py, color);

        }
        m_VertIdx[m_NextElemPos] = startVertIdx/6;
        m_VertIdx[m_NextElemPos+1] = (m_NextVertPos-m_nVertexVals)/6;
        m_VertIdx[m_NextElemPos+2] = startVertIdx/6 + 1;
        m_NextElemPos += 3;


    }

    void ImageBuilder::setDimensions(int w, int h){
        m_Width = w;
        m_Height = h;
    }

    void ImageBuilder::drawLine(Point2D* ps, int w, Color& color){
        Point2D center = (ps[0] + ps[1])/2;
        Point2D orientation = ps[1] - ps[0];
        float angle = 180 * atan2(orientation.y, orientation.x) / M_PI;
        
        int length = sqrt(orientation.x*orientation.x + orientation.y*orientation.y);
        drawRectangle(ANCHOR_C, center, length, w, angle, color);
        
    }
}