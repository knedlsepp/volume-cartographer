/**
 * @file SmoothNormals.h
 * @brief Smooth vertex normals within a specified radius.
 */
#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "core/vc_defines.h"
#include "meshing/DeepCopy.h"

namespace volcart
{
namespace meshing
{
/**
 * @author Abigail Coleman
 * @date June 2015
 *
 * @brief Smooth vertex normals within a specified radius.
 *
 * Uses ITK's neighborhood locator to get the list of neighboring vertices
 * within the provided spherical radius. Returns a DeepCopy of the
 * original mesh, with smoothed vertex normals.
 *
 * @ingroup Meshing
 *
 * @param radius Size of the spherical neighborhood
 */
ITKMesh::Pointer SmoothNormals(ITKMesh::Pointer input, double radius);
}
}
