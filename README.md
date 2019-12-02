# Ray Tracer

This Ray Tracer is a port of the the ray tracer I did at school
It has been re written in C++ from its code written in C
<br>
I rewrote it in C++ because I plan to add some more advanced features than the C version has

# Current Features

It's able to trace 4 different shapes:
- plane
- sphere
- cylinder
- cone

2 types of light sources
- Direct light source
- Point light source

Effects
- Dithering 
- Sepia
- Greyscale

Reflections with n dimensions
<br>Anti Aliasing
<br>Multi Threaded Tracing

<b>Settings file to make changes without needing to recompile the program</b>

# To Run
Make sure you have GCC, and Make installed and configured on your machine

<b>make all</b>
<br>
`./RT <map file>`

# Preview
<b>This map file will produce the image below
<br>[MAP FILE](maps/pyro.map)

<br>![](images/RT.png)
<br>![](images/RT_reflect.png)
<br>![](images/gab.png)
<br>![](images/pretty_ball_on_a_plane.png)



# Goldilox_API_Layer

This repo is specifically for testing out API concepts for a peer-to-peer API network.

The focus is on figuring out the differences in terms of performance and network structure between
high performance apps (a raytracer) and low to medium performance apps (integrating a Java application with a
Python application across a network).

### This design is probably trash so use anything here at your own rist


Distributed Raytracer by @haegan (George Sferos)<br>
Java/Python intergration by @icequeen88 (Karini Govender)
