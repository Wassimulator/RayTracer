#pragma once
#include "source.cpp"
#include "obj_parser.cpp"

typedef GLuint(APIENTRY *_glCreateProgram)(void);
_glCreateProgram glCreateProgram;
typedef GLuint(APIENTRY *_glCreateShader)(GLenum shaderType);
_glCreateShader glCreateShader;
typedef void(APIENTRY *_glAttachShader)(GLuint program, GLuint shader);
_glAttachShader glAttachShader;
typedef void(APIENTRY *_glCompileShader)(GLuint shader);
_glCompileShader glCompileShader;
typedef void(APIENTRY *_glShaderSource)(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
_glShaderSource glShaderSource;
typedef void(APIENTRY *_glLinkProgram)(GLuint program);
_glLinkProgram glLinkProgram;
typedef void(APIENTRY *_glGetProgramInfoLog)(GLuint program, GLsizei maxLengthofBuffer, GLsizei *ActualLengthOfBuffer, GLchar *buffer);
_glGetProgramInfoLog glGetProgramInfoLog;
typedef void(APIENTRY *_glGetShaderInfoLog)(GLuint Shader, GLsizei maxLengthofBuffer, GLsizei *ActualLengthOfBuffer, GLchar *buffer);
_glGetShaderInfoLog glGetShaderInfoLog;
typedef void(APIENTRY *_glGetProgramiv)(GLuint program, GLenum pname, GLint *params);
_glGetProgramiv glGetProgramiv;
typedef void(APIENTRY *_glGenVertexArrays)(GLsizei n, GLuint *arrays);
_glGenVertexArrays glGenVertexArrays;
typedef void(APIENTRY *_glBindVertexArray)(GLuint array);
_glBindVertexArray glBindVertexArray;
typedef void(APIENTRY *_glEnableVertexAttribArray)(GLuint array);
_glEnableVertexAttribArray glEnableVertexAttribArray;
typedef void(APIENTRY *_glGenBuffers)(GLsizei n, GLuint *buffers);
_glGenBuffers glGenBuffers;
typedef void(APIENTRY *_glBindBuffer)(GLenum target, GLuint buffer);
_glBindBuffer glBindBuffer;
typedef void(APIENTRY *_glUseProgram)(GLuint Program);
_glUseProgram glUseProgram;
typedef void(APIENTRY *_glUniform1f)(GLint location, GLfloat v0);
_glUniform1f glUniform1f;
typedef void(APIENTRY *_glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
_glUniform3f glUniform3f;
typedef void(APIENTRY *_glActiveTexture)(GLenum texture);
_glActiveTexture glActiveTexture;
typedef void(APIENTRY *_glBufferSubData)(GLenum target,
                                         GLintptr offset,
                                         GLsizeiptr size,
                                         const GLvoid *data);
_glBufferSubData glBufferSubData;
typedef GLint(APIENTRY *_glGetUniformLocation)(GLuint program,
                                               const GLchar *name);
_glGetUniformLocation glGetUniformLocation;
typedef void(APIENTRY *_glUniformMatrix2fv)(GLint location,
                                            GLsizei count,
                                            GLboolean transpose,
                                            const GLfloat *value);
_glUniformMatrix2fv glUniformMatrix2fv;
typedef void(APIENTRY *_glUniformMatrix4fv)(GLint location,
                                            GLsizei count,
                                            GLboolean transpose,
                                            const GLfloat *value);
_glUniformMatrix4fv glUniformMatrix4fv;
typedef void(APIENTRY *_glBufferData)(GLenum target,
                                      GLsizeiptr size,
                                      const GLvoid *data,
                                      GLenum usage);
_glBufferData glBufferData;
typedef void(APIENTRY *_glVertexAttribPointer)(GLuint index,
                                               GLint size,
                                               GLenum type,
                                               GLboolean normalized,
                                               GLsizei stride,
                                               const GLvoid *pointer);
_glVertexAttribPointer glVertexAttribPointer;

void LoadGLfunctions()
{
    glCreateProgram = (_glCreateProgram)SDL_GL_GetProcAddress("glCreateProgram");
    glCreateShader = (_glCreateShader)SDL_GL_GetProcAddress("glCreateShader");
    glAttachShader = (_glAttachShader)SDL_GL_GetProcAddress("glAttachShader");
    glCompileShader = (_glCompileShader)SDL_GL_GetProcAddress("glCompileShader");
    glShaderSource = (_glShaderSource)SDL_GL_GetProcAddress("glShaderSource");
    glLinkProgram = (_glLinkProgram)SDL_GL_GetProcAddress("glLinkProgram");
    glGetProgramInfoLog = (_glGetProgramInfoLog)SDL_GL_GetProcAddress("glGetProgramInfoLog");
    glGetShaderInfoLog = (_glGetShaderInfoLog)SDL_GL_GetProcAddress("glGetShaderInfoLog");
    glGetProgramiv = (_glGetProgramiv)SDL_GL_GetProcAddress("glGetProgramiv");
    glGenVertexArrays = (_glGenVertexArrays)SDL_GL_GetProcAddress("glGenVertexArrays");
    glBindVertexArray = (_glBindVertexArray)SDL_GL_GetProcAddress("glBindVertexArray");
    glEnableVertexAttribArray = (_glEnableVertexAttribArray)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
    glGenBuffers = (_glGenBuffers)SDL_GL_GetProcAddress("glGenBuffers");
    glBindBuffer = (_glBindBuffer)SDL_GL_GetProcAddress("glBindBuffer");
    glVertexAttribPointer = (_glVertexAttribPointer)SDL_GL_GetProcAddress("glVertexAttribPointer");
    glBufferData = (_glBufferData)SDL_GL_GetProcAddress("glBufferData");
    glUseProgram = (_glUseProgram)SDL_GL_GetProcAddress("glUseProgram");
    glUniformMatrix2fv = (_glUniformMatrix2fv)SDL_GL_GetProcAddress("glUniformMatrix2fv");
    glUniformMatrix4fv = (_glUniformMatrix4fv)SDL_GL_GetProcAddress("glUniformMatrix4fv");
    glGetUniformLocation = (_glGetUniformLocation)SDL_GL_GetProcAddress("glGetUniformLocation");
    glUniform1f = (_glUniform1f)SDL_GL_GetProcAddress("glUniform1f");
    glUniform3f = (_glUniform3f)SDL_GL_GetProcAddress("glUniform3f");
    glBufferSubData = (_glBufferSubData)SDL_GL_GetProcAddress("glBufferSubData");
    glActiveTexture = (_glActiveTexture)SDL_GL_GetProcAddress("glActiveTexture");
}

GLuint CreateGLProgram()
{
    GLuint Pipeline = glCreateProgram();
    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glAttachShader(Pipeline, VertexShader);
    glAttachShader(Pipeline, FragmentShader);

    const char *VertexCode = R"###(
            #version 330
            layout(location = 0) in vec3 Position;
            layout(location = 1) in vec4 ColorIn;
            uniform mat2 Matrix;
            uniform mat4 ProjectionMatrix;
            uniform mat4 SwapNScale;
            uniform float MovementY;
            uniform float MovementX;
            uniform float OffsetX;
            uniform float OffsetZ;
            uniform float Zoom;
            smooth out vec4 ColorOut;

            uniform mat4 CamRotMatrix;

            void main()
            {   
                vec4 Position_Scaled = SwapNScale * vec4(Position,1);
                vec2 PositionXZ = Matrix * Position_Scaled.xz;
                vec3 rotated = vec3(PositionXZ.x,Position_Scaled.y,PositionXZ.y);
                rotated.x += OffsetX;
                rotated.z += OffsetZ;
                rotated = vec3 (CamRotMatrix * vec4(rotated,1));
                rotated.z -= 7 + Zoom;
                gl_Position = ProjectionMatrix * vec4(rotated,  1); 
                ColorOut = ColorIn;
            }
    )###";
    const GLint VertexCodeLegth = strlen(VertexCode);
    glShaderSource(VertexShader, 1, &VertexCode, &VertexCodeLegth);
    glCompileShader(VertexShader);

    const char *FragmentCode = R"###(
            #version 330
            out vec4 FragmentShader;
            smooth in vec4 ColorOut;
            void main()
            {

            FragmentShader = ColorOut;
            } 
    )###";
    const GLint FragmentCodeLegth = strlen(FragmentCode);
    glShaderSource(FragmentShader, 1, &FragmentCode, &FragmentCodeLegth);
    glCompileShader(FragmentShader);

    glLinkProgram(Pipeline);

    int NoErrors = 0;
    glGetProgramiv(Pipeline, GL_LINK_STATUS, &NoErrors);

    if (NoErrors == 0)
    {
        GLsizei BufferLength;
        GLchar Buffer[4096];
        glGetShaderInfoLog(FragmentShader, 4096, &BufferLength, Buffer);
        if (BufferLength > 0)
            cerr << "Error (Shader):" << Buffer << endl;
        glGetShaderInfoLog(VertexShader, 4096, &BufferLength, Buffer);
        if (BufferLength > 0)
            cerr << "Error (Shader):" << Buffer << endl;
        glGetProgramInfoLog(Pipeline, 4096, &BufferLength, Buffer);
        if (BufferLength > 0)
            cerr << "Error(Program):" << Buffer << endl;
    }

    return Pipeline;
}

