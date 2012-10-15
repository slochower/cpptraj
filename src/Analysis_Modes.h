#ifndef INC_ANALYSIS_MODES_H
#define INC_ANALYSIS_MODES_H
#include "Analysis.h"
#include "DataSet_Modes.h"
class Analysis_Modes : public Analysis {
  public:
    Analysis_Modes();
    ~Analysis_Modes();

    int Setup(DataSetList*);
    int Analyze();

  private:
    static const double CONSQ;
    static const double TKBC2;
    static const double AVO;
    static const double CNST;
    static const double CMTOA;
    static const double CONT;

    enum modeAnalysisType { FLUCT=0, DISPLACE, CORR };
    static const char analysisTypeString[][22];
    typedef std::vector< std::pair<int,int> > modestackType;
    typedef modestackType::const_iterator modestack_it;

    modeAnalysisType type_; // iarg1
    int beg_;
    int end_;
    bool bose_;
    double factor_;
    DataSet_Modes* modinfo_;
    std::string filename_;
    modestackType atompairStack_;
    double* results_;

    void CheckDeprecated(std::string&, const char*);
    void CalcDipoleCorr();
};
#endif
