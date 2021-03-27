#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#ifndef VEC3_H
#define VEC3_H

#define STB_IMAGE_IMPLEMENTATION
#include <SDL.h>
#include <SDL_ttf.h>
#include <stb_image.h>

#include <cmath>
#include <iostream>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string>

#include <stdlib.h>
#include <ctype.h>

#include "atlasifier.cpp"

#include <SDL_opengl.h>

#ifdef _WIN32
//multifredding:

#include <Windows.h>
#else
//TODO: the linux thing
#endif

using namespace std;

//GLOBAL VARIABLES:

LONG volatile BARCOUNTER = 0;
LONG volatile BARCOUNTER2 = 0;
LONG volatile COUNT = 0;
int MAXTHREADS = 1;
LONG volatile PPFZT = 0;
int MAXTILES = 0;
int const MAXBUTTONS = 1;
bool RUNRAYTRACER = 0;

int WindowWidth = 1280;
int WindowHeight = 720;
SDL_Surface *WindowSurface;
SDL_Window *Window;
SDL_Renderer *Renderer;
bool Running = true;
////////////////////

struct image
{
    int w, h, n;
    unsigned char *data;
};

struct keys
{
    bool RightButton = false;
    bool LeftButton = false;
    bool UpButton = false;
    bool DownButton = false;
    bool W_Key = false;
    bool A_Key = false;
    bool S_Key = false;
    bool D_Key = false;
    bool Shift = false;
    bool F_Key = false;
    bool H_Key = false;
    bool E_Key = false;
    bool Tab_Key = false;
    bool Space_Key = false;

    int xrel = 0;
    int yrel = 0;
    int x = 0, y = 0;

    bool LeftMouseButton = false;
    bool RightMouseButton = false;
    bool MiddleMouseButton = false;
    int ScrollY = 0;
};


struct rect
{
    float x, y, z;
    float w, h;
    float r, g, b;
    float r_original, g_original, b_original;
    bool clickable = 0, highlighted = 0, clicked = 0;
};

image TEXTURE[2];

inline double random_double()
{
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}

struct vec3
{
    union
    {
        double e[3];
        struct
        {
            double x, y, z;
        };
    };

    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double &operator[](int i) { return e[i]; }