GLuint CreateGLProgramTextMode()
{
    GLuint Pipeline = glCreateProgram();
    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glAttachShader(Pipeline, VertexShader);
    glAttachShader(Pipeline, FragmentShader);

    const char *VertexCode = R"###(
            #version 330 core
            layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
            out vec2 TexCoords;

            // uniform mat4 ProjectionMatrix;

            void main()
            {
                // gl_Position = ProjectionMatrix * vec4(vertex.xy, 0.0, 1.0);
                gl_Position = vec4(vertex.xy, 0.0, 1.0);
                TexCoords = vertex.zw;
            } 
    )###";

    const GLint VertexCodeLegth = strlen(VertexCode);
    glShaderSource(VertexShader, 1, &VertexCode, &VertexCodeLegth);
    glCompileShader(VertexShader);

    const char *FragmentCode = R"###(
            #version 330 core
            in vec2 TexCoords;
            out vec4 color;

            uniform sampler2D text;
            uniform vec3 textColor;

            void main()
            {    
                vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
                color = vec4(textColor, 1.0) * sampled;
            }
    )###";

    const GLint FragmentCodeLegth = strlen(FragmentCode);
    glShaderSource(FragmentShader, 1, &FragmentCode, &FragmentCodeLegth);
    glCompileShader(FragmentShader);

    glLinkProgram(Pipeline);

    int NoErrors = 0;
    glGetProgramiv(Pipeline, GL_LINK_STATUS, &NoErrors);

    if (NoErrors == 0)
    {
        GLsizei BufferLength;
        GLchar Buffer[4096];
        glGetShaderInfoLog(FragmentShader, 4096, &BufferLength, Buffer);
        if (BufferLength > 0)
            cerr << "Error (Shader):" << Buffer << endl;
        glGetShaderInfoLog(VertexShader, 4096, &BufferLength, Buffer);
        if (BufferLength > 0)
            cerr << "Error (Shader):" << Buffer << endl;
        glGetProgramInfoLog(Pipeline, 4096, &BufferLength, Buffer);
        if (BufferLength > 0)
            cerr << "Error(Program):" << Buffer << endl;
    }

    return Pipeline;
}

