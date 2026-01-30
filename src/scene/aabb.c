#include "scene.h"
#include "libft_list.h"
#include <emmintrin.h>  // SSE2 intrinsics

static inline t_aabb	combine_aabb(const t_aabb *a, const t_aabb *b);

t_aabb	get_volume_bounds(t_object **objs, size_t n)
{
	t_aabb		result;
	t_aabb		aabb;
	size_t		i;

	result = get_object_bounds(objs[0]);
	objs[0]->bounds_center = vec3_div(vec3_add(result.min, result.max), 2.0f);
	i = 1;
	while (i < n)
	{
		aabb = get_object_bounds(objs[i]);
		objs[i]->bounds_center = vec3_div(vec3_add(aabb.min, aabb.max), 2.0f);
		result = combine_aabb(&result, &aabb);
		++i;
	}
	return (result);
}

t_aabb	get_object_bounds(const t_object *obj)
{
	t_aabb result;

	if (obj->type == OBJ_SPHERE)
		result = sphere_bounds(obj);
	else if (obj->type == OBJ_PLANE)
		result = plane_bounds(obj);
	else
		result = cylinder_bounds(obj);
	return(result);
}

static inline t_aabb	combine_aabb(const t_aabb *a, const t_aabb *b)
{
	t_aabb		result;

	result.min.x = fminf(a->min.x, b->min.x);
	result.min.y = fminf(a->min.y, b->min.y);
	result.min.z = fminf(a->min.z, b->min.z);
	result.max.x = fmaxf(a->max.x, b->max.x);
	result.max.y = fmaxf(a->max.y, b->max.y);
	result.max.z = fmaxf(a->max.z, b->max.z);
	return (result);
}

bool	hit_aabb(const t_aabb *aabb, const t_ray *ray, float closest_t)
{
	float		t_min;
	float		t_max;
	float		min;
	float		max;

	t_min = 0.0f;
	t_max = closest_t;
	min = (aabb->min.x - ray->origin.x) * ray->inv_dir.x;
	max = (aabb->max.x - ray->origin.x) * ray->inv_dir.x;
	t_min = fmaxf(t_min, fminf(min, max));
	t_max = fminf(t_max, fmaxf(min, max));
	min = (aabb->min.y - ray->origin.y) * ray->inv_dir.y;
	max = (aabb->max.y - ray->origin.y) * ray->inv_dir.y;
	t_min = fmaxf(t_min, fminf(min, max));
	t_max = fminf(t_max, fmaxf(min, max));
	min = (aabb->min.z - ray->origin.z) * ray->inv_dir.z;
	max = (aabb->max.z - ray->origin.z) * ray->inv_dir.z;
	t_min = fmaxf(t_min, fminf(min, max));
	t_max = fminf(t_max, fmaxf(min, max));
	return (t_max >= t_min);
}


// SIMD-optimized AABB intersection using SSE2
// SSE2 = Streaming SIMD Extensions 2 (available on all x86-64 CPUs)
// emmintrin.h provides intrinsics: C functions that map directly to CPU SIMD instructions
// __m128 = 128-bit register holding 4 floats processed in parallel
//
// Algorithm: Slab method intersection of 3 axis-aligned slabs
// For each axis, compute t-range where ray is inside that slab
// Ray hits box only if it's inside ALL 3 slabs simultaneously
// __attribute_noinline__
// bool	hit_aabb(const t_aabb *aabb, const t_ray *ray, float closest_t)
// {
// 	__m128		mins;      // Will hold min bounds - ray origin for all 3 axes
// 	__m128		maxs;      // Will hold max bounds - ray origin for all 3 axes
// 	__m128		t0s;       // t-values where ray enters each slab
// 	__m128		t1s;       // t-values where ray exits each slab
// 	__m128		tmin;      // Per-axis minimum t (entry points)
// 	__m128		tmax;      // Per-axis maximum t (exit points)
// 	__m128		temp;      // Temporary for shuffle operations
// 	float		final_tmin;
// 	float		final_tmax;

