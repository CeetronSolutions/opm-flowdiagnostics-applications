/*
  Copyright 2026 Equinor ASA.

  This file is part of the Open Porous Media Project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPM_ECLGRAPHGRIDSOURCE_HEADER_INCLUDED
#define OPM_ECLGRAPHGRIDSOURCE_HEADER_INCLUDED

#include <array>
#include <string>
#include <vector>

/// \file
///
/// Grid topology needed by \c ECLGraph, decoupled from any particular grid
/// reader.  Allows building the connection graph without an ERT/resdata
/// \c ecl_grid_type, and without loading cell geometry.

namespace Opm {

    /// Non-neighbouring connection between two global cells.
    struct ECLGraphNNC
    {
        /// Grid numbers.  Zero for main grid, positive for LGRs.
        int grid1;
        int grid2;

        /// Global (relative to grid) cell indices.
        int glob1;
        int glob2;

        /// Transmissibility in result set's unit conventions.
        double trans;

        /// Position in on-disk NNC arrays (TRANNNC, TRANGL, ...).  Maps
        /// NNC flux vectors in restart files to this connection.
        int inputIndex;
    };

    /// Raw corner-point geometry of the main grid.
    struct ECLGraphGeometry
    {
        std::array<int, 3>  size;
        std::vector<double> zcorn;
        std::vector<double> coord;
        std::vector<int>    actnum;
    };

    /// Abstract provider of grid topology for \c ECLGraph.
    class ECLGraphGridSource
    {
    public:
        virtual ~ECLGraphGridSource() = default;

        /// Number of grids.  Main grid plus number of LGRs.
        virtual int numGrids() const = 0;

        /// Grid name.  Empty for the main grid.
        virtual std::string gridName(int gridID) const = 0;

        /// Number of cells in each Cartesian direction.
        virtual std::array<int, 3> dimensions(int gridID) const = 0;

        /// Global cell index of each active cell, in active cell order.
        virtual std::vector<int> activeToGlobal(int gridID) const = 0;

        /// Whether each active cell is further subdivided by an LGR.
        virtual std::vector<bool> activeIsSubdivided(int gridID) const = 0;

        /// All non-neighbouring connections of the model, including those
        /// between main grid and LGRs.
        virtual std::vector<ECLGraphNNC> nncs() const = 0;

        /// Main grid geometry.  Optional, returns false if not provided.
        virtual bool geometry(ECLGraphGeometry& /* geom */) const
        {
            return false;
        }
    };

} // namespace Opm

#endif // OPM_ECLGRAPHGRIDSOURCE_HEADER_INCLUDED
