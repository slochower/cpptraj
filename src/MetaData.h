#ifndef INC_METADATA_H
#define INC_METADATA_H
#include "FileName.h" 
/** Attributes used for DataSet classification and selection. Name is typically
  * associated with the Action etc. that creates the DataSet, e.g. RMSD or
  * distance. Index is used when and action outputs numbered subsets of data,
  * e.g. with RMSD it is possible to output per-residue RMSD, where the DataSet
  * index corresponds to the residue number. Aspect is used to further subdivide
  * output data type; e.g. with nucleic acid analysis each base pair (denoted by
  * index) has shear, stagger etc calculated.
  */
class MetaData {
  public:
    /// Source of data stored in DataSet, used by Analysis_Statistics. Must match Smodes.
    enum scalarMode {
      M_DISTANCE=0, M_ANGLE, M_TORSION, M_PUCKER, M_RMS, M_MATRIX, UNKNOWN_MODE
    };
    /// Type of DataSet, used by Analysis_Statistics. Must match Stypes, TypeModes
    enum scalarType {
      ALPHA=0, BETA, GAMMA, DELTA, EPSILON, ZETA,  PUCKER,
      CHI,     H1P,  C2P,   PHI,   PSI,     PCHI,  OMEGA,
      NOE,
      DIST,   COVAR,     MWCOVAR, //FIXME: May need more descriptive names 
      CORREL, DISTCOVAR, IDEA,
      IRED,   DIHCOVAR,
      UNDEFINED
    };
    /// Mark whether this data set is a time series.
    enum tsType { UNKNOWN_TS = 0, IS_TS, NOT_TS };
    /// CONSTRUCTOR
    MetaData() : idx_(-1), ensembleNum_(-1), scalarmode_(UNKNOWN_MODE),
                 scalartype_(UNDEFINED), timeSeries_(UNKNOWN_TS) {}
    /// CONSTRUCTOR - name, aspect, index, ensemble number (for searching)
    MetaData(std::string const& n, std::string const& a, int i, int e) :
      name_(n), aspect_(a), idx_(i), ensembleNum_(e), scalarmode_(UNKNOWN_MODE),
      scalartype_(UNDEFINED), timeSeries_(UNKNOWN_TS) {}
    /// CONSTRUCTOR - name, aspect, index, ensemble number, legend
//    MetaData(std::string const& n, std::string const& a, int i, int e, std::string const& l) :
//      name_(n), aspect_(a), legend_(l), idx_(i), ensembleNum_(e), scalarmode_(UNKNOWN_MODE),
//      scalartype_(UNDEFINED), timeSeries_(UNKNOWN_TS) {}
    /// CONSTRUCTOR - name
    MetaData(const char* n) : name_(n), idx_(-1), ensembleNum_(-1), scalarmode_(UNKNOWN_MODE),
      scalartype_(UNDEFINED), timeSeries_(UNKNOWN_TS) {}
    /// CONSTRUCTOR - name
    MetaData(std::string const& n) : name_(n), idx_(-1), ensembleNum_(-1),
      scalarmode_(UNKNOWN_MODE), scalartype_(UNDEFINED), timeSeries_(UNKNOWN_TS) {}
    /// CONSTRUCTOR - name, aspect
    MetaData(std::string const& n, std::string const& a) : name_(n), aspect_(a), idx_(-1),
      ensembleNum_(-1), scalarmode_(UNKNOWN_MODE), scalartype_(UNDEFINED),
      timeSeries_(UNKNOWN_TS) {}
    /// CONSTRUCTOR - name, scalarmode
    MetaData(std::string const& n, scalarMode m) : name_(n), idx_(-1), ensembleNum_(-1),
      scalarmode_(m), scalartype_(UNDEFINED), timeSeries_(UNKNOWN_TS) {}
    /// CONSTRUCTOR - name, scalarmode, scalartype
    MetaData(std::string const& n, scalarMode m, scalarType t) : name_(n), idx_(-1),
      ensembleNum_(-1), scalarmode_(m), scalartype_(t), timeSeries_(UNKNOWN_TS) {}
    /// CONSTRUCTOR - File name, name
    MetaData(FileName const& f, std::string const& n) : fileName_(f), name_(n), idx_(-1),
      ensembleNum_(-1), scalarmode_(UNKNOWN_MODE), scalartype_(UNDEFINED),
      timeSeries_(UNKNOWN_TS) { if (name_.empty()) name_ = fileName_.Base(); }

