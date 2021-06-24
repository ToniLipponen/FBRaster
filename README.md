# FBRaster

A very barebones CPU rasterizer for drawing on linux /dev/fb. This is a hobby project, with the goal of implementing some kind of graphics pipeline outside of Xorg.

<h2>Features:</h2>
  <list>
    <li>Simple programmable vertex and fragment shaders.</li>
    <li>Different rendermodes for drawing points, lines and triangles.</li>
    <li>Simple vector and matrix types <t color=red>(not fully finished)</t>.</li>
  </list>
  
<h2>Building:</h2>
Run "make" in the project directory to build libFBRaster.a, or "make test" to build the library and a test program.

<h2>Using:</h2>
Running sudo ./test in a TTY session. You have to use sudo because the library needs access to the /dev/fb0 device. 
