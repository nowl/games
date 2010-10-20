#ifndef __TEXT_OBJECT_H__
#define __TEXT_OBJECT_H__

#include "GameObject.h"

class TextObject : public GameObject {
public:
    TextObject(const std::string& name,
               const int layer,
               const Position& pos,
               const std::string& fontName,
               int fontSize,
               int red,
               int green,
               int blue)
        : GameObject(name, layer), 
          _fontName(fontName),
          _fontSize(fontSize),
          _text(""),
          _red(red),
          _green(green),
          _blue(blue)
    {
        setWorldPosition(pos);
    }

    virtual ~TextObject() {}

    void setText(const std::string& text) { _text = text; }

    void setColor(int red, int green, int blue)
    {
        _red = red;
        _green = green;
        _blue = blue;
    }

    void specificRender(GraphicsEngine *graphicsEngine,
									  GameState *gameState,
									  double interpolation)
        {
            Position pos = getScreenPosition();
            
            graphicsEngine->drawTextSolid(_fontName,
                                          _fontSize,
                                          _text,
                                          pos.getX(),
                                          pos.getY(),
                                          _red,
                                          _green,
                                          _blue);            
        }
    
private:
    std::string _fontName;
    int _fontSize;
    std::string _text;
    int _red, _green, _blue;
};

#endif  // __TEXT_OBJECT_H__