struct vertex
{
    float X, Y, Z;
    float R, G, B;
    float Alpha = 1;
};

struct GL_Objects
{
    GLuint VBO = 0; // VBO = Vertex Buffer Objects
    GLuint VAO = 0; // VAO = Vertex Array Objects
};

void InitGrid(vertex *Grid, int Gridsize)
{
    const float cellsize = 0.5;
    const float HafGridDim = ((Gridsize - 1) * cellsize) * 0.5;
    const float GridY = -0.6;

    for (int i = 0; i < Gridsize; i++)
    {
        Grid[i * 2 + 0].X = -HafGridDim + i * cellsize;
        Grid[i * 2 + 0].Y = GridY;
        Grid[i * 2 + 0].Z = -HafGridDim;

        Grid[i * 2 + 1].X = -HafGridDim + i * cellsize;
        Grid[i * 2 + 1].Y = GridY;
        Grid[i * 2 + 1].Z = HafGridDim;
    }
    for (int i = 0; i < Gridsize; i++)
    {
        Grid[(i + Gridsize) * 2 + 0].Z = -HafGridDim + i * cellsize;
        Grid[(i + Gridsize) * 2 + 0].Y = GridY;
        Grid[(i + Gridsize) * 2 + 0].X = -HafGridDim;

        Grid[(i + Gridsize) * 2 + 1].Z = -HafGridDim + i * cellsize;
        Grid[(i + Gridsize) * 2 + 1].Y = GridY;
        Grid[(i + Gridsize) * 2 + 1].X = HafGridDim;
    }

    for (int i = 0; i < Gridsize * 2; i++)
    {
        Grid[i * 2 + 0].R = 0;
        Grid[i * 2 + 0].G = 0;
        Grid[i * 2 + 0].B = 0;
        Grid[i * 2 + 0].Alpha = 1;

        Grid[i * 2 + 1].R = 0;
        Grid[i * 2 + 1].G = 0;
        Grid[i * 2 + 1].B = 0;
        Grid[i * 2 + 1].Alpha = 1;
    }
}

