# Packing problems

## Problem 1
In a box(2D,3D) bounded by [-1, 1], given m balloon with variable radio r and 
position mu, find the optimal value of r and mu which maximize "sum r^2".

### Instruction
1. Packing-1 and Packing-2 are similar algorithms to solve the 
   question above. In Packing-1.cpp we search each circle with 
   decreasingly radius, and vice versus, in Packing-2.cpp we 
   search each circle with increasingly radius.

2. Since Packing-1 v0.0.2 we use Gradient Descent to search each 
   circle.

3. Theoretically we have sovled the question in Packing-1 v0.0.1,
   but actually it is difficult to run because it takes too much 
   time to search a circle meeting our conditions.

## Problem 2
In a box(2D, 3D) bounded by [-1, 1], given m balloons(they cannot overlap) 
with variable radio r and position mu. And some tiny blocks are in the box 
at given position {d};balloons cannot overlap with these blocks. find the 
optimal value of r and mu which maximize "sum r^2".

### Instruction
1. PackingWithBlocks.cpp solves the problem above.