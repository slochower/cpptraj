#!/bin/bash

. ../MasterTest.sh

CleanFiles ptraj.in cpptraj.nc mop.xtc temp.crd total?.out

if [ ! -z "$CPPTRAJ_NO_XDRFILE" ] ; then
  echo ""
  echo "Cpptraj was compiled without XDR file support. Skipping XTC tests."
  echo ""
  EndTest
  exit 0
fi

INPUT="-i ptraj.in"

GmxXtcRead() {
  MaxThreads 2 "XTC read/write"
  if [ $? -eq 0 ] ; then
    if [ -z "$CPPTRAJ_NETCDFLIB" ] ; then
      echo "XTC read/write test requires NetCDF, skipping."
    elif [ ! -z "$DO_PARALLEL" -a -z "$CPPTRAJ_PNETCDFLIB" ] ; then
      echo "XTC read/write test requires parallel NetCDF, skipping."
    else
      cat > ptraj.in <<EOF
parm ../Test_GromacsTrr/nvt.protein.mol2
trajin nvt.2frame.xtc
trajout cpptraj.nc
EOF
      RunCpptraj "XTC read test"
      NcTest cpptraj.nc.save cpptraj.nc
    fi
  fi
}

GmxXtcWrite() {
  NotParallel "XTC write test"
  if [ $? -eq 0 ] ; then
    cat > ptraj.in <<EOF
parm ../tz2.truncoct.parm7
trajin ../tz2.truncoct.crd
trajout mop.xtc xtc
EOF
    RunCpptraj "CRD => XTC"

    cat > ptraj.in <<EOF
parm ../tz2.truncoct.parm7
trajin mop.xtc
trajout temp.crd title "trajectory generated by ptraj"
EOF
    RunCpptraj "XTC => CRD"
    DoTest temp.crd.save temp.crd
  fi
}

# Gromacs XTC append
GmxXtcAppend() {
  NotParallel "GMX XTC append"
  if [ $? -eq 0 ] ; then
    cat > ptraj.in <<EOF
parm ../tz2.truncoct.parm7
trajin ../tz2.truncoct.crd 1 5
trajout mop.xtc xtc
EOF
    RunCpptraj "CRD(1-5) => XTC"
    cat > ptraj.in <<EOF
parm ../tz2.truncoct.parm7
trajin ../tz2.truncoct.crd 6 10
trajout mop.xtc stc append
EOF
    RunCpptraj "CRD(6-10) => XTC"
    cat > ptraj.in <<EOF
parm ../tz2.truncoct.parm7
trajin mop.xtc
trajout temp.crd title "trajectory generated by ptraj"
EOF
    RunCpptraj "XTC (appended) => CRD"
    DoTest temp.crd.save temp.crd
  fi
}

# Gromacs XTC with offsets
GmxXtcOffset() {
  #MaxThreads 5 "GMX XTC offset test"
  NotParallel "GMX XTC offset test"
  if [ $? -eq 0 ] ; then
    cat > ptraj.in <<EOF
parm ../tz2.truncoct.parm7
trajin temp.crd.save 2 10 2
trajout total1.out title "offset test"
EOF
    RunCpptraj "GMX: CRD with offset"
    cat > ptraj.in <<EOF
parm ../tz2.truncoct.parm7
trajin mop.xtc 2 10 2
trajout total2.out title "offset test"
EOF
    RunCpptraj "GMX: XTC with offset"
    DoTest total1.out total2.out
  fi
}

GmxXtcRead
GmxXtcWrite
GmxXtcAppend
GmxXtcOffset

EndTest
exit 0