// 	// Step 1: Load 3D vectors into 128-bit registers and compute (bounds - origin)
// 	// _mm_loadu_ps: Load 4 floats (unaligned) from memory into __m128 register
// 	// _mm_sub_ps: Subtract 4 floats in parallel (a-b for each component)
// 	// Result: [min.x-origin.x, min.y-origin.y, min.z-origin.z, garbage]
// 	mins = _mm_sub_ps(_mm_loadu_ps(&aabb->min.x), _mm_loadu_ps(&ray->origin.x));
// 	maxs = _mm_sub_ps(_mm_loadu_ps(&aabb->max.x), _mm_loadu_ps(&ray->origin.x));

// 	// Step 2: Multiply by inverse direction to get t-values
// 	// _mm_mul_ps: Multiply 4 floats in parallel
// 	// Computes 3 slab intersections simultaneously instead of sequentially!
// 	// t0s = [t_min_x, t_min_y, t_min_z, garbage]
// 	// t1s = [t_max_x, t_max_y, t_max_z, garbage]
// 	t0s = _mm_mul_ps(mins, _mm_loadu_ps(&ray->inv_dir.x));
// 	t1s = _mm_mul_ps(maxs, _mm_loadu_ps(&ray->inv_dir.x));

// 	// Step 3: Handle negative ray directions
// 	// _mm_min_ps/_mm_max_ps: Component-wise min/max of 4 floats
// 	// If inv_dir is negative, t0 > t1, so we need min(t0,t1) and max(t0,t1)
// 	// tmin = [entry_x, entry_y, entry_z, garbage]
// 	// tmax = [exit_x, exit_y, exit_z, garbage]
// 	tmin = _mm_min_ps(t0s, t1s);
// 	tmax = _mm_max_ps(t0s, t1s);

// 	// Step 4: Horizontal maximum reduction of tmin (find latest entry point)
// 	// We need: max(entry_x, entry_y, entry_z)
// 	// Ray enters box when it has entered ALL slabs
// 	//
// 	// _mm_shuffle_ps(a, a, mask): Rearrange float positions within register
// 	// _MM_SHUFFLE(3,2,1,0) selects which source elements go to dest[0..3]
// 	// _mm_max_ss: Only operates on lowest float (scalar single), leaves others unchanged
// 	temp = _mm_shuffle_ps(tmin, tmin, _MM_SHUFFLE(2, 2, 2, 1));  // [y, z, z, z]
// 	tmin = _mm_max_ss(tmin, temp);  // tmin[0] = max(x, y)
// 	temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));  // [z, z, z, z]
// 	tmin = _mm_max_ss(tmin, temp);  // tmin[0] = max(max(x,y), z)

// 	// Step 5: Horizontal minimum reduction of tmax (find earliest exit point)
// 	// We need: min(exit_x, exit_y, exit_z)
// 	// Ray exits box when it has exited ANY slab
// 	temp = _mm_shuffle_ps(tmax, tmax, _MM_SHUFFLE(2, 2, 2, 1));  // [y, z, z, z]
// 	tmax = _mm_min_ss(tmax, temp);  // tmax[0] = min(x, y)
// 	temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));  // [z, z, z, z]
// 	tmax = _mm_min_ss(tmax, temp);  // tmax[0] = min(min(x,y), z)

// 	// Step 6: Extract scalar results and apply ray parameter bounds
// 	// _mm_cvtss_f32: Convert lowest float from __m128 to regular float
// 	// Clamp to [0, closest_t] range (can't hit behind ray, can't hit farther than current closest)
// 	final_tmin = _mm_cvtss_f32(tmin);
// 	final_tmax = _mm_cvtss_f32(tmax);
// 	final_tmin = fmaxf(0.0f, final_tmin);
// 	final_tmax = fminf(closest_t, final_tmax);

// 	// Step 7: Ray hits if it enters before it exits
// 	// If final_tmax < final_tmin, the ray misses the box
// 	return (final_tmax >= final_tmin);
// }