void AddRectBL(vertex **VGUIs, float x, float y, float z, float w, float h, float r, float g, float b, int *sizeofVGUI)
{
    int Index = *sizeofVGUI / sizeof(vertex);
    *VGUIs = (vertex *)realloc(*VGUIs, *sizeofVGUI + sizeof(vertex) * 6);
    *sizeofVGUI += sizeof(vertex) * 6;

    y *= -1;
    r /= 255;
    g /= 255;
    b /= 255;

    vertex *VGUI = *VGUIs;
    VGUI[Index + 0].X = x;
    VGUI[Index + 0].Y = y;
    VGUI[Index + 0].Z = z;

    VGUI[Index + 1].X = x;
    VGUI[Index + 1].Y = y - h;
    VGUI[Index + 1].Z = z;

    VGUI[Index + 2].X = x + w;
    VGUI[Index + 2].Y = y - h;
    VGUI[Index + 2].Z = z;

    VGUI[Index + 3].X = x;
    VGUI[Index + 3].Y = y;
    VGUI[Index + 3].Z = z;

    VGUI[Index + 4].X = x + w;
    VGUI[Index + 4].Y = y - h;
    VGUI[Index + 4].Z = z;

    VGUI[Index + 5].X = x + w;
    VGUI[Index + 5].Y = y;
    VGUI[Index + 5].Z = z;

    for (int i = 0; i < 6; i++)
    {
        VGUI[Index + i].R = r;
        VGUI[Index + i].G = g;
        VGUI[Index + i].B = b;
    }
}

struct TextFloats
{
    float Text[24];
};

