# --- Simple Pseudo-Random Number Generator ---
# Emulates a Linear Congruential Generator (LCG) and generates the next pseudo-random number 
# based on the formula x_(n+1) = (a * x_n + c) mod m
# with previous parameters:
    # x_n: current random number
    # a: multiplier
    # c: increment
    # m: modulus (here m = 32)
    
# Input
li t0, 1234

# Initialize parameters
li t1, 166452     # Multiplier
li t2, 12454813   # Increment

mul t0, t0, t1    # a*x
add t0, t0, t2    # +c

# no rem needed since 32-bit overflow is mod 2^32