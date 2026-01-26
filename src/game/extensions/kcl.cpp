/*
 * This file contains a reimplementation of the KCL collision system used in many Nintendo games including Super Mario 64 DS.
 * This implementaiton of KCL is specific for our purposes and is not compatible with any other version of KCL.
 * References and Credits: 
 *   https://github.com/Gota7/SM64DSe-Ultimate/blob/master/Documentation/kcl_format.txt
 *   https://github.com/Gota7/SM64DSe-Ultimate/blob/master/src/core/formats/KCL.cs
 *   https://discord.com/channels/334848602130219009/1160241116583575704 (You can access this by joining the SM64DS Hacking Discord)
 */

#include "kcl.h"

void kcl_init(struct KCL *kcl) {
    if (kcl == NULL) { return; }
    
    memset(kcl, 0, sizeof(struct KCL));
}

void kcl_free(struct KCL *kcl) {
    if (kcl == NULL) { return; }
    
    // Set the next free to the current pool. We don't want KCL data
    // to leak in memory.
    kcl->pool->nextFree = kcl->pool->tail;
    // Free our dynamic pool.
    dynamic_pool_free_pool(&kcl->pool);
    // Clear our KCL structure, It can be reused now or freed itself.
    memset(kcl, 0, sizeof(struct KCL));
} 

// ray_surface_intersect in surface_collision.c has been used for reference.
bool kcl_raycast(struct KCL *kcl, Vec3f start, Vec3f dir, OUT Vec3f hit_pos) {
    if (kcl == NULL || kcl->nodes == NULL) { return false; }
    
    // Iterate our OctTree and check each node.
    for (u32 i = 0; i < kcl->numNodes; i++) {
        struct KCLOctreeNode *node = &kcl->nodes[i];
        
        // If our ray doesn't even intersect with the node. Just skip over it.
        if (!kcl_octree_intersects_ray(node, start, dir)) { continue; }
        
        // Iterate all of our triangles. Make sure none of them are hit!
        for (u32 j = 0; j < kcl->numTris; j++) {
            struct KCLTri *tri = &kcl->tris[j];
            
            // Check if we're perpendicular from the surface
            Vec3f e1, e2, h;
            vec3f_dif(e1, tri->point2, tri->point1);
            vec3f_dif(e2, tri->point3, tri->point1);
            vec3f_cross(h, dir, e2);
            
            f32 hdst = vec3f_dot(e1, h);
            if (hdst > -0.00001f && hdst < 0.00001f) { continue; } 
              
            // Check if we're making contact with the surface
            f32 f = 1.0f / hdst;
            Vec3f s;
            vec3f_dif(s, start, tri->point1);
            f32 u = f * vec3f_dot(s, h);
            if (u < 0.0f || u > 1.0f) { continue; }
            
            Vec3f q;
            vec3f_cross(q, s, e1);
            f32 v = f * vec3f_dot(dir, q);
            if (v < 0.0f || u + v > 1.0f) { continue; }
            
            // Get the length between our origin and the surface contact point.
            f32 length = f * vec3f_dot(e2, q);
            if (length <= 0.00001) { continue; }
            
            // Successful contact.
            Vec3f add_dir;
            vec3f_copy(add_dir, dir);
            vec3f_mul(add_dir, length);
            vec3f_sum(hit_pos, start, add_dir);
            return true;
        }
    }
    
    // We didn't manage to hit anything, Bummer.
    return false;
}

