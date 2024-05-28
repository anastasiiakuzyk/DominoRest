#pragma once

/**
 * @file domino.h
 * @brief Declaration of the Domino structure and its constructors.
 */

/**
 * @struct Domino
 * @brief Represents a single domino piece with two sides.
 *
 * A domino is a small tile that represents the roll of two dice with its two numbers, and it can be marked as used in certain algorithms.
 */
struct Domino {
    int side1, side2; ///< The values on the two sides of the domino.
    bool used = false; ///< Flag to indicate if the domino has been used.

    /**
     * @brief Constructs a domino with specified side values.
     * @param s1 Value of the first side.
     * @param s2 Value of the second side.
     */
    Domino(int s1, int s2);
};
