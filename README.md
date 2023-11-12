C++ ray tracer implementation, parallelized with the thread library. The code is partly inspired from https://raytracing.github.io/ 



Diffuse materials, 1200 * 800 pixels, 200 rays per pixel:

![alt text](https://github.com/BenoitMorel/RayTracer/blob/main/results/diffuse_balls.png)

Diffuse and reflecting materials, 2000 * 1300 pixels, 100 rays per pixel:

![alt text](https://github.com/BenoitMorel/RayTracer/blob/main/results/diffuse_and_reflect.png)

More spheres (3000) thanks to BVH structure, 1200 * 800 pixels, 100 rays per pixel:

![alt text](https://github.com/BenoitMorel/RayTracer/blob/main/results/bvh.png)

Rectangular mirror reflecting 300 spheres, 2000 * 1300 pixels, 100 rays per pixel:

![alt text](https://github.com/BenoitMorel/RayTracer/blob/main/results/quad_mirror.png)

Adding quads, parallelepipeds, and fuzzed reflection. Adding light sources. 1200 * 1200 pixels, 500 rays per pixel:

![alt text](https://github.com/BenoitMorel/RayTracer/blob/main/results/traffic_lights.png)


