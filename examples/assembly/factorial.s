# --- Factorial Calculator ---
# Calculate the factorial of input n > 0 in t0, denoted by n! = 1 * 2 * ... * n-1 * n
# n is treated as an unsigned integer.
# The result is written as an unsigned integer to t0, if an overflow occurs, 0 is returned instead


# Input
li t0, 13

# Initialize additional registers
li t1, 1                        # Current result
li t2, 0                        # Previous result
li t3, 2                        # Next factor

loop:
    addi t0, t0, -1             # Decrement counter
    beq t0, zero, noOverflow    # If counter == 0, finish
    mv t2, t1                   # Copy old current result to previous result
    mul t1, t2, t3              # Multiply previous result with next factor 
    blt t1, t2, overflow        # If previous result > current result -> overflow
    addi t3, t3, 1              # Prepare next factor
    j loop                      # Jump to loop
    
noOverflow:
    mv t0, t1                   # Copy result from t1 to t0
    j end                       # Jumo to end

overflow:
    li t0, 0                    # Load 0 into t0
      
end:
    nop                         # Use end as an anchor, do nothing