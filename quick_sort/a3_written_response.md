# Assignment 3: Quick Sort Revisited

## Part 1
### Files
1. quickSort.cpp
  * My quickSort implementation that allows any index to be used.
2. driver.cpp
  * A couple of tests to check my solution produces the correct results. Test cases written with the help of Claud Opus 4.6.

### Reflection
This solution works by picking a random pivot point at the start and moving it to a[lo]. It then follows the standard quickSort implementation shown in the text book. Finally is swaps the pivot in the correct location by swapping it with a[j].

This works because after the initial swap a[lo] isn't moved again until the end. Once the array has been fully scanned it will be partitioned into two sections. Everything <= j is <= pivot (except the pivot which is still in a[lo]) and everything from j+1 to hi is >= pivot. since we know a[j] is <= pivot we can safely swap a[j] and a[lo], giving us a complete correct partitioning.

## Part 2

### please explain why it is k ln (N/k) + (n-k) ln (N/(N-k)) and solve the equation

The equation for the expected cost comes from two places.
1. The cost from pivots that are to the right of k
$$ k\ln\frac{N}{k} $$
1. The cost of pivots to the that are to the left of k
$$ (N-k)\ln{\frac{N}{N-k}} $$

This leaves us with the equation:  
$$ k\ln\frac{N}{k} + (N-k)\ln{\frac{N}{N-k}} $$
If we want to evaluate this function we can take its derivative with respect to k like so.
$$ f(k) = N\ln{N} - k\ln{k} - (N-k)\ln{(N-k)} $$
$$ \frac{d}{dk}[N\ln{N}] = 0 $$
$$ \frac{d}{dk}[k\ln{k}] = -\ln{(k)}-1 $$
$$ \frac{d}{dk}[-(N-k)\ln{(N-k)}] = \ln{(N-k)} + 1 $$
$$ f'(k) = \ln{\frac{N-k}{k}} $$
Now we can set the derivative to 0 and solve for k.
$$ \ln{\frac{N-k}{k}}=0 $$
$$ \frac{N-k}{k}=1 $$
$$ N-k=k $$
$$ k=\frac{N}{2} $$
After that we plug k back into f(k)
$$ f(\frac{N}{2}) = \frac{N}{2}\ln{(\frac{N}{N/2})+(N-\frac{N}{2})\ln{(\frac{N}{N-N/2})}} $$
Simplify
$$ f(\frac{N}{2}) = \frac{N}{2}\ln{2} + \frac{N}{2}\ln{2} $$
$$ f(\frac{N}{2}) = N\ln{2}\approx{0.693N} $$