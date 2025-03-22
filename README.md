# Testing with OpenMP


## Snapgram OpenMP
We initially had the parallel for collapse(2) clause only for the matrix multiplication since we saw that it was a nested loop. This yielded a 1.0 speed in comparison to the reference. 
We also decided to test without the collapse(2) clause, which actually yielded a faster speed compared to the reference. We believe that this is due to the way we are chunking our threads. If we divide the threads into too many separate portions, it may create too much overhead, and the thread may also be computing in a non cache optimal way.
This makes sense since in project 1, we found that it was most optimal to do chunking along the ‘i’ index.

## Heat Serial
Our implementation can be broken into 3 parts: Initializing the walls, initializing the interior parts, and performing the iterations.
For the initialization of the walls, the edges of both "_hmap" and a second map were initialized together and the vertical and horizontal lines were initialized together.
For the interior points, we incorporated an equation that would calculate the average of the temperatures of the edges given the parameters without completing the calculations manually, which would save time as the map size increases.
For the iterations, we decided to dynamically switch the pointers for the two heatmaps into a reference and a destination map. This eliminated the need to copy from an intermediary map back into "_hmap" each iteration, which saves time per iteration. 

## Heat OpenMP
We used the ‘for nowait’ clause before each of the for loop that initialized the heatmap because the calculations and initializing the heat map are all independent for each point. If we excluded the ‘nowait’ clause, our four for loops would’ve taken more time since it would wait for all of the threads to complete its calculations before releasing the next set, even when those implicit barriers weren't needed.
Also for calculating each iteration/step of the heatmap, we took advantage of the reduction clause given that we are multithreading with a shared variable "max_ep". Having this reduction clause helped us prevent race conditions by letting "max_ep" become a thread local variable and allowed for the OpenMP optimizations to optimize the combining of the different versions of max_ep.
We also decided not to implement the collapse(2) for this nested for loop since it may create extra overhead splitting the nested for loops into i*j parallelable sections, similar to the OpenMP implementation of the Snapgram program.

** N = 512, fire_temp = 3000, wall_temp = 70, fire_loc = 0.3, epsilon = 0.01**
