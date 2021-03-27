#pragma once
#include "source.cpp"

struct face
{
    int v[4];
    int vt[4];
    int vn[4];
};

struct obj_mesh
{
    size_t VertCount;
    size_t NormalCount;
    size_t FaceCountR;
    size_t FaceCountC;
    vec3 *Verts;
    vec3 *Normals;
    face *Faces;
    char *mtlfilepath;
};

obj_mesh ParseOBJFileToMesh(const char *Path);
void FreeOBJMesh(obj_mesh *Mesh);

struct token
{
    char *Start;
    char *End;
};

static int
TokenParseInt(token Token)
{
    char *Start = Token.Start;
    char *End = Token.End;

    char *Tmp = (char *)malloc(End - Start + 1);
    memcpy(Tmp, Start, End - Start);
    Tmp[End - Start] = 0;

    int result = atoi(Tmp);
    free(Tmp);

    return result;
}

static float
TokenParseFloat(token Token)
{
    char *Start = Token.Start;
    char *End = Token.End;

    char *Tmp = (char *)malloc(End - Start + 1);
    memcpy(Tmp, Start, End - Start);
    Tmp[End - Start] = 0;

    float result = (float)atof(Tmp);
    free(Tmp);

    return result;
}

void AdvanceAt(char **At)
{
    if (*(*At + 1) != 0)
    {
        (*At)++;
    }
}

static int
TokenLength(token Token)
{
    return Token.End - Token.Start;
}

static char *
EatWhitespace(char *At, char *End)
{
    while (At < End && isspace(*At))
        At++;
    return At;
}

static char *
SkipUntilWhitespace(char *At, char *End)
{
    while (At < End && !isspace(*At))
        At++;
    return At;
}

static char *
SkipUntilCharOrSpace(char *At, char Char, char *End)
{
    while (At < End && *At != Char && !isspace(*At))
        At++;
    return At;
}

void GetNextToken(char **At, token *Token, int MaxReadCount)
{
    char *End = *At + MaxReadCount;

    *At = EatWhitespace(*At, End);
    while (**At == '#')
    {
        while ((*At) < End && **At != '\n')
        {
            (*At)++;
        }
        if ((*At) < End)
            (*At)++;
        *At = EatWhitespace(*At, End);
    }

    Token->Start = *At;
    Token->End = SkipUntilWhitespace(*At, End);

    *At = Token->End;
}

static char *
SkipToken(token Token)
{
    return Token.End;
}

bool TokenMatches(token Token, char *Text)
{
    if (Token.End - Token.Start == strlen(Text))
    {
        return (memcmp(Token.Start, Text, strlen(Text)) == 0);
    }
    else
        return false;
}

char *TokenToCstring(token Token)
{
    assert(Token.End > Token.Start);
    char *Data = (char *)malloc(Token.End - Token.Start + 1);
    memcpy(Data, Token.Start, Token.End - Token.Start);
    Data[Token.End - Token.Start] = 0;
    return Data;
}

int GetSlashNum(token Token, char *End)
{
    int i = 0;
    char *At = Token.Start;

    while (At < End && !isspace(*At))
    {
        ++At;
        if (*At == '/')
        {
            i++;
        }
    }
    return i;
}
int ParseTillSlash(token Token, char *End)
{
    char *At = Token.Start;

    At = SkipUntilCharOrSpace(At, '/', End);
    token Temp;
    Temp.Start = Token.Start;
    Temp.End = At;
    int result = TokenParseInt(Temp);

    return result;
}

