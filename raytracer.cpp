#pragma once
#include "source.cpp"

void RAYTRACER()
// int main(int argc, char **argv)
{
    TEXTURE[0] = {};
    TEXTURE[0].data = stbi_load("face1.jpg", &TEXTURE[0].w, &TEXTURE[0].h, &TEXTURE[0].n, 3);
    TEXTURE[1] = {};
    TEXTURE[1].data = stbi_load("face2.jpg", &TEXTURE[1].w, &TEXTURE[1].h, &TEXTURE[1].n, 3);

    MAXTHREADS = GetMaximumProcessorCount(ALL_PROCESSOR_GROUPS);
    // MAXTHREADS = 4;

    srand(1111111);

    cout << "TESSSST" << endl;

    sphere S[MaxSpheres];
    randoSpheres(S);

    player P = {};
    const int image_width = 1280;
    const int image_height = 720;

    vec3 something;

    // Render

    FILE *f = fopen("log.txt", "a");

    //std::cout << "P3\n"<< image_width << ' ' << image_height << "\n255\n";

    point3 Campus(0, 0, 25);
    double PPB = 512 / 12; // pixels per bigsle
    double Diagonal = sqrt(image_width * image_width + image_height * image_height);
    Diagonal /= PPB;
    double FOV = M_PI * 0.5;
    double focalDistance = Diagonal / (2 * tan(FOV / 2));
    cout << "f: " << focalDistance << endl;
    cout << "FOV: " << FOV * 180 / M_PI << endl;
    cout << "Camera Z: " << Campus.z << endl;
    double PixelWidth__whichisalsotheheightbytheway__ = 1 / PPB;

    int power = 6;
    int MaxSamples = 1 << power;

    int count = 0;
    int barcounter = 0;
    int MaxRays = image_height * image_width * MaxSamples;
    cout << endl;

    clock_t start = clock();

    int TileSize = 64;

    int TilesPerWidth = (image_width + TileSize - 1) / TileSize;
    int TilesPerHeight = (image_height + TileSize - 1) / TileSize;
    MAXTILES = TilesPerWidth * TilesPerHeight;

    vec3 *image = (vec3 *)malloc(image_width * image_height * sizeof(vec3));

    HANDLE *threads = (HANDLE *)malloc((MAXTHREADS - 1) * sizeof(HANDLE));

    tilesStuff = (renderTileStuff *)malloc(MAXTILES * sizeof(renderTileStuff));

    for (int t = 0; t < MAXTILES; ++t)
    {
        renderTileStuff *r = &tilesStuff[t];

        int startX = (t % TilesPerWidth) * TileSize;
        int startY = (t / TilesPerWidth) * TileSize;

        int endX = startX + TileSize;
        int endY = startY + TileSize;
        // cout << t << " : " << endX << " " << endY << endl;

        if (endX > image_width)
            endX = image_width;
        if (endY > image_height)
            endY = image_height;

        r->startY = startY;
        r->endY = endY;
        r->startX = startX;
        r->endX = endX;
        r->MaxSamples = MaxSamples;
        r->image_width = image_width;
        r->image_height = image_height;
        r->PixelWidth__whichisalsotheheightbytheway__ = PixelWidth__whichisalsotheheightbytheway__;
        r->PPB = PPB;
        r->focalDistance = focalDistance;
        r->count = &count;
        r->barcounter = &barcounter;
        r->MaxRays = MaxRays;
        r->Campus = Campus;
        r->image = image;
        r->S = S;
        //threads[t] = Thread(r);
    }

    for (int i = 0; i < MAXTHREADS - 1; i++)
    {
        threads[i] = Thread();
    }

    cout << "\nRendering ...\n";
    ThreadLoop(0);

    for (int t = 0; t < MAXTHREADS - 1; ++t)
    {
        WaitForSingleObjectEx(threads[t], INFINITE, FALSE);
    }

    clock_t end = clock();
    clock_t timediff = end - start;

    double result = (double)timediff * 1000000000 / CLOCKS_PER_SEC;

    cout << "\nDone.\n";
    cout << "Render time: " << (double)result / 1000 << " seconds\n\n";

    time_t rawtime = time(0);
    struct tm *timeinfo = localtime(&rawtime);

    cout << "f: " << focalDistance << endl;
    cout << "FOV: " << FOV * 180 / M_PI << endl;
    cout << "Camera Z: " << Campus.z << endl;
    cout << "writing to file...";
    writeToFile(image_width, image_height, image);
    cout << "done\n";

    char *temp = asctime(timeinfo);
    temp[strlen(temp) - 1] = 0;

    fprintf(f, "%s | %0.2f sec | %0.1f ns/ray | %ix%i | %i samples/pixel | %i total rays\n", temp, result / 1000000000,
            result / MaxRays, image_width, image_height, MaxSamples, MaxRays);

    BARCOUNTER = 0;
    BARCOUNTER2 = 0;
    COUNT = 0;
    PPFZT = 0;
    RUNRAYTRACER = 0;
}