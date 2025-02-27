# Manderblot
Simple C/C++ raylib implementation of Mandelbrot set.

## Controls
 * toggle button for render of orbit iterations
 * slider for number of iteration
 * mouse movement on complex plane and zoom
 
## Build
```
mkdir build
cmake .. && make
```

## ToDo
 * Zoom just crashes since float is used in the shader and no optimization was done to address that.
 * Speed optimization in regards to points that are stable.