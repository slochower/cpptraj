# CPPTRAJ - Main makefile
# Daniel R. Roe
# 2010-11-18

include ../config.h

# Create cpptraj binary in ./src/
all:
	cd src && $(MAKE)

# Create cpptraj binary and move to ./bin/
install:
	cd src && $(MAKE) install

# Create cpptraj binary within AmberTools
yes:
	cd src && $(MAKE) -f Makefile_at install

# Run Tests
check:
	cd test && $(MAKE) test

# Clean up
clean:
	cd src && $(MAKE) clean

# Clean up for AmberTools
atclean:
	cd src && $(MAKE) -f Makefile_at clean

# called if cpptraj was disabled in AT's configure
no:
	@echo "Skipping cpptraj"

uninstall:
	/bin/rm -f $(BINDIR)/cpptraj
