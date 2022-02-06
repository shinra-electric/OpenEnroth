#pragma once

#include "Engine/VectorTypes.h"

struct BLVFace;

struct CollisionState {
    /**
     * Prepares this struct by filling all necessary fields, and checks whether there is actually no movement.
     *
     * @param dt                        Time delta, in fixpoint seconds.
     * @return                          True if there is no movement, false otherwise.
     */
    bool PrepareAndCheckIfStationary(int dt);

    // actor is modeled as two spheres, basically "feet" & "head". Collisions are then done for both spheres.

    int check_hi;  // Check the hi sphere collisions. If not set, only the lo sphere is checked.
    int radius_lo;   // radius of the lo ("feet") sphere.
    int radius_hi;  // radius of the hi ("head") sphere.
    Vec3_int_ position_lo; // center of the lo sphere.
    Vec3_int_ position_hi; // center of the hi sphere.
    Vec3_int_ new_position_lo; // desired new position for the center of the lo sphere.
    Vec3_int_ new_position_hi; // desired new position for the center of the hi sphere.
    Vec3_int_ velocity;  // Movement vector.
    Vec3_int_ direction;  // Movement direction, basically velocity as a unit vector.
    int speed = 0;  // Velocity magnitude.
    int total_move_distance;  // Total move distance, accumulated between collision iterations, starts at 0.
    int move_distance;  // Desired movement distance for current iteration, minus the distance already covered.
    int adjusted_move_distance;  // Movement distance for current iteration, adjusted after collision checks.
    unsigned int uSectorID = 0;  // Indoor sector id.
    unsigned int pid;  // PID of the object that we're collided with.
    int ignored_face_id;  // Don't check collisions with this face.
    BBox_int_ bbox = { 0, 0, 0, 0, 0, 0 };
};

extern CollisionState collision_state;

/**
 * Original offset 0x46E44E.
 *
 * Performs collisions with level geometry in indoor levels. Updates `collision_state`.
 *
 * @param ignore_ethereal               Whether ethereal faces should be ignored by this function.
 */
void CollideIndoorWithGeometry(bool ignore_ethereal);

/**
 * Original offset 0x46E889.
 *
 * Performs collisions with models in outdoor levels. Updates `collision_state`.
 *
 * @param ignore_ethereal               Whether ethereal faces should be ignored by this function.
 */
void CollideOutdoorWithModels(bool ignore_ethereal);

/**
 * Original offset 0x46E0B2.
 */
void CollideIndoorWithDecorations();

/**
 * Original offset 0x46E26D.
 *
 * @param grid_x
 * @param grid_y
 */
void CollideOutdoorWithDecorations(int grid_x, int grid_y);

/**
 * Original offset 0x46F04E.
 *
 * Performs collision checks with portals. Updates `collision_state`. If the collision did happen, then
 * `adjusted_move_distance` member is set to `0xFFFFFF` (basically a large number).
 *
 * @return                              True if there were no collisions with portals.
 */
bool CollideIndoorWithPortals();

/**
 * Original offset 0x46DF1A.
 *
 * @param actor_idx                 Actor index.
 * @param override_radius           Override actor's radius. Pass zero to use original radius.
 * @return                          Whether the collision is possible.
 */
bool CollideWithActor(int actor_idx, int override_radius);

void _46ED8A_collide_against_sprite_objects(unsigned int _this);

int _46EF01_collision_chech_player(int a1);


// ================================================================================================================== //
// Helper functions (not really a part of public interface)
// ================================================================================================================== //

/**
 * Original offset 0x47531C.
 *
 * @param face                          Polygon to check collision against.
 * @param pos                           Actor position to check.
 * @param radius                        Actor radius.
 * @param dir                           Movement direction as a unit vector in fixpoint format.
 * @param move_distance[out]            Move distance along the `dir` axis required to touch the provided polygon.
 *                                      Always non-negative. This parameter is not set if the function returns false.
 *                                      Note that "touching" in this context means that the distance from the actor's
 *                                      center to the polygon equals actor's radius.
 * @param ignore_ethereal               Whether ethereal faces should be ignored by this function.
 * @return                              Whether the actor, basically modeled as a sphere, can actually collide with the
 *                                      polygon if moving along the `dir` axis.
 */
bool CollideIndoorWithFace(BLVFace *face, const Vec3_int_ &pos, int radius, const Vec3_int_ &dir,
    int *move_distance, bool ignore_ethereal);

/**
 * Original offset 0x4754BF
 *
 * @see CollideIndoorWithFace
 */
bool CollideOutdoorWithFace(int radius, int *move_distance, const Vec3_int_ &pos, const Vec3_int_ &dir,
    BLVFace *face, int model_index, bool ignore_ethereal);

/**
 * Original offset 0x475D85.
 *
 * @param face                          Polygon to check collision against.
 * @param pos                           Actor position to check.
 * @param dir                           Movement direction as a unit vector in fixpoint format.
 * @param move_distance[in,out]         Current movement distance along the `dir` axis. This parameter is not touched
 *                                      when the function returns false. If the function returns true, then the
 *                                      distance required to hit the polygon is stored here. Note that this effectively
 *                                      means that this function can only decrease `move_distance`, but never increase
 *                                      it.
 * @return                              Whether the actor, modeled as a point, hits the provided polygon if moving from
 *                                      `pos` along the `dir` axis by at most `move_distance`.
 *
 * @see CollideIndoorWithFace
 */
bool CollidePointIndoorWithFace(BLVFace *face, Vec3_int_ *pos, Vec3_int_ *dir, int *move_distance);

/**
 * Original offset 0x475F30.
 *
 * @see CollidePointIndoorWithFace
 */
bool CollidePointOutdoorWithFace(int *move_distance, BLVFace *face, const Vec3_int_ &pos, const Vec3_int_ &dir, int a9);

/**
 * Original offset 0x475665.
 *
 * \param face                          Face to check.
 * \param point                         Point to check.
 * \returns                             Projects the provided point and face onto the face's main plane (XY, YZ or ZX)
 *                                      and returns whether the resulting point lies inside the resulting polygon.
 */
bool IsProjectedPointInsideIndoorFace(BLVFace *face, const Vec3_short_ &point);

/**
 * Original offset 0x4759C9.
 *
 * @see IsProjectedPointInsideIndoorFace
 */
bool IsProjectedPointInsideOutdoorFace(BLVFace *face, int model_index, const Vec3_short_ &point);

