#include <stdio.h>
#include "inc/defines.h"
#include "inc/lib_math.h"
#include "inc/scene.h"
#include "inc/materials.h"

// Define a test object to check how sizes change
struct __attribute__((aligned(16))) s_object_test
{
        t_obj_type              type;
        uint32_t                flags;
        t_material              *mat;
        t_shape                 shape;
        t_transform             transform;
        t_vec3                  bounds_center;
        uint32_t                material_id;
};

struct __attribute__((aligned(16))) s_hit_test
{
        float                   t;
        bool                    is_primary;
        const t_object  *obj;
        t_vec3                  point;
        t_vec3                  normal;
        t_vec3                  tangent;
        t_vec3                  bitangent;
        t_vec2                  uv;
};

int main() {
    printf("Old t_object: %zu, New t_object_test: %zu\n", sizeof(t_object), sizeof(struct s_object_test));
    printf("Old t_hit: %zu, New t_hit_test: %zu\n", sizeof(t_hit), sizeof(struct s_hit_test));
    return 0;
}
