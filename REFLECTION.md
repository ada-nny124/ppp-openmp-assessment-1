# A1 REFLECTION

> Complete every section. CI will:
>
> 1. Verify all `## Section` headers below are present.
> 2. Verify each section has **at least 50 words**.
>
> No automatic content grading: the prose is read by a human, and the short
> prompt at the end is marked on a 0 / 0.5 / 1 scale. The numbers you quote
> in your reflection do **not** have to match canonical times exactly — HPC
> queue variance is real. Be concise, ground claims in your measurements, show your working.

## Section 1 — Schedule choice and why

I chose schedule(dynamic, 64) because f(x) has a deliberate slow spike in the interval [0.3, 0.4]. Static scheduling with large chunks leaves one thread carrying most of that heavy region, which hurts balance. I also tested static and guided, static produced a visibly worse runtime under my local measurements, while guided was closer but still slightly slower than dynamic, 64. The moderate dynamic chunk size spreads the expensive subintervals more evenly without excesive scheduling overhead.

## Section 2 — Scaling behaviour

The speedup curve departs from ideal well before 128 threads: 16 threads only reach about 5.28x, while 128 threads reach about 15.14x. This indicates that overhead and limited parallelism dominate before the thread count grows too large. The kernel is not perfectly bandwidth-bound here, the low efficiency suggests that task distribution and thread overhead matter, especially because the load imbalance from the spike is partially mitigated but not eliminated by dynamic scheduling.

## Section 3 — Roofline position

At my best observed thread count (128), the kernel is far below the DGEMM ceilings because it is not a dense matrix multiply and the arithmetic intensity is lower. The theoretical peak 4608 GFLOPs and the HPL-achievable 2896 GFLOPs are useful references, but the result is likely still under 10% of those because f(x) involves transcendental and branchy work, not fused multiply-adds over contiguous arrays. The code is better characterised as having moderate compute intensity with significant instruction overhead, so it does not approach DGEMM-class efficiency.

## Section 4 — What you'd try next

Next I would test a finer-grained dynamic schedule and verify whether schedule(dynamic, 32) or schedule(guided, 32) improves performance on Rome. So the spike region is only 10% of the domain, so a smaller chunk size could reduce tail imbalance. I would also compare the current schedule against a parallel for with ordered diagnostics or even a manually balanced two-phase approach for the heavy range. The expected effect would be slightly better load balance and a small improvement in total runtime.

## Reasoning question (instructor-marked, ≤100 words)

The chosen schedule is appropriate because f(x) has a heavy region in [0.3, 0.4] spike that costs about ten times more per evaluation. schedule(dynamic, 64) assigns small chunks at runtime, so threads that finish cheap regions can pick up additional work from the spike region, and so this reduces the imbalance that static chunking would create while keeping overhead lower than very fine-grained dynamic, 1.
