#ifndef INC_TOPOLOGYLIST_H
#define INC_TOPOLOGYLIST_H
#include "Topology.h"
#include "FileList.h"
#include "ArgList.h"
// Class: TopologyList
/// Holds a list of Topology classes.
/** Can either add new topology by filename, or add existing topology by 
  * address. Can search for topology in list by index, full/base filename,
  * or tag.
  * TopologyList also serves as the command interpreter for parm-related
  * commands. Currently recognized commands are: parm, parmlist, parminfo,
  * parmbondinfo, parmmolinfo, bondsearch, nobondsearch, molsearch, 
  * nomolsearch.
  */
class TopologyList : public FileList {
  public:

    TopologyList();
    ~TopologyList();

    int CheckCommand(ArgList &);
    Topology *GetParm(int);
    Topology *GetParm(ArgList&);
    int AddParmFile(char*); // TODO: Make obsolete
    int AddParmFile(char*,std::string&);
    int AddParm(Topology*);
    void Print();

/*   
    int GetParmIndex(char *);
    int GetParmIndexByTag(std::string&);
    int ReplaceParm(int, AmberParm *);
*/
  private:
    std::vector<Topology*> TopList_;
    bool hasCopies_;  ///< true: List contains addresses of topologies, do not delete
    bool bondsearch_; ///< true: When parm is opened bond info will be filled in
    bool molsearch_;  ///< true: When parm is opened molecule info will be filled in
};
#endif