TextFloats CreateTextRectTL(int X, int Y, int W, int H)
{
    TextFloats result;
    float x[6];
    float y[6];
    x[0] = X;
    y[0] = Y;
    x[1] = X + W;
    y[1] = Y;
    x[2] = X;
    y[2] = Y + H;
    x[3] = X;
    y[3] = Y + H;
    x[4] = X + W;
    y[4] = Y;
    x[5] = X + W;
    y[5] = Y + H;

    for (int i = 0; i < 6; i++)
    {
        x[i] = 2 * x[i] / WindowWidth - 1;
        y[i] = -2 * y[i] / WindowHeight + 1;
    }

    float Text[24] = {x[0], y[0], 0, 0,
                      x[1], y[1], 1, 0,
                      x[2], y[2], 0, 1,
                      x[3], y[3], 0, 1,
                      x[4], y[4], 1, 0,
                      x[5], y[5], 1, 1};

    for (int i = 0; i < 24; i++)
    {
        result.Text[i] = Text[i];
    }

    return result;
}

struct object
{
    obj_mesh Mesh;
    vertex *Vertex;
    GL_Objects GL;
};

struct glColor
{
    float r,g,b,a;
};

struct guiElements
{
    rect BottomBar = {0, 0, 0, (float)WindowWidth, 70, 25, 25, 25, 0, 0, 0};
    vertex *VGUI = 0;
    int sizeofVGUI = 0;
    rect Button[MAXBUTTONS];
};

struct glVariables
{
    GLint SwapNScaleLoc;
    GLint MatrixLoc;
    GLint ProjectionMatrixLoc;
    GLint CamRotMatrixLoc;
    GLint MovementYLoc;
    GLint MovementXLoc;
    GLint OffsetXLoc;
    GLint OffsetZLoc;
    GLint ZoomLoc;

    float th = 0;
    float n = 2; //near clipping plane
    float r = 1;
    float t = (float)WindowHeight / WindowWidth;
    float f = 100; //far clipping plane
    float movementY = 0;
    float movementX = 0;

    float OffsetX = 0;
    float OffsetZ = 0;
    float Zoom = 0;

    float rAx = 0;
    float rAy = 0;

    float S1 = sin(rAx);
    float C1 = cos(rAx);
    float S2 = sin(rAy);
    float C2 = cos(rAy);
    // float s = 0.05;
    // float SwapNScale[] = {s, 0, 0, 0,
    //                       0, 0, s, 0,
    //                       0, s, 0, 0,
    //                       0, 2, 0, 1};
    float SwapNScale[16] = {1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, -0.5, 0, 1};