    /// Comparison for sorting name/aspect/idx
    inline bool operator<(const MetaData&) const;
    /// \return string containing scalar mode and type if defined.
    std::string ScalarDescription() const;
    /// \return scalarMode that matches input keyword.
    static scalarMode ModeFromKeyword(std::string const&);
    /// \return true if DataSet is periodic.
    bool IsTorsionArray() const;
    /// \return scalarType that matches keyword; check that mode is valid if specified.
    static scalarType TypeFromKeyword(std::string const&, scalarMode&);
    static scalarType TypeFromKeyword(std::string const&, scalarMode const&);
    const char* ModeString()    const { return Smodes[scalarmode_];}
    const char* TypeString()    const { return Stypes[scalartype_];}
    static const char* ModeString(scalarMode m) { return Smodes[m]; }
    static const char* TypeString(scalarType t) { return Stypes[t]; }
    /// Set default legend from name/aspect/index/ensemble num
    void SetDefaultLegend();
    /// \return string containing name based on MetaData
    std::string PrintName() const;
    /// \return true if given MetaData matches this exactly.
    bool Match_Exact(MetaData const&) const;

    std::string const& Name()   const { return name_;        }
    std::string const& Aspect() const { return aspect_;      }
    std::string const& Legend() const { return legend_;      }
    int Idx()                   const { return idx_;         }
    int EnsembleNum()           const { return ensembleNum_; }
    tsType TimeSeries()         const { return timeSeries_;  }
    scalarType ScalarType()     const { return scalartype_;  }

    void SetName(std::string const& n)   { name_ = n;        }
    void SetLegend(std::string const& l) { legend_ = l;      }
    void SetIdx(int i)                   { idx_ = i;         }
    void SetEnsembleNum(int e)           { ensembleNum_ = e; }
    void SetScalarType(scalarType s)     { scalartype_ = s;  }
    void SetTimeSeries(tsType t)         { timeSeries_ = t;  }
  private:
    static const char* Smodes[]; ///< String for each scalar mode
    static const char* Stypes[]; ///< String for each scalar type
    static const scalarMode TypeModes[]; ///< The mode each type is associated with.
    FileName fileName_;       ///< Associated file name.
    std::string name_;        ///< Name of the DataSet (optionally tag)
    std::string aspect_;      ///< DataSet aspect.
    std::string legend_;      ///< DataSet legend.
    int idx_;                 ///< DataSet index
    int ensembleNum_;         ///< DataSet ensemble number.
    scalarMode scalarmode_;   ///< Source of data in DataSet.
    scalarType scalartype_;   ///< Specific type of data in DataSet (if any).
    tsType timeSeries_;       ///< DataSet time series status, for allocation. 
};
// ----- INLINE FUNCTIONS ------------------------------------------------------
bool MetaData::operator<(const MetaData& rhs) const {
  if ( name_ == rhs.name_ ) {
    if ( aspect_ == rhs.aspect_ ) {
      return ( idx_ < rhs.idx_ );
    } else {
      return ( aspect_ < rhs.aspect_ );
    }
  } else {
    return ( name_ < rhs.name_ );
  }
}
bool MetaData::IsTorsionArray() const {
  return ( scalarmode_ == M_TORSION || scalarmode_ == M_PUCKER || scalarmode_ == M_ANGLE );
}
#endif
