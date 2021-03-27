#include "source.cpp"
#include "opengl.cpp"
#include "obj_parser.cpp"
#include "raytracer.cpp"

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    //Font Stuff/////////////////////////////
    FT_Face face;
    FT_Library ft;
    FT_Init_FreeType(&ft);
    int Size = 25;
    FT_New_Face(ft, "PTSans-Regular.ttf", 0, &face);
    FT_Set_Char_Size(face, 0, Size << 6, 96, 96);
    FT_Set_Pixel_Sizes(face, 0, Size);
    bool useKerning = FT_HAS_KERNING(face);
    character Character[128];
    /////////////////////////////////////////

    object Object = ParseOBJtoObjectStruct("bunny.obj");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // <<
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2); // ^^ this means version 3.2 for some reason
    SDL_GL_SetSwapInterval(1);

    Window = SDL_CreateWindow("RaytRacer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_OPENGL);
    SDL_GLContext GLContext = SDL_GL_CreateContext(Window);
    LoadGLfunctions();

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glVariables Variables;

    GLuint Pipeline = CreateGLProgram();
    Variables.SwapNScaleLoc = glGetUniformLocation(Pipeline, "SwapNScale");
    Variables.MatrixLoc = glGetUniformLocation(Pipeline, "Matrix");
    Variables.ProjectionMatrixLoc = glGetUniformLocation(Pipeline, "ProjectionMatrix");
    Variables.CamRotMatrixLoc = glGetUniformLocation(Pipeline, "CamRotMatrix");
    Variables.MovementYLoc = glGetUniformLocation(Pipeline, "MovementY");
    Variables.MovementXLoc = glGetUniformLocation(Pipeline, "MovementX");
    Variables.OffsetXLoc = glGetUniformLocation(Pipeline, "OffsetX");
    Variables.OffsetZLoc = glGetUniformLocation(Pipeline, "OffsetZ");
    Variables.ZoomLoc = glGetUniformLocation(Pipeline, "Zoom");

    GLuint TextPipeline = CreateGLProgramTextMode();

    const int Gridsize = 50;
    vertex Grid[Gridsize * 2][2];
    InitGrid((vertex *)Grid, Gridsize);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //using the object
    glGenVertexArrays(1, &Object.GL.VAO);
    glBindVertexArray(Object.GL.VAO);

    glGenBuffers(1, &Object.GL.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, Object.GL.VBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, X)); // 3 means 3 componets per attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, R));

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * 3 * Object.Mesh.FaceCountR + sizeof(Grid), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex) * 3 * Object.Mesh.FaceCountR, Object.Vertex);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex) * 3 * Object.Mesh.FaceCountR, sizeof(Grid), Grid);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++)
    {
        FT_Load_Char(face, c, FT_LOAD_RENDER);
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        character Char = {
            texture,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            face->glyph->advance.x,
            c};
        Character[c] = Char;
    }

    GL_Objects glTextType;
    glGenVertexArrays(1, &glTextType.VAO);
    glGenBuffers(1, &glTextType.VBO);
    glBindVertexArray(glTextType.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, glTextType.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    ///////////////////////////////////////////////////////////////////////////////////////////////

    GL_Objects glText;

    glGenVertexArrays(1, &glText.VAO);
    glBindVertexArray(glText.VAO);

    glGenBuffers(1, &glText.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, glText.VBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0); // 3 means 3 componets per attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(sizeof(float) * 2));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GL_GetError();

    ///////////////////////////////////////////////////////////////////////////////////////////////

    GL_Objects GUI;

    glGenBuffers(1, &GUI.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, GUI.VBO);

    glGenVertexArrays(1, &GUI.VAO);
    glBindVertexArray(GUI.VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, X)); // 3 means 3 componets per attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, R));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    ///////////////////////////////////////////////////////////////////////////////////////////////

    image IMAGE;
    IMAGE.data = stbi_load("face1.jpg", &IMAGE.w, &IMAGE.h, &IMAGE.n, 3);

    glLineWidth(2);
    GL_GetError();

    SDL_Color Color = {255, 255, 255, 255};
    glColor White = {1, 1, 1, 1};
    TTF_Font *font = TTF_OpenFont("PTSans-Regular.ttf", 20);
    TTF_Font *font45 = TTF_OpenFont("PTSans-Regular.ttf", 45);

    guiElements GUIelements;
    unsigned int texture[2];
    glGenTextures(2, texture);

    float etha = 0;

    keys Keys;
    SDL_Event Event;
    while (Running)
    {

        glEnable(GL_DEPTH_TEST);

        // Keys.Space_Key = false;
        Keys.Tab_Key = false;
        Keys.ScrollY = 0;

        const int FPS = 60;
        const int frameDelay = 1000 / FPS;
        Uint32 frameStart = SDL_GetTicks();
        PollEvents(&Event, &Keys, GUIelements.Button);

        Update3DVariables(&Variables, &Keys, GUIelements.Button);

        glClearColor(0.3, 0.3, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderObject(Variables, Object, Pipeline);

        RenderGrid(Variables, Grid, Object, Pipeline);

        RenderGUI(font, Color, GUI, glText, TextPipeline, Variables, &GUIelements, texture);

        if (RUNRAYTRACER)
        {
            RenderTextGL("Rendering... Please wait...", Size, face, White, WindowWidth / 2, WindowHeight / 2, glTextType, TextPipeline, Character);
        }
        etha++;
        glColor LGBTQ = {(sin((etha +( (float)M_PI/2))/2)+1 )/ 2, (((etha +( (float)M_PI/4))/50)+1 )/ 2, (sin(etha/10)+1 )/ 2, 1};
        RenderTextGL("Copyright - Ragsterb Technologies (2021 - 2069)", 10, face, LGBTQ, WindowWidth / 2, WindowHeight - 50, glTextType, TextPipeline, Character);

        glDisable(GL_BLEND);

        SDL_GL_SwapWindow(Window);

        int frameEnd = SDL_GetTicks();
        int frameTime = frameEnd - frameStart;
        if (frameTime < frameDelay)
        {
            SDL_Delay(frameDelay - frameTime);
        }
        if (RUNRAYTRACER)
            RAYTRACER();
    }

    return 0;
}