    float ProjectionMatrix[16] = {n, 0, 0, 0, 0, n / t, 0, 0, 0, 0, -(f + n) / (f - n), -1, 0, 0, (-2 * f * n) / (f - n), 0};
    float CamRotMatrix[16] = {C2, -S1 *S2, C1 *S2, 0, 0, C1, S1, 0, -S2, -S1 *C2, C1 *C2, 0, 0, 0, 0, 1};
    float RotationMatrix[4] = {cos(th), sin(th), -sin(th), cos(th)};
    float identity_2x2[4] = {1, 0, 0, 1};
    float identity_4x4[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    float GUI_Projection[16] = {(float)2 / WindowWidth, 0, 0, 0, 0, (float)-2 / WindowHeight, 0, 0, 0, 0, 1, 0, -1, -1, 0, 1};
};

object ParseOBJtoObjectStruct(char *FileName)
{
    object Result;
    Result.Mesh = ParseOBJFileToMesh(FileName);
    Result.Vertex = (vertex *)malloc(sizeof(vertex) * 3 * Result.Mesh.FaceCountR);
    if (Result.Mesh.FaceCountC == 3)
    {
        for (int i = 0; i < Result.Mesh.FaceCountR; i++)
        {
            vec3 v0 = Result.Mesh.Verts[Result.Mesh.Faces[i].v[0] - 1];
            vec3 v1 = Result.Mesh.Verts[Result.Mesh.Faces[i].v[1] - 1];
            vec3 v2 = Result.Mesh.Verts[Result.Mesh.Faces[i].v[2] - 1];

            Result.Vertex[3 * i + 0].X = v0.x;
            Result.Vertex[3 * i + 0].Y = v0.y;
            Result.Vertex[3 * i + 0].Z = v0.z;

            Result.Vertex[3 * i + 1].X = v1.x;
            Result.Vertex[3 * i + 1].Y = v1.y;
            Result.Vertex[3 * i + 1].Z = v1.z;

            Result.Vertex[3 * i + 2].X = v2.x;
            Result.Vertex[3 * i + 2].Y = v2.y;
            Result.Vertex[3 * i + 2].Z = v2.z;

            float tea = (float)i / Result.Mesh.FaceCountR + 0.2;
            v0 = v0.normalize();
            v1 = v1.normalize();
            v2 = v2.normalize();

            Result.Vertex[3 * i + 0].R = v0.x;
            Result.Vertex[3 * i + 0].G = v0.y;
            Result.Vertex[3 * i + 0].B = v0.z;

            Result.Vertex[3 * i + 1].R = v1.x;
            Result.Vertex[3 * i + 1].G = v1.y;
            Result.Vertex[3 * i + 1].B = v1.z;

            Result.Vertex[3 * i + 2].R = v2.x;
            Result.Vertex[3 * i + 2].G = v2.y;
            Result.Vertex[3 * i + 2].B = v2.z;
        }
    }
    return Result;
}

void Update3DVariables(glVariables *Variables, keys *K, rect *Button)
{
    glVariables GL = *Variables;
    keys Keys = *K;

    float MouseSenV = 0.004;
    float MouseSenH = 0.007;

    if (Keys.Shift)
        GL.th += M_PI / 150;

    if (Keys.RightMouseButton)
    {
        GL.rAx += MouseSenH * Keys.yrel;
        GL.rAy -= MouseSenV * Keys.xrel;
    }
    if (Keys.LeftMouseButton)
    {
        float Sign = GL.rAx > 0 ? 1 : -1;
        GL.OffsetZ += MouseSenH * (Sign * Keys.yrel * cos(GL.rAy) - Keys.xrel * sin(GL.rAy));
        GL.OffsetX += MouseSenH * (Sign * Keys.yrel * sin(GL.rAy) + Keys.xrel * cos(GL.rAy));
    }

    GL.Zoom -= 0.5 * Keys.ScrollY;
    if (GL.Zoom < 0)
        GL.Zoom = 0;

    float RotationOffset = 0.01;
    if (Keys.W_Key)
        GL.rAx += RotationOffset;
    if (Keys.S_Key)
        GL.rAx -= RotationOffset;
    if (GL.rAx > M_PI / 2)
        GL.rAx = M_PI / 2;
    if (GL.rAx < -M_PI / 2)
        GL.rAx = -M_PI / 2;
    if (Keys.D_Key)
        GL.rAy += RotationOffset;
    if (Keys.A_Key)
        GL.rAy -= RotationOffset;

    SDL_GetMouseState(&Keys.x, &Keys.y);
    for (int i = 0; i < MAXBUTTONS; i++)
    {
        Button[i].highlighted = (Keys.x >= Button[i].x &&
                                 Keys.x <= Button[i].x + Button[i].w &&
                                 Keys.y <= (WindowHeight - Button[i].y) &&
                                 Keys.y >= (WindowHeight - (Button[i].y + Button[i].h)));
    }

    for (int i = 0; i < MAXBUTTONS; i++)
    {
        if (Button[i].highlighted)
        {
            Button[i].r = Button->r_original + 20;
            Button[i].g = Button->g_original + 20;
            Button[i].b = Button->b_original + 20;
            if (Keys.LeftMouseButton)
            {
                Button[i].r = Button->r_original - 20;
                Button[i].g = Button->g_original - 20;
                Button[i].b = Button->b_original - 20;
                RUNRAYTRACER = 1;
            }
        }
        else
        {
            Button->r = Button->r_original;
            Button->g = Button->g_original;
            Button->b = Button->b_original;
        }
    }

    GL.S1 = sin(GL.rAx);
    GL.C1 = cos(GL.rAx);
    GL.S2 = sin(GL.rAy);
    GL.C2 = cos(GL.rAy);
    float CamRotMatrix_temp[] = {GL.C2, -GL.S1 * GL.S2, GL.C1 * GL.S2, 0, 0, GL.C1, GL.S1, 0, -GL.S2, -GL.S1 * GL.C2, GL.C1 * GL.C2, 0, 0, 0, 0, 1};
    float RotationMatrix_temp[] = {cos(GL.th), sin(GL.th), -sin(GL.th), cos(GL.th)};
    std::copy(CamRotMatrix_temp, CamRotMatrix_temp + 16, GL.CamRotMatrix);
    std::copy(RotationMatrix_temp, RotationMatrix_temp + 4, GL.RotationMatrix);

    *Variables = GL;
    *K = Keys;
}

void RenderTextGL(char *TextChar, int Size, FT_Face face, glColor Color, int PosXcentered,
                  int PosYcentered, GL_Objects glText, GLint TextPipeline, character *Character)
{
    FT_Set_Char_Size(face, 0, Size << 6, 96, 96);
    FT_Set_Pixel_Sizes(face, 0, Size);

    string Text = TextChar;

    int TextLength = GetRenderTextLength(Text, Size, face, 1, Character);

    int penX = PosXcentered - (TextLength / 2);
    int penY = PosYcentered - Size / 2;

    glUseProgram(TextPipeline);

    GL_GetError();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(glText.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, glText.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUniform3f(glGetUniformLocation(TextPipeline, "textColor"), Color.r, Color.g, Color.b);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(glText.VAO);

    // iterate through all characterss
    int p = 0; //previous character
    for (int c = 0; c < Text.size(); c++)
    {
        int i = FT_Get_Char_Index(face, Text[c]);
        i += 29; //for some fucking reason.

        SDL_Rect rectD;
        rectD.h = Character[i].height;
        rectD.w = Character[i].width;
        rectD.x = penX + Character[i].BearingX;
        rectD.y = penY + 30 - Character[i].BearingY;

        penX += (Character[i].Advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)
        if (p > 0)
        {
            FT_Vector delta;
            FT_Get_Kerning(face, p, i, FT_KERNING_DEFAULT, &delta);
            penX += delta.x >> 6;
        }

        TextFloats TextFloatss = CreateTextRectTL(rectD.x, rectD.y, rectD.w, rectD.h);

        glBindTexture(GL_TEXTURE_2D, Character[i].TextureID);
        glBindVertexArray(glText.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, glText.VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 24, TextFloatss.Text);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        GL_GetError();

        p = i; //set previous values
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Zerofy3DVariables(glVariables Variables)
{
    glUniformMatrix2fv(Variables.MatrixLoc, 1, GL_FALSE, Variables.identity_2x2);
    glUniformMatrix4fv(Variables.ProjectionMatrixLoc, 1, GL_FALSE, Variables.identity_4x4);
    glUniformMatrix4fv(Variables.CamRotMatrixLoc, 1, GL_FALSE, Variables.identity_4x4);
    glUniform1f(Variables.MovementYLoc, 0);
    glUniform1f(Variables.MovementXLoc, 0);
    glUniform1f(Variables.OffsetXLoc, 0);
    glUniform1f(Variables.OffsetZLoc, 0);
    glUniform1f(Variables.ZoomLoc, -6);
}

void RenderGUI(TTF_Font *font, SDL_Color Color, GL_Objects GUI, GL_Objects glText,
               GLint TextPipeline, glVariables Variables,
               guiElements *E, unsigned int *texture)
{
    Zerofy3DVariables(Variables);

    AddRectBL(&E->VGUI, E->BottomBar.x, E->BottomBar.y, E->BottomBar.z, E->BottomBar.w, E->BottomBar.h, E->BottomBar.r, E->BottomBar.g, E->BottomBar.b, &E->sizeofVGUI);

    for (int i = 0; i < MAXBUTTONS; i++)
    {
        AddRectBL(&E->VGUI, E->Button[i].x, E->Button[i].y, E->Button[i].z, E->Button[i].w, E->Button[i].h, E->Button[i].r, E->Button[i].g, E->Button[i].b, &E->sizeofVGUI);
    }
    // RenderTextGL("Render", 20, face,
    SDL_Surface *Text_Render = TTF_RenderText_Blended(font, "Render", Color);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture[0]);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Text_Render->w, Text_Render->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Text_Render->pixels);
    E->Button[0] = {10, 10, 1, (float)(Text_Render->w + 20), (float)(Text_Render->h + 20), 150, 100, 100, 150, 100, 100, 1, 0, 0};
    // TextFloats Text_Render_Floats = CreateTextRectTL(E->Button[0].x + 10, WindowHeight - E->Button[0].y - 10 - Text_Render->h, Text_Render->w, Text_Render->h);

    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(GUI.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, GUI.VBO);
    glUniformMatrix4fv(Variables.ProjectionMatrixLoc, 1, GL_FALSE, Variables.GUI_Projection);
    glBufferData(GL_ARRAY_BUFFER, E->sizeofVGUI, E->VGUI, GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, E->sizeofVGUI / sizeof(vertex));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glBindVertexArray(glText.VAO);
    // glUseProgram(TextPipeline);
    // glBindTexture(GL_TEXTURE_2D, texture[0]);
    // glBindBuffer(GL_ARRAY_BUFFER, glText.VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, Text_Render_Floats.Text, GL_STREAM_DRAW);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // RenderTextGL("Render")

    E->sizeofVGUI = 0;
    // SDL_FreeSurface(Text_Render);
}

void RenderObject(glVariables Variables, object Object, GLint Pipeline)
{
    glUseProgram(Pipeline);
    glBindVertexArray(Object.GL.VAO);
    glUniformMatrix4fv(Variables.SwapNScaleLoc, 1, GL_FALSE, Variables.SwapNScale);
    glUniformMatrix4fv(Variables.CamRotMatrixLoc, 1, GL_FALSE, Variables.CamRotMatrix);
    glUniform1f(Variables.OffsetXLoc, Variables.OffsetX);
    glUniform1f(Variables.OffsetZLoc, Variables.OffsetZ);
    glUniform1f(Variables.ZoomLoc, Variables.Zoom);
    glUniform1f(Variables.MovementYLoc, Variables.movementY);
    glUniform1f(Variables.MovementXLoc, Variables.movementX);
    glUniformMatrix2fv(Variables.MatrixLoc, 1, GL_FALSE, Variables.RotationMatrix);
    glUniformMatrix4fv(Variables.ProjectionMatrixLoc, 1, GL_FALSE, Variables.ProjectionMatrix);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex) * 3 * Object.Mesh.FaceCountR / sizeof(vertex)); //3 means, draw 3 vertices, (each has 3 components)
}

template <typename TwoD> //Use templates to pass multidimentional arrays as function params. there's no other straightforward way for it in C. fucking morons.
void RenderGrid(glVariables Variables, TwoD &Grid, object Object, GLint Pipeline)
{
    glUseProgram(Pipeline);
    glUniform1f(Variables.MovementYLoc, 0);
    glUniform1f(Variables.MovementXLoc, 0);
    glUniformMatrix2fv(Variables.MatrixLoc, 1, GL_FALSE, Variables.identity_2x2);
    glUniformMatrix4fv(Variables.SwapNScaleLoc, 1, GL_FALSE, Variables.identity_4x4);
    glUniformMatrix4fv(Variables.ProjectionMatrixLoc, 1, GL_FALSE, Variables.ProjectionMatrix);
    glDrawArrays(GL_LINES, sizeof(vertex) * 3 * Object.Mesh.FaceCountR / sizeof(vertex), sizeof(Grid) / sizeof(vertex)); //TODO: needs changing when we add more objects.
}