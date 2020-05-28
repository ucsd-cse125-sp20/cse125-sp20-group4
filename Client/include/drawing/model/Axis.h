#ifndef AXIS_H
#define AXIS_H

#include "drawing/model/Geometry.h"

class Axis : public Geometry {

    public:
    Axis( bool enabled = true, float scale = 1.0f );

    static void setEnabledAll( bool enabled );
    static void toggleAll();

    void setEnabled( bool enabled );
    void toggle();

    void draw( const glm::mat4x4 & model, const glm::mat4x4 & view, const glm::vec3 & direction ) const;

    protected:
    mutable GLfloat normalPointSize;

    void setup() const;
    void restore() const;

    private:
    static bool enabledAll;
    bool enabled;

};

#endif