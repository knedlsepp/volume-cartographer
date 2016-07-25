//
// Created by Hannah Hatch on 7/25/16.
/*Algorithm takes in a base mesh and reduces the number of points and faces by removing every other point along
 * horizantal and vertical axis*/

#ifndef VC_ORDEREDRESAMPLING_H
#define VC_ORDEREDRESAMPLING_H

#include <iostream>
#include <vector>
#include <stdexcept>

#include <vc_defines.h>
#include "vc_datatypes.h"

namespace volcart {
    namespace meshing {
        class orderedResampling {
        public:
            orderedResampling();
            orderedResampling(VC_MeshType::Pointer mesh, int in_width, int in_height);

            void setMesh(VC_MeshType::Pointer mesh, int in_width, int in_height);
            VC_MeshType::Pointer getOutputMesh();
            int getOutputWidth();
            int getOutputHeight();

            void compute();

        private:
            VC_MeshType::Pointer _input;
            int _inWidth;  // how many rows
            int _inHeight; // how many points per row

            VC_MeshType::Pointer _output;
            int _outWidth;
            int _outHeight;

            void _addCell(unsigned long a, unsigned long b, unsigned long c);

        }; //orderedResampling
    } //meshing
} //volcart


#endif //VC_ORDEREDRESAMPLING_H
