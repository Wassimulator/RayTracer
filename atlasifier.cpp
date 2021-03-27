
/*
A simple Font atlas generator using FreeType and SDL Surfaces

past these within int main()
{
    FT_Face face;
	FT_Library ft;
	FT_Init_FreeType(&ft);
	FT_New_Face(ft, "arial.ttf", 0, &face);
	bool useKerning = FT_HAS_KERNING(face);
    FT_Set_Char_Size(face, 0, 30 << 6, 96, 96);
}

//a simple example to render a text:

int p = 0;
for (int t = 0; t < text.size(); t++)
{
	int i = FT_Get_Char_Index(face, text[t]);
	i += 29; //for some fucking reason.

	SDL_Rect rectS;
	rectS.h = GlyphInfo[i].height;
	rectS.w = GlyphInfo[i].width;
	rectS.x = GlyphInfo[i].x0;
	rectS.y = GlyphInfo[i].y0;

	SDL_Rect rectD;
	rectD.h = GlyphInfo[i].height;
	rectD.w = GlyphInfo[i].width;
	rectD.x = penX + GlyphInfo[i].BearingX;
	rectD.y = 30 - GlyphInfo[i].BearingY;

	penX += GlyphInfo[i].Advance;
	if (t > 0 && useKerning)
	{
		FT_Vector delta;
		FT_Get_Kerning(face, p, i, FT_KERNING_DEFAULT, &delta);
		penX += delta.x >> 6;
	}
	SDL_RenderCopy(Renderer, Texture, &rectS, &rectD);
	p = i;
}

*/

#pragma once
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

#include <ft2build.h>
#include <iostream>
#include <string>
#include <SDL.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#define NUM_GLYPHS 128

using namespace std;

struct glyphInfo
{
    int x0, y0, x1, y1;
    int x_off, y_off;
    int advance;
    char name[10];
    int width;
    int height;
    int BearingX;
    int BearingY;
    int Advance;
} GlyphInfo[NUM_GLYPHS];

struct character
{
    unsigned int TextureID; // ID handle of the glyph texture
    unsigned int width;
    unsigned int height; // Size of glyph
    FT_Int BearingX;
    FT_Int BearingY; // Offset from baseline to left/top of glyph
    FT_Pos Advance;  // Offset to advance to next glyph
    unsigned char c;
};

int GetRenderTextLength(string text, int Size, FT_Face face, bool useKerning, character *C)
{
    FT_Set_Pixel_Sizes(face, 0, Size);
    FT_Set_Char_Size(face, 0, Size << 6, 96, 96);

    int RenderLength = 0;
    int p = 0;
    for (int t = 0; t < text.size(); t++)
    {
        int i = FT_Get_Char_Index(face, text[t]);
        i += 29;
        if (t > 0 && useKerning)
        {
            FT_Vector delta;
            FT_Get_Kerning(face, p, i, FT_KERNING_DEFAULT, &delta);
            RenderLength += delta.x >> 6;
        }
        RenderLength += C[i].Advance >> 6;
        p = i;
    }
    return RenderLength;
}

SDL_Surface *GenerateFontAlphaSurface(char *FontFile, int Size)
{
    FT_Face face;
    FT_Library ft;
    FT_Init_FreeType(&ft);
    FT_New_Face(ft, FontFile, 0, &face);
    FT_Set_Char_Size(face, 0, Size << 6, 96, 96);

    int max_dim = (1 + (face->size->metrics.height >> 6)) * ceilf(sqrtf(NUM_GLYPHS));
    int tex_width = 1;
    while (tex_width < max_dim)
        tex_width <<= 1;
    int tex_height = tex_width;

    char *PixelsMono = (char *)calloc(tex_width * tex_height, 1);
    int pen_x = 0, pen_y = 0;

    FT_Select_Charmap(face, ft_encoding_unicode);

    for (int i = 0; i < NUM_GLYPHS; ++i)
    {
        FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
        FT_Bitmap *bmp = &face->glyph->bitmap;

        if (pen_x + bmp->width >= tex_width)
        {
            pen_x = 0;
            pen_y += ((face->size->metrics.height >> 6) + 1);
        }

        for (int row = 0; row < bmp->rows; ++row)
        {
            for (int col = 0; col < bmp->width; ++col)
            {
                int x = pen_x + col;
                int y = pen_y + row;
                PixelsMono[y * tex_width + x] = bmp->buffer[row * bmp->pitch + col];
            }
        }

        GlyphInfo[i].x0 = pen_x;
        GlyphInfo[i].y0 = pen_y;
        GlyphInfo[i].x1 = pen_x + bmp->width;
        GlyphInfo[i].y1 = pen_y + bmp->rows;

        GlyphInfo[i].x_off = face->glyph->bitmap_left;
        GlyphInfo[i].y_off = face->glyph->bitmap_top;
        GlyphInfo[i].advance = face->glyph->advance.x >> 6;

        GlyphInfo[i].width = face->glyph->metrics.width;
        GlyphInfo[i].height = face->glyph->metrics.height;
        GlyphInfo[i].BearingX = face->glyph->metrics.horiBearingX;
        GlyphInfo[i].BearingY = face->glyph->metrics.horiBearingY;
        GlyphInfo[i].Advance = face->glyph->metrics.horiAdvance;

        GlyphInfo[i].width >>= 6;
        GlyphInfo[i].height >>= 6;
        GlyphInfo[i].BearingX >>= 6;
        GlyphInfo[i].BearingY >>= 6;
        GlyphInfo[i].Advance >>= 6;

        pen_x += bmp->width + 1;
    }

    char *PixelsRGBA = (char *)calloc(tex_width * tex_height * 4 + 1, 1);
    for (int i = 0; i < (tex_width * tex_height); ++i)
    {
        PixelsRGBA[i * 4 + 0] |= PixelsMono[i];
        PixelsRGBA[i * 4 + 1] |= PixelsMono[i];
        PixelsRGBA[i * 4 + 2] |= PixelsMono[i];
        PixelsRGBA[i * 4 + 3] = 0xff;

        if (PixelsRGBA[i * 4] == 0)
            PixelsRGBA[i * 4 + 3] = 0;
    }

    SDL_Surface *Surface = SDL_CreateRGBSurfaceWithFormatFrom(PixelsRGBA, tex_width, tex_height, 0, tex_width * 4, SDL_PIXELFORMAT_RGBA32);

    return Surface;
}