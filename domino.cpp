#include "domino.h"

/**
 * @brief Constructs a domino with specified side values.
 * @param s1 Value of the first side.
 * @param s2 Value of the second side.
 */
Domino::Domino(int s1, int s2) : side1(s1), side2(s2), used(false) {}
