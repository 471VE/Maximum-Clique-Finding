# Branch-and-Bound Method for Maximum CLique Problem

This program finds maximum cliques in undirected graphs in <b>DIMACS</b> format using <i>Branch-and-Bound</i> algorithm. This exact algorithm includes a part of calculating an approximate initial heuristic, which in my case consists of <i>Greedy Randomized Adaptive Search Procedure</i> (GRASP), <i>Variable Neighborhood Search</i> (VNS), and <i>Tabu Search</i> (TS). 

## Instructions

Place the graphs you want to know maximum clique of in ```graphs``` folder. Compile the program with <b>-O3</b> optimization level in Release mode for fastest computation times. To run the program, specify the path to the executable.

## Results

The program was executed on a rather old and slow machine with Intel Core i5-3230M CPU and 6 GB of DDR3 RAM.

|File              |Heuristic Time (sec)|BnB Time (sec)|Heuristic clique size|Clique size|
|------------------|--------------------|--------------|---------------------|-----------|
|brock200_1.clq    |0.988               |26.934        |21                   |21         |
|brock200_2.clq    |0.604               |0.08          |12                   |12         |
|brock200_3.clq    |0.727               |0.673         |15                   |15         |
|brock200_4.clq    |0.902               |1.799         |17                   |17         |
|C125.9.clq        |0.898               |2.757         |34                   |34         |
|gen200_p0.9_44.clq|1.879               |32.07         |44                   |44         |
|gen200_p0.9_55.clq|2.009               |0.953         |55                   |55         |
|hamming8-4.clq    |0.981               |0.839         |16                   |16         |
|johnson16-2-4.clq |0.346               |2.476         |8                    |8          |
|johnson8-2-4.clq  |0.039               |0             |4                    |4          |
|keller4.clq       |0.551               |0.402         |11                   |11         |
|MANN_a27.clq      |23.819              |126.954       |124                  |126        |
|MANN_a9.clq       |0.231               |0.001         |16                   |16         |
|p_hat1000-1.clq   |4.339               |32.299        |10                   |10         |
|p_hat1500-1.clq   |7.453               |541.974       |12                   |12         |
|p_hat300-3.clq    |2.963               |81.728        |36                   |36         |
|san1000.clq       |6.322               |1072.28       |10                   |15         |
|sanr200_0.9.clq   |2.011               |2101.03       |42                   |42         |


As you can see, Branch-and-Bound algorithm gives exact solutions in quite reasonable times, and our initial heuristic in most cases is pretty fast and matches the exact solution.