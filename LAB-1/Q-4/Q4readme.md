# DAA Lab 01 - Question 4

## Objective

Implement the Tower of Hanoi algorithm in C and analyze the number of moves required for different numbers of disks.

## Approach

- Solve the problem recursively.
- Count the total number of moves.
- Record the moves for different values of n.
- Plot the graph using the generated CSV file.

## Formula

Moves = 2ⁿ − 1

## Time Complexity

O(2ⁿ)

## Space Complexity

O(n)

## Conclusion

The graph shows exponential growth in the number of moves. As the number of disks increases, the execution time increases rapidly, making the algorithm impractical for large values of n. Also it verifies the formula that moves =  2ⁿ − 1.