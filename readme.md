# Branch-and-Bound Method for Maximum CLique Problem

This program finds maximum cliques in undirected graphs in <b>DIMACS</b> format using <i>Branch-and-Bound</i> algorithm. This exact algorithm includes a part of calculating an approximate initial heuristic, which in my case consists of <i>Greedy Randomized Adaptive Search Procedure</i> (GRASP), <i>Variable Neighborhood Search</i> (VNS), and <i>Tabu Search</i> (TS). 

## Instructions

Place the graphs you want to know maximum clique of in ```graphs``` folder. Compile the program with <b>-O3</b> optimization level in Release mode for fastest computation times. To run the program, specify the path to the executable.

## Results

The program was executed on a rather old and slow machine with Intel Core i5-3230M CPU and 6 GB of DDR3 RAM.

***place for table***

As you can see, Branch-and-Bound algorithm gives exact solutions in quite reasonable times, and our initial heuristic in most cases is pretty fast and matches the exact solution.