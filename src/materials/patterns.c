#include "lib_math.h"
#include "materials.h"

// static t_vec3 pattern_checkerboard2(t_hit *hit, t_material *mat);
static t_vec3   pattern_checkerboard(const t_hit *hit, const t_material *mat);
static t_vec3   pattern_gradient(const t_hit *hit, const t_material *mat);
static t_vec3   pattern_stripe(const t_hit *hit, const t_material *mat);
static t_vec3   pattern_spiral(const t_hit *hit, const t_material *mat);
static t_vec3   pattern_grid(const t_hit *hit, const t_material *mat);
static t_vec3   pattern_brick(const t_hit *hit, const t_material *mat);

t_vec3	eval_pattern(t_material *mat, t_hit *hit)
{
	if (mat->pattern == PAT_NONE)
		return (mat->albedo);
	if (mat->pattern == PAT_CHECKERBOARD)
		return (pattern_checkerboard(hit, mat));
	if (mat->pattern == PAT_GRADIENT)
		return (pattern_gradient(hit, mat));
	if (mat->pattern == PAT_STRIPES)
        return (pattern_stripe(hit, mat));
	if (mat->pattern == PAT_SPIRAL)
        return (pattern_spiral(hit, mat));
	if (mat->pattern == PAT_GRID)
        return (pattern_grid(hit, mat));
	if (mat->pattern == PAT_BRICK)
		return (pattern_brick(hit, mat));
	if (mat->pattern == PAT_MARBLE)
		return (pattern_perlin_marble(hit, mat));
	if (mat->pattern == PAT_WOOD)
		return (pattern_perlin_wood(hit, mat));
	if (mat->pattern == PAT_TURBULENCE)
		return (pattern_perlin_turb(hit, mat));
    return (mat->albedo);
}

//(Note: While this handles translation/moving perfectly, fully supporting rotation and scaling would require dividing by the object's scale and multiplying by the inverse of its rotation matrix instead of a simple subtraction, but subtraction handles xyz placement perfectly!)
/*
** 3D Checkerboard pattern.
** Divides world space into cubes using floor(), then alternates colors
** based on the parity of the sum of the integer coordinates.
**
** The pattern_scale controls the size of each checker square.
** Smaller scale = more squares, larger scale = bigger squares.
**
** @param hit  - Hit record with world-space point
** @param mat  - Material with primary and secondary albedo colors
** @return     - albedo or albedo2 depending on checker position
*/
static t_vec3	pattern_checkerboard(const t_hit *hit, const t_material *mat)
{
	float	scale;
	t_vec3	p;

	scale = mat->pattern_scale;
	if (scale < 0.001f)
		scale = 1.0f;
	p = vec3_sub(hit->point, hit->obj->transform.pos);
	if (((int)floorf(p.x * scale + 0.5f) + (int)floorf(p.y * scale + 0.5f)
			+ (int)floorf(p.z * scale + 0.5f)) & 1)
		return (mat->albedo2);
	return (mat->albedo);
}


/*
** Gradient pattern using UV v-coordinate.
** Produces a smooth linear blend between albedo and albedo2.
**
** For spheres : v goes from north pole (0) to south pole (1)
** For cylinders : v goes along the height
** For planes  : v is based on world position
**
** @param hit  - Hit record with UV coordinates
** @param mat  - Material with primary and secondary albedo colors
** @return     - Linearly interpolated color based on hit->uv.v
*/
static t_vec3	pattern_gradient(const t_hit *hit, const t_material *mat)
{
	float	scale;
	float	t;
	float	y;

	scale = mat->pattern_scale;
	if (scale < 0.001f)
		scale = 1.0f;
	y = hit->point.y - hit->obj->transform.pos.y;
	t = 0.5f * (sinf(y * scale) + 1.0f);
	return (vec3_lerp(mat->albedo, mat->albedo2, t));
}

/*
** Stripe pattern along the Y-axis in world space.
** Creates parallel bands by using a sine wave on the scaled Y coordinate.
** The sign of sin() determines which color to use.
**
** @param hit  - Hit record with world-space point
** @param mat  - Material with primary and secondary albedo colors
** @return     - albedo or albedo2 in alternating bands
*/
static t_vec3	pattern_stripe(const t_hit *hit, const t_material *mat)
{
	float	scale;
	float	value;
	float	y;

	scale = mat->pattern_scale;
	if (scale < 0.001f)
		scale = 1.0f;
	y = hit->point.y - hit->obj->transform.pos.y;
	value = sinf(y * scale);
	if (value < 0.0f)
		return (mat->albedo2);
	return (mat->albedo);
}

/*
** Spiral (helical) stripe pattern.
** Creates diagonal stripes that twist around the Y-axis, like a
** barber pole or candy cane.  Combines the height (y) with the
** angle around the Y-axis (atan2) so the bands genuinely spiral.
**
** The angular component atan2(x,z) is divided by PI to normalize
** its range from [-PI,PI] to [-1,1], giving one full twist per
** 2 units of height at scale 1.
**
** @param hit  - Hit record with world-space point
** @param mat  - Material with primary and secondary albedo colors
** @return     - albedo or albedo2 in spiraling bands
*/
static t_vec3	pattern_spiral(const t_hit *hit, const t_material *mat)
{
	float	scale;
	float	angle;
	float	value;
	t_vec3	p;

	scale = mat->pattern_scale;
	if (scale < 0.001f)
		scale = 1.0f;
	p = vec3_sub(hit->point, hit->obj->transform.pos);
	angle = atan2f(p.x, p.z);
	value = sinf(p.y * scale + angle);
	if (value < 0.0f)
		return (mat->albedo2);
	return (mat->albedo);
}

/*
** Grid (Wireframe) pattern.
** Renders a neon-style wireframe grid by checking if the local point
** fractional coordinate is very close to 0.0. Good for debugging.
*/
static t_vec3	pattern_grid(const t_hit *hit, const t_material *mat)
{
	float	scale;
	t_vec3	p;
	float	line_width;

	scale = mat->pattern_scale;
	if (scale < 0.001f)
		scale = 1.0f;
	p = vec3_scale(vec3_sub(hit->point, hit->obj->transform.pos), scale);
	p.x = p.x - floorf(p.x);
	p.y = p.y - floorf(p.y);
	p.z = p.z - floorf(p.z);
	line_width = 0.05f;
	if (p.x < line_width || p.y < line_width || p.z < line_width)
		return (mat->albedo2);
	return (mat->albedo);
}

/*
** Brick wall pattern.
** Staggers alternate rows by 0.5 in X and Z.
*/
static t_vec3   pattern_brick(const t_hit *hit, const t_material *mat)
{
	float	scale;
	t_vec3	p;
	int		y_int;

	scale = mat->pattern_scale;
	if (scale < 0.001f)
		scale = 1.0f;
	p = vec3_scale(vec3_sub(hit->point, hit->obj->transform.pos), scale);
	y_int = (int)floorf(p.y);
	if (y_int % 2 != 0)
	{
		p.x += 0.5f;
		p.z += 0.5f;
	}
	p.x = p.x - floorf(p.x);
	p.y = p.y - floorf(p.y);
	p.z = p.z - floorf(p.z);
	if (p.x < 0.05f || p.y < 0.05f || p.z < 0.05f)
		return (mat->albedo2);
	return (mat->albedo);
}
