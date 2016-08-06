#include "game.h"
#include "../graphic/glerror.h"
#include "../system/timer.h"
#include <stdio.h>

std::string NumberToString( double Number) {
    /*std::ostringstream ss;
    ss << Number;
    return ss.str();*/
    char buffer[255];
    sprintf( buffer, "%0.2f", Number);
    std::string temp = buffer;
    return temp;
}

Game::Game() {
    m_graphic = new Graphic( 1024, 800);

    // Runnig
    m_isRunnig = true;

    // Framenrate setzten
    framenrate.Set( 60);

    m_gui = new Gui;

    glGenBuffers(1, &m_vboCursor);
}

Game::~Game() {
    glDeleteBuffers(1, &m_vboCursor);
    delete m_gui;
    delete m_graphic;
}

void Game::ViewCross() {
    GLfloat cross[4][4] = {
                {-0.05, 0, 0, 13},
                {+0.05, 0, 0, 13},
                {0, -0.05, 0, 13},
                {0, +0.05, 0, 13},
            };

    glm::mat4 one(1);
    m_graphic->GetVertexShader()->BindArray( m_vboCursor, 0, GL_FLOAT);
    m_graphic->GetVertexShader()->Bind();// Shader
    m_graphic->GetVertexShader()->EnableVertexArray( 0);
    Transform f_form;

    //m_graphic->GetVertexShader()->Update( f_form, m_graphic->GetCamera(), m_graphic->GetCamera());
    m_graphic->GetVertexShader()->UpdateWithout( one, m_graphic->GetCamera());

    //m_graphic->GetVoxelShader()->UpdateWithout( one, m_graphic->GetCamera());
    glBindBuffer(GL_ARRAY_BUFFER, m_vboCursor);
    glBufferData(GL_ARRAY_BUFFER, sizeof cross, cross, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_LINES, 0, 4);
}

static float fract(float value) {
	float f = value - floorf(value);
	if(f > 0.5)
		return 1 - f;
	else
		return f;
}

void Game::Start() {
    m_config.SetTransparency( true);

    md2Model obj;

    obj.load("eagle/eagle.md2", "eagle/eagle.jpg");
    //obj.load("motor/motor.md2", "motor/motor.tga");

    obj.setFrame( 1);

    while( m_isRunnig) { // Runnig
        m_input.Reset();
        m_isRunnig = m_input.Handle( m_graphic->GetWidth(), m_graphic->GetHeight(), m_graphic->GetWindow());
        // Input einsehen
//        float Speed = 0.1f * framenrate.getMSframe() *framenrate.getLimit()/ 1000.0f;
        float Speed = 0.1f;
        //printf( "%f %f \n", Speed, framenrate.getMSframe());
        Camera *cam = m_graphic->GetCamera();
        cam->horizontalAngle ( -m_input.Map.MousePos.x * 2);
        cam->verticalAngle  ( m_input.Map.MousePos.y * 2);

        if( m_input.Map.Up )
            cam->MoveForwardCross( Speed);
        if( m_input.Map.Down )
            cam->MoveForwardCross(-Speed);
        if( m_input.Map.Right )
            cam->MoveRight(-Speed);
        if( m_input.Map.Left )
            cam->MoveRight( Speed);
        if( m_input.Map.Jump )
            cam->MoveUp( Speed);
        if( m_input.Map.Shift )
            cam->MoveUp( -Speed);
        if( m_input.Map.Inventory && !m_input.MapOld.Inventory) {
            /*if( m_config.GetSupersampling()) {

                m_config.SetSupersampling( false);

                printf( "Game::Start Supersampling deactive\n");
            } else {
                printf( "Game::Start Supersampling active\n");
                m_config.SetSupersampling( true);
            }*/
            cam->zoom( -1.5);
        }
        if( !m_input.Map.Inventory && m_input.MapOld.Inventory) {
            cam->zoom( 1.5);
        }

        if( m_input.Map.Place && !m_input.MapOld.Place)
        {
            obj.setFrame( obj.getFrame()+1);
            if( obj.getFrame() >= obj.GetNumFrames()-1)
                obj.setFrame( 0);
            printf( "%d\n",obj.GetNumFrames());
        }

        if( m_input.getResize())
            m_graphic->ResizeWindow( m_input.getResizeW(), m_input.getResizeH());
        //cos_i++;
        // Framenrate anfangen zu zählen
        framenrate.StartCount();


        Timer m_timer;
        m_timer.Start();

        m_graphic->GetDisplay()->Clear();

        obj.draw( m_graphic->GetObjectShader(), m_graphic->GetCamera());


		// View Cross
        ViewCross();

        //m_world->GetChunk( 0, -1, 0)->SetChange( true);

        // Swap die Buffer um keine Renderfehler zu bekommen
        m_graphic->GetDisplay()->SwapBuffers();

        // fehler anzeigen -> schleife eine meldung bedeutet ich habe verkackt
        GLenum error =  glGetError();
        if(error) {
            std::cout << error << std::endl;
        }

        // Framenrate begrenzen
        framenrate.CalcDelay();

        // Titel setzten
        //Title = "FPS: " + NumberToString( (double)(int)framenrate.GetFramenrate());
        Title = "FPS_" + NumberToString( framenrate.getFrameratePrecisely() );
        Title = Title + " ms_" + NumberToString( framenrate.getMSframe());
        Title = Title + " X_" + NumberToString( cam->GetPos().x) + " Y_" + NumberToString( cam->GetPos().y) + " Z_" + NumberToString( cam->GetPos().z );
//      Title = Title + "Tile X " + NumberToString( (float)x) + " Y " + NumberToString(  (float)y) + " Z " + NumberToString(  (float)z );
//        if( m_world->GetWorldTree() != NULL)
//            m_world->GetWorldTree()->SetTile( cam->GetPos().x, cam->GetPos().y, cam->GetPos().z, 1);
        //m_graphic->GetDisplay()->SetTitle( Title);

        // Measure speed
        //printf("%s\n", Title.c_str());
        // http://www.arcsynthesis.org/gltut/Positioning/Tutorial%2005.html

    }
    //delete obj;
}