// ray_surface_intersect in surface_collision.c has been used for reference.
bool kcl_raycast_ext(struct KCL *kcl, Vec3f start, Vec3f dir, f32 dir_length, OUT Vec3f hit_pos, OUT f32 *length) {
    if (kcl == NULL || kcl->nodes == NULL) { return false; }
    
    // Iterate our OctTree and check each node.
    for (u32 i = 0; i < kcl->numNodes; i++) {
        struct KCLOctreeNode *node = &kcl->nodes[i];
        
        // If our ray doesn't even intersect with the node. Just skip over it.
        if (!kcl_octree_intersects_ray(node, start, dir)) { continue; }
        
        // Iterate all of our triangles. Make sure none of them are hit!
        for (u32 j = 0; j < kcl->numTris; j++) {
            struct KCLTri *tri = &kcl->tris[j];
            
            // Check if we're perpendicular from the surface
            Vec3f e1, e2, h;
            vec3f_dif(e1, tri->point2, tri->point1);
            vec3f_dif(e2, tri->point3, tri->point1);
            vec3f_cross(h, dir, e2);
            
            f32 hdst = vec3f_dot(e1, h);
            if (hdst > -0.00001f && hdst < 0.00001f) { continue; } 
              
            // Check if we're making contact with the surface
            f32 f = 1.0f / hdst;
            Vec3f s;
            vec3f_dif(s, start, tri->point1);
            f32 u = f * vec3f_dot(s, h);
            if (u < 0.0f || u > 1.0f) { continue; }
            
            Vec3f q;
            vec3f_cross(q, s, e1);
            f32 v = f * vec3f_dot(dir, q);
            if (v < 0.0f || u + v > 1.0f) { continue; }
            
            // Get the length between our origin and the surface contact point.
            *length = f * vec3f_dot(e2, q);
            if (*length <= 0.00001 || *length > dir_length) { continue; }
            
            // Successful contact.
            Vec3f add_dir;
            vec3f_copy(add_dir, dir);
            vec3f_mul(add_dir, *length);
            vec3f_sum(hit_pos, start, add_dir);
            return true;
        }
    }
    
    // We didn't manage to hit anything, Bummer.
    return false;
}

s32 kcl_from_file(fs_file_t *file, OUT struct KCL *kcl) {
    // Invalid arguments check.
    if (file == NULL || fs_size(file) <= 0 || kcl == NULL) { return -2; } // Invalid arguments error.
    
    // Initialize the KCL.
    kcl_init(kcl);
    
    // Read our header information. This contains the amount of points, vectors, and planes (tris) we have.
    if (fs_remaining(file) < 3 * sizeof(u32)) { goto error; } // Out of read space error
    fs_read_u32(file, &kcl->numPoints);
    fs_read_u32(file, &kcl->numVectors);
    fs_read_u32(file, &kcl->numPlanes);
    kcl->numTris = kcl->numPlanes;
    
    if (fs_remaining(file) < kcl->numPoints * sizeof(Vec3f)) { goto error; } // Out of read space error
    // Allocate our memory pool for all of our points.
    kcl->points = dynamic_pool_alloc(&kcl->pool, kcl->numPoints * sizeof(Vec3f));
    // Read all of our points from the KCL file.
    for (u32 i = 0; i < kcl->numPoints; i++) {
        fs_read_vec3f(file, kcl->points[i]);
    }
    
    if (fs_remaining(file) < kcl->numVectors * sizeof(Vec3f)) { goto error; } // Out of read space error
    // Allocate our memory pool for all of our vectors.
    kcl->vectors = dynamic_pool_alloc(&kcl->pool, kcl->numVectors * sizeof(Vec3f));
    // Read all of our vectors from the KCL file.
    for (u32 i = 0; i < kcl->numVectors; i++) {
        fs_read_vec3f(file, kcl->vectors[i]);
    }
    
    if (fs_remaining(file) < kcl->numPlanes * sizeof(struct KCLPlane)) { goto error; } // Out of read space error
    // Allocate our memory pool for all of our planes and tris.
    kcl->planes = dynamic_pool_alloc(&kcl->pool, kcl->numPlanes * sizeof(struct KCLPlane));
    kcl->tris = dynamic_pool_alloc(&kcl->pool, kcl->numTris * sizeof(struct KCLTri));
    // Read all of our planes from the KCL file.
    for (u32 i = 0; i < kcl->numPlanes; i++) {
        struct KCLPlane *plane = &kcl->planes[i];
        fs_read_u32(file, &plane->originId);
        fs_read_u32(file, &plane->normalId);
        fs_read_u32(file, &plane->dirId1);
        fs_read_u32(file, &plane->dirId2);
        fs_read_u32(file, &plane->dirId3);
        fs_read_f32(file, &plane->length);
        fs_read_u32(file, &plane->terrainType);
        // Convert the plane to a tri.
        kcl_plane_to_tri(kcl, i);
    }
    
    s32 shift;
    fs_read_s32(file, &shift);
    
    f32 cubesize = (f32)(1 << shift) / 1024.0f;
    
    Vec3i octreesize;
    fs_read_vec3i(file, octreesize);
    vec3i_set(octreesize, (~octreesize[0] >> shift) + 1, (~octreesize[1] >> shift) + 1, (~octreesize[2] >> shift) + 1);
    
    Vec3f octreestartpos;
    fs_read_vec3f(file, octreestartpos);
    
    kcl->numNodes = octreesize[0] * octreesize[1] * octreesize[2];
    kcl->nodes = dynamic_pool_alloc(&kcl->pool, kcl->numNodes * sizeof(struct KCLOctreeNode));
    for (u32 x = 0; x < octreesize[0]; x++) {
        for (u32 y = 0; y < octreesize[1]; y++) {
            for (u32 z = 0; z < octreesize[2]; z++) {
                struct KCLOctreeNode *node = &kcl->nodes[x + y + z];
                vec3f_set(node->size, cubesize, cubesize, cubesize);
                vec3f_set(node->position, node->size[0] * x, node->size[1] * y, node->size[2] * z);
                vec3f_add(node->position, octreestartpos);
            }
        }
    }

    return 0;
    
error:
    kcl_free(kcl);
    return -1;
}

