@echo off
cl  main.cpp /O2 /EHsc /Zi /I include /link /debug /subsystem:console /LIBPATH:lib SDL2.lib SDL2main.lib shell32.lib SDL2_ttf.lib opengl32.lib freetype.lib
main.exe
