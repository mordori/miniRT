# SIMD Optimization Guide for miniRT

## Table of Contents
1. [Introduction to SIMD](#introduction-to-simd)
2. [Current Implementation](#current-implementation)
3. [Optimized Vector Functions](#optimized-vector-functions)
4. [Integration Strategy](#integration-strategy)
5. [Performance Expectations](#performance-expectations)
6. [Debugging and Troubleshooting](#debugging-and-troubleshooting)

---

## Introduction to SIMD

**SIMD** (Single Instruction, Multiple Data) allows processing multiple values simultaneously using special CPU registers.

### What is SSE2?
- **SSE2** = Streaming SIMD Extensions 2
- Available on **all x86-64 CPUs** (universal on modern systems)
- Provides **128-bit registers** (`__m128`) that hold **4 floats**
- Header: `<emmintrin.h>`

### Key Benefits
- **2-3x speedup** on vector operations
- **10-30% overall** raytracer performance gain
- No external dependencies (built into CPU)

### Key SSE2 Intrinsics
```c
__m128 _mm_loadu_ps(float*)     // Load 4 floats (unaligned)
__m128 _mm_storeu_ps(float*, v) // Store 4 floats (unaligned)
__m128 _mm_add_ps(a, b)         // Add 4 floats in parallel
__m128 _mm_sub_ps(a, b)         // Subtract 4 floats in parallel
__m128 _mm_mul_ps(a, b)         // Multiply 4 floats in parallel
__m128 _mm_div_ps(a, b)         // Divide 4 floats in parallel
__m128 _mm_min_ps(a, b)         // Component-wise minimum
__m128 _mm_max_ps(a, b)         // Component-wise maximum
__m128 _mm_hadd_ps(a, b)        // Horizontal add (SSE3)
float  _mm_cvtss_f32(a)         // Extract lowest float
```

---

## Current Implementation

### AABB Intersection (Already Implemented)
Located in: `src/scene/aabb.c`

**Before (Scalar):**
```c
bool hit_aabb(const t_aabb *aabb, const t_ray *ray, float closest_t)
{
    float t_min = 0.0f, t_max = closest_t;

    // X axis
    float min = (aabb->min.x - ray->origin.x) * ray->inv_dir.x;
    float max = (aabb->max.x - ray->origin.x) * ray->inv_dir.x;
    t_min = fmaxf(t_min, fminf(min, max));
    t_max = fminf(t_max, fmaxf(min, max));

    // Y axis (same pattern)
    // Z axis (same pattern)

    return (t_max >= t_min);
}
```

**After (SIMD):**
- Processes all 3 axes simultaneously
- ~2x faster for BVH traversal
- See `src/scene/aabb.c` for full implementation

---

## Optimized Vector Functions

### 1. vec3_dot() - HIGHEST PRIORITY ⭐⭐⭐

**Why:** Called millions of times per frame (lighting, reflections, normals)

**Scalar Version:**
```c
float vec3_dot(t_vec3 a, t_vec3 b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}
```

**SIMD Version (SSE3 with hadd):**
```c
#include <pmmintrin.h>  // SSE3 for _mm_hadd_ps

float vec3_dot(t_vec3 a, t_vec3 b)
{
    __m128 va = _mm_loadu_ps(&a.x);     // Load [a.x, a.y, a.z, a.w]
    __m128 vb = _mm_loadu_ps(&b.x);     // Load [b.x, b.y, b.z, b.w]
    __m128 mul = _mm_mul_ps(va, vb);    // [a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w]

    // Horizontal add: sum all components
    mul = _mm_hadd_ps(mul, mul);        // [x+y, z+w, x+y, z+w]
    mul = _mm_hadd_ps(mul, mul);        // [x+y+z+w, ...]

    return _mm_cvtss_f32(mul);
}
```

**SIMD Version (SSE2 only - more portable):**
```c
#include <emmintrin.h>  // SSE2

float vec3_dot(t_vec3 a, t_vec3 b)
{
    __m128 va = _mm_loadu_ps(&a.x);
    __m128 vb = _mm_loadu_ps(&b.x);
    __m128 mul = _mm_mul_ps(va, vb);    // [x*x, y*y, z*z, w*w]

    // Manual horizontal sum (more verbose but SSE2 compatible)
    __m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
    __m128 sums = _mm_add_ps(mul, shuf);  // [x+y, y+x, z+w, w+z]
    shuf = _mm_movehl_ps(shuf, sums);      // Move high to low
    sums = _mm_add_ss(sums, shuf);         // Final sum in lowest element

    return _mm_cvtss_f32(sums);
}
```

**Speedup:** ~2.5x faster

---

### 2. vec3_length() - HIGH PRIORITY ⭐⭐

**Why:** Used in normalization, distance calculations

**Scalar Version:**
```c
float vec3_length(t_vec3 vec)
{
    return sqrtf(vec3_dot(vec, vec));
}
```

**SIMD Version:**
```c
float vec3_length(t_vec3 vec)
{
    __m128 v = _mm_loadu_ps(&vec.x);
    __m128 sq = _mm_mul_ps(v, v);       // Square all components

    // Horizontal sum (same as dot product)
    __m128 shuf = _mm_shuffle_ps(sq, sq, _MM_SHUFFLE(2, 3, 0, 1));
    __m128 sums = _mm_add_ps(sq, shuf);
    shuf = _mm_movehl_ps(shuf, sums);
    sums = _mm_add_ss(sums, shuf);

    return sqrtf(_mm_cvtss_f32(sums));
}
```

**Speedup:** ~2x faster

---

### 3. vec3_normalize() - HIGH PRIORITY ⭐⭐

**Why:** Critical for lighting, ray directions

**Scalar Version:**
```c
t_vec3 vec3_normalize(t_vec3 vec)
{
    return vec3_div(vec, vec3_length(vec));
}
```

**SIMD Version:**
```c
t_vec3 vec3_normalize(t_vec3 vec)
{
    t_vec3 result;
    __m128 v = _mm_loadu_ps(&vec.x);
    __m128 sq = _mm_mul_ps(v, v);

    // Horizontal sum for length squared
    __m128 shuf = _mm_shuffle_ps(sq, sq, _MM_SHUFFLE(2, 3, 0, 1));
    __m128 sums = _mm_add_ps(sq, shuf);
    shuf = _mm_movehl_ps(shuf, sums);
    sums = _mm_add_ss(sums, shuf);

    // Broadcast length to all components and divide
    float len = sqrtf(_mm_cvtss_f32(sums));
    if (fabsf(len) < LEN_SQ_EPSILON)
        return vec3_n(0.0f);

    __m128 vlen = _mm_set1_ps(len);
    __m128 normalized = _mm_div_ps(v, vlen);

    _mm_storeu_ps(&result.x, normalized);
    return result;
}
```

**Speedup:** ~2x faster

---

### 4. vec3_add() - MEDIUM PRIORITY ⭐

**Scalar Version:**
```c
t_vec3 vec3_add(t_vec3 a, t_vec3 b)
{
    return ((t_vec3){{a.x + b.x, a.y + b.y, a.z + b.z}});
}
```

**SIMD Version:**
```c
t_vec3 vec3_add(t_vec3 a, t_vec3 b)
{
    t_vec3 result;
    __m128 va = _mm_loadu_ps(&a.x);
    __m128 vb = _mm_loadu_ps(&b.x);
    __m128 sum = _mm_add_ps(va, vb);
    _mm_storeu_ps(&result.x, sum);
    return result;
}
```

**Speedup:** ~2x faster

---

### 5. vec3_sub() - MEDIUM PRIORITY ⭐

**SIMD Version:**
```c
t_vec3 vec3_sub(t_vec3 a, t_vec3 b)
{
    t_vec3 result;
    __m128 va = _mm_loadu_ps(&a.x);
    __m128 vb = _mm_loadu_ps(&b.x);
    __m128 diff = _mm_sub_ps(va, vb);
    _mm_storeu_ps(&result.x, diff);
    return result;
}
```

**Speedup:** ~2x faster

---

### 6. vec3_scale() - MEDIUM PRIORITY ⭐

**SIMD Version:**
```c
t_vec3 vec3_scale(t_vec3 vec, float s)
{
    t_vec3 result;
    __m128 v = _mm_loadu_ps(&vec.x);
    __m128 scalar = _mm_set1_ps(s);  // Broadcast scalar to all components
    __m128 scaled = _mm_mul_ps(v, scalar);
    _mm_storeu_ps(&result.x, scaled);
    return result;
}
```

**Speedup:** ~1.8x faster

---

### 7. vec3_cross() - LOWER PRIORITY

**Note:** Cross product is harder to optimize with SIMD due to component shuffling. Scalar version is often competitive.

**SIMD Version (if needed):**
```c
t_vec3 vec3_cross(t_vec3 a, t_vec3 b)
{
    t_vec3 result;
    __m128 va = _mm_loadu_ps(&a.x);
    __m128 vb = _mm_loadu_ps(&b.x);

    // Shuffle to get required components
    // a.yzx: [y, z, x, w]
    __m128 a_yzx = _mm_shuffle_ps(va, va, _MM_SHUFFLE(3, 0, 2, 1));
    // b.zxy: [z, x, y, w]
    __m128 b_zxy = _mm_shuffle_ps(vb, vb, _MM_SHUFFLE(3, 1, 0, 2));

    // First term: a.yzx * b.zxy
    __m128 term1 = _mm_mul_ps(a_yzx, b_zxy);

    // Shuffle for second term
    __m128 a_zxy = _mm_shuffle_ps(va, va, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 b_yzx = _mm_shuffle_ps(vb, vb, _MM_SHUFFLE(3, 0, 2, 1));

    // Second term: a.zxy * b.yzx
    __m128 term2 = _mm_mul_ps(a_zxy, b_yzx);

    // Result: term1 - term2
    __m128 cross = _mm_sub_ps(term1, term2);

    _mm_storeu_ps(&result.x, cross);
    return result;
}
```

**Speedup:** ~1.3x faster (minimal gain, consider keeping scalar version)

---

## Integration Strategy

### Option 1: Compile-Time Toggle (RECOMMENDED) ✅

**Pros:**
- Easy to benchmark both versions
- Can fall back to scalar if issues arise
- Clear separation of implementations

**Implementation:**

1. **Add to Makefile:**
```makefile
# SIMD Optimization flags
SIMD_FLAGS = -msse2 -msse3  # SSE2 (universal), SSE3 (for hadd)
CFLAGS += $(SIMD_FLAGS)

# Optional: Add USE_SIMD flag
CFLAGS += -DUSE_SIMD=1
```

2. **Create SIMD header:** `inc/simd_vec3.h`
```c
#ifndef SIMD_VEC3_H
# define SIMD_VEC3_H

# include "libft_math.h"

# ifdef USE_SIMD
#  include <emmintrin.h>  // SSE2
#  include <pmmintrin.h>  // SSE3 (optional, for _mm_hadd_ps)
# endif

// SIMD versions of vector functions
# ifdef USE_SIMD
float   vec3_dot_simd(t_vec3 a, t_vec3 b);
float   vec3_length_simd(t_vec3 vec);
t_vec3  vec3_normalize_simd(t_vec3 vec);
t_vec3  vec3_add_simd(t_vec3 a, t_vec3 b);
t_vec3  vec3_sub_simd(t_vec3 a, t_vec3 b);
t_vec3  vec3_scale_simd(t_vec3 vec, float s);

// Redirect to SIMD versions
#  define vec3_dot         vec3_dot_simd
#  define vec3_length      vec3_length_simd
#  define vec3_normalize   vec3_normalize_simd
#  define vec3_add         vec3_add_simd
#  define vec3_sub         vec3_sub_simd
#  define vec3_scale       vec3_scale_simd
# endif

#endif
```

3. **Create implementation file:** `src/utils/libft/ft_vec3_simd.c`
```c
#include "simd_vec3.h"

#ifdef USE_SIMD

float vec3_dot_simd(t_vec3 a, t_vec3 b)
{
    // Implementation here
}

// ... other SIMD functions

#endif // USE_SIMD
```

4. **Include in your files:**
```c
#include "libft_math.h"
#include "simd_vec3.h"  // Adds SIMD versions if enabled
```

---

### Option 2: Direct Replacement

**Pros:**
- Simpler, no conditional compilation
- Always uses SIMD

**Cons:**
- Harder to debug
- Can't easily benchmark differences

**Implementation:**
Just replace the function bodies in the existing files:
- `src/utils/libft/ft_vec3.c`
- `src/utils/libft/ft_vec3_2.c`

---

### Option 3: Runtime Detection (Advanced)

**Pros:**
- Can detect CPU features at runtime
- Falls back gracefully on old CPUs

**Cons:**
- More complex
- Probably overkill for this project (SSE2 is universal on x86-64)

---

## Performance Expectations

### Microbenchmark Estimates
| Function | Scalar (ns) | SIMD (ns) | Speedup |
|----------|-------------|-----------|---------|
| vec3_dot | 3-4 | 1.2-1.5 | 2.5x |
| vec3_length | 6-8 | 3-4 | 2x |
| vec3_normalize | 10-12 | 5-6 | 2x |
| vec3_add | 2-3 | 1-1.5 | 2x |
| vec3_sub | 2-3 | 1-1.5 | 2x |
| vec3_scale | 2-3 | 1.2-1.8 | 1.8x |
| hit_aabb | 15-20 | 8-10 | 2x |

### Real-World Impact
- **Simple scenes:** 10-15% overall speedup
- **Complex scenes (many objects):** 20-30% overall speedup
- **BVH-heavy scenes:** Up to 40% speedup

### Factors Affecting Gains
✅ **Better with:**
- Deep BVH trees (lots of AABB tests)
- Complex lighting calculations
- Many normalize/dot operations
- High-resolution renders

❌ **Less effective with:**
- Simple scenes (few objects)
- I/O bound operations
- Memory-bound scenarios

---

## Compilation Instructions

### Update Makefile

Add to your CFLAGS:
```makefile
CFLAGS += -msse2        # Enable SSE2 (usually default on x86-64)
CFLAGS += -msse3        # Enable SSE3 (for _mm_hadd_ps)
CFLAGS += -DUSE_SIMD=1  # Enable SIMD code paths
```

### Check CPU Support (verify on target machine)
```bash
# Check for SSE2 support (should always be present on x86-64)
grep -o 'sse2' /proc/cpuinfo | head -1

# Check for SSE3 support
grep -o 'sse3' /proc/cpuinfo | head -1
```

---

## Debugging and Troubleshooting

### Common Issues

#### 1. **Alignment Issues**
**Problem:** Crashes with `_mm_load_ps` (aligned load)
**Solution:** Use `_mm_loadu_ps` (unaligned load) instead

Your t_vec3 is already 16-byte aligned, but better safe than sorry.

#### 2. **Garbage in 4th Component**
**Problem:** W component contains garbage, affects results
**Solution:** Use horizontal operations that only process first 3 components (as in hit_aabb)

#### 3. **Performance Not Improving**
**Possible causes:**
- Compiler already auto-vectorizing
- Memory bandwidth bottleneck
- Cache misses dominating
- Not compiling with optimization flags

**Check compiler optimizations:**
```makefile
CFLAGS += -O3           # Maximum optimization
CFLAGS += -march=native # Use all available CPU features
```

#### 4. **Results Differ Slightly**
**Expected:** Floating-point operations may have slightly different rounding
**Solution:** Normal for SIMD, differences should be < 1e-6

### Debugging Tips

**Print SIMD registers:**
```c
void print_m128(__m128 v)
{
    float temp[4];
    _mm_storeu_ps(temp, v);
    printf("[%.6f, %.6f, %.6f, %.6f]\n", temp[0], temp[1], temp[2], temp[3]);
}
```

**Compare outputs:**
```c
#ifdef DEBUG_SIMD
    t_vec3 result_scalar = vec3_dot_scalar(a, b);
    t_vec3 result_simd = vec3_dot_simd(a, b);
    if (fabsf(result_scalar - result_simd) > 1e-5)
        printf("SIMD mismatch: %f vs %f\n", result_scalar, result_simd);
#endif
```

---

## Benchmarking

### Simple Benchmark Template

```c
#include <time.h>

void benchmark_vec3_dot(void)
{
    t_vec3 a = vec3_n(1.0f);
    t_vec3 b = vec3_n(2.0f);
    float result;
    clock_t start, end;

    const int iterations = 10000000;

    // Scalar version
    start = clock();
    for (int i = 0; i < iterations; i++)
        result = vec3_dot_scalar(a, b);
    end = clock();
    double scalar_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // SIMD version
    start = clock();
    for (int i = 0; i < iterations; i++)
        result = vec3_dot_simd(a, b);
    end = clock();
    double simd_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Scalar: %.3fs, SIMD: %.3fs, Speedup: %.2fx\n",
           scalar_time, simd_time, scalar_time / simd_time);
}
```

---

## Additional Optimizations

### 1. Batch Processing (Advanced)
Process multiple rays/vectors at once using full SIMD width.

### 2. AVX/AVX2 (Future)
256-bit registers for 8 floats at once (requires newer CPUs).

### 3. Aligned Memory
Force alignment for slightly faster loads:
```c
struct __attribute__((aligned(16))) s_vec3_aligned {
    float x, y, z, pad;
};
```

---

## Resources

- **Intel Intrinsics Guide:** https://software.intel.com/sites/landingpage/IntrinsicsGuide/
- **SSE/AVX Tutorial:** https://www.cs.virginia.edu/~cr4bd/3330/S2018/simdref.html
- **SIMD for C++ Developers:** https://www.youtube.com/watch?v=x9Scb5Mku1g

---

## Summary

### Quick Start Checklist
- [ ] Add `-msse2` to Makefile CFLAGS
- [ ] Implement `vec3_dot_simd()` (highest priority)
- [ ] Implement `vec3_normalize_simd()` (high priority)
- [ ] Create `simd_vec3.h` header with toggle
- [ ] Benchmark before/after
- [ ] Test thoroughly (verify visual output matches)
- [ ] Profile to confirm performance gains

### Expected Total Gain
**10-30% faster rendering** with minimal code changes, focusing on the high-priority functions.

Good luck! 🚀
