// Average
#include "Action_Average.h"
#include "CpptrajStdio.h"
#include "TrajectoryFile.h"

// CONSTRUCTOR
Average::Average() {
  //fprintf(stderr,"Average Con\n");
  AvgFrame=NULL;
  Nframes=0;
  start=0;
  stop=0;
  avgfilename=NULL;
  AvgParm=NULL;
  parmStripped=false;
} 

// DESTRUCTOR
Average::~Average() {
  //fprintf(stderr,"Average Destructor.\n");
  if (AvgFrame!=NULL) delete AvgFrame;
  if (parmStripped && AvgParm!=NULL) delete AvgParm;
}

/* Average::init()
 * Expected call: average <filename> [mask] [start <start>] [stop <stop>] [offset <offset>]
 *                [TRAJOUT ARGS]  
 */
int Average::init( ) {
  char *mask1;

  // Get Keywords
  avgfilename = actionArgs.getNextString();
  if (avgfilename==NULL) {
    mprinterr("Error: average: No filename given.\n");
    return 1;
  }
  // User start/stop args are +1
  start = actionArgs.getKeyInt("start",1);
  start--;
  targetFrame=start;
  stop = actionArgs.getKeyInt("stop",-1);
  if (stop!=-1) stop--;
  offset = actionArgs.getKeyInt("offset",1);

  // Get Masks
  mask1 = actionArgs.getNextMask();
  //fprintf(stdout,"    Mask 1: %s\n",mask1);
  Mask1.SetMaskString(mask1);

  // Save all remaining arguments for setting up the trajectory at the end.
  trajArgs = actionArgs;
  // Mark all action args complete, otherwise cpptraj thinks there were
  // unhandled args and will print a phantom error.
  actionArgs.MarkAll(); 

  mprintf("    AVERAGE: Averaging over");
  if (mask1!=NULL)
    mprintf(" coordinates in mask [%s]",Mask1.maskString);
  else
    mprintf(" all atoms");
  if (stop==-1) 
    mprintf(", starting from frame %i",start+1);
  else
    mprintf(", frames %i-%i",start+1,stop+1);
  if (offset!=1)
    mprintf(", offset %i",offset);
  mprintf(".\n             Writing averaged coords to [%s]\n",avgfilename);

  Nframes = 0;

  return 0;
}

/* Average::setup()
 * On first call, set up Frame according to first parmtop. This will be
 * used for coordinate output.
 * On subsequent calls, determine if the number of atoms is greater than or
 * less than the original # atoms. Never calculate more than the original
 * # atoms.
 */
int Average::setup() {

  if ( Mask1.SetupMask(P,debug) ) return 1;

  if (Mask1.None()) {
    mprintf("    Error: Average::setup: No Atoms in mask.\n");
    return 1;
  }

  mprintf("    AVERAGE:");

  if (AvgFrame==NULL) {
    mprintf(" Averaging over %i atoms.\n",Mask1.Nselected);
    AvgFrame = new Frame();
    AvgFrame->SetupFrameFromMask(&Mask1,P->mass);
    AvgFrame->ZeroCoords();
    Natom = AvgFrame->natom; // Equal to Mask1.Nselected
    // AvgParm will be used for coordinate output
    // If the number of selected atoms is less than the current parm, strip
    // the parm for output purposes.
    if (Mask1.Nselected<P->natom) {
      mprintf("             Atom selection < natom, stripping parm for averaging only:\n");
      AvgParm = P->modifyStateByMask(Mask1.Selected, Mask1.Nselected);
      parmStripped=true;
      AvgParm->Summary();
    } else 
      AvgParm = P; 
  } else {
    // If the frame is already set up, check to see if the current number
    // of atoms is bigger or smaller. If bigger, only average Natom coords.
    // If smaller, only average P->natom coords.
    if (Mask1.Nselected > AvgFrame->natom) {
      Natom = AvgFrame->natom;
      mprintf("Warning: Average [%s]: Parm %s selected# atoms (%i) > original parm %s\n",
              avgfilename,P->parmName,Mask1.Nselected,AvgParm->parmName);
      mprintf("         selected# atoms (%i).\n",AvgFrame->natom);
    } else if (Mask1.Nselected < AvgFrame->natom) {
      Natom = Mask1.Nselected;
      mprintf("Warning: Average[%s]: Parm %s selected# atoms (%i) < original parm %s\n",
              avgfilename, P->parmName, Mask1.Nselected,AvgParm->parmName);
      mprintf("         selected# atoms (%i).\n",AvgFrame->natom);
    } else {
      Natom = AvgFrame->natom;
    }
    mprintf("    AVERAGE: %i atoms will be averaged for this parm.\n",Natom);
  }
        
  return 0;  
}

/* Average::action()
 */
int Average::action() {
  int atom3,i3;

  if (currentFrame != targetFrame) return 0;

  i3=0;
  for (int n = 0; n < Natom; n++) {
    atom3 = Mask1.Selected[n] * 3;
    AvgFrame->X[i3  ] += F->X[atom3  ];
    AvgFrame->X[i3+1] += F->X[atom3+1];
    AvgFrame->X[i3+2] += F->X[atom3+2];
    i3 += 3;
  }
  Nframes++; 

  targetFrame+=offset;
  // Since currentFrame will never be -1 this effectively disables the routine
  if (targetFrame>stop && stop!=-1) targetFrame=-1;
  return 0;
} 

/* Average::print()
 */
void Average::print() {
  TrajectoryFile outfile;
  double d_Nframes;

  if (Nframes < 1) return;
  d_Nframes = (double) Nframes;
  AvgFrame->Divide(d_Nframes);

  mprintf("    AVERAGE: [%s]\n",this->CmdLine());

  if (outfile.SetupWrite(avgfilename, &trajArgs, AvgParm, AMBERTRAJ)) {
    mprinterr("Error: AVERAGE: Could not set up %s for write.\n",avgfilename);
    return;
  }

  outfile.PrintInfo(0);

  outfile.WriteFrame(0, AvgParm, AvgFrame->X, AvgFrame->V, AvgFrame->box, AvgFrame->T);

  outfile.EndTraj();
}
