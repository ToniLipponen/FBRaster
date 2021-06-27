# FBRaster

A very barebones CPU rasterizer for drawing on linux /dev/fb. This is a hobby project, with the goal of implementing some kind of graphics pipeline outside of Xorg.

![screenshot](https://user-images.githubusercontent.com/86045205/123545505-03c56600-d761-11eb-99ad-a2efe9dd3078.png)

<h2>Features:</h2>
  <list>
    <li>Simple programmable vertex and fragment shaders.</li>
    <li>Different rendermodes for drawing points, lines and triangles.</li>
    <li>Simple vector and matrix types <t color=red>(not fully finished)</t>.</li>
  </list>
  
<h2>Building:</h2>
Run "make" in the project directory to build libFBRaster.a, or "make example" to build the library and a test program.

<h2>Using:</h2>
Running sudo ./example in a TTY session. You have to use sudo because the library needs access to the /dev/fb0 device. 
