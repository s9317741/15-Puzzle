# 15-Puzzle Solver

This project implements a solver for the 15-puzzle using the Iterative Deepening A* (IDA*) algorithm. The 15-puzzle is a sliding puzzle that consists of a 4x4 grid of numbered tiles with one tile missing. The objective of the puzzle is to move the tiles until they are in numerical order.

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [Usage](#usage)
5. [Algorithm Details](#algorithm-details)
6. [Contributing](#contributing)
7. [License](#license)

## Introduction

The 15-puzzle (also called Gem Puzzle, Boss Puzzle, or Game of Fifteen) is a classical problem for modeling various algorithms in artificial intelligence. This solver uses the IDA* algorithm, which is a memory-efficient version of the A* search algorithm, suitable for solving puzzles with large state spaces.

## Features

- Solves any valid 15-puzzle configuration
- Utilizes the efficient IDA* algorithm
- Provides step-by-step solution output
- Command-line interface for easy use

## Installation

To install this solver, clone the repository and install the required dependencies.


## Algorithm Details
### IDA* Algorithm

The IDA* algorithm combines the depth-first search's space efficiency and the A* algorithm's optimality. It iteratively deepens the search with increasing cost limits until the solution is found. This project uses the Manhattan distance heuristic for estimating the cost to reach the goal state.

1. Initial State: Start with the initial puzzle configuration.
2. Cost Limit: Set the initial cost limit to the heuristic cost of the initial state.
3. Depth-First Search: Perform a depth-first search up to the current cost limit.
4. Update Cost Limit: If the solution is not found, increase the cost limit and repeat the search.
5. Solution Found: The algorithm terminates when the goal state is reached.