obj_mesh ParseOBJFileToMesh(const char *Path)
{
    FILE *F = fopen(Path, "rb");

    obj_mesh ObjMesh = {};

    if (F)
    {
        fseek(F, 0, SEEK_END);
        size_t FileSize = ftell(F);
        fseek(F, 0, SEEK_SET);

        char *Data = (char *)malloc(FileSize + 1);
        assert(Data);

        fread(Data, FileSize, 1, F);
        Data[FileSize] = 0;

        char *At = Data;

        token Token = {};

#if 0
        GetNextToken(&At, &Token, FileSize - (At - Data));

        if (TokenMatches(Token, "mtllib"))
        {
            GetNextToken(&At, &Token, FileSize - (At - Data));
            ObjMesh.mtlfilepath = TokenToCstring(Token);
        }
#endif

        while (1)
        {
            GetNextToken(&At, &Token, FileSize - (At - Data));
            if (TokenLength(Token) == 0)
            {
                break;
            }

            if (TokenMatches(Token, "v"))
            {
                ObjMesh.VertCount++;
                ObjMesh.Verts = (vec3 *)realloc(ObjMesh.Verts, sizeof(vec3) * ObjMesh.VertCount);
                GetNextToken(&At, &Token, FileSize - (At - Data));
                ObjMesh.Verts[ObjMesh.VertCount - 1].x = TokenParseFloat(Token);
                GetNextToken(&At, &Token, FileSize - (At - Data));
                ObjMesh.Verts[ObjMesh.VertCount - 1].y = TokenParseFloat(Token);
                GetNextToken(&At, &Token, FileSize - (At - Data));
                ObjMesh.Verts[ObjMesh.VertCount - 1].z = TokenParseFloat(Token);
            }
            else if (TokenMatches(Token, "vn"))
            {
                ObjMesh.NormalCount++;
                ObjMesh.Normals = (vec3 *)realloc(ObjMesh.Normals, sizeof(vec3) * ObjMesh.NormalCount);
                GetNextToken(&At, &Token, FileSize - (At - Data));
                ObjMesh.Normals[ObjMesh.NormalCount - 1].x = TokenParseFloat(Token);
                GetNextToken(&At, &Token, FileSize - (At - Data));
                ObjMesh.Normals[ObjMesh.NormalCount - 1].y = TokenParseFloat(Token);
                GetNextToken(&At, &Token, FileSize - (At - Data));
                ObjMesh.Normals[ObjMesh.NormalCount - 1].z = TokenParseFloat(Token);
            }
            else if (TokenMatches(Token, "f"))
            {
                ObjMesh.FaceCountC = 0;
                int ColumnCount = 0;
                token StartToken = Token;
                while (1)
                {
                    GetNextToken(&At, &Token, FileSize - (At - Data));
                    if (TokenLength(Token) == 0 || TokenMatches(Token, "f") || ColumnCount == 4)
                        break;
                    ColumnCount++;
                }
                assert(ColumnCount < 4);
                Token = StartToken;
                At = StartToken.End;
                ObjMesh.FaceCountR++;
                ObjMesh.Faces = (face *)realloc(ObjMesh.Faces, sizeof(face) * ObjMesh.FaceCountR);

                for (int i = 0; i < ColumnCount; i++)
                {
                    GetNextToken(&At, &Token, FileSize - (At - Data));
                    int SlashNum = GetSlashNum(Token, Data + FileSize);
                    switch (SlashNum)
                    {
                    case 0:
                        ObjMesh.Faces[ObjMesh.FaceCountR - 1].v[ObjMesh.FaceCountC] = ParseTillSlash(Token, Data + FileSize);
                        ObjMesh.Faces[ObjMesh.FaceCountR - 1].vt[ObjMesh.FaceCountC] = -1;
                        ObjMesh.Faces[ObjMesh.FaceCountR - 1].vn[ObjMesh.FaceCountC] = -1;
                        break;
                    case 1:
                        ObjMesh.Faces[ObjMesh.FaceCountR - 1].v[ObjMesh.FaceCountC] = ParseTillSlash(Token, Data + FileSize);
                        ObjMesh.Faces[ObjMesh.FaceCountR - 1].vt[ObjMesh.FaceCountC] = ParseTillSlash(Token, Data + FileSize);
                        ObjMesh.Faces[ObjMesh.FaceCountR - 1].vn[ObjMesh.FaceCountC] = -1;
                        break;
                    case 2:
                        ObjMesh.Faces[ObjMesh.FaceCountR - 1].v[ObjMesh.FaceCountC] = ParseTillSlash(Token, Data + FileSize);
                        ObjMesh.Faces[ObjMesh.FaceCountR - 1].vt[ObjMesh.FaceCountC] = ParseTillSlash(Token, Data + FileSize);
                        ObjMesh.Faces[ObjMesh.FaceCountR - 1].vn[ObjMesh.FaceCountC] = ParseTillSlash(Token, Data + FileSize);
                        break;
                    default:
                        break;
                    }
                    ObjMesh.FaceCountC++;
                }
            }
        }

        free(Data);
        fclose(F);
    }


    return ObjMesh;
}

void FreeOBJMesh(obj_mesh *Mesh)
{
    if (Mesh->Verts)
    {
        free(Mesh->Verts);
    }
}
