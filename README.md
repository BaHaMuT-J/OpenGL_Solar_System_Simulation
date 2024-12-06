# Solar_System_Simulation

## Summary

I use openGL to draw and simulate basic elements in solar system

## File Description
- shader_m.h : for vertex and fragment shader
- stb_image.h : for loading image
- camera.h : for camera class
- solar.h : main header, for rendering and drawing
- h.cpp : for GLAD
- Render.cpp : contains constants, variables and implement functions in solar.h
- Sphere.cpp : contains function for creating Sphere
- solarScaledSize.cpp : draws solar system with scaled size, draws each star using true radius scale giving a true view for comparing each planet by their size
- solarScaledDistance : draws solar system with scaled distance, positions each planet with almost true scale distance from the sun, after Mars the distance will be halved so that they are not too far from Sun
- SolarSystem.cpp : main function, can switch which version do you want to see

## How to run

- compile and build solution in visual studio

## Resources

https://learnopengl.com/Introduction

https://www.songho.ca/opengl/gl_sphere.html

https://planetpixelemporium.com/sun.html