#ifndef GUI_H
#define GUI_H

#include "../graphic/graphic.h"

class Gui {
public:
    Gui() {
        glGenBuffers(1, &m_vbo);
    }

    ~Gui() {
        glDeleteBuffers(1, &m_vbo);
    }

    void SetBox( Graphic *graphic, float Width, float Height) {


        float m_width = Width;

        float m_height = Height;

        float factor = (float)graphic->GetWidth()/graphic->GetHeight();

        m_height = m_height * factor;

        GLfloat box[6][4] = {
                {+    0.0, +m_height, 0, 13},
                {+m_width, +     0.0, 0, 13},
                {+m_width, +m_height, 0, 13},
                {+    0.0, +     0.0, 0, 13},
                {+m_width, +     0.0, 0, 13},
                {+    0.0, +m_height, 0, 13},
        };
        // VBO �ndern
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
    }

    void DrawBox( Graphic *graphic, float x, float y) {
        float m_width = graphic->GetWidth();
        float m_hight = graphic->GetHeight();

        glm::mat4 one = glm::ortho( (double)-(m_width/2.0f), m_width/2.0, (double)-(m_hight/2.0f), m_hight/2.0);
        graphic->GetVertexShader()->BindArray( m_vbo, 0, GL_FLOAT);
        graphic->GetVertexShader()->Bind();// Shader
        graphic->GetVertexShader()->EnableVertexArray( 0);
        Transform f_form;
        f_form.GetPos().x = x;
        f_form.GetPos().y = y;

        graphic->GetVertexShader()->BindArray( m_vbo, 0, GL_FLOAT);
        graphic->GetVertexShader()->Bind();// Shader
        graphic->GetVertexShader()->EnableVertexArray( 0);

        //graphic->GetVertexShader()->Update( f_form, graphic->GetCamera(), graphic->GetCamera());
        graphic->GetVertexShader()->UpdateWithout( one, graphic->GetCamera(), f_form);
        //m_graphic->GetVoxelShader()->UpdateWithout( one, m_graphic->GetCamera());
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Draw( Graphic *graphic) {
        //SetBox( graphic, 0.08, 0.08);
        //DrawBox ( graphic, 0.2, 0.2);

        //glDisable(GL_DEPTH_TEST);


    }
protected:
private:
    GLuint m_vbo;
};

#endif // GUI_H
