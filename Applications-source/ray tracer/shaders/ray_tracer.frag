#version 410 core
in vec3  frag_wld_coord;
out vec4 frag_color;

uniform vec3    fst_sphere_pos;
uniform float   fst_sphere_radius;
uniform vec3    fst_sphere_color;

uniform vec3    snd_sphere_pos;
uniform float   snd_sphere_radius;
uniform vec3    snd_sphere_color;

uniform vec3   cam_wld_pos;
uniform vec3   light_pos;

struct Ray {
    vec3  origin;
    vec3  direction;
};
Ray     camera_ray(const in vec3 cam_pos, const in vec3 frag_coord);
bool    trace_ray(const in Ray ray, const in vec3 center_pos, const in float r, inout float t);
bool solve_quadratic(const in float a, const in float b, const in float c, inout float x0, inout float x1);


//camera_ray: get the camera's ray from fragment/sample position in the film/projection plane
Ray camera_ray(const in vec3 cam_pos, const in vec3 frag_coord)
{
    Ray ray = Ray(cam_pos, frag_coord - cam_pos);
    ray.direction = normalize(ray.direction);
    return ray;
}


bool solve_quadratic(const in float a, const in float b, const in float c, inout float x0, inout float x1)
{
    float discr = b * b - 4.0f * a * c;
    
    if (discr < 0) {
        return false;
    }
    else if(discr == 0) {
        x0 = x1 = - 0.5f * b / a;
    }
    else {
        float q = (b > 0) ?(-0.5 * (b + sqrt(discr))) :(-0.5 * (b - sqrt(discr)) );
        x0 = q / a;
        x1 = c / q;
    }
    
    return true;
}

bool trace_ray(const in Ray ray, const in vec3 center_pos, const in float r, inout float t)
{
    float  t0, t1;
    vec3   L =  ray.origin - center_pos;
    float  a =  dot(ray.direction, ray.direction);
    float  b =  2.0f * dot(ray.direction, L);
    float  c =  dot(L, L) - r * r;
    
    if (solve_quadratic(a, b, c, t0, t1)) {
        if (t0 > t1){
            //std::swap(t0, t1);
            float aux = t1;
            t1 = t0;
            t0 = aux;
        }
        
        if (t0 < 0) {
            t0 = t1; // if t0 is negative, let's use t1 instead
            if (t0 < 0) {
                return false; // both t0 and t1 are negative
            }
        }
        
        t = t0;
        return true;
    }
    else {
        return false;
    }
    
}

/* get ray from fragment's world position
 * trace ray into the scene and check for collision with sphere
 * if the ray collided with the sphere , shade the fragment's color
 * else shade with "background color"
 */
void main()
{
    Ray    ray = camera_ray(cam_wld_pos, frag_wld_coord);
    
   
    float  t1, t2;
    //check collision with the first sphere
    bool coll_fst = trace_ray(ray, fst_sphere_pos, fst_sphere_radius, t1);
    
    //check collision with the second sphere
    bool coll_snd = trace_ray(ray, snd_sphere_pos, snd_sphere_radius, t2);
    
    bool hit = coll_fst || coll_snd;
    
    float   t;
    float   hit_radius;
    vec3    hit_pos;
    vec3    hit_color;
    vec4    background_color = vec4(0.0f,0.0f,0.0f, 1.0f);
    
    //find the closest intercetion point, if any
    if (coll_fst) {
        if (coll_snd) {
            if (t1 < t2) {
                t = t1;
                hit_pos    = fst_sphere_pos;
                hit_radius = fst_sphere_radius;
                hit_color  = fst_sphere_color;
            }
            else {
                t = t2;
                hit_pos     =  snd_sphere_pos;
                hit_radius  =  snd_sphere_radius;
                hit_color  =   snd_sphere_color;
            }
            //t = (t1 < t2) ?(t1) :(t2);
        }
        else {
            t = t1;
            hit_pos    = fst_sphere_pos;
            hit_radius = fst_sphere_radius;
            hit_color  = fst_sphere_color;
        }
    }
    else {
        if (coll_snd) {
            t = t2;
            hit_pos     =  snd_sphere_pos;
            hit_radius  =  snd_sphere_radius;
            hit_color   =  snd_sphere_color;
        }
    }
    
    
    if(hit) {
        // get the point intercepted by the ray
        vec3 p_hit  =  ray.origin + t * ray.direction;
        
        //shade the point
        
        //ambient
        float ambient_factor = 0.10f;
        vec3 ambient = ambient_factor * vec3(1.0f, 1.0f, 1.0f);
        //diffuse
        vec3 normal =  normalize(p_hit - hit_pos);  // calculate the point's normal
        
        vec3 light_dir = normalize(light_pos - p_hit);  // get the vector from the light's source to the point
        vec3 diffuse   = max(dot(normal, light_dir), 0.0f) * vec3(1.0f, 1.0f, 1.0f);
        //specular
        vec3 refletected_dir = reflect(-light_dir, normal);
        float spec = pow(max(dot(-ray.direction, refletected_dir), 0.0), 64);
        
        vec3 specular = 0.5 * spec * vec3(1.0f,1.0f, 1.0f);
        //test
        frag_color = vec4(ambient + diffuse + specular, 1.0f) * vec4(hit_color, 1.0f);
    }
    else {
        frag_color = background_color;
    }
}