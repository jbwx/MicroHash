# MicroHash

[jbwx](https://jbwx.github.io)

Hash tables are convenient in allowing lookup of data in a fixed amount of time. However, this assumes the input is a fixed size. What if the input size varies? The time taken by any hash function is dependent on the size of the input.

How, then, can we guarantee that the hash will take a constant amount of time? That’s what I aimed for with this project.

# Process

To ensure the hash is generated in O(1) time, only a fixed amount of samples can be taken. In this case, 8 bytes are selected from the file.

How are the sample indexes decided? The input is divided into 8 segments, and the sample is taken from a pseudo-random spot in each segment.

Next, each sample is xored with a mask value hard-coded into the function. These masks are 7-digit primes. The result is then added to a sum value.

Once done, the sum is multiplied by a large prime to magnify small changes. Each sample is xored with this sum, ensuring a “butterfly effect” in the hash output.

We see the constant-time nature of the function reflected in time trials. Here’s a comparison of time taken to hash a .txt file containing the character “a”, and a hash of the function’s own source code.

```
MicroHash jbwx$ time ./microhash a.txt
506d37fbc57947d5
real    0m0.007s
user    0m0.002s
sys     0m0.003s

MicroHash jbwx$ time ./microhash src.c
7a7342a4c02100c3
real    0m0.007s
user    0m0.002s
sys     0m0.003s
```

Additionally, here’s the time taken to produce a hash for the [entire Bee Movie Script](https://gist.githubusercontent.com/AnthonyNixon/6ae63abd1834485811200daefc319b40/raw/2411e31293a35f3e565f61e7490a806d4720ea7e/bee%2520movie%2520script):

```
MicroHash jbwx$ time ./microhash beemoviescript.txt
b1a5b26758b1c958
real    0m0.007s
user    0m0.003s
sys     0m0.002s
```

The time-complexity and relatively small filesize would make this ideal for embedded systems, or other applications with limited time and resources.

This being said, it does have some shortcomings. For one, because the file is sampled in only 8 places, finding collisions would be relatively easy. This (or any constant-time hash function) should not be used in any place where security is critical.

Additionally, the function is not proven in any way to be mathematically sound. There is no guarantee that all outputs are equally likely, or that every output is possible. The output appears to be random from the limited tests I’ve done, although it’s unlikely there isn’t some sort of underlying pattern.

Lastly, some indexes will get sampled multiple times with inputs larger than 8 bytes. This is a bug I hope to iron out in future versions.
