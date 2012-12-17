#ifndef INC_DATASET_COORDS_H
#define INC_DATASET_COORDS_H
#include "DataSet.h"
#include "Topology.h"
class DataSet_Coords : public DataSet {
  public:
    DataSet_Coords();
    // DataSet routines
    int Allocate(int);
    int Size() { return (int)coords_.size(); }
    void Info();
    /// Add a frame
    void AddFrame(Frame const&);
    void AddCrd(Frame::CRDtype const&);
    /// Set frame at position
    void SetFrame(int, Frame const&);
    /// Set topology and number of box coords.
    void SetTopology(Topology const&);
    Topology const& Top()                     const { return top_;          }
    size_t NumBoxCrd()                        const { return numBoxCrd_;    }
    const Frame::CRDtype& operator[](int idx) const { return coords_[idx];  }
  private:
    typedef std::vector<Frame::CRDtype> CRDarray;
    CRDarray coords_;                  ///< Array of coordinate frames.
    Topology top_;                     ///< Topology corresponding to coordinates.
    size_t numBoxCrd_;                 ///< Number of box coords (0 or 6).
};
#endif