    vec3 &operator+=(const vec3 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3 &operator*=(const double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3 &operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    double length() const
    {
        return sqrt(length_squared());
    }

    vec3 normalize()
    {
        double lengf = length();
        vec3 thomas(x / lengf, y / lengf, z / lengf);
        return thomas;
    }

    double dot(vec3 V)
    {
        double result = x * V.x + y * V.y + z * V.z;
        return result;
    }

    vec3 hadamard(vec3 V)
    {
        vec3 result(x * V.x, y * V.y, z * V.z);
        return result;
    }
};

vec3 operator-(vec3 a, vec3 b)
{
    vec3 tojesus(a.x - b.x, a.y - b.y, a.z - b.z);
    return tojesus;
}

vec3 operator+(vec3 a, vec3 b)
{
    vec3 tojesus(a.x + b.x, a.y + b.y, a.z + b.z);
    return tojesus;
}

vec3 operator*(vec3 a, double b)
{
    vec3 tojesus(a.x * b, a.y * b, a.z * b);
    return tojesus;
}

vec3 operator*(double b, vec3 a)
{
    vec3 tojesus(a.x * b, a.y * b, a.z * b);
    return tojesus;
}

vec3 operator/(vec3 a, double b)
{
    vec3 tojesus(a.x / b, a.y / b, a.z / b);
    return tojesus;
}

// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3;  // RGB color

struct player
{
    int Posx, Posy;
};

#endif

struct sphere
{
    point3 C;
    color Color;
    double r;
    int n, m;
    int textureID = 0;
};

int const MaxSpheres = 17;

color sun(vec3 Rae, vec3 N, color Color)
{
    color Ip;
    vec3 Lm = vec3(10, -5, 0).normalize();

    vec3 R = -Lm + 2 * N * Lm.dot(N);
    vec3 V = -Rae;

    color Intensity(1, 1, 1);
    Intensity = Intensity * 2;

    double shininess = 50;

    //https://en.wikipedia.org/wiki/Phong_reflection_model

    Ip += (Lm.dot(N) > 0 ? Lm.dot(N) : 0) * Color.hadamard(Intensity);
    return Ip;
}

color doTheRaeThing_Color(vec3 Rae, point3 Campus, sphere *S)
{

    point3 Lightbulb(6, 6, 6); //the color of the beast 666
    image Image;

    for (int i = 0; i < MaxSpheres; i++)
    {
        if (S[i].textureID == 0)
            Image = TEXTURE[0];
        if (S[i].textureID == 1)
            Image = TEXTURE[1];
        auto C = S[i].C;
        auto r = S[i].r;
        auto Color = S[i].Color;

        vec3 OmC = Campus - C;
        double delta = Rae.dot(OmC) * Rae.dot(OmC) - OmC.length_squared() + r * r;

        if (delta < 0)
        {
            vec3 result = (Rae + vec3(1, 1, 1)) / 2;
            continue;
        }

        else
        {
            double d = -(Rae.dot(OmC));
            double SRdelta = sqrt(delta);

            double d1, d2;
            d1 = d + SRdelta;
            d2 = d - SRdelta;

            d = (d1 > d2 ? d2 : d1);

            vec3 Hitpoint = Campus + d * Rae;
            vec3 N = (Hitpoint - C).normalize();

            double u, v;
            point3 spherePee = Hitpoint - C;
            // spherePee = -spherePee;

            /*double theta = acos(spherePee.y / S->r);
            spherePee.y = cos(theta + M_PI / 4) * S->r;*/

            assert(abs(spherePee.length() - r) < 0.0000001);

            double theta = S[i].m * M_PI / 12;
            double eta = S[i].n * M_PI / 12;

            // spherePee = spherePee.normalize();

            vec3 TEMP = spherePee;

            spherePee.x = cos(theta) * (TEMP.x) - sin(theta) * (TEMP.z);
            spherePee.z = sin(theta) * (TEMP.x) + cos(theta) * (TEMP.z);

            TEMP = spherePee;

            spherePee.x = cos(eta) * TEMP.x - sin(eta) * TEMP.y;
            spherePee.y = sin(eta) * TEMP.x + cos(eta) * TEMP.y;

            spherePee = spherePee.normalize();

            u = 0.5 + (atan2(spherePee.x, spherePee.z)) / (2 * M_PI);
            v = 0.5 - (asin(spherePee.y)) / (M_PI);

            int xi, yi;
            double ri, gi, bi;

            xi = u * Image.w;
            yi = v * Image.h;

            ri = Image.data[(xi + yi * Image.w) * 3];
            gi = Image.data[(xi + yi * Image.w) * 3 + 1];
            bi = Image.data[(xi + yi * Image.w) * 3 + 2];

            ri /= 255;
            gi /= 255;
            bi /= 255;

            ri /= 2;
            gi /= 2;
            bi /= 2;

            color TextureColor(ri, gi, bi);

            Color = TextureColor;

            color Ip;

            for (int i = 0; i < 1; i++)
            {
                vec3 Lm = (Lightbulb - Hitpoint).normalize();

                vec3 R = -Lm + 2 * N * Lm.dot(N);
                vec3 V = -Rae;

                color Intensity(1, 1, 1);
                Intensity = Intensity * 50 / (Lightbulb - Hitpoint).length_squared();

                double shininess = 50;

                //https://en.wikipedia.org/wiki/Phong_reflection_model

                Ip += (Lm.dot(N) > 0 ? Lm.dot(N) : 0) * Color.hadamard(Intensity) + pow((R.dot(V) > 0 ? R.dot(V) : 0), shininess) * Intensity;
            }

            Ip += sun(Rae, N, Color);
            Ip += 0.5 * Color;

            return Ip;
        }
    }
    vec3 result = (Rae + vec3(1, 1, 1)) / 2;
    return result;
}

void randoSpheres(sphere *S)
{
    for (int i = 0; i < MaxSpheres; i++)
    {
        int Cx = rand() % 30 - 15;
        int Cy = rand() % 15 - 7;
        int Cz = rand() % 100 - 28;

        int r = rand() % 5 + 1;

        int R = rand() % 100;
        int G = rand() % 100;
        int B = rand() % 100;

        S[i].C = vec3(Cx, Cy, Cz);
        S[i].Color = vec3(R / 100.0, G / 100.0, B / 100.0);
        S[i].r = r;
        S[i].m = rand() % 5;
        S[i].n = rand() % 5;
        S[i].textureID = rand() % 4;
    }
}

void writeToFile(int image_width, int image_height, vec3 *image)
{
    FILE *o = fopen("image.ppm", "w");
    fprintf(o, "P3\n%i %i\n255\n", image_width, image_height);
    for (int j = 0; j < image_height; ++j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            vec3 t = image[i + j * image_width];
            fprintf(o, "%i %i %i\n", (int)t.x, (int)t.y, (int)t.z);
        }
    }
}

struct renderTileStuff
{
    int startY, endY;
    int startX, endX;
    int MaxSamples;
    int image_width, image_height;
    double PixelWidth__whichisalsotheheightbytheway__;
    double PPB, focalDistance;
    int *count, *barcounter, MaxRays;
    vec3 Campus;
    vec3 *image;
    sphere *S;
    struct image Texture_1;
    struct image Texture_2;
};

renderTileStuff *tilesStuff; ///GLOBAL VARIABLE TODO: TODO: TODO:

DWORD WINAPI RenderTile(void *param)
{

    renderTileStuff *R = (renderTileStuff *)param;
    int startY = R->startY, endY = R->endY;
    int startX = R->startX, endX = R->endX;
    int MaxSamples = R->MaxSamples;
    int image_width = R->image_width, image_height = R->image_height;
    double PixelWidth__whichisalsotheheightbytheway__ = R->PixelWidth__whichisalsotheheightbytheway__;
    double PPB = R->PPB, focalDistance = R->focalDistance;
    int *count = R->count, *barcounter = R->barcounter, MaxRays = R->MaxRays;
    vec3 Campus = R->Campus;
    vec3 *image = R->image;
    sphere *S = R->S;

    // cout << " : " << startX << " " << startY << endl;

    for (int j = startY; j < endY; ++j)
    {
        for (int i = startX; i < endX; ++i)
        {
            color Color(0, 0, 0);
            // cout << i << "x" << j << endl;

            for (int sample = 0; sample < MaxSamples; ++sample)
            {
                double x = (double)i / (image_width - 1) * 2 - 1;

                double y = -((double)j / (image_height - 1) * 2 - 1);

                double PosX = x * 0.5 * (image_width - 1) / PPB;
                double PosY = y * 0.5 * (image_height - 1) / PPB;

                double offsetX = 0;
                double offsetY = 0;

                double stepsize = PixelWidth__whichisalsotheheightbytheway__ / sqrt(MaxSamples);

                offsetX = sample % (int)sqrt(MaxSamples) - sqrt(MaxSamples) + 0.5;
                offsetY = sample / (int)sqrt(MaxSamples) - sqrt(MaxSamples) + 0.5;

                PosX += offsetX * stepsize;
                PosY += offsetY * stepsize;

                vec3 offset;
                while (true)
                {
                    offset = vec3(random_double(-1, 1), random_double(-1, 1), 0);
                    if (offset.length_squared() >= 1)
                        continue;
                    break;
                }
                double aperture = 0.12;
                offset *= aperture;
                vec3 Steam = Campus + offset;

                vec3 Rae(-Steam.x + PosX, -Steam.y + PosY, -focalDistance);
                Rae = Rae.normalize();

                //if(S[i].textureID == 0)
                Color += doTheRaeThing_Color(Rae, Steam, S);
                //if(S[i].textureID == 1) Color += doTheRaeThing_Color(Rae, Steam, S, R->Texture_2);

                COUNT = InterlockedAdd(&COUNT, 1);
                BARCOUNTER = InterlockedAdd(&BARCOUNTER, 1);
            }
            Color /= MaxSamples;

            if (Color.x > 1)
                Color.x = 1;
            if (Color.y > 1)
                Color.y = 1;
            if (Color.z > 1)
                Color.z = 1;

            double ir = (255.999 * Color.x);
            double ig = (255.999 * Color.y);
            double ib = (255.999 * Color.z);

            //cout << PosX << ' ' << PosY << ' ' << endl;

            //std::cout << ir << ' ' << ig << ' ' << ib << '\n';

            image[i + j * image_width] = vec3(ir, ig, ib);
        }
    }

    R->count = count;
    R->barcounter = barcounter;
    R->image = image;

    return 0;
}

DWORD WINAPI ThreadLoop(void *param)
{
    while (PPFZT < MAXTILES)
    {
        int temp = InterlockedExchangeAdd(&PPFZT, 1);
        RenderTile(&tilesStuff[temp]);
        {
            if (temp >= MAXTILES)
            {
                printf("\r100%%");
            }
            else
                printf("\r%.0f%%", (double)temp / MAXTILES * 100);
        }
    }
    return 0;
}

void PollEvents(SDL_Event *Event, keys *Keys, rect *R)
{
    SDL_GetRelativeMouseState(&Keys->xrel, &Keys->yrel);
    SDL_GetMouseState(&Keys->x, &Keys->y);

    while (SDL_PollEvent(Event))
    {
        if (Event->type == SDL_KEYDOWN)
        {

            if (Event->key.keysym.sym == SDLK_TAB && Event->key.repeat == false)
            {
                Keys->Tab_Key = true;
            }
        }
        if (Event->type == SDL_QUIT)
        {
            Running = false;
        }
        if (Event->type == SDL_QUIT)
        {
            Running = false;
        }

        // Mouse Events:
        {
            if (Event->type == SDL_MOUSEBUTTONDOWN)
            {
                if (Event->button.button == SDL_BUTTON_RIGHT && Event->button.state == SDL_PRESSED)
                {
                    Keys->RightMouseButton = true;
                }
            }
            if (Event->type == SDL_MOUSEBUTTONUP && Event->button.button == SDL_BUTTON_RIGHT)
            {
                Keys->RightMouseButton = false;
            }
            if (Event->type == SDL_MOUSEBUTTONDOWN)
            {
                if (Event->button.button == SDL_BUTTON_LEFT && Event->button.state == SDL_PRESSED)
                {
                    Keys->LeftMouseButton = true;
                }
            }
            if (Event->type == SDL_MOUSEBUTTONUP && Event->button.button == SDL_BUTTON_LEFT)
            {
                Keys->LeftMouseButton = false;
            }

            if (Event->type == SDL_MOUSEWHEEL)
            {
                if (Event->wheel.y > 0) // scroll up
                {
                    Keys->ScrollY++;
                }
                if (Event->wheel.y < 0) // scroll down
                {
                    Keys->ScrollY--;
                }
            }
        }

        if (Event->type == SDL_KEYDOWN || Event->type == SDL_KEYUP)
        {
            bool KeyState = Event->type == SDL_KEYDOWN ? true : false;

            switch (Event->key.keysym.sym)
            {
            case SDLK_UP:
                Keys->UpButton = KeyState;
                break;
            case SDLK_DOWN:
                Keys->DownButton = KeyState;
                break;
            case SDLK_LEFT:
                Keys->LeftButton = KeyState;
                break;
            case SDLK_RIGHT:
                Keys->RightButton = KeyState;
                break;
            case SDLK_LSHIFT:
                Keys->Shift = KeyState;
                break;
            case SDLK_RSHIFT:
                Keys->Shift = KeyState;
                break;
            case SDLK_h:
                Keys->H_Key = KeyState;
                break;
            case SDLK_w:
                Keys->W_Key = KeyState;
                break;
            case SDLK_a:
                Keys->A_Key = KeyState;
                break;
            case SDLK_s:
                Keys->S_Key = KeyState;
                break;
            case SDLK_d:
                Keys->D_Key = KeyState;
                break;
            case SDLK_q:
                Running = false;
                break;
            case SDLK_SPACE:
                Keys->Space_Key = false;
                break;
            default:
                break;
            }
        }
    }
}


void GL_GetError(){
        int ErrorCode = glGetError();
        GLenum Error = glGetError();
        if (Error)
        {
            cout << "Encountered OpenGL error code: " << ErrorCode  << " " << Error<< endl;
        }
    }

#ifdef _WIN32
//multifredding:
HANDLE Thread()
{
    return CreateThread(0, 0, ThreadLoop, 0, 0, 0);
}

#else
//TODO: the linux thing
#endif