bool kcl_plane_to_tri(struct KCL *kcl, u32 id) {
    if (kcl == NULL || kcl->planes == NULL || kcl->tris == NULL) { return false; }
    
    struct KCLPlane *plane = &kcl->planes[id];
    struct KCLTri *tri = &kcl->tris[id];
    
    if (plane->originId >= kcl->numPoints) { return false; }
    if (plane->normalId >= kcl->numVectors) { return false; }
    if (plane->dirId1 >= kcl->numVectors) { return false; }
    if (plane->dirId2 >= kcl->numVectors) { return false; }
    if (plane->dirId3 >= kcl->numVectors) { return false; }
    
    vec3f_copy(tri->point1, kcl->points[plane->originId]);
    vec3f_copy(tri->point2, tri->point1);
    vec3f_copy(tri->point3, tri->point1);
    vec3f_copy(tri->normal, kcl->vectors[plane->normalId]);
    tri->terrainType = plane->terrainType;
  
    Vec3f cross1;
    Vec3f cross2;
    vec3f_cross(cross1, tri->normal, kcl->vectors[dirId1]);
    vec3f_cross(cross2, tri->normal, kcl->vectors[dirId2]);
    f32 dot1 = vec3f_dot(cross1, kcl->vectors[dirId3]);
    f32 dot2 = vec3f_dot(cross2, kcl->vectors[dirId3]);
    
    vec3f_add(tri->point2, vec3f_mul(cross2, dot2 != 0.0f ? plane->length / dot2 : 0.0f));
    vec3f_add(tri->point3, vec3f_mul(cross1, dot1 != 0.0f ? plane->length / dot1 : 0.0f));
    
    return true;
}

bool kcl_octree_contains_point(struct KCLOctreeNode *node, Vec3f point) {
    if (node == NULL) { return false; }
    
    return (point[0] >= node->position[0] && point[0] <= node->position[0] + node->size[0] &&
            point[1] >= node->position[1] && point[1] <= node->position[1] + node->size[1] && 
            point[2] >= node->position[2] && point[2] <= node->position[2] + node->size[2]);
}

bool kcl_octree_intersects_ray(struct KCLOctreeNode *node, Vec3f start, Vec3f dir) {
    if (node == NULL) { return false; }
    
    f32 imin = -0.1f;
    f32 imax = 1.1f;
    
    f32 x0 = (node->position[0] - start[0]) / dir[0];
    f32 x1 = (node->position[0] + node->size[0] - start[0]) / dir[0];
    imin = max(imin, min(x0, x1));
    imax = min(imax, max(x0, x1));
    
    f32 y0 = (node->position[1] - start[1]) / dir[1];
    f32 y1 = (node->position[1] + node->size[1] - start[1]) / dir[1];
    imin = max(imin, min(y0, y1));
    imax = min(imax, max(y0, y1));
    
    f32 z0 = (node->position[2] - start[2]) / dir[2];
    f32 z1 = (node->position[2] + node->size[2] - start[2]) / dir[2];
    imin = max(imin, min(z0, z1));
    imax = min(imax, max(z0, z1));
    
    return (imin <= 1.0f && imax >= 0.0f && imax >= imin) || (isnan(imin) || isnan(imax));
}