/*void Game::DrawBox( GLshort bx, GLshort by, GLshort bz) {
    std::vector<ChunkVboDataStruct> t_box;

    // Chunk Vbo Data Struct
    t_box.resize( 24 );

    // x-y side
    t_box[0] = CVDS_SetBlock(0, 0, 0, 14);
    t_box[1] = CVDS_SetBlock(1, 0, 0, 14);
    t_box[2] = CVDS_SetBlock(0, 0, 0, 14);
    t_box[3] = CVDS_SetBlock(0, 1, 0, 14);
    t_box[4] = CVDS_SetBlock(1, 0, 0, 14);
    t_box[5] = CVDS_SetBlock(1, 1, 0, 14);
    t_box[6] = CVDS_SetBlock(1, 1, 0, 14);
    t_box[7] = CVDS_SetBlock(0, 1, 0, 14);
    // x-y & z+1
    t_box[8] = CVDS_SetBlock(0, 0, 1, 14);
    t_box[9] = CVDS_SetBlock(1, 0, 1, 14);
    t_box[10] = CVDS_SetBlock(0, 0, 1, 14);
    t_box[11] = CVDS_SetBlock(0, 1, 1, 14);
    t_box[12] = CVDS_SetBlock(1, 0, 1, 14);
    t_box[13] = CVDS_SetBlock(1, 1, 1, 14);
    t_box[14] = CVDS_SetBlock(1, 1, 1, 14);
    t_box[15] = CVDS_SetBlock(0, 1, 1, 14);
    // restlichen linien
    t_box[16] = CVDS_SetBlock(0, 0, 0, 14);
    t_box[17] = CVDS_SetBlock(0, 0, 1, 14);
    t_box[18] = CVDS_SetBlock(0, 1, 0, 14);
    t_box[19] = CVDS_SetBlock(0, 1, 1, 14);
    t_box[20] = CVDS_SetBlock(1, 0, 0, 14);
    t_box[21] = CVDS_SetBlock(1, 0, 1, 14);
    t_box[22] = CVDS_SetBlock(1, 1, 0, 14);
    t_box[23] = CVDS_SetBlock(1, 1, 1, 14);

    Transform f_form;
    f_form.GetPos().x = bx;
    f_form.GetPos().y = by;
    f_form.GetPos().z = bz;

    // Shader einstellen
    m_graphic->GetVertexShader()->BindArray( m_vboCursor, 0);
    m_graphic->GetVertexShader()->Bind();// Shader
    m_graphic->GetVertexShader()->EnableVertexArray( 0);
    m_graphic->GetVertexShader()->Update( f_form, m_graphic->GetCamera(), m_graphic->GetCamera());

    // Vbo übertragen
    glBindBuffer(GL_ARRAY_BUFFER, m_vboCursor);
    glBufferData(GL_ARRAY_BUFFER, t_box.size() * sizeof(ChunkVboDataStruct), &t_box[0], GL_DYNAMIC_DRAW);

    // Zeichnen
    glDrawArrays( GL_LINES, 0, 24);
}*/
