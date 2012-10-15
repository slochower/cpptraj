#ifndef INC_ANALYSIS_MATRIX_H
#define INC_ANALYSIS_MATRIX_H
#include "Analysis.h"
#include "DataSet_Matrix.h"
#include "DataSet_Modes.h"
class Analysis_Matrix : public Analysis {
  public:
    Analysis_Matrix();

    int Setup(DataSetList*);
    int Analyze();
    void Print(DataFileList*);
  private:
    DataSet_Matrix* matrix_;
    DataSet_Modes* modes_;
    std::string outfilename_;
    std::string outthermo_;
    int nevec_;
    bool thermopt_;
    bool reduce_;
    bool eigenvaluesOnly_;
};
#endif
